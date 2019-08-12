// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../../state.hpp"
#include "../stateless.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    // TODO:
    // TODO: `component_instance` ?
    // TODO: what about non-stateful component instances?
    template <typename TSettings, typename TSystemSignature>
    class stateful_instance
        : public stateless_instance<TSettings, TSystemSignature>
    {
    private:
        using base_type = stateless_instance<TSettings, TSystemSignature>;

    public:
        using system_tag_type = typename base_type::system_tag_type;
        using system_type = typename base_type::system_type;

    protected:
        using this_type = stateful_instance<TSettings, TSystemSignature>;

        using state_manager_type = // .
            impl::state_manager::data<this_type, TSettings, TSystemSignature>;

        using parallel_parameters_type = // .
            typename base_type::parallel_parameters_type;

        using parallel_executor_type = // .
            typename base_type::parallel_executor_type;

        state_manager_type _sm;

    public:
        stateful_instance() noexcept : _sm{*this}
        {
        }

        /// @brief Executes `f` on all subtask states.
        template <typename TF>
        decltype(auto) for_states(TF&& f)
        {
            return this->state_manager().for_states(FWD(f));
        }

    protected:
        auto& state_manager() noexcept
        {
            return _sm;
        }

        const auto& state_manager() const noexcept
        {
            return _sm;
        }
    };
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
