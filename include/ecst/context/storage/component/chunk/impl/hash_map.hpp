// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <unordered_map>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/context/types.hpp>

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    namespace chunk
    {
        template <typename TComponent>
        class hash_map
        {
        public:
            using component_type = TComponent;

            struct metadata
            {
            };

        private:
            std::unordered_map<sz_t, TComponent> _data;

            auto valid_index(sz_t i) const noexcept
            {
                return _data.count(i) > 0;
            }

            auto entity_id_to_index(entity_id eid) const noexcept
            {
                return vrmc::to_sz_t(eid);
            }

            template <typename TSelf>
            decltype(auto) get_impl(
                TSelf&& self, entity_id eid, const metadata&) noexcept
            {
                auto i = self.entity_id_to_index(eid);
                ECST_ASSERT(self.valid_index(i));

                return vrmc::forward_like<TSelf>(_data[i]);
            }

            template <typename TSelf>
            decltype(auto) add_impl(
                TSelf&& self, entity_id eid, const metadata&) noexcept
            {
                auto i = self.entity_id_to_index(eid);
                return vrmc::forward_like<TSelf>(_data[i]);
            }

        public:
            template <typename... Ts>
                auto& get(Ts&&... xs) & noexcept
            {
                return get_impl(*this, FWD(xs)...);
            }

            template <typename... Ts>
            const auto& get(Ts&&... xs) const& noexcept
            {
                return get_impl(*this, FWD(xs)...);
            }

            template <typename... Ts>
            auto& add(Ts&&... xs)
            {
                return add_impl(*this, FWD(xs)...);
            }
        };
    }
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
