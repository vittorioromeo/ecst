// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/list.hpp>
#include "./tag.hpp"
#include "./settings.hpp"

// TODO: cleanup

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <                           // .
            typename TTag,                   // .
            typename TParallelizability,     // .
            typename TTagDependencyList,     // .
            typename TReadComponentTagList,  // .
            typename TWriteComponentTagList, // .
            typename TOutput                 // .
            >
        constexpr auto make_impl(   // .
            TTag,                   // .
            TParallelizability,     // .
            TTagDependencyList,     // .
            TReadComponentTagList,  // .
            TWriteComponentTagList, // .
            TOutput                 // .
            );

        template <                           // .
            typename TTag,                   // .
            typename TParallelism,           // .
            typename TTagDependencyList,     // .
            typename TReadComponentTagList,  // .
            typename TWriteComponentTagList, // .
            typename TOutput                 // .
            >
        class data
        {
            // ECST_S_ASSERT_DT(is_tag(TTag{}));

            // TODO:
            // ECST_S_ASSERT(parallelism::is_valid<TParallelism>);

            // ECST_S_ASSERT(is_tag_dependency_list<TTagDependencyList, TTag>);
            // ECST_S_ASSERT(output::is_valid<TOutput>);

        public:
            using tag_type = TTag;
            using parallelism_type = TParallelism;
            using tag_dependency_list_type = TTagDependencyList;
            using read_component_tag_list_type = TReadComponentTagList;
            using write_component_tag_list_type = TWriteComponentTagList;
            using output_type = TOutput;

        private:
        public:
            // TODO: checks/assertions
            template <typename TNewParallelism>
            constexpr auto parallelism(TNewParallelism np) noexcept
            {
                return make_impl(tag_type{}, np, tag_dependency_list_type{},
                    read_component_tag_list_type{},
                    write_component_tag_list_type{}, output_type{});
            }

            template <typename... TSystemTags>
            constexpr auto dependencies(TSystemTags... sts) noexcept
            {
                return make_impl(tag_type{}, parallelism_type{},
                    mp::list::make(sts...), read_component_tag_list_type{},
                    write_component_tag_list_type{}, output_type{});
            }

            template <typename... TComponentTags>
            constexpr auto read(TComponentTags... cts) noexcept
            {
                return make_impl(tag_type{}, parallelism_type{},
                    tag_dependency_list_type{}, mp::list::make(cts...),
                    write_component_tag_list_type{}, output_type{});
            }

            template <typename... TComponentTags>
            constexpr auto write(TComponentTags... cts) noexcept
            {
                return make_impl(tag_type{}, parallelism_type{},
                    tag_dependency_list_type{}, read_component_tag_list_type{},
                    mp::list::make(cts...), output_type{});
            }

            template <typename TWrappedOutput>
            constexpr auto output(TWrappedOutput wo) noexcept
            {
                return make_impl(tag_type{}, parallelism_type{},
                    tag_dependency_list_type{}, read_component_tag_list_type{},
                    write_component_tag_list_type{}, wo);
            }
        };


        template <                           // .
            typename TTag,                   // .
            typename TParallelizability,     // .
            typename TTagDependencyList,     // .
            typename TReadComponentTagList,  // .
            typename TWriteComponentTagList, // .
            typename TOutput                 // .
            >
        constexpr auto make_impl(   // .
            TTag,                   // .
            TParallelizability,     // .
            TTagDependencyList,     // .
            TReadComponentTagList,  // .
            TWriteComponentTagList, // .
            TOutput                 // .
            )
        {
            return data<                // .
                TTag,                   // .
                TParallelizability,     // .
                TTagDependencyList,     // .
                TReadComponentTagList,  // .
                TWriteComponentTagList, // .
                TOutput                 // .
                >{};
        }

        template <typename TSystemSignature> // .
        using signature_tag_type =           // .
            typename mp::unwrap<TSystemSignature>::tag_type;

        template <typename TSystemSignature> // .
        using signature_parallelism_type =   // .
            typename mp::unwrap<TSystemSignature>::parallelism_type;

        template <typename TSystemSignature>      // .
        using signature_tag_depedency_list_type = // .
            typename mp::unwrap<TSystemSignature>::tag_dependency_list_type;

        template <typename TSystemSignature>           // .
        using signature_read_component_tag_list_type = // .
            typename mp::unwrap<TSystemSignature>::read_component_tag_list_type;

        template <typename TSystemSignature>            // .
        using signature_write_component_tag_list_type = // .
            typename mp::unwrap<
                TSystemSignature>::write_component_tag_list_type;

        template <typename TSystemSignature> // .
        using signature_output_type =        // .
            typename mp::unwrap<TSystemSignature>::output_type;
    }

    template <typename TSystemSignature>
    using parallelism_policy =
        impl::signature_parallelism_type<TSystemSignature>;

    template <typename TSystemSignature, typename TComponent>
    constexpr auto can_write()
    {
        return mp::bh::contains( // .
            impl::signature_write_component_tag_list_type<
                TSystemSignature>{}, // .
            impl::mutate_impl<TComponent>{});
    }

    template <typename TSystemSignature, typename TComponent>
    constexpr auto can_read()
    {
        return can_write<TSystemSignature, TComponent>() || // .
               mp::bh::contains(                            // .
                   impl::signature_read_component_tag_list_type<
                       TSystemSignature>{}, // .
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
    using read_component_tag_list_type =
        impl::signature_read_component_tag_list_type<TSystemSignature>;

    template <typename TSystemSignature>
    using write_component_tag_list_type =
        impl::signature_write_component_tag_list_type<TSystemSignature>;


    template <typename TSystemSignature>
    using tag_dependency_list_type =
        impl::signature_tag_depedency_list_type<TSystemSignature>;
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
