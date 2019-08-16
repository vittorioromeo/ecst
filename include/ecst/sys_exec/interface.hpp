// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./impl/predicate_holder.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/mp.hpp>
#include <ecst/sig.hpp>

namespace ecst::sys_exec
{
    /// @brief Prepares to execute a function only on the system types that
    /// satisfy the passed predicate.
    template <typename TPredicate>
    constexpr auto matching(TPredicate) noexcept
    {
        return impl::predicate_holder<TPredicate>{};
    }

    /// @brief Prepares to execute a function on all systems.
    constexpr auto all() noexcept
    {
        return matching(bh::always(bh::true_c));
    }

    /// @brief Prepares to execute a function only on the systems matching the
    /// passed `sts...` tags.
    template <typename... TSystemTags>
    constexpr auto t(TSystemTags... sts) noexcept
    {
        // Predicate.
        auto p = [sts...](auto& system) mutable {
            // Tag of the system currently being analyzed.
            auto system_tag = tag::system::make(system);

            // Is the current tag contained in the set of passed tags?
            return bh::contains(bh::make_set(sts...), system_tag);
        };

        return matching(p);
    }
} // namespace ecst::sys_exec
