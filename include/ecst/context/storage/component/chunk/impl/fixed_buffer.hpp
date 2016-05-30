// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <array>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/context/types.hpp>

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    namespace chunk
    {
        template <typename TComponentTagList, typename TN>
        class fixed_buffer
        {
        public:
            using component_tag_list_type = TComponentTagList;

        private:
            using component_tuple_type =
                impl::component_tuple_type<TComponentTagList>;

        public:
            struct metadata
            {
            };

        private:
            std::array<component_tuple_type, TN{}> _data;

            auto valid_index(sz_t i) const noexcept
            {
                return i >= 0 && i < _data.size();
            }

            auto entity_id_to_index(entity_id eid) const noexcept
            {
                return vrmc::to_sz_t(eid);
            }

            template <typename TComponentTag, typename TSelf>
            decltype(auto) get_impl(TComponentTag ct, TSelf&& self,
                entity_id eid, const metadata&) noexcept
            {
                using component_type =
                    tag::component::unwrap<ECST_DECAY_DECLTYPE(ct)>;

                auto i = self.entity_id_to_index(eid);
                ECST_ASSERT(self.valid_index(i));

                return vrmc::forward_like<TSelf>(
                    std::get<component_type>(_data[i]));
            }

        public:
            template <typename TComponentTag, typename... Ts>
                auto& get(TComponentTag ct, Ts&&... xs) & noexcept
            {
                return get_impl(ct, *this, FWD(xs)...);
            }

            template <typename TComponentTag, typename... Ts>
            const auto& get(TComponentTag ct, Ts&&... xs) const& noexcept
            {
                return get_impl(ct, *this, FWD(xs)...);
            }

            template <typename TComponentTag>
            auto& add(TComponentTag ct, entity_id eid, metadata& m) noexcept
            {
                return get(ct, eid, m);
            }
        };
    }
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
