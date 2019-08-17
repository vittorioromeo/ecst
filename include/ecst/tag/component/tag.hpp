// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>

namespace ecst::tag::component
{
    namespace impl
    {
        /// @brief Type-wrapping structure holding a component type.
        template <typename TComponent>
        struct tag_impl : public bh::type<TComponent>
        {
        };

        template <typename T>
        constexpr auto is_tag_impl = is_specialization_of_v<T, tag_impl>;
    } // namespace impl

    /// @brief Template variable wrapping a component type into a `tag`.
    template <typename TComponent>
    constexpr auto v = impl::tag_impl<TComponent>{};

    /// @brief Returns a tag from a component reference.
    template <typename TComponent>
    constexpr auto make(TComponent&&) noexcept
    {
        return v<decay_t<TComponent>>;
    }

    namespace impl
    {
        struct valid_t
        {
            template <typename... Ts>
            constexpr auto operator()(Ts...) const noexcept
            {
                return (is_tag_impl<Ts> && ...);
            }
        };
    } // namespace impl

    /// @brief Evaluates to true if all `xs...` are component tags.
    inline constexpr impl::valid_t valid{};

    /// @brief Evaluates to true if `T` is a list of component tags.
    template <typename T>
    constexpr auto is_list(T x) noexcept
    {
        return bh::all_of(x, valid);
    }

    using mp::unwrap;
} // namespace ecst::tag::component
