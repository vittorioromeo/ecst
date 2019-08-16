// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../test/ecst/settings_generator.hpp"
#include <ecst.hpp>

namespace example
{
    using ft = float;

    using vrm::core::sz_t;
    using vrm::core::uint;

    namespace component
    {
        struct position
        {
            float _x;
            float _y;
        };

        struct velocity
        {
            float _x;
            float _y;
        };

        struct acceleration
        {
            float _x;
            float _y;
        };

        struct curve
        {
            float _radians;
        };

        struct color
        {
        };

        struct circle_shape
        {
            float _radius;
        };

        struct life
        {
            float _v;
        };
    } // namespace component

    namespace c = example::component;

    namespace ct
    {
        using namespace ecst;

        constexpr auto position = tag::component::v<c::position>;
        constexpr auto velocity = tag::component::v<c::velocity>;
        constexpr auto acceleration = tag::component::v<c::acceleration>;
        constexpr auto curve = tag::component::v<c::curve>;
        constexpr auto color = tag::component::v<c::color>;
        constexpr auto circle_shape = tag::component::v<c::circle_shape>;
        constexpr auto life = tag::component::v<c::life>;
    } // namespace ct

    namespace actions
    {
        void accelerate(ft dt, c::velocity& v, const c::acceleration& a)
        {
            v._x += a._x * dt;
            v._y += a._y * dt;
        }

        void move(ft dt, c::position& p, const c::velocity& v)
        {
            p._x += v._x * dt;
            p._y += v._y * dt;
        }

        void curve(ft dt, c::velocity& v, const c::curve& c)
        {
            (void)dt;
            (void)v;
            (void)c;

            // vec2f vel_vector{v._x, v._y};

            // auto vel_radians = ssvs::getRad(vel_vector);
            // auto vel_magnitude = ssvs::getMag(vel_vector);

            // auto new_radians = vel_radians + c._radians * dt;
            // auto new_vel = ssvs::getVecFromRad(new_radians, vel_magnitude);

            // v._x = new_vel.x;
            // v._y = new_vel.y;
        }

        void hurt(ft dt, c::life& l)
        {
            l._v -= dt;
        }

        void fade(c::color& c, c::circle_shape& cs, const c::life& l)
        {
            (void)c;

            cs._radius = l._v * 0.2f;
        }
    } // namespace actions


#define SYS_TAG(x)                                          \
    namespace system                                        \
    {                                                       \
        struct x;                                           \
    }                                                       \
    namespace st                                            \
    {                                                       \
        constexpr auto x = ecst::tag::system::v<system::x>; \
    }

    SYS_TAG(acceleration)
    SYS_TAG(velocity)
    SYS_TAG(curve)
    SYS_TAG(fade)
    SYS_TAG(life)
    SYS_TAG(render_colored_circle)


    namespace system
    {
        struct curve
        {
            template <typename TData>
            void process(ft dt, TData& data)
            {
                data.for_entities([&](auto eid) {
                    auto& v = data.get(ct::velocity, eid);
                    const auto& c = data.get(ct::curve, eid);
                    actions::curve(dt, v, c);
                });
            }
        };


        struct render_colored_circle
        {
            void prepare()
            {
            }

            template <typename TRenderTarget>
            void render_on(TRenderTarget&)
            {
            }

            template <typename TRenderTarget, typename TData>
            void process(TRenderTarget, TData& data)
            {
                auto& va = data.output();
                va.clear();

                data.for_entities([this, &va, &data](auto eid) {
                    const auto& p = data.get(ct::position, eid);
                    // const auto& c = data.get(ct::color, eid);
                    auto& cs = data.get(ct::circle_shape, eid);

                    // actions::update_circle(cs, p, c);

                    auto mk_triangle = [this, &va, &data, &p, &cs](auto, auto) {
                        // va.emplace_back(1);
                        // va.emplace_back(2);
                        // va.emplace_back(3);

                        // va.emplace_back(p0, color);
                        // va.emplace_back(p1, color);
                        // va.emplace_back(p2, color);
                    };

                    float tau = 6.28f;
                    sz_t precision = 3;
                    float inc = tau / precision;

                    for(sz_t i = 0; i < precision; ++i)
                    {
                        mk_triangle(inc * i, inc * (i + 1));
                    }
                });
            }
        };

        struct acceleration
        {
            template <typename TData>
            void process(ft dt, TData& data)
            {
                (void)dt;


                data.for_entities([&](auto eid) {
                    auto& v = data.get(ct::velocity, eid);
                    const auto& a = data.get(ct::acceleration, eid);
                    actions::accelerate(dt, v, a);
                });
            }
        };


        struct velocity
        {
            template <typename TData>
            void process(ft dt, TData& data)
            {
                data.for_previous_outputs(
                    st::acceleration, [](auto&, auto&&) {});

                data.for_entities([&](auto eid) {
                    auto& p = data.get(ct::position, eid);
                    const auto& v = data.get(ct::velocity, eid);
                    actions::move(dt, p, v);
                });
            }
        };


        struct life
        {
            template <typename TData>
            void process(ft dt, TData& data)
            {
                data.for_entities([&](auto eid) {
                    auto& l = data.get(ct::life, eid);
                    actions::hurt(dt, l);

                    if(l._v <= 0.f)
                    {
                        data.kill_entity(eid);
                    }
                });
            }
        };

        struct fade
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid) {
                    const auto& l = data.get(ct::life, eid);
                    // auto& c = data.get(ct::color, eid);
                    auto& cs = data.get(ct::circle_shape, eid);
                    // actions::fade(c, cs, l);

                    (void)l;
                    // (void)c;
                    (void)cs;
                });
            }
        };
    } // namespace system



    constexpr auto entity_count = ecst::sz_v<1>;

    namespace ecst_setup
    {
        constexpr auto make_csl()
        {
            namespace cs = ecst::sig::component;
            namespace csl = ecst::sig_list::component;

            return csl::make(               // .
                cs::make(ct::position),     // .
                cs::make(ct::velocity),     // .
                cs::make(ct::acceleration), // .
                cs::make(ct::curve),        // .
                cs::make(ct::color),        // .
                cs::make(ct::circle_shape), // .
                cs::make(ct::life)          // .
            );
        }

        constexpr auto make_ssl()
        {
            using ecst::sz_v;

            namespace c = example::component;
            namespace s = example::system;
            namespace ss = ecst::sig::system;
            namespace sls = ecst::sig_list::system;

            namespace ips = ecst::inner_par::strategy;
            namespace ipc = ecst::inner_par::composer;


            constexpr auto test_p2 = ips::none::v();
            constexpr auto test_p =                       // .
                ipc::none_below_threshold::v(sz_v<10000>, // .
                    ips::split_evenly_fn::v_cores()       // .
                );

            (void)test_p2;

            constexpr auto ssig_acceleration = // .
                ss::make(st::acceleration)     // .
                    .parallelism(test_p)       // .
                    .read(ct::acceleration)    // .
                    .write(ct::velocity);      // .

            constexpr auto ssig_curve =             // .
                ss::make(st::curve)                 // .
                    .dependencies(st::acceleration) // .
                    .parallelism(test_p)            // .
                    .read(ct::curve)                // .
                    .write(ct::velocity);           // .

            constexpr auto ssig_velocity =   // .
                ss::make(st::velocity)       // .
                    .parallelism(test_p)     // .
                    .dependencies(st::curve) // .
                    .read(ct::velocity)      // .
                    .write(ct::position);    // .

            constexpr auto ssig_render_colored_circle =    // .
                ss::make(st::render_colored_circle)        // .
                    .parallelism(test_p)                   // .
                    .dependencies(st::velocity)            // .
                    .read(ct::position, ct::color)         // .
                    .write(ct::circle_shape)               // .
                    .output(ss::output<std::vector<int>>); // .

            constexpr auto ssig_life =   // .
                ss::make(st::life)       // .
                    .parallelism(test_p) // .
                    .write(ct::life);    // .

            constexpr auto ssig_fade =                   // .
                ss::make(st::fade)                       // .
                    .parallelism(test_p)                 // .
                    .dependencies(st::life)              // .
                    .read(ct::life)                      // .
                    .write(ct::color, ct::circle_shape); // .

            return sls::make(               // .
                ssig_acceleration,          // .
                ssig_curve,                 // .
                ssig_velocity,              // .
                ssig_render_colored_circle, // .
                ssig_life,                  // .
                ssig_fade                   // .
            );
        }
    } // namespace ecst_setup

    namespace c = example::component;
    namespace s = example::system;

    auto rndf = [](float, float max) { return max; };

    template <typename TContext>
    class game_app
    {
    private:
        TContext& _ctx;

        void init_loops()
        {
            ft dt = 1;

            while(true)
            {
                update_ctx(dt);

                dt = 0.5f;

                if(!_running)
                {
                    break;
                }
            }
        }

        template <typename TProxy>
        void mk_particle(TProxy& proxy)
        {
            auto eid = proxy.create_entity();

            auto& ca = proxy.add_component(ct::acceleration, eid);
            ca._x = rndf(-2, 2);
            ca._y = rndf(-2, 2);

            auto& cv = proxy.add_component(ct::velocity, eid);
            cv._x = rndf(-5, 5);
            cv._y = rndf(-5, 5);

            auto& cp = proxy.add_component(ct::position, eid);
            cp._x = rndf(0, 1024);
            cp._y = rndf(0, 768);

            auto& ccrv = proxy.add_component(ct::curve, eid);
            ccrv._radians = rndf(-5, 5);

            proxy.add_component(ct::color, eid);

            auto& ccs = proxy.add_component(ct::circle_shape, eid);
            (void)ccs;

            auto& cl = proxy.add_component(ct::life, eid);
            static int x = 0;

            cl._v = 10 + x;
            x += 10;
            x = x % 60;
        }

        bool _running = true;

        void update_ctx(ft dt)
        {
            // SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT("execution");
            // SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("execution");

            namespace sea = ::ecst::sys_exec;

            _ctx.step([this, dt](auto& proxy) {
                proxy.system(st::render_colored_circle).prepare();

                proxy.execute_systems_from(st::acceleration, st::life)( // .
                    sea::t(st::acceleration, st::velocity, st::curve, st::life)
                        .for_subtasks(
                            [dt](auto& s, auto& data) { s.process(dt, data); }),
                    sea::t(st::fade).for_subtasks(
                        [](auto& s, auto& data) { s.process(data); }),
                    sea::t(st::render_colored_circle)
                        .for_subtasks([this, dt](auto& s, auto& data) {
                            s.process(0, data);
                        }));

                if(!_ctx.any_entity_in(st::life))
                {
                    _running = false;
                }
            });
        }

        void init()
        {
            _ctx.step([this](auto& proxy) {
                for(sz_t i = 0; i < entity_count; ++i)
                {
                    this->mk_particle(proxy);
                }
            });

            init_loops();
        }

    public:
        game_app(TContext& ctx) : _ctx{ctx}
        {
            init();
        }
    };
} // namespace example

int main()
{
    using namespace example;
    using namespace example::ecst_setup;

    auto test_impl = [&](auto& ctx) {
        using ct = ECST_DECAY_DECLTYPE(ctx);
        game_app<ct> a{ctx};
        (void)a;
    };

    test::run_tests(test_impl, entity_count, make_csl(), make_ssl());

    return 0;
}
