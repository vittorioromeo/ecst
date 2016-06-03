// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/context/types.hpp>
#include <ecst/settings.hpp>
#include <ecst/mp.hpp>

ECST_CONTEXT_BITSET_NAMESPACE
{
    namespace impl
    {
        /// @brief Helper class that assigns a specific bit to a specific
        /// component type.
        template <typename TSettings>
        class bits
        {
        private:
            static constexpr auto csl() noexcept
            {
                return settings::component_signature_list(TSettings());
            }

        public:
            static constexpr auto component_count() noexcept
            {
                return bh::size(
                    signature_list::component::all_components(csl()));
            }

            static constexpr auto total_count() noexcept
            {
                return component_count();
            }

            using bitset_type = ecst::impl::bitset<total_count()>;

            template <typename TID>
            static constexpr auto valid_component_id(TID id) noexcept
            {
                return id >= mp::sz_v<0> && id < component_count();
            }

            template <typename TID>
            static constexpr auto component_id_to_bit(TID id) noexcept
            {
                return id;
            }

            template <typename TComponent>
            static constexpr auto component_id() noexcept
            {
                constexpr auto ct = tag::component::v<TComponent>;
                return mp::list::index_of(
                    signature_list::component::all_components(csl()), ct);
            }

            template <typename TComponent>
            static constexpr auto component_bit() noexcept
            {
                return component_id_to_bit(component_id<TComponent>());
            }

            template <typename TBit>
            static constexpr auto valid_component_bit(TBit bit) noexcept
            {
                return bit >= mp::sz_v<0> && bit < component_count();
            }
        };
    }
}
ECST_CONTEXT_BITSET_NAMESPACE_END
