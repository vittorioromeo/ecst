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
        constexpr auto default_parallelism =
            ecst::inner_parallelism::strategy::none::v();

        using default_parallelism_type = // .
            std::remove_const_t<decltype(default_parallelism)>;

        using default_output_type = // .
            std::remove_const_t<decltype(no_output)>;

        return impl::data<TSystemTag, default_parallelism_type, mp::list::t<>,
            mp::list::t<>, mp::list::t<>, default_output_type>{};
    }
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
