// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../type.hpp"
#include "../utils.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/context/types.hpp>
#include <ecst/debug.hpp>
#include <ecst/settings.hpp>

namespace ecst::context::storage::component::chunk::impl
{
    template <typename TDerived, typename TMetadata,
        typename TComponentTagListType>
    class buffer_base
    {
    private:
        using derived_type = TDerived;
        using metadata_type = TMetadata;
        using component_tag_list_type = TComponentTagListType;

        auto& derived() noexcept
        {
            return vrmc::to_derived<derived_type>(*this);
        }
        const auto& derived() const noexcept
        {
            return vrmc::to_derived<derived_type>(*this);
        }

    protected:
        auto valid_index(sz_t i) const noexcept
        {
            return i >= 0 && i < derived()._data.size();
        }

        constexpr auto ECST_CONST_FN entity_id_to_index(entity_id eid) const
            noexcept
        {
            return vrmc::to_sz_t(eid);
        }

    private:
        template <typename TComponentTag, typename TSelf>
        decltype(auto) get_impl(TComponentTag ct, TSelf&& self, entity_id eid,
            const metadata_type&) noexcept
        {
            using component_type =
                tag::component::unwrap<ECST_DECAY_DECLTYPE(ct)>;

            auto i = self.entity_id_to_index(eid);
            ECST_ASSERT(self.valid_index(i));

            return vrmc::forward_like<TSelf>(
                std::get<component_type>(derived()._data[i]));
        }

    public:
        template <typename TComponentTag, typename... Ts>
            auto& get(TComponentTag ct, Ts&&... xs) & noexcept
        {
            return this->get_impl(ct, *this, FWD(xs)...);
        }

        template <typename TComponentTag, typename... Ts>
        const auto& get(TComponentTag ct, Ts&&... xs) const& noexcept
        {
            return this->get_impl(ct, *this, FWD(xs)...);
        }

        template <typename TComponentTag>
        auto& add(TComponentTag ct, entity_id eid, metadata_type& m)
        {
            auto i = this->entity_id_to_index(eid);
            derived().grow_if_required(i);
            return this->get(ct, eid, m);
        }
    };
} // namespace ecst::context::storage::component::chunk::impl
