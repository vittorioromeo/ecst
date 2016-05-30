// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp.hpp>
#include <ecst/signature/component/data.hpp>

ECST_SIGNATURE_COMPONENT_NAMESPACE
{
    namespace impl
    {
        // TODO: move, make it easy for users to build their own
        class contiguous_buffer_maker_t
        {
        public:
            template <typename TSettings, typename TComponentTagList>
            constexpr auto make_type() const noexcept
            {
                namespace sc = ecst::context::storage::component;

                return settings::dispatch_on_storage_type(TSettings{},
                    [](auto fixed_capacity)
                    {
                        return mp::type_c< // .
                            sc::chunk::fixed_buffer<TComponentTagList,
                                ECST_DECAY_DECLTYPE(fixed_capacity)> // .
                            >;
                    },
                    [](auto)
                    {
                        return mp::type_c< // .
                            sc::chunk::dynamic_buffer<TSettings,
                                TComponentTagList> // .
                            >;
                    });
            }
        };

        constexpr contiguous_buffer_maker_t contiguous_buffer_maker{};
    }

    template <typename... TComponentTags>
    constexpr auto make(TComponentTags... cts) noexcept
    {
        auto ct_list = mp::list::make(cts...);

        constexpr auto d_opts =    // .
            mp::option_map::make() // .
                // TODO: sensible defaults for zero-sized components
                .add(impl::keys::storage, impl::contiguous_buffer_maker);

        return impl::data<                // .
            ECST_DECAY_DECLTYPE(ct_list), // .
            ECST_DECAY_DECLTYPE(d_opts)   // .
            >{};
    }

    namespace impl
    {
        template <typename TComponentTagList, typename TOptions>
        constexpr auto data<TComponentTagList,
            TOptions>::contiguous_buffer() noexcept
        {
            return change_self(keys::storage, contiguous_buffer_maker);
        }
    }
}
ECST_SIGNATURE_COMPONENT_NAMESPACE_END
