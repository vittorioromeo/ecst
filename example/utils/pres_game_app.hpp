// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./dependencies.hpp"

namespace example
{
    template <typename TProxy>
    void mk_particle(TProxy& proxy, const vec2f& position, float radius);

    template <typename TContext>
    class game_app : public boilerplate::app
    {
    private:
        using this_type = game_app;
        TContext& _ctx;

        float _delay{0.f};
        bool _draw_grid{false}, _kill_pls{false};

        void init_loops()
        {
            std::chrono::high_resolution_clock hrc;
            using ft_dur = std::chrono::duration<ft, std::ratio<1, 1000>>;

            ft dt = 0.04f;

            while(true)
            {
                auto focus = window().hasFocus();
                auto cb = hrc.now();


                sf::Event e;
                while(window().pollEvent(e))
                {
                }

                window().clear();

                if(focus &&
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                {
                    /*
                    _ctx.step([](auto& proxy)
                        {
                            proxy.instance(st::acceleration)
                                .subscribed()
                                .for_each([&](auto eid)
                                    {
                                        auto& v =
                                            proxy.get_component(ct::velocity,
                                                     ecst::entity_id(eid))
                                                ._v;

                                        v.x = rndf(-25, 25);
                                        v.y = rndf(-100, -25);
                                    });
                        });
                        */
                }


                if(focus &&
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                {
                    _running = false;
                }

                if(_delay <= 0 && focus &&
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
                {
                    _delay = 0.4f;
                    _draw_grid = !_draw_grid;
                }

                auto mposi = sf::Mouse::getPosition(window());
                auto ws = window().getSize();

                if(mposi.x < (int)0) mposi.x = 0;
                if(mposi.x > (int)ws.x) mposi.x = ws.x;

                if(mposi.y < (int)0) mposi.y = 0;
                if(mposi.y > (int)ws.y) mposi.y = ws.y;

                vec2f mpos = window().mapPixelToCoords(mposi);


                if(_delay <= 0 && focus &&
                    sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    _delay = 0.06f;
                    _ctx.step([&](auto& proxy) {
                        mk_particle(proxy, mpos, rndf(1, 4));
                    });
                }

                _kill_pls = false;

                if(_delay <= 0 && focus &&
                    sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))

                {
                    _delay = 0.1f;
                    _kill_pls = true;
                }

                std::vector<std::unique_ptr<sf::Drawable>> debug_drawables;

                _ctx.step([&](auto& proxy) {
                    auto& sp = proxy.system(st::spatial_partition);
                    auto& cell = sp.cell_by_pos(mpos);
                    auto cs = s::spatial_partition::cell_size;

                    if(_kill_pls)
                    {
                        for(auto eid : cell)
                        {
                            proxy.kill_entity(eid);
                        }
                    }

                    if(_draw_grid)
                    {
                        auto rsp = std::make_unique<sf::RectangleShape>();
                        auto& rs = *rsp;
                        rs.setFillColor(sf::Color(255, 0, 0, 100));
                        rs.setOrigin(0, 0);
                        rs.setSize(vec2f(cs, cs));

                        auto x = std::floor(mpos.x / cs) * cs;
                        auto y = std::floor(mpos.y / cs) * cs;
                        rs.setPosition(x, y);

                        debug_drawables.emplace_back(std::move(rsp));

                        for(auto eid : cell)
                        {
                            auto rsp2 = std::make_unique<sf::RectangleShape>();
                            auto& rs2 = *rsp2;

                            const auto& p =
                                proxy.get_component(ct::position, eid)._v;

                            const auto& r =
                                proxy.get_component(ct::circle, eid)._radius;

                            rs2.setFillColor(sf::Color(0, 255, 255, 130));
                            rs2.setOrigin(r, r);
                            rs2.setSize(vec2f(r * 2, r * 2));
                            rs2.setPosition(p.x, p.y);
                            debug_drawables.emplace_back(std::move(rsp2));
                        }
                    }
                });

                if(_delay > 0)
                {
                    _delay -= dt;
                }


                /*for(int i = 0; i < 2; ++i)
                {
                    update_ctx(dt, false);
                }*/

                update_ctx(_ctx, this->window(), dt);

                for(auto& dd : debug_drawables)
                {
                    window().draw(*dd);
                }

                window().display();
                auto ce = hrc.now();
                auto real_dt =
                    std::chrono::duration_cast<ft_dur>(ce - cb).count();

                // TODO:
                auto fps = 1.f / real_dt * 1000.f;

                window().setTitle(std::string{"DT: "} +
                                  std::to_string(real_dt) +
                                  "  |  FPS: " + std::to_string(fps));



                if(!_running)
                {
                    break;
                }
            }
        }

        bool _running = true;

        void init()
        {

            init_ctx(_ctx);
            init_loops();
        }

    public:
        game_app(sf::RenderWindow& window, TContext& ctx) noexcept
            : boilerplate::app{window}, _ctx{ctx}
        {
            init();
        }
    };

    template <typename TContext>
    void run_simulation(TContext& ctx)
    {
        boilerplate::app_runner<game_app<ECST_DECAY_DECLTYPE(ctx)>> x{
            "Particle ECST test", 1440, 900, ctx};

        x.run();
    }
} // namespace example
