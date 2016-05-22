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
    // TODO: builder pattern?

    // TODO: inl, cleanup
    template <typename TSystemTag>
    constexpr auto make(TSystemTag st)
    {
        constexpr auto default_parallelism =
            ecst::inner_parallelism::strategy::none::v();

        return impl::make_impl(st, default_parallelism, mp::list::empty_v,
            mp::list::empty_v, mp::list::empty_v, output::none);
    }
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
