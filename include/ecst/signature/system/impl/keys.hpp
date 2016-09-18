// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/tag/system.hpp>

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    namespace impl
    {
        namespace keys
        {
            constexpr auto parallelism = sz_v<0>;
            constexpr auto dependencies = sz_v<1>;
            constexpr auto read_components = sz_v<2>;
            constexpr auto write_components = sz_v<3>;
            constexpr auto output = sz_v<4>;

            // TODO: rename?
            constexpr auto kind = sz_v<5>;
        }

        // TODO: move, docs, rename
        namespace kind
        {
            struct stateless_t
            {
            };

            struct stateful_t
            {
            };

            struct entity_t
            {
            };

            constexpr stateless_t stateless{};
            constexpr stateful_t stateful{};
            constexpr entity_t entity{};
        }
    }
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
