// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp.hpp>
#include "./data.hpp"

ECST_SIGNATURE_COMPONENT_NAMESPACE
{
    template <typename... TComponentTags>
    constexpr auto make(TComponentTags... cts) noexcept
    {
        auto ct_list = mp::list::make(cts...);

        constexpr auto d_opts =    // .
            mp::option_map::make() // .
                .add(impl::keys::storage, impl::contiguous_buffer_maker);

        return impl::data<                // .
            ECST_DECAY_DECLTYPE(ct_list), // .
            ECST_DECAY_DECLTYPE(d_opts)   // .
            >{};
    }
}
ECST_SIGNATURE_COMPONENT_NAMESPACE_END
