// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./predicate_holder.hpp"

namespace ecst::system_execution_adapter
{
    namespace impl
    {
        template <typename TPredicate>
        template <typename TF>
        constexpr auto predicate_holder<TPredicate>::detailed_instance(
            TF&& f) noexcept
        {
            return [f = FWD(f)](auto& instance, auto& executor) mutable
                   -> enabler<predicate_result<decltype(instance)>> {
                f(instance, executor);
            };
        }

        template <typename TPredicate>
        template <typename TF>
        constexpr auto predicate_holder<TPredicate>::detailed(TF&& f) noexcept
        {
            return detailed_instance(
                [f = FWD(f)](auto& instance, auto& executor) mutable {
                    f(instance.system(), executor);
                });
        }

        template <typename TPredicate>
        template <typename TF>
        constexpr auto predicate_holder<TPredicate>::for_subtasks(
            TF&& f) noexcept
        {
            return detailed([f = FWD(f)](auto& system, auto& executor) mutable {
                executor.for_subtasks(
                    [&system, &f](auto& data) mutable { f(system, data); });
            });
        }
    } // namespace impl
} // namespace ecst::system_execution_adapter
