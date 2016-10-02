// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/tag.hpp>
#include <ecst/signature/system.hpp>

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace impl
    {
        /// @brief Type of the system tag used by an instance with `TSettings`
        /// and `TSystemSignature` template parameters.
        template <typename TSettings, typename TSystemSignature>
        using instance_system_tag_type =
            signature::system::tag_type<TSystemSignature>;

        /// @brief Type of the system used by an instance with `TSettings` and
        /// `TSystemSignature` template parameters.
        template <typename TSettings, typename TSystemSignature>
        using instance_system_type = tag::system::unwrap<
            instance_system_tag_type<TSettings, TSystemSignature>>;

        /// @brief TODO:
        /// @details Uses EBO for the stored system.
        template <typename TSettings, typename TSystemSignature>
        class instance_base
            : public instance_system_type<TSettings, TSystemSignature>
        {
        public:
            using signature_type = mp::unwrap<TSystemSignature>;

            using system_tag_type =
                instance_system_tag_type<TSettings, TSystemSignature>;

            using system_type =
                instance_system_type<TSettings, TSystemSignature>;

            instance_base() = default;

            // Prevent copies.
            instance_base(const instance_base&) = delete;
            instance_base& operator=(const instance_base&) = delete;

            // Allow moves.
            instance_base(instance_base&&) = default;
            instance_base& operator=(instance_base&&) = default;

            /// @brief Returns a reference to the stored system instance.
            auto& system() noexcept;

            /// @brief Returns a const reference to the stored system instance.
            const auto& system() const noexcept;

            // TODO:
            constexpr auto signature() const noexcept
            {
                return signature_type{};
            }

            template <typename TSystem>
            constexpr auto system_is() const noexcept
            {
                return std::is_same<system_type, TSystem>{};
            }

            template <typename TKind>
            constexpr auto kind_is(TKind kind) const noexcept
            {
                return signature().is_kind(kind);
            }

            // TODO:
            // constexpr auto is_stateless() const noexcept
            // {
            //     return std::is_same<ECST_DECAY_DECLTYPE(
            // }
        };

        // TODO: component-only systems with no knowledge of entities for SIMD
        // operations (?)

        // TODO: hierarchy of instances:
        /*
            `execution_step` (base type):
                * can produce/consume outputs
                * can have dependencies
                * can be parallelized (but quite explicitly)

            `entity_system : execution_step`: instance

            `component_system : execution_step`:
                * no knowledge of entities
                * iterates over component
                * SIMD support, etc
                * (necessary?) is it just execution_step?
        */
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
