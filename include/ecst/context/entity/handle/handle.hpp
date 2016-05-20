// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/context/types.hpp>

ECST_CONTEXT_STORAGE_ENTITY_NAMESPACE
{
    namespace impl
    {
        template <typename TContainer>
        class data;
    }
}
ECST_CONTEXT_STORAGE_ENTITY_NAMESPACE_END

ECST_CONTEXT_ENTITY_NAMESPACE
{
    namespace impl
    {
        using counter = ::ecst::impl::counter;

        /// @brief Handle class used to track a specific entity.
        class handle
        {
            template <typename>
            friend class ::ecst::context::storage::entity::impl::data;

        private:
            entity_id _id;
            counter _ctr;

        public:
            handle() noexcept;
            handle(entity_id id, counter ctr) noexcept;

            handle(const handle&) noexcept = default;
            handle& operator=(const handle&) noexcept = default;

            handle(handle&&) noexcept = default;
            handle& operator=(handle&&) noexcept = default;
        };
    }
}
ECST_CONTEXT_ENTITY_NAMESPACE_END
