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

    namespace c
    {
    }

    namespace ct
    {
    }

    namespace s
    {
        struct s0;
        struct s1;
        struct s2;
        struct s3;
    }

    namespace st
    {
        namespace sct = ecst::signature::system;

        constexpr auto s0 = ecst::tag::system::v<s::s0>;
        constexpr auto s1 = ecst::tag::system::v<s::s1>;
        constexpr auto s2 = ecst::tag::system::v<s::s2>;
        constexpr auto s3 = ecst::tag::system::v<s::s3>;
    }

    namespace s
    {
        struct s0
        {
        };

        struct s1
        {
        };

        struct s2
        {
        };

        struct s3
        {
        };
    }

    constexpr auto entity_count = ecst::sz_v<200>;
    constexpr auto split_count = ecst::sz_v<4>;

    namespace ecst_setup
    {
        constexpr auto make_csl()
        {
            namespace sc = ecst::signature::component;
            namespace slc = ecst::signature_list::component;

            return slc::make();
        }

        constexpr auto make_ssl()
        {
            using ecst::sz_v;

            namespace ss = ecst::signature::system;
            namespace sls = ecst::signature_list::system;

            namespace ips = ecst::inner_parallelism::strategy;
            namespace ipc = ecst::inner_parallelism::composer;

            constexpr auto ip_none = ips::none::v();
            constexpr auto ip_split = ips::split_evenly::v(split_count);

            constexpr auto ss_s0 =         // .
                ss::make(st::s0)           // .
                    .parallelism(ip_none); // .

            constexpr auto ss_s1 =          // .
                ss::make(st::s1)            // .
                    .dependencies(st::s0)   // .
                    .parallelism(ip_split); // .

            constexpr auto ss_s2 =          // .
                ss::make(st::s2)            // .
                    .dependencies(st::s0)   // .
                    .parallelism(ip_split); // .

            constexpr auto ss_s3 =                // .
                ss::make(st::s3)                  // .
                    .dependencies(st::s1, st::s2) // .
                    .parallelism(ip_none);        // .

            return sls::make(ss_s0, ss_s1, ss_s2, ss_s3);

            /*
                   1a
                   1b
                   1c
                   1d
                 /    \
                0      3
                 \    /
                   2a
                   2b
                   2c
                   2d
            */
        }
    }
}

int main()
{
    using namespace example;
    using namespace example::ecst_setup;

    constexpr auto times = 50;

    auto test_impl = [](auto& ctx)
    {
        using ct = ECST_DECAY_DECLTYPE(ctx);
        using ct_handle = typename ct::handle;

        enum class outer_state
        {
            none,
            begin,
            done
        };

        auto os_s0(outer_state::none);
        auto os_s1(outer_state::none);
        auto os_s2(outer_state::none);
        auto os_s3(outer_state::none);

        std::atomic<int> c12{0};

        namespace sea = ecst::system_execution_adapter;

        ctx.step([&](auto& proxy)
            {
                for(auto i = 0; i < 100; ++i)
                {
                    proxy.create_entity();
                }
            });

        ctx.step([&](auto& proxy)
            {
                proxy.execute_systems()( // .
                                         // .
                    sea::t(st::s0).detailed_instance([&](auto&, auto& ep)
                        {
                            TEST_ASSERT_NS(os_s0 == outer_state::none);
                            os_s0 = outer_state::begin;

                            ep.for_subtasks([&](auto&)
                                {
                                    TEST_ASSERT_NS(os_s0 == outer_state::begin);
                                });

                            os_s0 = outer_state::done;
                            TEST_ASSERT_NS(os_s0 == outer_state::done);
                        }), // .
                            // .
                    sea::t(st::s1).detailed_instance([&](auto&, auto& ep)
                        {
                            TEST_ASSERT_NS(os_s0 == outer_state::done);

                            TEST_ASSERT_NS(os_s1 == outer_state::none);
                            os_s1 = outer_state::begin;

                            std::atomic<int> c{0};
                            ep.for_subtasks([&](auto&)
                                {
                                    TEST_ASSERT_NS(os_s1 == outer_state::begin);
                                    ++c;
                                    ++c12;
                                });

                            if(ctx.inner_parallelism_allowed())
                            {
                                TEST_ASSERT_NS(c == split_count);
                            }
                            else
                            {
                                TEST_ASSERT_NS(c == 1);
                            }

                            os_s1 = outer_state::done;
                            TEST_ASSERT_NS(os_s1 == outer_state::done);
                        }), // .
                            // .
                    sea::t(st::s2).detailed_instance([&](auto&, auto& ep)
                        {
                            TEST_ASSERT_NS(os_s0 == outer_state::done);

                            TEST_ASSERT_NS(os_s2 == outer_state::none);
                            os_s2 = outer_state::begin;

                            std::atomic<int> c{0};
                            ep.for_subtasks([&](auto&)
                                {
                                    TEST_ASSERT_NS(os_s2 == outer_state::begin);
                                    ++c;
                                    ++c12;
                                });

                            if(ctx.inner_parallelism_allowed())
                            {
                                TEST_ASSERT_NS(c == split_count);
                            }
                            else
                            {
                                TEST_ASSERT_NS(c == 1);
                            }

                            os_s2 = outer_state::done;
                            TEST_ASSERT_NS(os_s2 == outer_state::done);
                        }), // .
                            // .
                    sea::t(st::s3).detailed_instance([&](auto&, auto& ep)
                        {
                            TEST_ASSERT_NS(os_s0 == outer_state::done);
                            TEST_ASSERT_NS(os_s1 == outer_state::done);
                            TEST_ASSERT_NS(os_s2 == outer_state::done);

                            if(ctx.inner_parallelism_allowed())
                            {
                                TEST_ASSERT_NS(c12 == split_count * 2);
                            }
                            else
                            {
                                TEST_ASSERT_NS(c12 == 2);
                            }

                            TEST_ASSERT_NS(os_s3 == outer_state::none);
                            os_s3 = outer_state::begin;

                            ep.for_subtasks([&](auto&)
                                {
                                    TEST_ASSERT_NS(os_s3 == outer_state::begin);
                                });

                            os_s3 = outer_state::done;
                            TEST_ASSERT_NS(os_s3 == outer_state::done);
                        }) // .
                    );
            });
    };

    for(auto i = 0; i < times; ++i)
    {
        test::run_tests(test_impl, entity_count, make_csl(), make_ssl(), true);
    }

    return 0;
}
