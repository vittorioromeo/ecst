// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include <ecst.hpp>
#include "./settings_generator.hpp"

#define EXAMPLE_COMPONENT_TAG(x)                              \
                                                              \
    namespace example                                         \
    {                                                         \
        namespace c                                           \
        {                                                     \
            struct x;                                         \
        }                                                     \
        namespace ct                                          \
        {                                                     \
            constexpr auto x = ecst::tag::component::v<c::x>; \
            void VRM_CORE_UNUSED_FN ECST_CONST_FN VRM_PP_CAT( \
                x, __LINE__, warning_suppressor)()            \
            {                                                 \
                (void) x;                                     \
            }                                                 \
        }                                                     \
    }                                                         \
    ECST_SPECIALIZE_COMPONENT_NAME(example::c::x)

#define EXAMPLE_SYSTEM_TAG(x)                                 \
                                                              \
    namespace example                                         \
    {                                                         \
        namespace s                                           \
        {                                                     \
            struct x;                                         \
        }                                                     \
        namespace st                                          \
        {                                                     \
            constexpr auto x = ecst::tag::system::v<s::x>;    \
            void VRM_CORE_UNUSED_FN ECST_CONST_FN VRM_PP_CAT( \
                x, __LINE__, warning_suppressor)()            \
            {                                                 \
                (void) x;                                     \
            }                                                 \
        }                                                     \
    }                                                         \
    ECST_SPECIALIZE_SYSTEM_NAME(example::s::x)

using ecst::sz_t;
using ecst::sz_v;
using ft = float;

namespace example
{
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
            float _v;
        };

        struct velocity
        {
            float _v;
        };

        struct acceleration
        {
            float _v;
        };

        struct color
        {
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
}

// Component tags, in namespace `example::ct`.
EXAMPLE_COMPONENT_TAG(acceleration);
EXAMPLE_COMPONENT_TAG(velocity);
EXAMPLE_COMPONENT_TAG(position);
EXAMPLE_COMPONENT_TAG(circle);
EXAMPLE_COMPONENT_TAG(color);
EXAMPLE_COMPONENT_TAG(life);

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

//
namespace example
{
    template <typename TProxy>
    void mk_particle(TProxy& proxy, const float& position, float radius);
}

namespace example
{
    // System definitions.
    namespace s
    {
        // This system accelerates the subscribed particles.
        struct acceleration
        {
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

                        (void)p;
                        (void)v;
                        (void)radius;
                    });
            }
        };

        // This system stores a spatial partitioning grid (to speed-up
        // broadphase collision detection) and outputs a vector of `sp_data`,
        // which is used in a later step to actually fill the spatial
        // partitioning grid.
        struct spatial_partition
        {
            template <typename TData>
            void process(TData& data)
            {
                // Get a reference to the output vector and clear it.
                auto& o = data.output();
                o.clear();

                data.for_entities([&](auto eid)
                    {
                        // Access component data.
                        const auto& p = data.get(ct::position, eid)._v;
                        const auto& c = data.get(ct::circle, eid)._radius;

                        (void)p;
                        (void)c;
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
                        (void)p0;
                        (void)r0;
                        (void)sp;
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

                            (void)p0;
                            (void)v0;
                            (void)r0;
                            (void)p1;
                            (void)v1;
                            (void)r1;
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
                        auto& radius = data.get(ct::circle, eid)._radius;

                        (void)p0;
                        (void)radius;
                    });
            }
        };

        struct cycle_color
        {
            template <typename TData>
            void process(ft dt, TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        (void)eid;
                        (void)dt;
                    });
            }
        };

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
                        }
                    });
            }
        };
    }

    // Compile-time `std::size_t` entity limit.
    constexpr auto entity_limit = ecst::sz_v<5000>;

    // Compile-time initial particle count.
    constexpr auto entity_count = ecst::sz_v<4000>;

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

            //
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

            // TODO:
            constexpr auto split_evenly_per_core =
                    ips::split_evenly_fn::v_cores()          ; // .
                //ipc::none_below_threshold::v(ecst::sz_v<100>, // .
                  //  );

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
            constexpr auto ssig_render_colored_circle =        // .
                ss::make(st::render_colored_circle)            // .
                    .parallelism(split_evenly_per_core)        // .
                    .dependencies(st::solve_contacts)          // .
                    .read(ct::circle, ct::position, ct::color) // .
                    .output(ss::output<std::vector<int>>);     // .

            //
            constexpr auto ssig_cycle_color =           // .
                ss::make(st::cycle_color)               // .
                    .parallelism(split_evenly_per_core) // .
                    .write(ct::color);                  // .

            //
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

    std::random_device rnd_device;
    std::default_random_engine rnd_gen{rnd_device()};

    auto rndf = [](float min, float max)
    {
        using dist_t = std::uniform_real_distribution<float>;
        return dist_t(min, max)(rnd_gen);
    };

    template <typename TProxy>
    void mk_particle(TProxy& proxy, const float& position, float radius)
    {
        auto eid = proxy.create_entity();

        auto& ca = proxy.add_component(ct::acceleration, eid);
        ca._v = 1;

        auto& cv = proxy.add_component(ct::velocity, eid);
        cv._v = 10;

        auto& cp = proxy.add_component(ct::position, eid);
        cp._v = position;

        auto& cclr = proxy.add_component(ct::color, eid);
        (void)cclr;

        auto& ccs = proxy.add_component(ct::circle, eid);
        ccs._radius = radius;

        auto& ccl = proxy.add_component(ct::life, eid);
        ccl._v = rndf(2, 10);
    }

    template <typename TContext>
    void init_ctx(TContext& ctx)
    {
        ctx.step([&](auto& proxy)
            {
                for(sz_t i = 0; i < entity_count; ++i)
                {
                    mk_particle(proxy, rndf(1, 4), rndf(1, 4));
                }
            });
    }

    template <typename TContext>
    void update_ctx(TContext& ctx, ft dt)
    {
        namespace sea = ::ecst::system_execution_adapter;

        auto ft_tags =
            sea::t(st::acceleration, st::velocity, st::cycle_color, st::life);

        auto nonft_tags = sea::t(st::keep_in_bounds, st::collision,
            st::solve_contacts, st::render_colored_circle);

        ctx.step([dt, &ft_tags, &nonft_tags](auto& proxy)
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
                                // s.clear_cells();
                                (void)s;

                                executor.for_subtasks([&s](auto& data)
                                    {
                                        s.process(data);
                                    });

                                i.for_outputs([](auto& xs, auto& sp_vector)
                                    {
                                        (void)xs;
                                        for(const auto& x : sp_vector)
                                        {
                                            (void)x;
                                        }
                                    });
                            }));

                proxy.for_system_outputs(st::render_colored_circle,
                    [](auto&, auto&)
                    {
                    });
            });
    }

    auto test_impl_f = [](auto& ctx)
    {
        init_ctx(ctx);

        for(int i = 0; i < 100; ++i)
        {
            update_ctx(ctx, 0.5f);
        }

        while(ctx.any_entity_in(st::acceleration))
        {
            update_ctx(ctx, 0.5f);
        }

        for(int i = 0; i < 100; ++i)
        {
            update_ctx(ctx, 0.5f);
        }
    };
}


int main()
{
    using namespace example;
    using namespace example::ecst_setup;

    test::run_tests(test_impl_f, entity_count, make_csl(), make_ssl());
    return 0;
}
