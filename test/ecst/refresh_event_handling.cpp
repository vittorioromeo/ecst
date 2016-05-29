// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include <random>
#include <iostream>
#include <chrono>
#include <ecst.hpp>
#include "./settings_generator.hpp"

namespace example
{
    using vrm::core::uint;
    using vrm::core::sz_t;

    namespace c
    {
        struct c0
        {
            int _v = 0;
        };

        struct c1
        {
            int _v = 0;
        };
    }

    namespace ct
    {
        constexpr auto c0 = ecst::tag::component::v<c::c0>;
        constexpr auto c1 = ecst::tag::component::v<c::c1>;
    }

    namespace s
    {
        struct s0
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        auto& cc0 = data.get(ct::c0, eid)._v;
                        ++cc0;
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
                        auto& cc1 = data.get(ct::c1, eid)._v;
                        ++cc1;
                    });
            }
        };
    }

#define SYS_TAG(x)                                     \
    namespace s                                        \
    {                                                  \
        struct x;                                      \
    }                                                  \
    namespace st                                       \
    {                                                  \
        constexpr auto x = ecst::tag::system::v<s::x>; \
    }

    SYS_TAG(s0)
    SYS_TAG(s1)

    namespace ecst_setup
    {
        constexpr auto make_csl()
        {
            namespace c = example::c;
            namespace slc = ecst::signature_list::component;

            return slc::make(ct::c0, ct::c1);
        }

        constexpr auto entity_count = ecst::sz_v<1000>;

        constexpr auto make_ssl()
        {
            using ecst::sz_v;

            namespace c = example::c;
            namespace s = example::s;
            namespace ss = ecst::signature::system;
            namespace sls = ecst::signature_list::system;
            namespace ips = ecst::inner_parallelism::strategy;

            constexpr auto test_p = // .
                ips::split_every_n::v(sz_v<entity_count / 8>);

            constexpr auto ssig_s0 =     // .
                ss::make(st::s0)         // .
                    .parallelism(test_p) // .
                    .write(ct::c0);      // .

            constexpr auto ssig_s1 =     // .
                ss::make(st::s1)         // .
                    .parallelism(test_p) // .
                    .write(ct::c1);      // .

            return sls::make(ssig_s0, ssig_s1);
        }
    }

    constexpr sz_t cycles = 10000;

    namespace sea = ::ecst::system_execution_adapter;

    std::atomic<sz_t> subs_0{0};
    std::atomic<sz_t> subs_1{0};
    std::atomic<sz_t> unsubs_0{0};
    std::atomic<sz_t> unsubs_1{0};
    std::atomic<sz_t> reclaims{0};

    auto test_impl_f = [](auto& ctx)
    {
        ctx.step(
            [&ctx](auto& proxy)
            {
                for(sz_t ie = 0; ie < example::ecst_setup::entity_count; ++ie)
                {
                    auto e0 = proxy.create_entity();
                    proxy.add_component(ct::c0, e0);

                    auto e1 = proxy.create_entity();
                    proxy.add_component(ct::c1, e1);

                    auto e01 = proxy.create_entity();
                    proxy.add_component(ct::c0, e01);
                    proxy.add_component(ct::c1, e01);
                }
            },
            ecst::refresh_event::on_subscribe(st::s0,
                [](auto&, auto)
                {
                    ++subs_0;
                }),
            ecst::refresh_event::on_subscribe(st::s1, [](auto&, auto)
                {
                    ++subs_1;
                }));

        TEST_ASSERT_OP(
            subs_0.load(), ==, example::ecst_setup::entity_count * 2);
        TEST_ASSERT_OP(
            subs_1.load(), ==, example::ecst_setup::entity_count * 2);

        ctx.step(
            [&ctx](auto& proxy)
            {
                for(sz_t ie = 0; ie < example::ecst_setup::entity_count; ++ie)
                {
                    proxy.kill_entity(ecst::entity_id{ie});
                }
            },
            ecst::refresh_event::on_unsubscribe(st::s0,
                [](auto&, auto)
                {
                    ++unsubs_0;
                }),
            ecst::refresh_event::on_unsubscribe(st::s1,
                [](auto&, auto)
                {
                    ++unsubs_1;
                }),
            ecst::refresh_event::on_reclaim([](auto)
                {
                    ++reclaims;
                }));

        TEST_ASSERT_OP(
            unsubs_0.load(), ==, example::ecst_setup::entity_count * 2);
        TEST_ASSERT_OP(
            unsubs_1.load(), ==, example::ecst_setup::entity_count * 2);
        TEST_ASSERT_OP(
            reclaims.load(), ==, example::ecst_setup::entity_count * 3);
    };
}

int main()
{
    using namespace example;
    using namespace example::ecst_setup;

    test::run_tests(test_impl_f, entity_count, make_csl(), make_ssl());
    return 0;
}
