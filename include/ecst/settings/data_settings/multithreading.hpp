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
        namespace system_parallelism
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

            /// @brief Evaluates to true if `T` is a valid system parallelism
            /// policy.
            template <typename T>
            constexpr auto is = std::is_base_of<base, T>{};
        }

        namespace threading
        {
            struct base
            {
            };

            template <typename TSystemParallelism>
            struct multi : base
            {
                ECST_S_ASSERT(system_parallelism::is<TSystemParallelism>);
                using system_parallelism = TSystemParallelism;
            };

            struct single : base
            {
            };

            /// @brief Evaluates to true if `T` is a valid multithreading
            /// policy.
            template <typename T>
            constexpr auto is = std::is_base_of<base, T>{};
        }

        template <typename TMultithreading>
        constexpr auto allows_inner_parallelism()
        {
            return std::is_same<                             // .
                system_parallelism::enabled,                 // .
                typename TMultithreading::system_parallelism //
                >{};
        }
    }

    constexpr auto allow_inner_parallelism =
        impl::system_parallelism::enabled{};

    constexpr auto disallow_inner_parallelism =
        impl::system_parallelism::disabled{};

    constexpr auto singlethreaded()
    {
        return impl::threading::single{};
    }

    template <typename TSystemParallelism>
    constexpr auto multithreaded(TSystemParallelism)
    {
        return impl::threading::multi<TSystemParallelism>{};
    }
}
ECST_SETTINGS_NAMESPACE_END
