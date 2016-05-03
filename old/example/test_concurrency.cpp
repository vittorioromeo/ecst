// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "./boilerplate.hpp"

#include <random>
#include <iostream>
#include <chrono>
#include <ecst.hpp>
#include "../test/ecst/settings_generator.hpp"
#include "./utils/trig.hpp"

// #define ENABLE_RENDERING 1
// #define ENABLE_RANDOMNESS 1

namespace example
{
    using ft = float;

    using vrm::core::uint;
    using vrm::core::sz_t;

    using sfc = sf::Color;
    using vec2f = sf::Vector2f;
    using vec2i = sf::Vector2i;
    using vec2u = sf::Vector2u;

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
            sfc _color;
        };

        struct circle_shape
        {
            float _radius;
        };

        struct life
        {
            float _v;
        };
    }

    namespace c = example::component;

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
            c._color.a = std::min(255.f, l._v * 4.5f);
        }
    }

    namespace system
    {


        struct curve
        {
            static constexpr const char* name()
            {
                return "curve";
            }

            template <typename TData>
            void process(ft dt, TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        auto& v = ecst::get<c::velocity>(data, eid);
                        const auto& c = ecst::get<c::curve>(data, eid);
                        actions::curve(dt, v, c);

                        std::cout << "start\n";
                        std::cout
                            << "thread ID: " << ecst::this_thread::get_id()
                            << "\n";

                        ecst::this_thread::sleep_for(
                            std::chrono::milliseconds(2000));

                        std::cout << "end\n";
                    });
            }
        };


        struct render_colored_circle
        {
            static constexpr const char* name()
            {
                return "render_colored_circle";
            }

            sf::VertexArray va{sf::PrimitiveType::Triangles};

            void prepare()
            {
                va.clear();
            }

            template <typename TRenderTarget>
            void render_on(TRenderTarget& rt)
            {
                rt.setActive(true);
                rt.draw(va);
                rt.setActive(false);
            }

            template <typename TRenderTarget, typename TData>
            void process(TRenderTarget&, TData& data)
            {
                data.for_entities([this, &data](auto eid)
                    {
                        (void)eid;

                        const auto& p = ecst::get<c::position>(data, eid);
                        const auto& c = ecst::get<c::color>(data, eid);
                        auto& cs = ecst::get<c::circle_shape>(data, eid);

                        // actions::update_circle(cs, p, c);

                        auto mk_triangle = [this, &data, &p, &c, &cs](
                            auto angle_begin, auto angle_end)
                        {
                            const auto& center = vec2f(p._x, p._y);
                            const auto& radius = cs._radius;
                            const auto& color = c._color;

                            auto p0 = center;
                            auto p1 =
                                vec2f(radius * tbl_cos(angle_begin) + p0.x,
                                    radius * tbl_sin(angle_begin) + p0.y);
                            auto p2 = vec2f(radius * tbl_cos(angle_end) + p0.x,
                                radius * tbl_sin(angle_end) + p0.y);

                            va.append(sf::Vertex(p0, color));
                            va.append(sf::Vertex(p1, color));
                            va.append(sf::Vertex(p2, color));
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

        class acceleration
        {
        public:
            static constexpr const char* name()
            {
                return "acceleration";
            }

            template <typename TData>
            void process(ft dt, TData& data)
            {
                (void)dt;


                data.for_entities([&](auto eid)
                    {
                        auto& v = ecst::get<c::velocity>(data, eid);
                        const auto& a = ecst::get<c::acceleration>(data, eid);
                        actions::accelerate(dt, v, a);
                    });
            }
        };


        struct velocity
        {
            static constexpr const char* name()
            {
                return "velocity";
            }

            template <typename TData>
            void process(ft dt, TData& data)
            {
                data.template for_previous_outputs<acceleration>(
                    [](auto&, auto&&)
                    {
                    });

                data.for_entities([&](auto eid)
                    {
                        auto& p = ecst::get<c::position>(data, eid);
                        const auto& v = ecst::get<c::velocity>(data, eid);
                        actions::move(dt, p, v);
                    });
            }
        };


        struct life
        {
            static constexpr const char* name()
            {
                return "life";
            }

            template <typename TData>
            void process(ft dt, TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        auto& l = ecst::get<c::life>(data, eid);
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
            static constexpr const char* name()
            {
                return "fade";
            }

            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        const auto& l = ecst::get<c::life>(data, eid);
                        auto& c = ecst::get<c::color>(data, eid);
                        auto& cs = ecst::get<c::circle_shape>(data, eid);
                        actions::fade(c, cs, l);
                    });
            }
        };
    }

    constexpr auto entity_count = ecst::sz_v<200000>;

    namespace ecst_setup
    {
        constexpr auto make_csl()
        {
            namespace c = example::component;
            namespace slc = ecst::signature_list::component;

            return slc::v<                                 // .
                c::position, c::velocity, c::acceleration, // .
                c::curve, c::color,                        // .
                c::circle_shape, c::life                   // .
                >;
        }

        constexpr auto make_ssl()
        {
            using ecst::sz_v;

            namespace c = example::component;
            namespace s = example::system;
            namespace ss = ecst::signature::system;
            namespace sls = ecst::signature_list::system;

            namespace ips = ecst::inner_parallelism::strategy;
            namespace ipc = ecst::inner_parallelism::composer;

            constexpr auto test_p =                       // .
                ipc::none_below_threshold::v(sz_v<10000>, // .
                    ips::split_evenly_fn::v_cores()       // .
                    );

            constexpr auto none = ips::none::v();

            constexpr auto ssig_acceleration =    // .
                ss::make<s::acceleration>(        // .
                    test_p,                       // .
                    ss::no_dependencies,          // .
                    ss::component_use(            // .
                        ss::mutate<c::velocity>,  // .
                        ss::read<c::acceleration> // .
                        ),                        // .
                    ss::output::none              // .
                    );

            constexpr auto ssig_curve =              // .
                ss::make<s::curve>(                  // .
                    test_p,                          // .
                    ss::depends_on<s::acceleration>, // .
                    ss::component_use(               // .
                        ss::mutate<c::velocity>,     // .
                        ss::read<c::curve>           // .
                        ),                           // .
                    ss::output::none                 // .
                    );

            constexpr auto ssig_velocity =       // .
                ss::make<s::velocity>(           // .
                    test_p,                      // .
                    ss::depends_on<s::curve>,    // .
                    ss::component_use(           // .
                        ss::mutate<c::position>, // .
                        ss::read<c::velocity>    // .
                        ),                       // .
                    ss::output::none             // .
                    );

            constexpr auto ssig_render_colored_circle = // .
                ss::make<s::render_colored_circle>(     // .
                    none,                               // .
                    ss::depends_on<s::velocity>,        // .
                    ss::component_use(                  // .
                        ss::mutate<c::circle_shape>,    // .
                        ss::read<c::position>,          // .
                        ss::read<c::color>              // .
                        ),                              // .
                    ss::output::none                    // .
                    );

            constexpr auto ssig_life =      // .
                ss::make<s::life>(          // .
                    test_p,                 // .
                    ss::no_dependencies,    // .
                    ss::component_use(      // .
                        ss::mutate<c::life> // .
                        ),                  // .
                    ss::output::none        // .
                    );

            constexpr auto ssig_fade =               // .
                ss::make<s::fade>(                   // .
                    test_p,                          // .
                    ss::depends_on<s::life>,         // .
                    ss::component_use(               // .
                        ss::mutate<c::color>,        // .
                        ss::mutate<c::circle_shape>, // .
                        ss::read<c::life>            // .
                        ),                           // .
                    ss::output::none                 // .
                    );


            return sls::make(               // .
                ssig_acceleration,          // .
                ssig_curve,                 // .
                ssig_velocity,              // .
                ssig_render_colored_circle, // .
                ssig_life,                  // .
                ssig_fade                   // .
                );
        }
    }

    namespace c = example::component;
    namespace s = example::system;

    std::random_device rnd_device;
    std::default_random_engine rnd_gen{rnd_device()};

    auto rndf = [](float min, float max)
    {
#if defined(ENABLE_RANDOMNESS)
        using dist_t = std::uniform_real_distribution<float>;
        return dist_t(min, max)(rnd_gen);
#else
        (void)min;
        return max;
#endif
    };

    template <typename TContext>
    class game_app : public boilerplate::app
    {
    private:
        using this_type = game_app;

        TContext& _ctx;

        void init_loops()
        {
            std::chrono::high_resolution_clock hrc;
            using ft_dur = std::chrono::duration<ft, std::ratio<1, 1000>>;

            ft dt = 1;

            while(true)
            {
                auto cb = hrc.now();
                (void)cb;

                window().setActive(true);

                sf::Event e;
                while(window().pollEvent(e))
                {
                }

                window().clear();

                window().setActive(false);

                update_ctx(dt);

                window().setActive(true);
                window().display();

                // auto ce = hrc.now();
                // dt = std::chrono::duration_cast<ft_dur>(ce - cb).count();
                // std::cout << dt << "\n";

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

            auto& ca = proxy.template add_component<c::acceleration>(eid);
            ca._x = rndf(-2, 2);
            ca._y = rndf(-2, 2);

            auto& cv = proxy.template add_component<c::velocity>(eid);
            cv._x = rndf(-5, 5);
            cv._y = rndf(-5, 5);

            auto& cp = proxy.template add_component<c::position>(eid);
            cp._x = rndf(0, 1024);
            cp._y = rndf(0, 768);

            auto& ccrv = proxy.template add_component<c::curve>(eid);
            ccrv._radians = rndf(-5, 5);

            auto& cclr = proxy.template add_component<c::color>(eid);
            cclr._color = sfc(rndf(0, 255), rndf(0, 255), rndf(0, 255), 255);

            auto& ccs = proxy.template add_component<c::circle_shape>(eid);
            (void)ccs;

            auto& cl = proxy.template add_component<c::life>(eid);
            cl._v = rndf(15, 60);
        }

        bool _running = true;

        void update_ctx(ft dt)
        {
            // SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT("execution");
            // SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("execution");

            _ctx.step([this, dt](auto& proxy)
                {
#if defined(ENABLE_RENDERING)
                    proxy.template system<s::render_colored_circle>().prepare();
#endif
                    proxy.execute_systems_overload( // .
                        [dt](s::acceleration& s, auto& data)
                        {
                            // s.process(dt, data);
                        },
                        [dt](s::velocity& s, auto& data)
                        {
                            // s.process(dt, data);
                        },
                        [dt](s::curve& s, auto& data)
                        {
                            s.process(dt, data);
                        },
                        [dt](s::life& s, auto& data)
                        {
                            // s.process(dt, data);
                        },
                        [](s::fade& s, auto& data)
                        {
                            // s.process(data);
                        },
                        [this, dt](s::render_colored_circle& s, auto& data)
                        {
                            (void)s;
                            (void)data;
#if defined(ENABLE_RENDERING)
                            s.process(this->window(), data);
#endif
                        });

#if defined(ENABLE_RENDERING)
                    proxy.template system<s::render_colored_circle>().render_on(
                        this->window());
#endif

                    if(!_ctx.template any_entity_in<s::life>())
                    {
                        _running = false;
                        this->stop();
                    }
                });
        }

        void init()
        {
            _ctx.step([this](auto& proxy)
                {
                    for(sz_t i = 0; i < entity_count; ++i)
                    {
                        this->mk_particle(proxy);
                    }
                });

            init_loops();
        }

    public:
        game_app(sf::RenderWindow& window, TContext& ctx) noexcept
            : boilerplate::app{window},
              _ctx{ctx}
        {
            init();
        }
    };
}

int main()
{
    using namespace example;
    using namespace example::ecst_setup;

    auto test_impl = [&](auto& ctx)
    {
        using ct = std::remove_reference_t<decltype(ctx)>;

        boilerplate::app_runner<game_app<ct>> x{
            "Particle ECST test", 1024, 768, ctx};

        x.run();
    };

    test::run_tests(test_impl, entity_count, make_csl(), make_ssl());

    return 0;
}
