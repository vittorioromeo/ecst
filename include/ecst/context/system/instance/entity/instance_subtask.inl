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
        return this->state_manager().get(n);
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
        // Assert range validity.
        ECST_ASSERT_OP(i_begin, <=, i_end);

        return i_end - i_begin;
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN
    instance<TSettings, TSystemSignature>::other_entity_range_count(
        sz_t i_begin, sz_t i_end) const noexcept
    {
        // Assert range validity.
        ECST_ASSERT_OP(i_begin, <=, i_end);

        return (i_begin - sz_t(0)) + (subscribed_count() - i_end);
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TF>
    void instance<TSettings, TSystemSignature>::for_all_entities(TF && f)
    {
        this->_subscribed.for_each(FWD(f));
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TF>
    void instance<TSettings, TSystemSignature>::for_entities(
        sz_t i_begin, sz_t i_end, TF && f)
    {
        // Note: it makes sense to call this function even on empty ranges -
        // imagine a user-defined function that does some bookkeeping before
        // calling any of the entity iteration functions.

        // Assert range validity.
        ECST_ASSERT_OP(i_begin, <=, i_end);

        for(sz_t i = i_begin; i < i_end; ++i)
        {
            f(this->nth_subscribed(i));
        }
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TF>
    void instance<TSettings, TSystemSignature>::for_other_entities(
        sz_t i_begin, sz_t i_end, TF && f)
    {
        // Note: it makes sense to call this function even on empty ranges -
        // imagine a user-defined function that does some bookkeeping before
        // calling any of the entity iteration functions.

        // Assert range validity.
        ECST_ASSERT_OP(i_begin, <=, i_end);

        for(sz_t i = 0; i < i_begin; ++i)
        {
            f(this->nth_subscribed(i));
        }

        for(sz_t i = i_end; i < this->subscribed_count(); ++i)
        {
            f(this->nth_subscribed(i));
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
