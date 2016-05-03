// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ecst/config.hpp>

ECST_MP_NAMESPACE
{
    template <template <typename...> class TTemplate, typename T>
    struct is_specialization_of : std::false_type
    {
    };

    template <template <typename...> class TTemplate, typename... Ts>
    struct is_specialization_of<TTemplate, TTemplate<Ts...>> : std::true_type
    {
    };

    template <template <typename...> class TTemplate, typename T>
    constexpr auto is_specialization_of_v =
        is_specialization_of<TTemplate, T>{};
}
ECST_MP_NAMESPACE_END
