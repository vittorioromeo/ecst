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
    namespace impl
    {
        template <typename... TSystemTags>
        constexpr auto depends_on_impl(TSystemTags... sts)
        {
            return mp::list::make(sts...);
        }
    }

    template <typename... TSystemTags>
    constexpr auto depends_on_by_tags(TSystemTags... sts)
    {
        return impl::depends_on_impl(sts...);
    }

    namespace impl
    {
        template <typename... TSystems>
        constexpr auto depends_on_impl()
        {
            return mp::bh::basic_tuple<decltype(tag<TSystems>)...>{};
        }
    }

    template <typename... TSystems>
    constexpr auto depends_on = impl::depends_on_impl<TSystems...>();

    namespace impl
    {
        constexpr auto no_dependencies_impl()
        {
            return impl::depends_on_impl();
        }
    }

    constexpr auto no_dependencies = impl::no_dependencies_impl();

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
