// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../data/data.hpp"
#include "./step.hpp"

namespace ecst::context::impl::step
{
    template <typename TSettings>
    proxy<TSettings>::proxy(
        context_type& context, refresh_state_type& refresh_state) noexcept
        : base_type{context, refresh_state}
    {
    }

    template <typename TSettings>
    template <typename... TStartSystemTags>
    auto proxy<TSettings>::execute_systems_from(
        TStartSystemTags... sts) noexcept
    {
        return this->context().execute_systems_from(this->context(), sts...);
    }

    template <typename TSettings>
    auto proxy<TSettings>::execute_systems() noexcept
    {
        return this->context().execute_systems(this->context());
    }
} // namespace ecst::context::impl::step
