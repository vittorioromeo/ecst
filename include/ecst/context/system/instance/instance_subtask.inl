// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./instance.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN instance<TSettings, TSystemSignature>::nth_subscribed(
        sz_t n) noexcept
    {
        return entity_id{_subscribed.at(n)};
    }

    template <typename TSettings, typename TSystemSignature>
    auto& ECST_PURE_FN instance<TSettings, TSystemSignature>::nth_state(
        sz_t n) noexcept
    {
        return _sm.get(n);
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN instance<TSettings, TSystemSignature>::all_entity_count()
        const noexcept
    {
        return subscribed_count();
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN instance<TSettings, TSystemSignature>::entity_range_count(
        sz_t i_begin, sz_t i_end) const noexcept
    {
        return i_end - i_begin;
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN instance<TSettings,
        TSystemSignature>::other_entity_range_count(sz_t i_begin, sz_t i_end)
        const noexcept
    {
        return (i_begin - sz_t(0)) + (subscribed_count() - i_end);
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
