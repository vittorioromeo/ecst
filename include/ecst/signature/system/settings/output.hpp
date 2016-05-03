// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/core.hpp>

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    namespace output
    {
        namespace impl
        {
            struct o_base
            {
            };

            struct empty_type
            {
            };

            struct o_none : o_base
            {
                using type = empty_type;
            };

            template <typename T>
            struct o_data : o_base
            {
                using type = T;
            };
        }

        template <typename T>
        constexpr auto is_valid = std::is_base_of<impl::o_base, T>{};

        template <typename T>
        constexpr auto is_none = std::is_same<impl::o_none, T>{};

        template <typename T>
        constexpr auto is_data = mp::is_specialization_of_v<impl::o_data, T>;

        constexpr auto none = impl::o_none{};

        template <typename T>
        constexpr auto data = impl::o_data<T>{};
    }
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
