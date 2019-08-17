// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/inner_par.hpp>

namespace ecst::sig::system
{
    template <typename TSystemTag>
    constexpr auto make(TSystemTag)
    {
        using namespace mp;
        using namespace sig::system::impl;
        namespace ips = inner_par::strategy;

        // By default, a system has the following characteristics:
        // * Single subtask.
        // * Without dependencies.
        // * Reads no components.
        // * Writes no components.
        // * Produces no output.
        // * Is an "entity" system.

        constexpr auto d_opts =                                 // .
            mp::opt_map::make()                                 // .
                .add(keys::parallelism, ips::none::v())         // .
                .add(keys::dependencies, mp::list::empty_v)     // .
                .add(keys::read_components, mp::list::empty_v)  // .
                .add(keys::write_components, mp::list::empty_v) // .
                .add(keys::output, no_output)                   // .
                .add(keys::kind, kind::entity);                 // .

        return impl::data<TSystemTag, ECST_DECAY_DECLTYPE(d_opts)>{};
    }
} // namespace ecst::sig::system
