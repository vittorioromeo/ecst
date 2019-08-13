// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

namespace ecst::settings
{
    namespace impl
    {
        namespace keys
        {
            constexpr auto threading = sz_v<0>;
            constexpr auto entity_storage = sz_v<1>;
            constexpr auto component_signature_list = sz_v<2>;
            constexpr auto system_signature_list = sz_v<3>;
            constexpr auto scheduler = sz_v<4>;
            constexpr auto refresh_parallelism = sz_v<5>;
        } // namespace keys
    }     // namespace impl
} // namespace ecst::settings
