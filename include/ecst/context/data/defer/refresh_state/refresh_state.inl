// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./refresh_state.hpp"

namespace ecst::context
{
    namespace impl
    {
        namespace defer
        {
            template <typename TSettings>
            void refresh_state<TSettings>::clear() noexcept
            {
                _to_match.clear();
                _to_kill.clear();
            }
        } // namespace defer
    }     // namespace impl
} // namespace ecst::context
