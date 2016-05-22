// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/tag/system.hpp>

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    namespace impl
    {
        /// @brief Empty struct representing the lack of an output.
        struct empty_output_type
        {
        };

        /// @brief Tag type for the system output.
        template <typename T>
        struct output_impl : mp::bh::type<T>
        {
        };

        /// @brief Evaluates to true if `T` is a valid wrapped output type.
        template <typename T>
        constexpr auto is_valid_output =
            mp::is_specialization_of_v<output_impl, T>;
    }

    /// @brief Wrapper for the system's output type `T`.
    template <typename T>
    constexpr impl::output_impl<T> output{};

    /// @brief Wrapper for the system's empty output type.
    constexpr auto no_output = output<impl::empty_output_type>;

    namespace impl
    {
        template <                           // .
            typename TTag,                   // .
            typename TParallelism,           // .
            typename TTagDependencyList,     // .
            typename TReadComponentTagList,  // .
            typename TWriteComponentTagList, // .
            typename TOutput                 // .
            >
        struct data
        {
            // Assert system tag validity.
            ECST_S_ASSERT_DT(tag::system::valid(TTag{}));

            // TODO:
            // ECST_S_ASSERT(parallelism::is_valid<TParallelism>);

            // Assert dependency list validity.
            ECST_S_ASSERT_DT(tag::system::is_valid(TTagDependencyList{}));

            // Assert read component list validity.
            ECST_S_ASSERT_DT(
                tag::component::is_valid(TReadComponentTagList{}));

            // Assert write component list validity.
            ECST_S_ASSERT_DT(
                tag::component::is_valid(TWriteComponentTagList{}));

            // Assert that no component tags are shared between the write and
            // read lists.
            ECST_S_ASSERT_DT(!mp::list::any_common_element(
                TReadComponentTagList{}, TWriteComponentTagList{}));

            ECST_S_ASSERT_DT(impl::is_valid_output<TOutput>);

            using tag_type = TTag;
            using parallelism_type = TParallelism;
            using tag_dependency_list_type = TTagDependencyList;
            using read_component_tag_list_type = TReadComponentTagList;
            using write_component_tag_list_type = TWriteComponentTagList;
            using output_type = TOutput;

            template <typename TNewParallelism>
            constexpr auto parallelism(TNewParallelism) noexcept
            {
                return data<tag_type, TNewParallelism, tag_dependency_list_type,
                    read_component_tag_list_type, write_component_tag_list_type,
                    output_type>{};
            }

            template <typename... TSystemTags>
            constexpr auto dependencies(TSystemTags...) noexcept
            {
                return data<tag_type, parallelism_type,
                    mp::list::t<TSystemTags...>, read_component_tag_list_type,
                    write_component_tag_list_type, output_type>{};
            }

            template <typename... TComponentTags>
            constexpr auto read(TComponentTags...) noexcept
            {
                return data<tag_type, parallelism_type,
                    tag_dependency_list_type, mp::list::t<TComponentTags...>,
                    write_component_tag_list_type, output_type>{};
            }

            template <typename... TComponentTags>
            constexpr auto write(TComponentTags...) noexcept
            {
                return data<tag_type, parallelism_type,
                    tag_dependency_list_type, read_component_tag_list_type,
                    mp::list::t<TComponentTags...>, output_type>{};
            }

            template <typename TWrappedOutput>
            constexpr auto output(TWrappedOutput) noexcept
            {
                return data<tag_type, parallelism_type,
                    tag_dependency_list_type, read_component_tag_list_type,
                    write_component_tag_list_type, TWrappedOutput>{};
            }
        };
    }

    template <typename TSystemSignature> // .
    using tag_type =                     // .
        typename mp::unwrap<TSystemSignature>::tag_type;

    template <typename TSystemSignature> // .
    using parallelism_type =             // .
        typename mp::unwrap<TSystemSignature>::parallelism_type;

    template <typename TSystemSignature> // .
    using tag_depedency_list_type =      // .
        typename mp::unwrap<TSystemSignature>::tag_dependency_list_type;

    template <typename TSystemSignature> // .
    using read_component_tag_list_type = // .
        typename mp::unwrap<TSystemSignature>::read_component_tag_list_type;

    template <typename TSystemSignature>  // .
    using write_component_tag_list_type = // .
        typename mp::unwrap<TSystemSignature>::write_component_tag_list_type;

    template <typename TSystemSignature> // .
    using output_type =                  // .
        typename mp::unwrap<TSystemSignature>::output_type;

    // TODO: use
    template <typename TSystemSignature, typename TComponentTag>
    constexpr auto can_write(TComponentTag ct)
    {
        return mp::bh::contains(                               // .
            write_component_tag_list_type<TSystemSignature>{}, // .
            ct);
    }

    // TODO: use
    template <typename TSystemSignature, typename TComponentTag>
    constexpr auto can_read(TComponentTag ct)
    {
        return can_write<TSystemSignature>(ct) ||                    // .
               mp::bh::contains(                                     // .
                   read_component_tag_list_type<TSystemSignature>{}, // .
                   ct);
    }

    template <typename TSystemSignature>
    constexpr auto has_data_output()
    {
        return !std::is_same<output_type<TSystemSignature>,
            impl::output_impl<impl::empty_output_type>>{};
    }
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
