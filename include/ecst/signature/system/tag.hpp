// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    namespace impl
    {
        /// @brief Type-wrapping structure holding a system type.
        template <typename TSystem>
        struct tag_impl
        {
            using type = TSystem;
        };

        /// @brief Template variable for `tag_impl`.
        template <typename TSystem>
        constexpr tag_impl<TSystem> tag_v_impl{};

        /// @brief Alias for the system type inside a system tag.
        template <typename TSystemTag>
        using tag_system_type = typename TSystemTag::type;

        // TODO:
        template <typename T>
        constexpr auto is_tag_impl = true;
    }

    /// @brief Template variable wrapping a system type into a `tag`.
    template <typename TSystem>
    constexpr auto tag = bh::type_c<TSystem>;

    /// @brief Evaluates to true if `T` is a system tag.
    template <typename T>
    constexpr auto is_tag(T)
    {
        return mp::bool_v<impl::is_tag_impl<T>>;
    }

    namespace impl
    {
        template <typename T>
        auto is_tag_list_impl(T x)
        {
            return bh::all_of(x, [](auto xe)
                {
                    return is_tag(xe);
                });
        }
    }

    /// @brief Evaluates to true if `T` is a list of tags.
    template <typename T>
    constexpr auto is_tag_list(T x)
    {
        return impl::is_tag_list_impl(x);
    }

    template <typename TTag>
    using unwrap_tag = impl::tag_system_type<TTag>;
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
