// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>

ECST_TAG_SYSTEM_NAMESPACE
{
    namespace impl
    {
        /// @brief Type-wrapping structure holding a system type.
        template <typename TSystem>
        struct tag_impl : public mp::bh::type<TSystem>
        {
        };

        template <typename T>
        constexpr auto is_tag_impl = mp::is_specialization_of_v<tag_impl, T>;
    }

    /// @brief Template variable wrapping a system type into a `tag`.
    template <typename TSystem>
    constexpr auto v = impl::tag_impl<TSystem>{};

    /// @brief Evaluates to true if all `xs...` are component tags.
    template <typename... Ts>
    constexpr auto valid(Ts... xs) noexcept
    {
        return mp::list::all_variadic(impl::is_tag_impl<decltype(xs)>...);
    }

    /// @brief Evaluates to true if `T` is a list of tags.
    template <typename T>
    constexpr auto is_valid(T x)
    {
        return mp::bh::all_of(x, [](auto xe)
            {
                return valid(xe);
            });
    }

    /// @brief Alias for the system type inside a system tag.
    template <typename TTag>
    using unwrap = mp::unwrap<TTag>;
}
ECST_TAG_SYSTEM_NAMESPACE_END
