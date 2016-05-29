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
        class deferred_fns_vector
        {
        private:
            using defer_proxy_type = context::impl::defer::proxy<TSettings>;

            /// @brief A "deferred function" is a void-returning `std::function`
            /// that takes a "defer proxy" by reference as its only parameter.
            using fn_type = std::function<void(defer_proxy_type&)>;

            std::vector<fn_type> _fns;

        public:
            void clear() noexcept
            {
                _fns.clear();
            }

            template <typename TF>
            void add(TF&& f)
            {
                _fns.emplace_back(FWD(f));
            }

            template <typename TProxy>
            void execute_all(TProxy& proxy)
            {
                for(auto& f : _fns)
                {
                    f(proxy);
                }
            }
        };

        // TODO: use function_queue

        /// @brief A "system state" is a storage class bound to a particular
        /// subtask during system execution.
        /// @details Contains a vector of deferred functions and a "killed
        /// entities" sparse set.
        template <typename TSettings>
        class state : public ecst::impl::to_kill_set_wrapper<TSettings>
        {
        private:
            using set_type = dispatch_set<TSettings>;

            deferred_fns_vector<TSettings> _deferred_fns;

        public:
            void clear() noexcept
            {
                ecst::impl::to_kill_set_wrapper<TSettings>::clear();
                _deferred_fns.clear();
            }

            template <typename TF>
            void add_deferred_fn(TF&& f)
            {
                _deferred_fns.add(FWD(f));
            }

            template <typename TProxy>
            void execute_deferred_fns(TProxy& proxy)
            {
                _deferred_fns.execute_all(proxy);
            }
        };
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
