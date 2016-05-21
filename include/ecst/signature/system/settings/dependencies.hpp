// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/system/tag.hpp>

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    template <typename... TSystemTags>
    constexpr auto depends_on(TSystemTags... sts) noexcept
    {
        // TODO: asserts, checks, uniqueness
        return mp::list::make(sts...);
    }

    constexpr auto no_dependencies = depends_on();

    namespace impl
    {
        template <typename T, typename TCurrentSystemTag>
        auto is_tag_dependency_list_impl()
        {
            return mp::bh::all_of(T{}, [](auto xt)
                {
                    return is_tag(xt);
                });
        }

        template <typename T, typename TCurrentSystemTag>
        constexpr auto is_tag_dependency_list =
            decltype(is_tag_dependency_list_impl<T, TCurrentSystemTag>()){};
    }
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
