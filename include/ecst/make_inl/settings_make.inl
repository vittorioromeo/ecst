// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/ctx/scheduler/atomic_counter.hpp>

namespace ecst::settings
{
    constexpr auto make()
    {
        using namespace mp;
        using namespace settings::impl;

        constexpr auto d_opts =                          // .
            mp::opt_map::make()                          // .
                .add(keys::threading, v_allow_inner_par) // .
                .add(
                    keys::entity_storage, dynamic_impl<bh::size_t<1000>>{}) // .
                .add(keys::component_sig_list, mp::list::empty_v)           // .
                .add(keys::system_sig_list, mp::list::empty_v)              // .
                .add(keys::scheduler,
                    impl::scheduler_wrapper<scheduler::s_atomic_counter>{}) // .
                .add(keys::refresh_parallelism,
                    impl::refresh_parallelism::enabled{}); // .

        return impl::data<ECST_DECAY_DECLTYPE(d_opts)>{};
    }
} // namespace ecst::settings
