// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/thread_pool.hpp>
#include <ecst/mp.hpp>
#include <ecst/signature_list.hpp>
#include <ecst/settings.hpp>
#include <ecst/context/storage.hpp>
#include <ecst/context/scheduler.hpp>

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    template <typename TSettings>
    class runner
    {
    public:
        using scheduler_wrapper_type = settings::impl::ctx_scheduler<TSettings>;

        using scheduler_type =
            typename scheduler_wrapper_type::template instantiate<TSettings>;

    private:
        scheduler_type _scheduler;

    public:
        template <typename TSystemStorage>
        void initialize(TSystemStorage& ss)
        {
            _scheduler.initialize(ss);
        }

        // TODO: static_assert independency
        template <typename TContext, typename TSystemTag, typename TF>
        void execute(TContext& sp, TSystemTag st, TF&& f)
        {
            _scheduler.execute(sp, st, FWD(f));
        }
    };
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
