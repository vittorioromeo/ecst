// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "./settings_generator.hpp"
#include <chrono>
#include <ecst.hpp>
#include <iostream>
#include <random>

namespace example
{
    using vrm::core::sz_t;
    using vrm::core::uint;

    namespace component
    {
        struct position
        {
            int _x;
            int _y;
        };

        struct velocity
        {
            int _x;
            int _y;
        };

        struct acceleration
        {
            int _x;
            int _y;
        };

        struct counter
        {
            int _v;
        };

        struct countable
        {
        };
    } // namespace component

    namespace c = example::component;

    namespace ct
    {
        using namespace ecst;

        constexpr auto position = tag::component::v<c::position>;
        constexpr auto velocity = tag::component::v<c::velocity>;
        constexpr auto acceleration = tag::component::v<c::acceleration>;
        constexpr auto counter = tag::component::v<c::counter>;
        constexpr auto countable = tag::component::v<c::countable>;
    } // namespace ct


    namespace actions
    {
        void accelerate(c::velocity& v, const c::acceleration& a)
        {
            v._x += a._x;
            v._y += a._y;
        }

        void move(c::position& p, const c::velocity& v)
        {
            p._x += v._x;
            p._y += v._y;
        }

        void count(c::counter& c)
        {
            c._v += 1;
        }
    } // namespace actions

    namespace system
    {
        struct acceleration
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid) {
                    auto& v = data.get(ct::velocity, eid);
                    const auto& a = data.get(ct::acceleration, eid);
                    actions::accelerate(v, a);
                });
            }
        };

        struct velocity
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid) {
                    auto& p = data.get(ct::position, eid);
                    const auto& v = data.get(ct::velocity, eid);
                    actions::move(p, v);
                });
            }
        };

        struct counter
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid) {
                    auto& c = data.get(ct::counter, eid);
                    actions::count(c);
                });
            }
        };
    } // namespace system

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
    SYS_TAG(counter)


    namespace ecst_setup
    {
        constexpr auto make_csl()
        {
            namespace c = example::component;
            namespace sc = ecst::sig::component;
            namespace slc = ecst::sig_list::component;

            return slc::make(                                      // .
                sc::make(ct::position), sc::make(ct::velocity),    // .
                sc::make(ct::acceleration), sc::make(ct::counter), // .
                sc::make(ct::countable)                            // .
            );
        }

        constexpr auto entity_count = ecst::sz_v<1000>;

        constexpr auto make_ssl()
        {
            using ecst::sz_v;

            namespace c = example::component;
            namespace s = example::system;
            namespace ss = ecst::sig::system;
            namespace sls = ecst::sig_list::system;
            namespace ips = ecst::inner_par::strategy;

            constexpr auto test_p = // .
                ips::split_every_n::v(sz_v<entity_count / 8>);

            constexpr auto ssig_acceleration = // .
                ss::make(st::acceleration)     // .
                    .parallelism(test_p)       // .
                    .read(ct::acceleration)    // .
                    .write(ct::velocity);      // .

            constexpr auto ssig_velocity =          // .
                ss::make(st::velocity)              // .
                    .parallelism(test_p)            // .
                    .dependencies(st::acceleration) // .
                    .read(ct::velocity)             // .
                    .write(ct::position);           // .

            constexpr auto ssig_counter = // .
                ss::make(st::counter)     // .
                    .parallelism(test_p)  // .
                    .read(ct::countable)  // .
                    .write(ct::counter);  // .

            return sls::make(      // .
                ssig_counter,      // .
                ssig_acceleration, // .
                ssig_velocity      // .
            );
        }
    } // namespace ecst_setup

    namespace c = example::component;
    namespace s = example::system;

    auto test_impl_f = [](auto& ctx) {
        auto r_e0 = ctx.step([&ctx](auto& proxy) {
            auto e0 = proxy.create_entity();
            TEST_ASSERT(ctx.alive(e0));

            auto& e0_c_counter = proxy.add_component(ct::counter, e0);
            (void)e0_c_counter;
            proxy.add_component(ct::countable, e0);

            TEST_ASSERT(!ctx.is_in_system(st::counter, e0));

            return e0;
        });

        TEST_ASSERT(ctx.alive(r_e0));
        TEST_ASSERT(ctx.is_in_system(st::counter, r_e0));

        ctx.step([&ctx, r_e0](auto& proxy) {
            TEST_ASSERT(ctx.alive(r_e0));
            TEST_ASSERT(ctx.is_in_system(st::counter, r_e0));

            proxy.kill_entity(r_e0);

            TEST_ASSERT(ctx.alive(r_e0));
            TEST_ASSERT(ctx.is_in_system(st::counter, r_e0));
        });

        TEST_ASSERT(ctx.alive(r_e0) == false);
        TEST_ASSERT(!ctx.is_in_system(st::counter, r_e0));
    };
} // namespace example

int main()
{
    using namespace example;
    using namespace example::ecst_setup;

    test::run_tests(test_impl_f, entity_count, make_csl(), make_ssl());
    return 0;
}
