// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/system/tag.hpp>
#include <ecst/signature/system/settings.hpp>

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <                       // .
            typename TTag,               // .
            typename TParallelism,       // .
            typename TTagDependencyList, // .
            typename TComponentUseList,  // .
            typename TOutput             // .
            >
        struct data
        {
            ECST_S_ASSERT_DT(is_tag(TTag{}));

            // TODO:
            // ECST_S_ASSERT(parallelism::is_valid<TParallelism>);

            ECST_S_ASSERT(is_tag_dependency_list<TTagDependencyList, TTag>);
            ECST_S_ASSERT(output::is_valid<TOutput>);

            using tag = TTag;
            using parallelism = TParallelism;
            using tag_dependency_list = TTagDependencyList;
            using component_list = TComponentUseList;
            using output = TOutput;
        };

        template <typename TSystemSignature> // .
        using signature_tag_type =           // .
            typename TSystemSignature::tag;

        template <typename TSystemSignature> // .
        using signature_parallelism_type =   // .
            typename TSystemSignature::parallelism;

        template <typename TSystemSignature>      // .
        using signature_tag_depedency_list_type = // .
            typename TSystemSignature::tag_dependency_list;

        template <typename TSystemSignature>  // .
        using signature_component_list_type = // .
            typename TSystemSignature::component_list;

        template <typename TSystemSignature> // .
        using signature_output_type =        // .
            typename TSystemSignature::output;
    }

    template <typename TSystemSignature>
    using parallelism_policy =
        impl::signature_parallelism_type<TSystemSignature>;

    template <typename TSystemSignature, typename TComponent>
    constexpr auto can_mutate()
    {
        return bh::contains(                                   // .
            impl::signature_component_list_type<TSystemSignature>{}, // .
            impl::mutate_impl<TComponent>{});
    }

    template <typename TSystemSignature, typename TComponent>
    constexpr auto can_read()
    {
        return can_mutate<TSystemSignature, TComponent>() ||                // .
               bh::contains(                                          // .
                   impl::signature_component_list_type<TSystemSignature>{}, // .
                   impl::read_impl<TComponent>{});
    }

    template <typename TSystemSignature>
    constexpr auto has_data_output()
    {
        return output::is_data<impl::signature_output_type<TSystemSignature>>;
    }

    template <typename TSystemSignature>
    using output_data_type =
        typename impl::signature_output_type<TSystemSignature>::type;

    template <typename TSystemSignature>
    using component_uses_type =
        impl::signature_component_list_type<TSystemSignature>;

    template <typename TSystemSignature>
    using tag_dependency_list_type =
        impl::signature_tag_depedency_list_type<TSystemSignature>;
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
