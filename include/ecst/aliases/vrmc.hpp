// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <vrm/core/for_args.hpp>
#include <vrm/core/type_aliases.hpp>
#include <vrm/core/type_traits.hpp>

ECST_NAMESPACE
{
    namespace vrmc = vrm::core;

    using vrmc::bool_;
    using vrmc::bool_v;
    using vrmc::for_args;
    using vrmc::int_;
    using vrmc::int_v;
    using vrmc::is_specialization_of_t;
    using vrmc::is_specialization_of_v;
    using vrmc::likely;
    using vrmc::sz_t;
    using vrmc::sz_t_;
    using vrmc::sz_v;
    using vrmc::unlikely;
}
ECST_NAMESPACE_END
