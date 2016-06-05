// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include <random>
#include <iostream>
#include <map>
#include <chrono>
#include <ecst.hpp>
#include "../utils/test_utils.hpp"
#include "./settings_generator.hpp"

namespace example
{
    using ft = float;

    using vrm::core::uint;
    using vrm::core::sz_t;

    namespace component
    {
        struct c0
        {
            int _v;
        };

        struct c1
        {
            int _v;
        };
    }

    namespace c = example::component;

    namespace ct
    {
        namespace sct = ecst::signature::component;

        constexpr auto c0 = ecst::tag::component::v<c::c0>;
        constexpr auto c1 = ecst::tag::component::v<c::c1>;
    }

    namespace system
    {
        struct s0;
        struct s1;
        struct s01;
    }

    namespace st
    {
        namespace sct = ecst::signature::system;

        constexpr auto s0 = ecst::tag::system::v<system::s0>;
        constexpr auto s1 = ecst::tag::system::v<system::s1>;
        constexpr auto s01 = ecst::tag::system::v<system::s01>;
    }

    namespace system
    {
        struct s0
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        auto& cc0 = data.get(ct::c0, eid);
                        ++cc0._v;
                    });
            }
        };

        struct s1
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        auto& cc1 = data.get(ct::c1, eid);
                        ++cc1._v;
                    });
            }
        };

        struct s01
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        auto& cc0 = data.get(ct::c0, eid);
                        auto& cc1 = data.get(ct::c1, eid);

                        (void)cc0;
                        (void)cc1;
                    });
            }
        };
    }

    constexpr auto entity_count = ecst::sz_v<200>;

    namespace ecst_setup
    {
        constexpr auto make_csl()
        {
            namespace c = example::component;
            namespace sc = ecst::signature::component;
            namespace slc = ecst::signature_list::component;

            return slc::make(sc::make(ct::c0), sc::make(ct::c1));
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

            constexpr auto test_p =                     // .
                ipc::none_below_threshold::v(sz_v<100>, // .
                    ips::split_evenly::v(sz_v<8>)       // .
                    );

            constexpr auto ssig_s0 =     // .
                ss::make(st::s0)         // .
                    .parallelism(test_p) // .
                    .write(ct::c0);      // .

            constexpr auto ssig_s1 =     // .
                ss::make(st::s1)         // .
                    .parallelism(test_p) // .
                    .write(ct::c1);      // .

            constexpr auto ssig_s01 =       // .
                ss::make(st::s01)           // .
                    .parallelism(test_p)    // .
                    .write(ct::c0, ct::c1); // .

            return sls::make(ssig_s0, ssig_s1, ssig_s01);
        }
    }

    namespace c = example::component;
    namespace s = example::system;

    std::random_device rnd_device;
    std::default_random_engine rnd_gen{rnd_device()};

    auto rndi = [](int min, int max)
    {
        using dist_t = std::uniform_int_distribution<int>;
        return dist_t(min, max)(rnd_gen);
    };

    template <typename TContext>
    class game_app
    {
    private:
        TContext& _ctx;

        void init_loops()
        {
            while(_running)
            {
                update_ctx();
            }
        }

        using handle = typename TContext::handle;
        std::map<int, handle> _hs;
        std::vector<handle> _hs_killed;

        template <typename TProxy>
        auto mk_particle(TProxy& proxy)
        {
            auto eh = proxy.create_entity_and_handle();
            TEST_ASSERT(proxy.valid_handle(eh));

            auto& cc0 = proxy.add_component(ct::c0, proxy.access(eh));
            cc0._v = rndi(0, 10);

            auto& cc1 = proxy.add_component(ct::c1, proxy.access(eh));
            cc1._v = rndi(0, 10);

            TEST_ASSERT(proxy.valid_handle(eh));

            _hs[proxy.access(eh)] = eh;

            return eh;
        }

        bool _running = true;
        int times = 2;

        void update_ctx()
        {
            for(const auto& h : _hs_killed)
            {
                TEST_ASSERT(!_ctx.valid_handle(h));
            }

            namespace sea = ::ecst::system_execution_adapter;

            _ctx.step([this](auto& proxy)
                {
                    int to_kill;
                    handle h;

                    if(!_ctx.any_entity_in(st::s0))
                    {
                        std::cout << "finished\n";

                        --times;

                        if(times == 0)
                        {
                            _running = false;
                        }
                        else
                        {
                            this->refill(proxy);
                        }
                    }
                    else
                    {
                        do
                        {
                            to_kill = rndi(0, entity_count);
                            h = _hs[to_kill];
                        } while(!proxy.valid_handle(h));

                        proxy.kill_entity(proxy.access(h));
                        _hs_killed.emplace_back(h);
                        proxy.execute_systems_from(
                            st::s0, st::s1, st::s01)( // .
                            sea::all().for_subtasks([](auto& s, auto& data)
                                {
                                    s.process(data);
                                }));
                    }
                });
        }

        template <typename TProxy>
        void refill(TProxy& proxy)
        {
            _hs.clear();
            _hs_killed.clear();

            for(sz_t i = 0; i < entity_count; ++i)
            {
                auto eh = this->mk_particle(proxy);
                TEST_ASSERT(_ctx.valid_handle(eh));
            }

            for(const auto& hkv : _hs)
            {
                TEST_ASSERT(_ctx.valid_handle(hkv.second));
            }
        }

        void init()
        {

            _ctx.step([this](auto& proxy)
                {
                    this->refill(proxy);
                });

            init_loops();
        }

    public:
        game_app(TContext& ctx) : _ctx{ctx}
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
        using ct = ECST_DECAY_DECLTYPE(ctx);
        game_app<ct> a{ctx};
        (void)a;
    };

    test::run_tests(test_impl, entity_count, make_csl(), make_ssl());

    return 0;
}
