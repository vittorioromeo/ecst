// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>

namespace ecst::mp::list
{
    namespace impl
    {
        // A `type_list` is an `bh::basic_tuple`.
        template <typename... Ts>
        using type_list = bh::basic_tuple<Ts...>;

        // An empty list is an empty `type_list`.
        using empty_type_list = type_list<>;
    } // namespace impl

    // Type of `type_list<Ts...>`.
    template <typename... Ts>
    using t = impl::type_list<Ts...>;

    // Variable template for `type_list`.
    template <typename... Ts>
    constexpr impl::type_list<Ts...> v{};

    // Variable template for an empty list.
    constexpr impl::empty_type_list empty_v{};
} // namespace ecst::mp::list
