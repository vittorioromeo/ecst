// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "../../utils/test_utils.hpp"

namespace testbed
{
    using vrm::core::uint;
    using vrm::core::sz_t;

    constexpr sz_t test_entity_count = 10000;
    // constexpr sz_t test_entity_count = 100;

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
    }

    namespace ctags
    {
        constexpr auto position = // .
            ecst::signature::component::tag<component::position>;

        constexpr auto velocity = // .
            ecst::signature::component::tag<component::velocity>;

        constexpr auto acceleration = // .
            ecst::signature::component::tag<component::acceleration>;
    }

    namespace c = testbed::component;
    namespace ct = testbed::ctags;

    namespace action
    {
        void move(c::position& p, const c::velocity& v)
        {
            p._x += v._x;
            p._y += v._y;
        }

        void accelerate(c::velocity& v, const c::acceleration& a)
        {
            v._x += a._x;
            v._y += a._y;
        }
    }


    namespace system
    {
        struct movement
        {
            template <typename TData>
            void process(TData& data)
            {
                data.output() = 0;
                data.for_entities([&](auto eid)
                    {
                        data.output() += 1;

                        auto& p = data.get(ct::position, eid);
                        const auto& v = data.get(ct::velocity, eid);

                        action::move(p, v);
                    });
            }
        };

        struct acceleration
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid)
                    {
                        auto& v = data.get(ct::velocity, eid);
                        const auto& a = data.get(ct::acceleration, eid);
                        action::accelerate(v, a);
                    });
            }
        };
    }

    namespace stags
    {
        constexpr auto movement = // .
            ecst::tag::system::v<system::movement>;

        constexpr auto acceleration = // .
            ecst::tag::system::v<system::acceleration>;
    }

    namespace ecst_setup
    {
        constexpr auto make_csl()
        {
            namespace c = testbed::component;
            namespace slc = ecst::signature_list::component;

            return slc::v<                                // .
                c::position, c::velocity, c::acceleration // .
                >;
        }

        constexpr auto make_ssl()
        {
            namespace c = testbed::component;
            namespace s = testbed::system;
            namespace ss = ecst::signature::system;
            namespace sls = ecst::signature_list::system;

            constexpr auto ssig_acceleration =      // .
                ss::make<s::acceleration>(          // .
                    ss::parallelism::basic_default, // .
                    ss::no_dependencies,            // .
                    ss::component_use(              // .
                        ss::mutate<c::velocity>,    // .
                        ss::read<c::acceleration>   // .
                        ),                          // .
                    ss::output::none                // .
                    );

            constexpr auto ssig_velocity =           // .
                ss::make<s::movement>(               // .
                    ss::parallelism::basic_default,  // .
                    ss::depends_on<s::acceleration>, // .
                    ss::component_use(               // .
                        ss::mutate<c::position>,     // .
                        ss::read<c::velocity>        // .
                        ),                           // .
                    ss::output<int>            // .
                    );

            return sls::make(      // .
                ssig_acceleration, // .
                ssig_velocity      // .
                );
        }

        constexpr auto make_settings()
        {
            namespace cs = ecst::settings;
            namespace ss = ecst::scheduler;

            constexpr auto csl = make_csl();
            constexpr auto ssl = make_ssl();

            constexpr auto settings = cs::make( // .
                cs::multithreaded(              // .
                    cs::allow_inner_parallelism // .
                    ),                          // .
                // cs::fixed<test_entity_count>, // .
                cs::dynamic<test_entity_count>,
                csl,                                // .
                ssl,                                // .
                cs::scheduler<ss::s_atomic_counter> // .
                );

            return settings;
        }
    }

    auto make_ecst_context()
    {
        return ecst::context::make(ecst_setup::make_settings());
    }

    namespace c = testbed::component;
    namespace s = testbed::system;

    using context_type = decltype(make_ecst_context());
}
