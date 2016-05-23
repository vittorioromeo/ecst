// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./scheduler/atomic_counter.hpp"

// TODO: move
ECST_SETTINGS_NAMESPACE
{
    // TODO: move, inl
    constexpr auto make()
    {
        using namespace mp;
        using namespace settings::impl;

        constexpr auto dopts =                                   // .
            mp::option_map::make()                               // .
                .add(keys::threading, v_allow_inner_parallelism) // .
                .add(keys::entity_storage,
                    dynamic_impl<mp::bh::size_t<1000>>{})               // .
                .add(keys::component_signature_list, mp::list::empty_v) // .
                .add(keys::system_signature_list, mp::list::empty_v)    // .
                .add(keys::scheduler,
                    impl::scheduler_wrapper<
                         ecst::scheduler::s_atomic_counter>{}) // .
                .add(keys::refresh_parallelism,
                    impl::refresh_parallelism::enabled{}); // .

        return impl::data<std::decay_t<decltype(dopts)>>{};
    }
}
ECST_SETTINGS_NAMESPACE_END
