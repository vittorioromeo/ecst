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
        template <typename TF>
        void system_manager<TSettings>::for_instances_sequential(TF&& f)
        {
            _system_storage.for_instances(FWD(f));
        }

        template <typename TSettings>
        template <typename TF>
        void system_manager<TSettings>::for_instances_parallel(TF&& f)
        {
            counter_blocker b{_system_storage.system_count()};
            b.execute_and_wait_until_zero([ this, &b, f = FWD(f) ]
                {
                    _system_storage.for_instances([this, &b, &f](auto& system)
                        {
                            // Use of multithreading:
                            // * Unsubscribe dead entities from instances.
                            // * Match new/modified entities to instances.
                            this->post_in_thread_pool([this, &b, &system, &f]
                                {
                                    f(system);
                                    b.decrement_and_notify_one();
                                });
                        });
                });
        }

        template <typename TSettings>
        template <typename TF>
        void system_manager<TSettings>::for_instances_dispatch(TF&& f)
        {
            static_if(settings::refresh_parallelism_allowed<settings_type>())
                .then([this](auto&& xf)
                    {
                        this->for_instances_parallel(FWD(xf));
                    })
                .else_([this](auto&& xf)
                    {
                        this->for_instances_sequential(FWD(xf));
                    })(FWD(f));
        }

        template <typename TSettings>
        template <typename TF>
        auto system_manager<TSettings>::post_in_thread_pool(TF&& f)
        {
            return _thread_pool->post(FWD(f));
        }

        template <typename TSettings>
        template <typename TID>
        auto& ECST_CONST_FN system_manager<TSettings>::instance_by_id(
            TID) noexcept
        {
            return _system_storage.instance_by_id(TID{});
        }

        template <typename TSettings>
        template <typename TContext, typename TStartSystemTagList,
            typename... TFs>
        void system_manager<TSettings>::execute_systems_impl(
            TContext& context, TStartSystemTagList sstl, TFs&&... fs)
        {
            ECST_S_ASSERT_DT(tag::system::is_list(sstl));

            auto os = bh::overload_linearly(FWD(fs)...);
            _scheduler.execute(context, sstl, os);
        }

        template <typename TSettings>
        template <typename TContext, typename... TStartSystemTags>
        auto system_manager<TSettings>::execute_systems_from(
            TContext& context, TStartSystemTags... sts) noexcept
        {
            auto sstl = mp::list::make(sts...);
            return [this, &context, sstl](auto&&... fns)
            {
                this->execute_systems_impl(context, sstl, FWD(fns)...);
            };
        }

        template <typename TSettings>
        template <typename TContext>
        auto system_manager<TSettings>::execute_systems(
            TContext& context) noexcept
        {
            auto ssl = settings::system_signature_list(TSettings{});

            auto independent_ssl =
                bh::filter(ssl, signature::system::indepedent);

            auto independent_stl = bh::transform(independent_ssl, [](auto ss)
                {
                    return signature::system::tag_of(ss);
                });

            return [this, &context, independent_stl](auto&&... fns)
            {
                this->execute_systems_impl(
                    context, independent_stl, FWD(fns)...);
            };
        }


        template <typename TSettings>
        template <typename TSystemTag, typename TF>
        decltype(auto) system_manager<TSettings>::for_system_outputs(
            TSystemTag st, TF&& f)
        {
            return instance(st).for_outputs(FWD(f));
        }

        template <typename TSettings>
        template <typename TSystemTag>
        auto& system_manager<TSettings>::instance(TSystemTag st) noexcept
        {
            return _system_storage.instance_by_tag(st);
        }

        template <typename TSettings>
        template <typename TSystemTag>
        const auto& system_manager<TSettings>::instance(TSystemTag st) const
            noexcept
        {
            return _system_storage.instance_by_tag(st);
        }

        template <typename TSettings>
        template <typename TSystemTag>
        auto& system_manager<TSettings>::system(TSystemTag st) noexcept
        {
            return instance(st).system();
        }

        template <typename TSettings>
        template <typename TSystemTag>
        const auto& system_manager<TSettings>::system(TSystemTag st) const
            noexcept
        {
            return instance(st).system();
        }

        template <typename TSettings>
        template <typename TSystemTag>
        auto system_manager<TSettings>::is_in_system(
            TSystemTag st, entity_id eid) const noexcept
        {
            return instance(st).is_subscribed(eid);
        }

        template <typename TSettings>
        template <typename TSystemTag>
        auto system_manager<TSettings>::count_entities_in(TSystemTag st) const
            noexcept
        {
            return instance(st).subscribed_count();
        }

        template <typename TSettings>
        template <typename TSystemTag>
        auto system_manager<TSettings>::any_entity_in(TSystemTag st) const
            noexcept
        {
            return count_entities_in(st) > 0;
        }

        template <typename TSettings>
        constexpr auto system_manager<TSettings>::inner_parallelism_allowed()
            const noexcept
        {
            return settings::inner_parallelism_allowed(TSettings{});
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
