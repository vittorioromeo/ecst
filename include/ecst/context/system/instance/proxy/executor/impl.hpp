// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

namespace ecst::context::system
{
    namespace executor_proxy
    {
        template <typename TInstance, typename TFExecution>
        class data
        {
        private:
            TInstance& _instance;
            TFExecution _execution;

        public:
            template <typename TFwdFExecution>
            data(TInstance& instance, TFwdFExecution&& execution) noexcept
                : _instance{instance}, _execution(FWD(execution))
            {
            }

            template <typename TF>
            auto for_subtasks(TF&& f) noexcept
            {
                return _execution(f);
            }

            auto& instance() noexcept
            {
                return _instance;
            }

            const auto& instance() const noexcept
            {
                return _instance;
            }

            auto& system() noexcept
            {
                return _instance.system();
            }

            const auto& system() const noexcept
            {
                return _instance.system();
            }
        };
    } // namespace executor_proxy
} // namespace ecst::context::system
