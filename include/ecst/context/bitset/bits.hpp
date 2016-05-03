// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/context/types.hpp>
#include <ecst/settings.hpp>

ECST_CONTEXT_BITSET_NAMESPACE
{
    namespace impl
    {
        template <typename TSettings>
        class bits
        {
        private:
            static constexpr auto my_settings = TSettings{};
            static constexpr auto my_csl =
                settings::component_signature_list(my_settings);

        public:
            static constexpr auto component_count() noexcept
            {
                return mp::list::size(my_csl);
            }

            static constexpr auto total_count() noexcept
            {
                return component_count();
            }

            using bitset_type = ecst::impl::bitset<total_count()>;

            template <typename TID>
            static constexpr auto valid_component_id(TID id) noexcept
            {
                return id >= 0 && id < component_count();
            }

            template <typename TID>
            static constexpr auto component_id_to_bit(TID id) noexcept
            {
                return id;
            }

            template <typename TComponent>
            static constexpr auto component_id() noexcept
            {
                return signature_list::component::id_by_type<TComponent>(
                    my_csl);
            }

            template <typename TComponent>
            static constexpr auto component_bit() noexcept
            {
                return component_id_to_bit(component_id<TComponent>());
            }

            template <typename TBit>
            static constexpr auto valid_component_bit(TBit bit) noexcept
            {
                return bit >= 0 && bit < component_count();
            }
        };
    }
}
ECST_CONTEXT_BITSET_NAMESPACE_END
