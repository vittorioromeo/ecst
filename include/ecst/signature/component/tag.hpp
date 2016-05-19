// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>
#include <ecst/mp/list.hpp>

ECST_SIGNATURE_COMPONENT_NAMESPACE
{
    namespace impl
    {
        /// @brief Type-wrapping structure holding a component type.
        template <typename TComponent>
        struct tag_impl
        {
            using type = TComponent;
        };

        /// @brief Template variable for `tag_impl`.
        template <typename TComponent>
        constexpr tag_impl<TComponent> tag_v_impl{};

        /// @brief Alias for the component type inside a component tag.
        template <typename TComponentTag>
        using tag_component_type = typename TComponentTag::type;

        template <typename T>
        constexpr auto is_tag_impl = mp::is_specialization_of_v<tag_impl, T>;
    }

    /// @brief Template variable wrapping a component type into a `tag`.
    template <typename TComponent>
    constexpr auto tag = impl::tag_v_impl<TComponent>;

    /// @brief Evaluates to true if `T` is a component tag.
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
    using unwrap_tag = impl::tag_component_type<TTag>;
}
ECST_SIGNATURE_COMPONENT_NAMESPACE_END
