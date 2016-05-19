// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/core/static_if.hpp>
#include <vrm/core/for_args.hpp>
#include <vrm/core/tuple_utils.hpp>
#include <vrm/core/type_aliases.hpp>
#include <vrm/core/y_combinator.hpp>
#include <ecst/config.hpp>
#include <boost/hana.hpp>
#include <boost/hana/experimental/types.hpp>

namespace ecst
{
    namespace bh = boost::hana;
}
ECST_MP_NAMESPACE
{
    namespace vrmc = vrm::core;

    using vrmc::bool_;
    using vrmc::bool_v;
    using vrmc::sz_t;
    using vrmc::sz_t_;

    template <std::size_t TX>
    constexpr boost::hana::size_t<TX> sz_v{};


    using vrmc::int_;
    using vrmc::int_v;
    using vrmc::static_if;
    using vrmc::apply;

    template <typename TF>
    constexpr auto y_combinator(TF && f) noexcept
    {
        return vrmc::y_combinator(FWD(f));
    }
}
ECST_MP_NAMESPACE_END
