// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "./macros.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

ECST_NAMESPACE
{
    namespace impl
    {
        using name_type = const char*;

        template <typename TSystem>
        constexpr impl::name_type system_name{"unnamed system"};

        template <typename TComponent>
        constexpr impl::name_type component_name{"unnamed component"};
    } // namespace impl
}
ECST_NAMESPACE_END

#define ECST_IMPL_SPECIALIZE_NAME(type, ...)                            \
                                                                        \
    namespace ecst                                                      \
    {                                                                   \
        namespace impl                                                  \
        {                                                               \
            template <>                                                 \
            constexpr name_type VRM_PP_CAT(type, _name)<__VA_ARGS__>{   \
                VRM_PP_TOSTR(__VA_ARGS__)};                             \
                                                                        \
            ECST_IMPL_UNUSED_FN()                                       \
            {                                                           \
                (void)ecst::impl::VRM_PP_CAT(type, _name)<__VA_ARGS__>; \
            }                                                           \
        }                                                               \
    }                                                                   \
                                                                        \
    ECST_IMPL_SEMICOLON_SUPPRESSOR()

#define ECST_SPECIALIZE_SYSTEM_NAME(...) \
    ECST_IMPL_SPECIALIZE_NAME(system, __VA_ARGS__)

#define ECST_SPECIALIZE_COMPONENT_NAME(...) \
    ECST_IMPL_SPECIALIZE_NAME(component, __VA_ARGS__)
