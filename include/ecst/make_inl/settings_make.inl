// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/context/scheduler/atomic_counter.hpp>

ECST_SETTINGS_NAMESPACE
{
    constexpr auto make()
    {
        using namespace mp;
        using namespace settings::impl;

        constexpr auto d_opts =                                  // .
            mp::option_map::make()                               // .
                .add(keys::threading, v_allow_inner_parallelism) // .
                .add(
                    keys::entity_storage, dynamic_impl<bh::size_t<1000>>{}) // .
                .add(keys::component_signature_list, mp::list::empty_v)     // .
                .add(keys::system_signature_list, mp::list::empty_v)        // .
                .add(keys::scheduler,
                    impl::scheduler_wrapper<
                         ecst::scheduler::s_atomic_counter>{}) // .
                .add(keys::refresh_parallelism,
                    impl::refresh_parallelism::enabled{}); // .

        return impl::data<std::decay_t<decltype(d_opts)>>{};
    }
}
ECST_SETTINGS_NAMESPACE_END
