// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/tag.hpp>
#include <ecst/context/types.hpp>
#include <ecst/system_execution_adapter.hpp>
#include "./refresh_event.hpp"

ECST_NAMESPACE
{
    namespace refresh_event
    {
        // TODO: the lambdas should be mutable, but overloading does not seem to
        // work. Maybe `do_nothing::operator()` has to be non-const?

        // TODO: cleanup, refactor, fix mutable

        template <typename TF>
        auto on_subscribe(TF&& f) noexcept
        {
            return [f = FWD(f)](impl::subscribed_t, auto& instance, auto eid)
                ->decltype(f(instance, eid))
            {
                return f(instance, eid);
            };
        }

        template <typename TF>
        auto on_unsubscribe(TF&& f) noexcept
        {
            return [f = FWD(f)](impl::unsubscribed_t, auto& instance, auto eid)
                ->decltype(f(instance, eid))
            {
                return f(instance, eid);
            };
        }

        template <typename TF>
        auto on_reclaim(TF&& f) noexcept
        {
            return [f = FWD(f)](impl::reclaimed_t, entity_id eid)
                ->decltype(f(eid))
            {
                return f(eid);
            };
        }

        namespace impl
        {
            template <typename TInstance, typename TSystemTagToCheck>
            constexpr auto check_tag() noexcept
            {
                ECST_S_ASSERT_DT(tag::system::valid(TSystemTagToCheck{}));

                using system_type = typename decay_t<TInstance>::system_type;

                constexpr auto system_tag = tag::system::v<system_type>;
                ECST_S_ASSERT(tag::system::valid(system_tag));

                return std::is_same<ECST_DECAY_DECLTYPE(system_tag),
                    decay_t<TSystemTagToCheck>>{};
            }

            template <typename TInstance, typename TSystemTag>
            using enable_matching_instance =
                std::enable_if_t<check_tag<TInstance, TSystemTag>()>;
        }

        template <typename TSystemTag, typename TF>
        auto on_subscribe(TSystemTag, TF&& f) noexcept
        {
            return [f = FWD(f)](impl::subscribed_t, auto& instance, auto eid)
                ->impl::enable_matching_instance<decltype(instance), TSystemTag>
            {
                // TODO: instance?
                return f(instance.system(), eid);
            };
        }

        template <typename TSystemTag, typename TF>
        auto on_unsubscribe(TSystemTag, TF&& f) noexcept
        {
            return [f = FWD(f)](impl::unsubscribed_t, auto& instance, auto eid)
                ->impl::enable_matching_instance<decltype(instance), TSystemTag>
            {
                // TODO: instance?
                return f(instance.system(), eid);
            };
        }
    }
}
ECST_NAMESPACE_END
