// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/context/storage/component/chunk.hpp>
#include <ecst/mp.hpp>
#include <ecst/signature/component/data.hpp>

ECST_SIGNATURE_COMPONENT_NAMESPACE
{
    namespace impl
    {
        // TODO: move, make it easy for users to build their own
        struct contiguous_buffer_maker_t
        {
            template <typename TSettings, typename TComponentTagList>
            constexpr auto make_type(TSettings s, TComponentTagList) const
                noexcept
            {
                namespace sc = context::storage::component;

                return settings::dispatch_on_storage_type(
                    s,
                    [](auto fixed_capacity) {
                        return mp::type_c< // .
                            sc::chunk::fixed_buffer<TComponentTagList,
                                ECST_DECAY_DECLTYPE(fixed_capacity)> // .
                            >;
                    },
                    [](auto) {
                        return mp::type_c< // .
                            sc::chunk::dynamic_buffer<TSettings,
                                TComponentTagList> // .
                            >;
                    });
            }
        };

        struct empty_maker_t
        {
            template <typename TSettings, typename TComponentTagList>
            constexpr auto make_type(TSettings, TComponentTagList) const
                noexcept
            {
                namespace sc = context::storage::component;
                return mp::type_c<sc::chunk::empty<TComponentTagList>>;
            }
        };

        constexpr contiguous_buffer_maker_t contiguous_buffer_maker{};
        constexpr empty_maker_t empty_maker{};

        struct default_maker_dispatch_t
        {
            template <typename TSettings, typename TComponentTagList>
            constexpr auto make_type(TSettings s, TComponentTagList ctl) const
                noexcept
            {
                namespace sc = context::storage::component;

                auto all_empty = bh::all_of(ctl, [](auto ct) {
                    using component_type =
                        tag::component::unwrap<ECST_DECAY_DECLTYPE(ct)>;

                    return std::is_empty<component_type>{};
                });

                if constexpr(ECST_DECAY_DECLTYPE(all_empty){})
                {
                    return empty_maker.make_type(s, ctl);
                }
                else
                {
                    return contiguous_buffer_maker.make_type(s, ctl);
                }
            }
        };

        constexpr default_maker_dispatch_t default_maker{};
    } // namespace impl

    template <typename... TComponentTags>
    constexpr auto make(TComponentTags... cts) noexcept
    {
        auto ct_list = mp::list::make(cts...);

        constexpr auto d_opts =    // .
            mp::option_map::make() // .
                .add(impl::keys::storage, impl::default_maker);

        return impl::data<                // .
            ECST_DECAY_DECLTYPE(ct_list), // .
            ECST_DECAY_DECLTYPE(d_opts)   // .
            >{};
    }

    namespace impl
    {
        template <typename TComponentTagList, typename TOptions>
        constexpr auto
        data<TComponentTagList, TOptions>::contiguous_buffer() const noexcept
        {
            return change_self(keys::storage, contiguous_buffer_maker);
        }
    } // namespace impl
}
ECST_SIGNATURE_COMPONENT_NAMESPACE_END
