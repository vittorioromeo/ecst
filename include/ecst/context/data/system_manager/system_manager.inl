// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include "../defer.hpp"
#include "./system_manager.hpp"

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        using handle = ecst::context::entity::impl::handle;

        template <typename TSettings>
        system_manager<TSettings>::system_manager()
        {
            _system_runner.initialize(_system_storage);
        }

        template <typename TSettings>
        template <typename TF>
        void system_manager<TSettings>::for_systems_sequential(TF&& f)
        {
            _system_storage.for_systems(FWD(f));
        }

        template <typename TSettings>
        template <typename TF>
        void system_manager<TSettings>::for_systems_parallel(TF&& f)
        {
            counter_blocker b{_system_storage.system_count()};
            execute_and_wait_until_counter_zero(b, [ this, &b, f = FWD(f) ]()
                {
                    _system_storage.for_systems([this, &b, &f](auto& system)
                        {
                            this->post_in_thread_pool([this, &b, &system, &f]()
                                {
                                    f(system);
                                    decrement_cv_counter_and_notify_one(b);
                                });
                        });
                });
        }

        template <typename TSettings>
        template <typename TF>
        void system_manager<TSettings>::for_systems_dispatch(TF&& f)
        {
            static_if(settings::refresh_parallelism_allowed<settings_type>())
                .then([this](auto&& xf)
                    {
                        this->for_systems_parallel(FWD(xf));
                    })
                .else_([this](auto&& xf)
                    {
                        this->for_systems_sequential(FWD(xf));
                    })(FWD(f));
        }

        template <typename TSettings>
        template <typename TF>
        auto system_manager<TSettings>::post_in_thread_pool(TF&& f)
        {
            return _thread_pool.post(FWD(f));
        }

        template <typename TSettings>
        template <typename TID>
        auto& ECST_CONST_FN system_manager<TSettings>::instance_by_id(
            TID) noexcept
        {
            return _system_storage.system_by_id(TID{});
        }

        template <typename TSettings>
        template <typename TContext, typename... TFs>
        void system_manager<TSettings>::execute_systems(
            TContext& context, TFs&&... fs)
        {
            auto os = make_overload(FWD(fs)...);
            _system_runner.execute(context, os);
        }

        template <typename TSettings>
        template <typename TSystem, typename TF>
        decltype(auto) system_manager<TSettings>::for_system_outputs(TF&& f)
        {
            return instance<TSystem>().for_outputs(FWD(f));
        }

        template <typename TSettings>
        template <typename TSystemTag, typename TF>
        decltype(auto) system_manager<TSettings>::for_system_outputs(
            TSystemTag, TF&& f)
        {
            return for_system_outputs<system_from_tag<TSystemTag>>(FWD(f));
        }

        template <typename TSettings>
        template <typename TSystem, typename TAcc, typename TF>
        auto system_manager<TSettings>::foldl_system_outputs(TAcc acc, TF&& f)
        {
            return instance<TSystem>().foldl_outputs(acc, FWD(f));
        }

        template <typename TSettings>
        template <typename TSystemTag, typename TAcc, typename TF>
        auto system_manager<TSettings>::foldl_system_outputs(
            TSystemTag, TAcc acc, TF&& f)
        {
            return foldl_system_outputs<system_from_tag<TSystemTag>>(
                acc, FWD(f));
        }

        template <typename TSettings>
        template <typename TSystem>
        auto& ECST_CONST_FN system_manager<TSettings>::instance() noexcept
        {
            return _system_storage.template instance<TSystem>();
        }

        template <typename TSettings>
        template <typename TSystem>
        const auto& ECST_CONST_FN system_manager<TSettings>::instance() const
            noexcept
        {
            return _system_storage.template instance<TSystem>();
        }

        template <typename TSettings>
        template <typename TSystemTag>
        auto& system_manager<TSettings>::instance(TSystemTag) noexcept
        {
            return instance<system_from_tag<TSystemTag>>();
        }

        template <typename TSettings>
        template <typename TSystemTag>
        const auto& system_manager<TSettings>::instance(TSystemTag) const
            noexcept
        {
            return instance<system_from_tag<TSystemTag>>();
        }

        template <typename TSettings>
        template <typename TSystem>
        auto& system_manager<TSettings>::system() noexcept
        {
            return instance<TSystem>().system();
        }

        template <typename TSettings>
        template <typename TSystem>
        const auto& system_manager<TSettings>::system() const noexcept
        {
            return instance<TSystem>().system();
        }

        template <typename TSettings>
        template <typename TSystemTag>
        auto& system_manager<TSettings>::system(TSystemTag) noexcept
        {
            return system<system_from_tag<TSystemTag>>();
        }

        template <typename TSettings>
        template <typename TSystemTag>
        const auto& system_manager<TSettings>::system(TSystemTag) const noexcept
        {
            return system<system_from_tag<TSystemTag>>();
        }

        template <typename TSettings>
        template <typename TSystem>
        auto system_manager<TSettings>::is_in_system(entity_id eid) const
            noexcept
        {
            const auto& si(this->template instance<TSystem>());
            return si.is_subscribed(eid);
        }

        template <typename TSettings>
        template <typename TSystemTag>
        auto system_manager<TSettings>::is_in_system(
            TSystemTag, entity_id eid) const noexcept
        {
            return is_in_system<system_from_tag<TSystemTag>>(eid);
        }

        template <typename TSettings>
        template <typename TSystem>
        auto ECST_PURE_FN system_manager<TSettings>::count_entities_in() const
            noexcept
        {
            const auto& si(this->template instance<TSystem>());
            return si.subscribed_count();
        }

        template <typename TSettings>
        template <typename TSystemTag>
        auto system_manager<TSettings>::count_entities_in(TSystemTag) const
            noexcept
        {
            return count_entities_in<system_from_tag<TSystemTag>>();
        }

        template <typename TSettings>
        template <typename TSystem>
        auto ECST_PURE_FN system_manager<TSettings>::any_entity_in() const
            noexcept
        {
            return count_entities_in<TSystem>() > 0;
        }

        template <typename TSettings>
        template <typename TSystemTag>
        auto system_manager<TSettings>::any_entity_in(TSystemTag) const noexcept
        {
            return any_entity_in<system_from_tag<TSystemTag>>();
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
