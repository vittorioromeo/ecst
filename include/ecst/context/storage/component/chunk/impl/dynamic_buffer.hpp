// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vector>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/settings.hpp>
#include <ecst/debug.hpp>
#include <ecst/context/types.hpp>

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    namespace chunk
    {
        template <typename TSettings, typename TComponentTagList>
        class dynamic_buffer
        {
        public:
            using component_tag_list_type = TComponentTagList;

        private:
            using component_tuple_type =
                impl::component_tuple_type<TComponentTagList>;

        public:
            using settings_type = TSettings;

            struct metadata
            {
            };

        private:
            std::vector<component_tuple_type> _data;

            auto valid_index(sz_t i) const noexcept
            {
                return i >= 0 && i < _data.size();
            }

            auto entity_id_to_index(entity_id eid) const noexcept
            {
                return vrmc::to_sz_t(eid);
            }

            void grow_to(sz_t i)
            {
                auto target(std::max(10 + _data.size() * 2, i + 10));

                ELOG(                            // .
                    debug::lo_component_memory() // .
                        << "Growing dynamic component buffer chunk (i: "
                        << i // .
                        << ") (from " << _data.size() << " to " << target
                        << ")\n"; // .
                    );

                _data.resize(target);

                ECST_ASSERT_OP(_data.size(), >, i);
                ECST_ASSERT(valid_index(i));

                ELOG(                            // .
                    debug::lo_component_memory() // .
                        << "New occupied memory by dynamic component buffer " // .
                           "chunk: " // .
                        << (_data.capacity() * sizeof(component_tuple_type)) /
                               1024.f / 1024.f // .
                        << "MB\n";             // .
                    );
            }

            void grow_if_required(sz_t i)
            {
                if(unlikely(i >= _data.size()))
                {
                    grow_to(i);
                }
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
            dynamic_buffer()
            {
                grow_to(settings_type{}.get_dynamic_capacity());
            }

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
            auto& add(TComponentTag ct, entity_id eid, metadata& m)
            {
                auto i = entity_id_to_index(eid);
                grow_if_required(i);
                return get(ct, eid, m);
            }
        };
    }
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
