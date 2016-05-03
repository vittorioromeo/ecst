// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vector>
#include <functional>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/utils/sparse_int_set.hpp>
#include <ecst/thread_pool.hpp>
#include <ecst/mp.hpp>
#include <ecst/signature_list.hpp>
#include <ecst/settings.hpp>
#include <ecst/context/types.hpp>
#include <ecst/utils/sparse_int_set.hpp>

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        namespace defer
        {
            template <typename TSettings>
            class proxy;
        }
    }
}
ECST_CONTEXT_NAMESPACE_END

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <typename TSettings>
        using deferred_fn_type =
            std::function<void(context::impl::defer::proxy<TSettings>&)>;

        template <typename TSettings>
        class state : public ecst::impl::to_kill_set_wrapper<TSettings>
        {
        private:
            using my_deferred_fn_type = deferred_fn_type<TSettings>;
            using set_type = dispatch_set<TSettings>;

            std::vector<my_deferred_fn_type> _deferred_fns;

        public:
            void clear()
            {
                ecst::impl::to_kill_set_wrapper<TSettings>::clear();
                _deferred_fns.clear();
            }

            template <typename TF>
            void add_deferred_fn(TF&& f)
            {
                _deferred_fns.emplace_back(FWD(f));
            }

            template <typename TProxy>
            void execute_deferred_fns(TProxy& proxy)
            {
                for(auto& f : _deferred_fns)
                {
                    f(proxy);
                }
            }
        };
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
