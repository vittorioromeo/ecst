// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/core.hpp>

ECST_SETTINGS_NAMESPACE
{
    namespace impl
    {
        namespace refresh_parallelism
        {
            struct base
            {
            };

            struct enabled : base
            {
            };

            struct disabled : base
            {
            };

            /// @brief Evaluates to true if `T` is a valid refresh parallelism
            /// policy.
            template <typename T>
            constexpr auto is = std::is_base_of<base, T>{};
        }

        template <typename TRefreshParallelism>
        constexpr auto allows_refresh_parallelism()
        {
            return std::is_same<              // .
                refresh_parallelism::enabled, // .
                TRefreshParallelism           //
                >{};
        }
    }

    constexpr auto allow_refresh_parallelism =
        impl::refresh_parallelism::enabled{};

    constexpr auto disallow_refresh_parallelism =
        impl::refresh_parallelism::disabled{};
}
ECST_SETTINGS_NAMESPACE_END
