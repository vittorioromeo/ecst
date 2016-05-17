// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./instance/data_proxy.hpp"
#include "./instance/instance.hpp"

// TODO: move
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>

ECST_NAMESPACE
{
    namespace impl
    {
        using name_type = const char*;

        template <typename TSystem>
        constexpr impl::name_type system_name{"unnamed system"};

        template <typename TComponent>
        constexpr impl::name_type component_name{"unnamed component"};
    }
}
ECST_NAMESPACE_END

#define ECST_IMPL_SEMICOLON_SUPPRESSOR()                                     \
    struct VRM_CORE_UNUSED_FN VRM_PP_CAT(ss, __LINE__, semicolon_suppressor) \
    {                                                                        \
    }

#define ECST_SPECIALIZE_SYSTEM_NAME(...)                      \
                                                              \
    namespace ecst                                            \
    {                                                         \
        namespace impl                                        \
        {                                                     \
            template <>                                       \
            constexpr name_type system_name<__VA_ARGS__>{     \
                VRM_PP_TOSTR(__VA_ARGS__)};                   \
                                                              \
            void VRM_CORE_UNUSED_FN ECST_CONST_FN VRM_PP_CAT( \
                ws, __LINE__, warning_suppressor)()           \
            {                                                 \
                (void) ecst::impl::system_name<__VA_ARGS__>;  \
            }                                                 \
        }                                                     \
    }                                                         \
                                                              \
    ECST_IMPL_SEMICOLON_SUPPRESSOR()


#define ECST_SPECIALIZE_COMPONENT_NAME(...)                     \
                                                                \
    namespace ecst                                              \
    {                                                           \
        namespace impl                                          \
        {                                                       \
            template <>                                         \
            constexpr name_type component_name<__VA_ARGS__>{    \
                VRM_PP_TOSTR(__VA_ARGS__)};                     \
                                                                \
            void VRM_CORE_UNUSED_FN ECST_CONST_FN VRM_PP_CAT(   \
                ws, __LINE__, warning_suppressor)()             \
            {                                                   \
                (void) ecst::impl::component_name<__VA_ARGS__>; \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    ECST_IMPL_SEMICOLON_SUPPRESSOR()



// TODO: move up
#include "./instance/instance.inl"
#include "./instance/instance_subtask.inl"
