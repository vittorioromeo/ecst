// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/pair.hpp>
#include <ecst/mp/map/basic.hpp>

ECST_MP_MAP_NAMESPACE
{
    namespace impl
    {
        auto VRM_CORE_CONST_FN picker_key()
        {
            return [](auto xxp)
            {
                return key(xxp);
            };
        }

        auto VRM_CORE_CONST_FN picker_value()
        {
            return [](auto xxp)
            {
                return value(xxp);
            };
        }

        template <typename TKey>
        auto by_key(TKey k)
        {
            return [=](auto p)
            {
                return pair::fst_is(p, k);
            };
        }

        template <typename TValue>
        auto by_value(TValue v)
        {
            return [=](auto p)
            {
                return pair::snd_is(p, v);
            };
        }
    }
}

