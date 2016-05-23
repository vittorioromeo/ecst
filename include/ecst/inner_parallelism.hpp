// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./inner_parallelism/types.hpp"
#include "./inner_parallelism/utils.hpp"
#include "./inner_parallelism/strategy.hpp"
#include "./inner_parallelism/composer.hpp"

// TODO: move
ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    // TODO: inl, cleanup
    template <typename TSystemTag>
    constexpr auto make(TSystemTag)
    {
        using namespace mp;
        using namespace signature::system::impl;

        constexpr auto default_options = // .
            mp::option_map::make()       // .
                .add(keys::parallelism,
                    inner_parallelism::strategy::none::v())     // .
                .add(keys::dependencies, mp::list::empty_v)     // .
                .add(keys::read_components, mp::list::empty_v)  // .
                .add(keys::write_components, mp::list::empty_v) // .
                .add(keys::output, no_output);                  // .

        return impl::data<TSystemTag,
            std::decay_t<decltype(default_options)>>{};
    }
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
