// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <random>
#include <iostream>
#include <chrono>
#include <ecst/mp.hpp>
#include <ecst/settings.hpp>
#include <ecst/context.hpp>
#include "../utils/test_utils.hpp"

namespace test
{
    namespace impl
    {
        template <typename TEntityCount, typename TCSL, typename TSSL>
        auto make_settings_list(TEntityCount ec, TCSL csl, TSSL ssl)
        {
            namespace cs = ecst::settings;
            namespace ss = ecst::scheduler;
            namespace mp = ecst::mp;
            namespace bh = ecst::bh;

            (void)csl;
            (void)ssl;

            // List of threading policies.
            constexpr auto l_threading = mp::list::make( // .
                ecst::settings::impl::v_allow_inner_parallelism,
                ecst::settings::impl::v_disallow_inner_parallelism);

            // List of storage policies.
            constexpr auto l_storage = mp::list::make( // .
                ecst::settings::fixed<decltype(ec){}>,
                ecst::settings::dynamic<500>);

            (void)l_threading;
            (void)l_storage;

            return bh::fold_right(l_threading, mp::list::empty_v,
                [=](auto x_threading, auto xacc)
                {
                    auto fold2 = bh::fold_right(l_storage, mp::list::empty_v,
                        [=](auto y_storage, auto yacc)
                        {
                            auto zsettings =                    // .
                                cs::make()                      // .
                                    .set_threading(x_threading) // .
                                    .set_storage(y_storage)     // .
                                    .component_signatures(csl)  // .
                                    .system_signatures(ssl)     // .
                                    .scheduler(
                                        cs::scheduler<ss::s_atomic_counter>);


                            return bh::append(yacc, zsettings);
                        });

                    return bh::concat(xacc, fold2);
                });
        }

        template <typename TSettings>
        auto make_ecst_context(TSettings)
        {
            return ecst::context::make(TSettings{});
        }

        template <typename TSettings, typename TF>
        void do_test(TSettings, TF&& f)
        {
            // Create context.
            using context_type = decltype(make_ecst_context(TSettings{}));
            auto ctx_uptr = std::make_unique<context_type>();
            auto& ctx = *ctx_uptr;

            f(ctx);
        }
    }

    template <typename TF, typename TEntityCount, typename TCSL, typename TSSL>
    void run_tests(
        TF&& f, TEntityCount ec, TCSL csl, TSSL ssl, bool silent = false)
    {
        using vrm::core::sz_t;
        constexpr sz_t times = 2;

        using hr = std::chrono::high_resolution_clock;
        using d_type = std::chrono::duration<float, std::milli>;


        for(sz_t t = 0; t < times; ++t)
        {
            ecst::bh::for_each(impl::make_settings_list(ec, csl, ssl),
                [f, &silent](auto s)
                {
                    if(!silent)
                    {
                        std::cout
                            << ecst::settings::str::entity_storage<decltype(
                                   s)>()
                            << "\n"
                            << ecst::settings::str::multithreading<decltype(
                                   s)>()
                            << "\n";
                    }

                    auto last(hr::now());
                    impl::do_test(s, f);

                    auto time(hr::now() - last);
                    auto timeMs(
                        std::chrono::duration_cast<d_type>(time).count());

                    if(!silent)
                    {
                        std::cout << "time: " << timeMs << "ms\n\n";
                    }
                });
        }
    }
}
