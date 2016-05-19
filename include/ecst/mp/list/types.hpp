// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <tuple>
#include <utility>
#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>
#include <boost/hana.hpp>
#include <boost/hana/experimental/types.hpp>


namespace ecst
{
    namespace bh = boost::hana;
}

ECST_MP_LIST_NAMESPACE
{
    template <typename Haystack, typename Needle>
    constexpr auto get_index_of_first_matching(Haystack&&, Needle && n)
    {
        using Pred = decltype(boost::hana::equal.to(n));
        using Pack = typename boost::hana::detail::make_pack<Haystack>::type;
        constexpr auto index = boost::hana::detail::index_if<Pred, Pack>::value;
        return boost::hana::int_c<index>;
    }


    template <typename Iterable, typename T>
    constexpr auto index_of_impl(Iterable const& iterable, T const& element)
    {
        return get_index_of_first_matching(
            bh::transform(iterable,
                [](auto x)
                {
                    return bh::type_c<std::decay_t<decltype(x)>>;
                }),
            bh::type_c<std::decay_t<decltype(element)>>);
    }


    template <typename Iterable, typename T>
    constexpr auto index_of(Iterable const& iterable, T const& element)
    {
        return decltype(index_of_impl(iterable, element)){};
    }

    namespace impl
    {
        // TODO: abstract tuple operations away, in order to quickly replace
        // `std::tuple` with something else if required

        // A `type_list` is an `std::tuple`.
        template <typename... Ts>
        using type_list = bh::tuple<Ts...>;

        // An empty list is an empty `type_list`.
        using empty_type_list = type_list<>;
    }

    // Variable template for `type_list`.
    template <typename... Ts>
    constexpr impl::type_list<Ts...> v{};

    // Variable template for an empty list.
    constexpr impl::empty_type_list empty_v{};
}
ECST_MP_LIST_NAMESPACE_END
