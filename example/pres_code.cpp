// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

// #define EXAMPLE_LIMIT_FRAMERATE 1
#include "./utils/dependencies.hpp"

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

        struct life
        {
            float _v;
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
EXAMPLE_COMPONENT_TAG(life);

// A macro is used to define tags to suppress "unused variable" warnings and
// to avoid code repetition. Essentially, it expands to:
/*
    constexpr auto x = ecst::tag::component::vc::x>;
*/

// System tags, in namespace `example::st`.
EXAMPLE_SYSTEM_TAG(acceleration);
EXAMPLE_SYSTEM_TAG(velocity);
EXAMPLE_SYSTEM_TAG(keep_in_bounds);
EXAMPLE_SYSTEM_TAG(spatial_partition);
EXAMPLE_SYSTEM_TAG(collision);
EXAMPLE_SYSTEM_TAG(solve_contacts);
EXAMPLE_SYSTEM_TAG(render_colored_circle);
EXAMPLE_SYSTEM_TAG(cycle_color);
EXAMPLE_SYSTEM_TAG(life);

// TODO:
namespace example
{
    template <typename TProxy>
    void mk_particle(TProxy& proxy, const vec2f& position, float radius);
}

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

            struct cell_type
            {
                std::vector<ecst::entity_id> _v;
            };

            std::array<std::array<cell_type, grid_height>, grid_width> _grid;

            // Clear all cells from the particles.
            void clear_cells() noexcept
            {
                for(auto& y : _grid)
                {
                    for(auto& c : y)
                    {
                        c._v.clear();
                    }
                }
            }

            auto& cell_by_idxs(sz_t x, sz_t y) noexcept
            {
                return _grid[x + offset][y + offset]._v;
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

        // TODO:
        struct cycle_color
        {
            template <typename TData>
            void process(ft dt, TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        auto& c = data.get(ct::color, eid)._v;
                        c.r += 50.f * dt;
                        // float ca = c.a;
                        // c.a = static_cast<sf::Uint8>(std::fmod(ca, 255.f));
                        // c.a += dt;
                        // c.a = c.a % 255;
                        // std::cout << (int)c.a << "\n";
                    });
            }
        };

        // TODO:
        struct life
        {
            template <typename TData>
            void process(ft dt, TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        auto& l = data.get(ct::life, eid)._v;
                        l -= 10.f * dt;

                        if(l <= 0.f)
                        {
                            data.kill_entity(eid);
                            data.defer([](auto& proxy)
                                {
                                    auto random_position = []
                                    {
                                        return vec2f{                      // .
                                            rndf(left_bound, right_bound), // .
                                            rndf(top_bound, bottom_bound)};
                                    };

                                    mk_particle(
                                        proxy, random_position(), rndf(1, 4));
                                });
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
            namespace cs = ecst::signature::component;
            namespace csl = ecst::signature_list::component;

            // Store `c::acceleration`, `c::velocity` and `c::position` in three
            // separate contiguous buffers (SoA).
            constexpr auto cs_acceleration = // .
                cs::make(ct::acceleration).contiguous_buffer();

            constexpr auto cs_velocity = // .
                cs::make(ct::velocity).contiguous_buffer();

            constexpr auto cs_position = // .
                cs::make(ct::position).contiguous_buffer();

            // Store `c::color` and `c::circle` in the same contiguous buffer,
            // interleaved (AoS).
            constexpr auto cs_rendering = // .
                cs::make(ct::color, ct::circle).contiguous_buffer();

            // TODO:
            constexpr auto cs_life = // .
                cs::make(ct::life).contiguous_buffer();

            return csl::make(    // .
                cs_acceleration, // .
                cs_velocity,     // .
                cs_position,     // .
                cs_rendering,    // .
                cs_life          // .
                );
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
            constexpr auto split_evenly_per_core =
                ips::split_evenly_fn::v_cores();

            // Acceleration system.
            // * Multithreaded.
            // * No dependencies.
            constexpr auto ssig_acceleration =          // .
                ss::make(st::acceleration)              // .
                    .parallelism(split_evenly_per_core) // .
                    .read(ct::acceleration)             // .
                    .write(ct::velocity);               // .

            // Velocity system.
            // * Multithreaded.
            constexpr auto ssig_velocity =              // .
                ss::make(st::velocity)                  // .
                    .parallelism(split_evenly_per_core) // .
                    .dependencies(st::acceleration)     // .
                    .read(ct::velocity)                 // .
                    .write(ct::position);               // .

            // Keep in bounds system.
            // * Multithreaded.
            constexpr auto ssig_keep_in_bounds =        // .
                ss::make(st::keep_in_bounds)            // .
                    .parallelism(split_evenly_per_core) // .
                    .dependencies(st::velocity)         // .
                    .read(ct::circle)                   // .
                    .write(ct::velocity, ct::position); // .

            // Spatial partition system.
            // * Multithreaded.
            // * Output: `std::vector<sp_data>`.
            constexpr auto ssig_spatial_partition =            // .
                ss::make(st::spatial_partition)                // .
                    .parallelism(split_evenly_per_core)        // .
                    .dependencies(st::keep_in_bounds)          // .
                    .read(ct::position, ct::circle)            // .
                    .output(ss::output<std::vector<sp_data>>); // .

            // Collision detection system.
            // * Multithreaded.
            // * Output: `std::vector<contact>`.
            constexpr auto ssig_collision =                    // .
                ss::make(st::collision)                        // .
                    .parallelism(split_evenly_per_core)        // .
                    .dependencies(st::spatial_partition)       // .
                    .read(ct::circle)                          // .
                    .write(ct::position, ct::velocity)         // .
                    .output(ss::output<std::vector<contact>>); // .

            // Solve contacts system.
            // * Singlethreaded.
            constexpr auto ssig_solve_contacts =        // .
                ss::make(st::solve_contacts)            // .
                    .parallelism(none)                  // .
                    .dependencies(st::collision)        // .
                    .read(ct::circle)                   // .
                    .write(ct::velocity, ct::position); // .

            // Render colored circle system.
            // * Multithreaded.
            // * Output: `std::vector<sf::Vertex>`.
            constexpr auto ssig_render_colored_circle =           // .
                ss::make(st::render_colored_circle)               // .
                    .parallelism(split_evenly_per_core)           // .
                    .dependencies(st::solve_contacts)             // .
                    .read(ct::circle, ct::position, ct::color)    // .
                    .output(ss::output<std::vector<sf::Vertex>>); // .

            // TODO:
            constexpr auto ssig_cycle_color =           // .
                ss::make(st::cycle_color)               // .
                    .parallelism(split_evenly_per_core) // .
                    .write(ct::color);                  // .

            // TODO:
            constexpr auto ssig_life =                  // .
                ss::make(st::life)                      // .
                    .parallelism(split_evenly_per_core) // .
                    .write(ct::life);                   // .

            // Build and return the "system signature list".
            return sls::make(               // .
                ssig_acceleration,          // .
                ssig_velocity,              // .
                ssig_keep_in_bounds,        // .
                ssig_spatial_partition,     // .
                ssig_collision,             // .
                ssig_solve_contacts,        // .
                ssig_render_colored_circle, // .
                ssig_cycle_color,           // .
                ssig_life                   // .
                );
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

        auto& ccl = proxy.add_component(ct::life, eid);
        ccl._v = rndf(2, 10);
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

        auto ft_tags =
            sea::t(st::acceleration, st::velocity, st::cycle_color, st::life);

        auto nonft_tags = sea::t(st::keep_in_bounds, st::collision,
            st::solve_contacts, st::render_colored_circle);

        ctx.step([&rt, dt, &ft_tags, &nonft_tags](auto& proxy)
            {
                proxy.execute_systems()(
                    ft_tags.for_subtasks([dt](auto& s, auto& data)
                        {
                            s.process(dt, data);
                        }),
                    nonft_tags.for_subtasks([](auto& s, auto& data)
                        {
                            s.process(data);
                        }),
                    sea::t(st::spatial_partition)
                        .detailed_instance([&proxy](auto& i, auto& executor)
                            {
                                auto& s(i.system());
                                s.clear_cells();

                                executor.for_subtasks([&s](auto& data)
                                    {
                                        s.process(data);
                                    });

                                i.for_outputs([](auto& xs, auto& sp_vector)
                                    {
                                        for(const auto& x : sp_vector)
                                        {
                                            xs.add_sp(x);
                                        }
                                    });
                            }));

                proxy.for_system_outputs(st::render_colored_circle,
                    [&rt](auto&, auto& va)
                    {
                        // TODO:
                        if(false)
                        {
                            rt.draw(va.data(), va.size(),
                                sf::PrimitiveType::Triangles,
                                sf::RenderStates::Default);
                        }
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
    constexpr auto s =                        // .
        ecst::settings::make()                // .
            .allow_inner_parallelism()        // .
            .fixed_entity_limit(entity_limit) // .
            .component_signatures(make_csl()) // .
            .system_signatures(make_ssl())    // .
            .scheduler(cs::scheduler<ss::s_atomic_counter>);

    using ssss = decltype(s);
    struct hs : public ssss
    {
    };

    // Create an ECST context.
    auto ctx = ecst::context::make_uptr(hs{});

    // Run the simulation.
    run_simulation(*ctx);
}
