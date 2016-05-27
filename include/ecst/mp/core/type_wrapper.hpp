
// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>

ECST_MP_NAMESPACE
{
    // Bring hana's `type_c` into the `mp` namespace.
    using bh::type_c;

    /// @brief Returns the inner type wrapped by a `type_c`.
    template <typename T>
    using unwrap = typename T::type;

    namespace impl
    {
        // TODO: docs, test
        template <typename T>
        using is_wrapped = std::is_base_of<bh::basic_type<T>, T>;

        struct unwrapped_t
        {
            template <typename T>
            constexpr auto operator()(T) const noexcept
            {
                return unwrap<T>{};
            }
        };

        struct wrap_t
        {
            template <typename T>
            constexpr auto operator()(T) const noexcept
            {
                return type_c<T>;
            }
        };
    }

    /// @brief Returns a default-constructed instance of the inner type wrapped
    /// by a `type_c`.
    constexpr impl::unwrapped_t unwrapped{};

    /// @brief Returns a `type_c` wrapping the passed parameter.
    constexpr impl::wrap_t wrap{};
}
ECST_MP_NAMESPACE_END
