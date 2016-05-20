// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

// #define EXAMPLE_LIMIT_FRAMERATE 1
#include "./utils/dependencies.hpp"
#include <vrm/pp.hpp>

// The following example consists in a particle simulations. All particles are
// massless and collide with each other in a perfectly inelastic way. The
// particles also collide with the boundaries of the window.

// Every particle will have the following components:
// * Position (2D float vector).
// * Velocity (2D float vector).
// * Acceleration (2D float vector).
// * Color (SFML color struct).
// * Circle (radius float).

// The following systems will execute the simulation:
//
// * Acceleration: accelerates the particles.
//   (inner parallelism allowed)
//   (no dependencies)
//
// * Velocity: moves the particles.
//   (inner parallelism allowed)
//   (depends on: Acceleration)
//
// * Keep in bounds: prevents the particles from leaving the simulation's
//                   boundaries.
//   (inner parallelism allowed)
//   (depends on: Velocity)
//
// * Spatial partition: partitions the simulation space in a 2D grid to speed
//                      up broadphase collision detection. Produces lists of
//                      {entity_id; grid_index} pairs that will be later
//                      evaluated to fill the 2D grid.
//   (inner parallelism allowed)
//   (depends on: Keep in bounds)
//
// * Collision: detects collisions between particles and produces lists of
//              contacts that will be later evaluated to solve the collisions.
//   (inner parallelism allowed)
//   (depends on: Spatial partition)
//
// * Solve contacts: reads the contacts produced by Collision and solves them
//                   by moving particles and changing their velocities.
//   (inner parallelism disallowed)
//   (depends on: Collision)
//
// * Render colored circle: produces lists of vertices that will be rendered
//                          later on the SFML RenderWindow.
//   (inner parallelism allowed)
//   (depends on: Solve contacts)

namespace example
{
    // Boundaries of the simulation.
    constexpr auto left_bound = 0;
    constexpr auto right_bound = 1440;
    constexpr auto top_bound = 0;
    constexpr auto bottom_bound = 900;

    // Data of a collision contact.
    struct contact
    {
        // IDs of the colliding entities.
        ecst::entity_id _e0, _e1;

        // Distance between entities.
        float _dist;

        contact(ecst::entity_id e0, ecst::entity_id e1, float dist) noexcept
            : _e0(e0),
              _e1(e1),
              _dist(dist)
        {
        }
    };

    // Data for the assignment of an entity to a cell of the spatial
    // partitioning grid.
    struct sp_data
    {
        ecst::entity_id _e;
        sz_t _cell_x, _cell_y;

        sp_data(ecst::entity_id e, sz_t cell_x, sz_t cell_y) noexcept
            : _e(e),
              _cell_x(cell_x),
              _cell_y(cell_y)
        {
        }
    };

    // Component definitions.
    namespace c
    {
        // Components are simple classes, usually POD structs. There is no need
        // for components to derive from a "base component" class or to satisfy
        // a particular interface.

        struct position
        {
            vec2f _v;
        };

        struct velocity
        {
            vec2f _v;
        };

        struct acceleration
        {
            vec2f _v;
        };

        struct color
        {
            sfc _v;
        };

        struct circle
        {
            float _radius;
        };
    }

    // In order to avoid the annoying `data.template <component_type>` syntax
    // and to have an uniform "type-value encoding" interface both in the
    // implementation and user code, we define component and system "tags".

    // These "tags" are empty `constexpr` objects that wrap the type of the
    // component/system in a value, which ECST's functions accept:
    /*
        // Traditional:
        data.template get<c::position>(eid);

        // With tags:
        data.get(ct::position, eid);
    */
}

// Component tags, in namespace `example::ct`.
EXAMPLE_COMPONENT_TAG(acceleration);
EXAMPLE_COMPONENT_TAG(velocity);
EXAMPLE_COMPONENT_TAG(position);
EXAMPLE_COMPONENT_TAG(circle);
EXAMPLE_COMPONENT_TAG(color);

// A macro is used to define tags to suppress "unused variable" warnings and
// to avoid code repetition. Essentially, it expands to:
/*
    constexpr auto x = ecst::signature::component::tag<c::x>;
*/

// System tags, in namespace `example::st`.
EXAMPLE_SYSTEM_TAG(acceleration);
EXAMPLE_SYSTEM_TAG(velocity);
EXAMPLE_SYSTEM_TAG(keep_in_bounds);
EXAMPLE_SYSTEM_TAG(spatial_partition);
EXAMPLE_SYSTEM_TAG(collision);
EXAMPLE_SYSTEM_TAG(solve_contacts);
EXAMPLE_SYSTEM_TAG(render_colored_circle);

namespace example
{
    // System definitions.
    namespace s
    {
        // Systems are simple classes as well, that do not need to satisfy any
        // particular interface. They can store data and have any method the
        // user desires.

        // This system accelerates the subscribed particles.
        struct acceleration
        {
            // The `process` method is not hardcoded or specially recognized by
            // ECST in any way. Using a lambda in the execution code, we can
            // tell an ECST context to execute a particular method (also
            // forwarding extra arguments to it).

            // The `data` parameter is a proxy object generated by the system
            // execution strategy that abstracts away the eventual underlying
            // parallelism.

            template <typename TData>
            void process(ft dt, TData& data)
            {
                // Notice that the code below does not know anything about the
                // multithreading strategy employed by the system: the same
                // syntax works with any kind (or lack) of parallel execution.
                data.for_entities([&](auto eid)
                    {
                        auto& v = data.get(ct::velocity, eid)._v;
                        const auto& a = data.get(ct::acceleration, eid)._v;

                        v += a * dt;
                    });
            }
        };

        // This system moves the subscribed particles.
        struct velocity
        {
            template <typename TData>
            void process(ft dt, TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        auto& p = data.get(ct::position, eid)._v;
                        const auto& v = data.get(ct::velocity, eid)._v;

                        p += v * dt;
                    });
            }
        };

        // This system prevents the particles to get out of bounds.
        struct keep_in_bounds
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        auto& p = data.get(ct::position, eid)._v;
                        auto& v = data.get(ct::velocity, eid)._v;
                        const auto& radius = data.get(ct::circle, eid)._radius;

                        // Calculate the edge positions of the particle.
                        auto left = p.x - radius;
                        auto right = p.x + radius;
                        auto top = p.y - radius;
                        auto bottom = p.y + radius;

                        // Move and invert X velocity if necessary.
                        if(left < left_bound)
                        {
                            p.x = left_bound + radius;
                            v.x *= -1;
                        }
                        else if(right > right_bound)
                        {
                            p.x = right_bound - radius;
                            v.x *= -1;
                        }

                        // Move and invert Y velocity if necessary.
                        if(top < top_bound)
                        {
                            p.y = top_bound + radius;
                            v.y *= -1;
                        }
                        else if(bottom > bottom_bound)
                        {
                            p.y = bottom_bound - radius;
                            v.y *= -1;
                        }
                    });
            }
        };

        // This system stores a spatial partitioning grid (to speed-up
        // broadphase collision detection) and outputs a vector of `sp_data`,
        // which is used in a later step to actually fill the spatial
        // partitioning grid.
        struct spatial_partition
        {
            // Partitioning constants.
            static constexpr sz_t cell_size = 8;
            static constexpr sz_t offset = 2;

            static constexpr sz_t grid_width =
                right_bound / cell_size + (offset * 2);

            static constexpr sz_t grid_height =
                bottom_bound / cell_size + (offset * 2);

            static constexpr sz_t cell_count = grid_width * grid_height;

            // The grid is a simple 1D array of cells.
            // A cell is a vector of entity IDs.
            using cell_type = std::vector<ecst::entity_id>;
            std::array<std::array<cell_type, grid_height>, grid_width> _grid;

            // Clear all cells from the particles.
            void clear_cells() noexcept
            {
                for(auto& y : _grid)
                {
                    for(auto& c : y)
                    {
                        c.clear();
                    }
                }
            }

            auto& cell_by_idxs(sz_t x, sz_t y) noexcept
            {
                return _grid[x + offset][y + offset];
            }

            // Given an `sp_data`, emplaces an entity ID in a target cell.
            void add_sp(const sp_data& x)
            {
                cell_by_idxs(x._cell_x, x._cell_y).emplace_back(x._e);
            }

            // From world coordinates to cell index.
            auto idx(float x) noexcept
            {
                return x / cell_size;
            }

            // Returns the cell containing the position `p`.
            auto& cell_by_pos(const vec2f& p) noexcept
            {
                return cell_by_idxs(idx(p.x), idx(p.y));
            }

            // Executes `f` on every cell contaning the circle described by `p`
            // and `r`.
            template <typename TF>
            void for_cells_of(const vec2f& p, float r, TF&& f)
            {
                auto left = p.x - r;
                auto right = p.x + r;
                auto top = p.y - r;
                auto bottom = p.y + r;

                auto s_ix = fFloor(idx(left));
                auto e_ix = fCeil(idx(right));
                auto s_iy = fFloor(idx(top));
                auto e_iy = fCeil(idx(bottom));

                for(auto ix(s_ix); ix <= e_ix; ++ix)
                {
                    for(auto iy(s_iy); iy <= e_iy; ++iy)
                    {
                        f(ix, iy);
                    }
                }
            }

            template <typename TData>
            void process(TData& data)
            {
                // Get a reference to the output vector and clear it.
                auto& o = data.output();
                o.clear();

                // For every entity in the subtask...
                data.for_entities([&](auto eid)
                    {
                        // Access component data.
                        const auto& p = data.get(ct::position, eid)._v;
                        const auto& c = data.get(ct::circle, eid)._radius;

                        // Figure out the broadphase cell and emplace an
                        // `sp_data` instance in the output vector.
                        this->for_cells_of(p, c, [eid, &o](auto cx, auto cy)
                            {
                                o.emplace_back(eid, cx, cy);
                            });
                    });
            }
        };

        // This system detects collisions between particles and produces an
        // output vector of `contact` instances.
        struct collision
        {
            template <typename TData>
            void process(TData& data)
            {
                // Get a reference to the output vector and clear it.
                auto& out = data.output();
                out.clear();

                // Get a reference to the `spatial_partition` system.
                auto& sp = data.system(st::spatial_partition);

                // For every entity in the subtask...
                data.for_entities([&](auto eid)
                    {
                        // Access the component data.
                        auto& p0 = data.get(ct::position, eid)._v;
                        const auto& r0 = data.get(ct::circle, eid)._radius;

                        // Access the grid cell containing position `p0`.
                        auto& cell = sp.cell_by_pos(p0);

                        // For every unique entity ID pair...
                        for_unique_pairs(cell, eid, [&](auto eid2)
                            {
                                // Access the second particle's component data.
                                auto& p1 = data.get(ct::position, eid2)._v;
                                const auto& r1 =
                                    data.get(ct::circle, eid2)._radius;

                                // Check for a circle-circle collision.
                                auto sd = distance(p0, p1);
                                if(sd <= r0 + r1)
                                {
                                    // Emplace a `contact` in the output.
                                    out.emplace_back(eid, eid2, sd);
                                }
                            });
                    });
            }
        };

        // This single-threaded system solves contacts by preventing penetration
        // between particles and by modifying their velocities to simulate
        // bouncing.
        struct solve_contacts
        {
            template <typename TData>
            void process(TData& data)
            {
                // For every output produced by the collision detection
                // system...
                data.for_previous_outputs(st::collision,
                    [&](auto&, const auto& out)
                    {
                        for(const auto& x : out)
                        {
                            // Access the f-Dirst particle's data.
                            auto& p0 = data.get(ct::position, x._e0)._v;
                            auto& v0 = data.get(ct::velocity, x._e0)._v;
                            const auto& r0 =
                                data.get(ct::circle, x._e0)._radius;

                            // Access the second particle's data.
                            auto& p1 = data.get(ct::position, x._e1)._v;
                            auto& v1 = data.get(ct::velocity, x._e1)._v;
                            const auto& r1 =
                                data.get(ct::circle, x._e1)._radius;

                            // Solve.
                            solve_penetration(x._dist, p0, v0, r0, p1, v1, r1);
                        }
                    });
            }
        };

        // This system builds a vector of vertices for every subtask.
        // The vertices will then be rendered in a later step.
        struct render_colored_circle
        {
            static constexpr float tau = 6.28f;
            static constexpr sz_t precision = 5;
            static constexpr float inc = tau / precision;

            template <typename TData>
            void process(TData& data)
            {
                // Get a reference to the output vector, and clear it.
                auto& va = data.output();
                va.clear();

                // For every entity in the subtask...
                data.for_entities([this, &data, &va](auto eid)
                    {
                        // Access the component data.
                        const auto& p0 = data.get(ct::position, eid)._v;
                        const auto& c = data.get(ct::color, eid)._v;
                        auto& radius = data.get(ct::circle, eid)._radius;

                        // Function to create and emplace 3 vertices.
                        auto mk_triangle = [&va, &data, &p0, &c, &radius](
                            auto a0, auto a1)
                        {
                            auto a0cos = radius * tbl_cos(a0);
                            auto a0sin = radius * tbl_sin(a0);
                            auto a1cos = radius * tbl_cos(a1);
                            auto a1sin = radius * tbl_sin(a1);

                            vec2f p1(a0cos + p0.x, a0sin + p0.y);
                            vec2f p2(a1cos + p0.x, a1sin + p0.y);

                            va.emplace_back(p0, c);
                            va.emplace_back(p1, c);
                            va.emplace_back(p2, c);
                        };

                        // Build a circle.
                        for(sz_t i = 0; i < precision; ++i)
                        {
                            mk_triangle(inc * i, inc * (i + 1));
                        }
                    });
            }
        };
    }

    // Compile-time `std::size_t` entity limit.
    constexpr auto entity_limit = ecst::sz_v<65536>;

    // Compile-time initial particle count.
    constexpr auto initial_particle_count = ecst::sz_v<50000>;

    namespace ecst_setup
    {
        // Builds and returns a "component signature list".
        constexpr auto make_csl()
        {
            namespace slc = ecst::signature_list::component;

            return slc::v<                                 // .
                c::position, c::velocity, c::acceleration, // .
                c::color, c::circle                        // .
                >;
        }

        // Builds and returns a "system signature list".
        constexpr auto make_ssl()
        {
            // Signature namespace aliases.
            namespace ss = ecst::signature::system;
            namespace sls = ecst::signature_list::system;

            // Inner parallelism aliases and definitions.
            namespace ips = ecst::inner_parallelism::strategy;
            namespace ipc = ecst::inner_parallelism::composer;
            constexpr auto none = ips::none::v();
            constexpr auto par = ips::split_evenly_fn::v_cores();

            // Acceleration system.
            // * Multithreaded.
            // * No dependencies.
            constexpr auto ssig_acceleration =    // .
                ss::make<s::acceleration>(        // .
                    par,                          // .
                    ss::no_dependencies,          // .
                    ss::component_use(            // .
                        ss::mutate<c::velocity>,  // .
                        ss::read<c::acceleration> // .
                        ),                        // .
                    ss::output::none              // .
                    );

            // Velocity system.
            // * Multithreaded.
            constexpr auto ssig_velocity =           // .
                ss::make<s::velocity>(               // .
                    par,                             // .
                    ss::depends_on<s::acceleration>, // .
                    ss::component_use(               // .
                        ss::mutate<c::position>,     // .
                        ss::read<c::velocity>        // .
                        ),                           // .
                    ss::output::none                 // .
                    );


            // Keep in bounds system.
            // * Multithreaded.
            constexpr auto ssig_keep_in_bounds = // .
                ss::make<s::keep_in_bounds>(     // .
                    par,                         // .
                    ss::depends_on<s::velocity>, // .
                    ss::component_use(           // .
                        ss::mutate<c::velocity>, // .
                        ss::mutate<c::position>, // .
                        ss::read<c::circle>      // .
                        ),                       // .
                    ss::output::none             // .
                    );

            // Spatial partition system.
            // * Multithreaded.
            // * Output: `std::vector<sp_data>`.
            constexpr auto ssig_spatial_partition =        // .
                ss::make<s::spatial_partition>(            // .
                    par,                                   // .
                    ss::depends_on<s::keep_in_bounds>,     // .
                    ss::component_use(                     // .
                        ss::read<c::position>,             // .
                        ss::read<c::circle>                // .
                        ),                                 // .
                    ss::output::data<std::vector<sp_data>> // .
                    );

            // Collision detection system.
            // * Multithreaded.
            // * Output: `std::vector<contact>`.
            constexpr auto ssig_collision =                // .
                ss::make<s::collision>(                    // .
                    par,                                   // .
                    ss::depends_on<s::spatial_partition>,  // .
                    ss::component_use(                     // .
                        ss::mutate<c::velocity>,           // .
                        ss::mutate<c::position>,           // .
                        ss::read<c::circle>                // .
                        ),                                 // .
                    ss::output::data<std::vector<contact>> // .
                    );

            // Solve contacts system.
            // * Singlethreaded.
            constexpr auto ssig_solve_contacts =  // .
                ss::make<s::solve_contacts>(      // .
                    none,                         // .
                    ss::depends_on<s::collision>, // .
                    ss::component_use(            // .
                        ss::mutate<c::velocity>,  // .
                        ss::mutate<c::position>,  // .
                        ss::read<c::circle>       // .
                        ),                        // .
                    ss::output::none              // .
                    );

            // Render colored circle system.
            // * Multithreaded.
            // * Output: `std::vector<sf::Vertex>`.
            constexpr auto ssig_render_colored_circle =       // .
                ss::make<s::render_colored_circle>(           // .
                    par,                                      // .
                    ss::depends_on<s::solve_contacts>,        // .
                    ss::component_use(                        // .
                        ss::read<c::circle>,                  // .
                        ss::read<c::position>,                // .
                        ss::read<c::color>                    // .
                        ),                                    // .
                    ss::output::data<std::vector<sf::Vertex>> // .
                    );

// TODO:

#define ERASED(x)                                                         \
    using VRM_PP_CAT(VRM_PP_EXPAND(x), _tt) = decltype(VRM_PP_EXPAND(x)); \
    struct VRM_PP_CAT(e_, VRM_PP_EXPAND(x), _tt) : VRM_PP_CAT(x, _tt)     \
    {                                                                     \
    };                                                                    \
    constexpr VRM_PP_CAT(e_, VRM_PP_EXPAND(x), _tt)                       \
        VRM_PP_CAT(erased_, VRM_PP_EXPAND(x));


#define MAKE_ERASED_SLS_STEP0_BODY(i, d, a) ERASED(VRM_PP_EXPAND(a));
#define MAKE_ERASED_SLS_STEP1_BODY(i, d, a) \
    VRM_PP_CAT(erased_, VRM_PP_EXPAND(a)) VRM_PP_COMMA_IF(i)

#define MAKE_ERASED_SLS(...)                                     \
    VRM_PP_FOREACH_REVERSE(                                      \
        MAKE_ERASED_SLS_STEP0_BODY, VRM_PP_EMPTY(), __VA_ARGS__) \
    return sls::make(VRM_PP_FOREACH_REVERSE(                     \
        MAKE_ERASED_SLS_STEP1_BODY, VRM_PP_EMPTY(), __VA_ARGS__))


            MAKE_ERASED_SLS(               // .
                ssig_acceleration,         // .
                ssig_velocity,             // .
                ssig_keep_in_bounds,       // .
                ssig_spatial_partition,    // .
                ssig_collision,            // .
                ssig_solve_contacts,       // .
                ssig_render_colored_circle // .
                );

            /*


                        // Build and return the "system signature list".
                        return sls::make(                     // .
                            ssig_acceleration,         // .
                            ssig_velocity,             // .
                            ssig_keep_in_bounds,       // .
                            ssig_spatial_partition,    // .
                            ssig_collision,            // .
                            ssig_solve_contacts,       // .
                            ssig_render_colored_circle // .
                            );

                            */
        }
    }

    template <typename TProxy>
    void mk_particle(TProxy& proxy, const vec2f& position, float radius)
    {
        auto eid = proxy.create_entity();

        auto& ca = proxy.add_component(ct::acceleration, eid);
        ca._v.y = 1;

        auto& cv = proxy.add_component(ct::velocity, eid);
        cv._v = rndvec2f(-3, 3);

        auto& cp = proxy.add_component(ct::position, eid);
        cp._v = position;

        auto& cclr = proxy.add_component(ct::color, eid);
        cclr._v = sfc(rndf(0, 255), rndf(0, 255), rndf(0, 255), 255);

        auto& ccs = proxy.add_component(ct::circle, eid);
        ccs._radius = radius;
    }

    template <typename TContext>
    void init_ctx(TContext& ctx)
    {
        auto random_position = []
        {
            return vec2f{                      // .
                rndf(left_bound, right_bound), // .
                rndf(top_bound, bottom_bound)};
        };

        ctx.step([&](auto& proxy)
            {
                for(sz_t i = 0; i < initial_particle_count; ++i)
                {
                    mk_particle(proxy, random_position(), rndf(1, 4));
                }
            });
    }

    template <typename TContext, typename TRenderTarget>
    void update_ctx(TContext& ctx, TRenderTarget& rt, ft dt)
    {
        namespace sea = ::ecst::system_execution_adapter;

        ctx.step([&rt, dt](auto& proxy)
            {
                proxy.execute_systems( // .
                    sea::tag::for_subtasks(st::acceleration,
                        [dt](auto& s, auto& data)
                        {

                            s.process(dt, data);
                        }),
                    sea::tag::for_subtasks(st::velocity,
                        [dt](auto& s, auto& data)
                        {
                            s.process(dt, data);
                        }),
                    sea::tag::for_subtasks(st::keep_in_bounds,
                        [](auto& s, auto& data)
                        {
                            s.process(data);
                        }),
                    sea::tag::detailed(st::spatial_partition,
                        [&proxy](auto& s, auto& executor)
                        {
                            s.clear_cells();

                            executor.for_subtasks([&s](auto& data)
                                {
                                    s.process(data);
                                });

                            proxy.instance(st::spatial_partition)
                                .for_outputs([](auto& xs, auto& sp_vector)
                                    {
                                        for(const auto& x : sp_vector)
                                        {
                                            xs.add_sp(x);
                                        }
                                    });
                        }),
                    sea::tag::for_subtasks(st::collision,
                        [](auto& s, auto& data)
                        {
                            s.process(data);
                        }),
                    sea::tag::for_subtasks(st::solve_contacts,
                        [](auto& s, auto& data)
                        {
                            s.process(data);
                        }),
                    sea::tag::for_subtasks(st::render_colored_circle,
                        [](auto& s, auto& data)
                        {
                            s.process(data);
                        }));

                proxy.for_system_outputs(st::render_colored_circle,
                    [&rt](auto&, auto& va)
                    {
                        rt.draw(va.data(), va.size(),
                            sf::PrimitiveType::Triangles,
                            sf::RenderStates::Default);
                    });
            });
    }
}

#include "./utils/pres_game_app.hpp"

int main()
{
    // Namespace aliases.
    using namespace example;
    using namespace example::ecst_setup;
    namespace cs = ecst::settings;
    namespace ss = ecst::scheduler;

    // Define ECST context settings.
    constexpr auto s = ecst::settings::make(            // .
        cs::multithreaded(cs::allow_inner_parallelism), // .
        cs::fixed<entity_limit>,                        // .
        make_csl(),                                     // .
        make_ssl(),                                     // .
        cs::scheduler<ss::s_atomic_counter>             // .
        );

    using ssss = decltype(s);
    struct hs : public decltype(s)
    {
        using ssss::ssss;
    };

    // Create an ECST context.
    auto ctx = ecst::context::make_uptr(hs{});


    // Run the simulation.
    run_simulation(*ctx);
}

// TODO: celeron/nonius benchmarks
