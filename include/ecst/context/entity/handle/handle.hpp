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
        struct uninitialized_handle_init
        {
        };

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
            /// @brief Default-construct an "invalid" handle.
            handle() noexcept;

            /// @brief Uninitialized handle.
            handle(uninitialized_handle_init) noexcept;

            /// @brief Handle pointing to `id; ctr`.
            handle(entity_id id, counter ctr) noexcept;

            // `noexcept` copies.
            handle(const handle&) noexcept = default;
            handle& operator=(const handle&) noexcept = default;

            // `noexcept` moves.
            handle(handle&&) noexcept = default;
            handle& operator=(handle&&) noexcept = default;
        };
    }

    /// @brief Special `constexpr` value that can be passed to `handle`'s
    /// constructor to create uninitialized handles.
    constexpr impl::uninitialized_handle_init uninitialized_handle{};
}
ECST_CONTEXT_ENTITY_NAMESPACE_END
