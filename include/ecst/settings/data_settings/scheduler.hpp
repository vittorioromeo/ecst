// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <type_traits>

namespace ecst::settings
{
    namespace impl
    {
        template <template <typename> class TScheduler>
        struct scheduler_wrapper
        {
            template <typename TSettings>
            using instantiate = TScheduler<TSettings>;
        };
    } // namespace impl

    template <template <typename> class TScheduler>
    constexpr auto scheduler = impl::scheduler_wrapper<TScheduler>{};
} // namespace ecst::settings
