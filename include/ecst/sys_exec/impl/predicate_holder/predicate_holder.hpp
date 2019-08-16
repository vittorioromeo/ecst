// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/mp.hpp>
#include <ecst/sig.hpp>

namespace ecst::sys_exec
{
    namespace impl
    {
        template <typename TPredicate>
        class predicate_holder
        {
        private:
            template <typename TInstance>
            using predicate_result = decltype( // .
                std::declval<TPredicate>()(    // .
                    std::declval<              // .
                        decay_t<TInstance>>()  // .
                        .system()              // .
                    )                          // .
            );

            template <typename TPredicateResult>
            using enabler = std::enable_if_t<TPredicateResult{}, void>;

        public:
            /// @brief Executes `f` by passing the instance as the first
            /// argument, and an "executor proxy" as the second argument.
            template <typename TF>
            constexpr auto detailed_instance(TF&& f) noexcept;

            /// @brief Executes `f` by passing the system as the first
            /// argument, and an "executor proxy" as the second argument.
            template <typename TF>
            constexpr auto detailed(TF&& f) noexcept;

            /// @brief Executes `f` on every system subtask, passing the system
            /// as the first argument, and a "data proxy" as the second
            /// argument.
            template <typename TF>
            constexpr auto for_subtasks(TF&& f) noexcept;
        };
    } // namespace impl
} // namespace ecst::sys_exec
