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

        template <typename TSystemTag, typename TF>
        auto on_subscribe(TSystemTag, TF&& f) noexcept
        {
            using system_type = tag::system::unwrap<TSystemTag>;

            return [f = FWD(f)](impl::subscribed_t, auto& instance, auto eid)
                ->std::enable_if_t<
                    std::is_same<system_type,
                        ECST_DECAY_DECLTYPE(instance.system())>{},
                    decltype(f(instance.system(), eid))>
            {
                return f(instance.system(), eid);
            };
        }

        template <typename TSystemTag, typename TF>
        auto on_unsubscribe(TSystemTag, TF&& f) noexcept
        {
            using system_type = tag::system::unwrap<TSystemTag>;

            return [f = FWD(f)](impl::unsubscribed_t, auto& instance, auto eid)
                ->std::enable_if_t<
                    std::is_same<system_type,
                        ECST_DECAY_DECLTYPE(instance.system())>{},
                    decltype(f(instance.system(), eid))>
            {
                return f(instance.system(), eid);
            };
        }
    }
}
ECST_NAMESPACE_END
