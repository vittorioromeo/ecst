// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/tag/component.hpp>

ECST_SIGNATURE_COMPONENT_NAMESPACE
{
    namespace impl
    {
        namespace keys
        {
            constexpr auto storage = sz_v<0>;
        }

        struct base_data
        {
        };

        template <typename TComponentTagList, typename TOptions>
        class data : base_data
        {
            ECST_S_ASSERT_DT(tag::component::is_list(TComponentTagList{}));

        private:
            template <typename TKey, typename T>
            constexpr auto change_self(const TKey& key, T&& x) const noexcept
            {
                auto new_options = TOptions{}.set(key, FWD(x));
                return data<TComponentTagList,
                    ECST_DECAY_DECLTYPE(new_options)>{};
            }

        public:
            constexpr auto contiguous_buffer() const noexcept;

            template <typename TSettings>
            constexpr auto make_storage_type(TSettings s) const noexcept
            {
                return TOptions{}
                    .at(keys::storage)
                    .make_type(s, TComponentTagList{});
            }

            template <typename TComponentTag>
            constexpr auto has(TComponentTag ct) const noexcept
            {
                return bh::contains(TComponentTagList{}, ct);
            }

            constexpr auto tag_list() const noexcept
            {
                return TComponentTagList{};
            }
        };

        // TODO: `data.set_storage_type(contiguous_buffer_maker)`
        // TODO: `data.set_storage_type(hash_map_maker)`
    }
}
ECST_SIGNATURE_COMPONENT_NAMESPACE_END
