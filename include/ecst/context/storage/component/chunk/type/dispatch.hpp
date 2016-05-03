// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/settings.hpp>
#include "../utils.hpp"
#include "../impl.hpp"

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    namespace impl
    {
        template <typename TComponent, typename TSettings>
        auto dispatch_normal() noexcept
        {
            return static_if(settings::has_fixed_entity_storage<TSettings>)
                .then([](auto ts)
                    {
                        return mp::type_v<                           // .
                            chunk::fixed_buffer<TComponent,          // .
                                sz_t_<settings::fixed_capacity(ts)>> // .
                            >;
                    })
                .else_([](auto)
                    {
                        return mp::type_v<                               // .
                            chunk::dynamic_buffer<TSettings, TComponent> // .
                            >;
                    })(TSettings{});
        }

        template <typename TComponent, typename TSettings>
        auto dispatch_tw() noexcept
        {
            return static_if(std::is_empty<TComponent>{})
                .then([]()
                    {
                        return mp::type_v<chunk::empty<TComponent>>;
                    })
                .else_([]()
                    {
                        return dispatch_normal<TComponent, TSettings>();
                    })();
        }
    }

    // TODO: use hash_map, refactor, add "force_container (?)" to component
    // signature.
    template <typename TComponent, typename TSettings>
    using chunk_dispatch_type =
        mp::unwrap<decltype(impl::dispatch_tw<TComponent, TSettings>())>;
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
