// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../base.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/context/bitset.hpp>
#include <ecst/context/types.hpp>
#include <ecst/inner_parallelism.hpp>
#include <ecst/mp.hpp>
#include <ecst/settings.hpp>
#include <ecst/signature.hpp>
#include <ecst/signature_list.hpp>
#include <ecst/utils/sparse_int_set.hpp>

namespace ecst::context::system
{
    // TODO:
    // TODO: `component_instance` ?
    // TODO: what about non-stateful component instances?
    template <typename TSettings, typename TSystemSignature>
    class stateless_instance
        : public impl::instance_base<TSettings, TSystemSignature>
    {
    private:
        using base_type = impl::instance_base<TSettings, TSystemSignature>;

    public:
        using system_tag_type = typename base_type::system_tag_type;
        using system_type = typename base_type::system_type;

    protected:
        using this_type = stateless_instance<TSettings, TSystemSignature>;

        using parallel_parameters_type = // .
            signature::system::parallelism_type<TSystemSignature>;

        using parallel_executor_type = // .
            inner_parallelism::executor_type<parallel_parameters_type>;

        parallel_executor_type _parallel_executor;

    public:
        stateless_instance() = default;

        // TODO: implement
        template <typename TContext, typename TF>
        void execute(TContext& ctx, TF& f)
        {
            // TODO: should probably have its own executor_proxy
            (void)ctx;
            (void)f;
        }

    protected:
        auto& parallel_executor() noexcept
        {
            return _parallel_executor;
        }

        const auto& parallel_executor() const noexcept
        {
            return _parallel_executor;
        }
    };
} // namespace ecst::context::system
