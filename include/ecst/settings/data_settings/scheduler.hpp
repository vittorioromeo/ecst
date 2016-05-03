// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ecst/config.hpp>

ECST_SETTINGS_NAMESPACE
{
    namespace impl
    {
        template <template <typename> class TScheduler>
        struct scheduler_wrapper
        {
            template <typename TSettings>
            using instantiate = TScheduler<TSettings>;
        };
    }

    template <template <typename> class TScheduler>
    constexpr auto scheduler = impl::scheduler_wrapper<TScheduler>{};
}
ECST_SETTINGS_NAMESPACE_END
