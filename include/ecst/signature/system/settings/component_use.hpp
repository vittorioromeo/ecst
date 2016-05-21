// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/core.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/component.hpp>

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <typename TComponent>
        struct read_impl
        {
            using component = TComponent;
        };

        template <typename TComponent>
        struct mutate_impl
        {
            using component = TComponent;
        };

        template <typename T>
        using is_read_component_use_impl =
            mp::is_specialization_of<read_impl, T>;

        template <typename T>
        using is_mutate_component_use_impl =
            mp::is_specialization_of<mutate_impl, T>;

        template <typename T>
        using is_component_use_impl =
            mp::bh::bool_<is_read_component_use_impl<T>{} ||
                          is_mutate_component_use_impl<T>{}>;
    }

    /// @brief Evaluates to true if `T` is a valid read-only component use.
    template <typename T>
    constexpr auto is_read_component_use(T)
    {
        return impl::is_read_component_use_impl<T>{};
    }

    /// @brief Evaluates to true if `T` is a valid mutating component use.
    template <typename T>
    constexpr auto is_mutate_component_use(T)
    {
        return impl::is_mutate_component_use_impl<T>{};
    }

    /// @brief Evaluates to true if `T` is a valid component use.
    template <typename T>
    constexpr auto is_component_use(T)
    {
        return impl::is_component_use_impl<T>{};
    }

    /// @brief Evaluates to true if all `Ts...` are valid component uses.
    template <typename... Ts>
    constexpr auto are_component_uses(Ts... xs)
    {
        return mp::list::all_variadic(is_component_use(xs)...);
    }

    // TODO: variadic
    template <typename TComponentTag>
    constexpr auto read(TComponentTag) noexcept
    {
        // TODO: asserts, etc
        return impl::read_impl<                             // .
            signature::component::unwrap_tag<TComponentTag> // .
            >{};
    }

    // TODO: variadic
    template <typename TComponentTag>
    constexpr auto mutate(TComponentTag) noexcept
    {
        // TODO: rename to write?
        // TODO: asserts, etc
        return impl::mutate_impl<                           // .
            signature::component::unwrap_tag<TComponentTag> // .
            >{};
    }

    template <typename... TSystemComponents>
    constexpr auto component_use(TSystemComponents... scs)
    {
        ECST_S_ASSERT_DT(are_component_uses(scs...));
        return mp::list::make(scs...);
    }

    constexpr auto no_component_use = component_use();
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
