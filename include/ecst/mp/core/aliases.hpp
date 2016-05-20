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
#include <ecst/aliases.hpp>
#include <boost/hana.hpp>

ECST_MP_NAMESPACE
{
    namespace bh = boost::hana;
    namespace vrmc = vrm::core;

    using ecst::sz_t;
    using ecst::sz_t_;

    template <std::size_t TX>
    constexpr bh::size_t<TX> sz_v{};


    using ecst::int_;
    using ecst::int_v;
    using ecst::static_if;

    template <typename TF>
    constexpr auto y_combinator(TF && f) noexcept
    {
        return vrmc::y_combinator(FWD(f));
    }
}
ECST_MP_NAMESPACE_END
