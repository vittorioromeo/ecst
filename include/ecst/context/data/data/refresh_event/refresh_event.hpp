// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>

ECST_NAMESPACE
{
    /// @namespace Contains functions to handle events in `step` calls.
    namespace refresh_event
    {
        namespace impl
        {
            struct subscribed_t
            {
            };

            struct unsubscribed_t
            {
            };

            struct reclaimed_t
            {
            };

            constexpr subscribed_t subscribed{};
            constexpr unsubscribed_t unsubscribed{};
            constexpr reclaimed_t reclaimed{};
        }

        /// @brief Calls a provided `(instance_type&, id)` function `f` when an
        /// entity is subscribed to a system.
        /// @details Not thread-safe.
        template <typename TF>
        auto on_subscribe(TF&& f) noexcept;

        /// @brief Calls a provided `(instance_type&, id)` function `f` when an
        /// entity is unsubscribed from a system.
        /// @details Not thread-safe.
        template <typename TF>
        auto on_unsubscribe(TF&& f) noexcept;

        /// @brief Calls a provided `(id)` function `f` when an entity is
        /// reclaimed.
        /// @details Thread-safe.
        template <typename TF>
        auto on_reclaim(TF&& f) noexcept;

        /// @brief Calls a provided `(system_type&, id)` function `f` when an
        /// entity is subscribed to a system.
        /// @details Not thread-safe.
        template <typename TSystemTag, typename TF>
        auto on_subscribe(TSystemTag st, TF&& f) noexcept;

        /// @brief Calls a provided `(system_type&, id)` function `f` when an
        /// entity is unsubscribed from a system.
        /// @details Not thread-safe.
        template <typename TSystemTag, typename TF>
        auto on_unsubscribe(TSystemTag st, TF&& f) noexcept;
    }
}
ECST_NAMESPACE_END
