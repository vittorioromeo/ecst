// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/tag/component.hpp>
#include <ecst/context/storage/component/chunk.hpp>

ECST_SIGNATURE_COMPONENT_NAMESPACE
{
    namespace impl
    {
        namespace keys
        {
            constexpr auto storage = sz_v<0>;
        }

        class contiguous_buffer_maker_t
        {
        public:
            template <typename TSettings, typename TComponentTagList>
            constexpr auto make_type() const noexcept
            {
                namespace sc = ecst::context::storage::component;

                // TODO:
                return static_if(settings::has_fixed_entity_storage<TSettings>)
                    .then([](auto s)
                        {
                            auto sz = settings::fixed_capacity(s);

                            return mp::type_c< // .
                                sc::chunk::fixed_buffer<TComponentTagList,
                                    ECST_DECAY_DECLTYPE(sz)> // .
                                >;
                        })
                    .else_([](auto s)
                        {
                            return mp::type_c<             // .
                                sc::chunk::dynamic_buffer< // .
                                    ECST_DECAY_DECLTYPE(s),
                                    TComponentTagList> // .
                                >;
                        })(TSettings{});
            }
        };

        constexpr contiguous_buffer_maker_t contiguous_buffer_maker{};

        template <typename TComponentTagList, typename TOptions>
        class data
        {
            ECST_S_ASSERT_DT(tag::component::is_list(TComponentTagList{}));

        private:
            template <typename TKey, typename T>
            constexpr auto change_self(const TKey& key, T&& x) noexcept
            {
                auto new_options = TOptions{}.set(key, FWD(x));
                return data<TComponentTagList,
                    ECST_DECAY_DECLTYPE(new_options)>{};
            }

        public:
            constexpr auto contiguous_buffer() noexcept
            {
                return change_self(keys::storage, contiguous_buffer_maker);
            }

            // TODO:
            template <typename TSettings>
            constexpr auto make_storage_type() noexcept
            {
                return TOptions{}
                    .at(keys::storage)
                    .template make_type<TSettings, TComponentTagList>();
            }

            template <typename TComponentTag>
            constexpr auto has(TComponentTag ct) noexcept
            {
                return bh::contains(TComponentTagList{}, ct);
            }

            constexpr auto tag_list() noexcept
            {
                return TComponentTagList{};
            }
        };

        /*
        /// @brief Alias for the tag type inside `TComponentSignature`.
        template <typename TComponentSignature>
        using tag_type = typename mp::unwrap<TComponentSignature>::tag;
        */
    }
}
ECST_SIGNATURE_COMPONENT_NAMESPACE_END
