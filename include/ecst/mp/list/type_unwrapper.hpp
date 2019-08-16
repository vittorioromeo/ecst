// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <boost/hana/ext/std/tuple.hpp>
#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>
#include <ecst/mp/list/basic.hpp>

namespace ecst::mp::list
{
    namespace impl
    {
        template <typename>
        struct list_unwrapper;

        template <typename... Ts>
        struct list_unwrapper<type_list<Ts...>>
        {
            using type = std::tuple<typename Ts::type...>;
        };

        template <typename>
        struct list_bh_unwrapper;

        template <typename... Ts>
        struct list_bh_unwrapper<type_list<Ts...>>
        {
            using type = bh::tuple<typename Ts::type...>;
        };
    } // namespace impl

    /// @brief Unwraps a `type_list<type_c<xs>...>` into an `std::tuple<xs...>`.
    template <typename T>
    using unwrap_tuple = typename impl::list_unwrapper<T>::type;

    /// @brief Unwraps a `type_list<type_c<xs>...>` into a `bh::tuple<xs...>`.
    template <typename T>
    using unwrap_bh_tuple = typename impl::list_bh_unwrapper<T>::type;
} // namespace ecst::mp::list
