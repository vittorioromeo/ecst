// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp.hpp>
#include <ecst/tag/system.hpp>
#include "./impl/keys.hpp"
#include "./impl/output_wrapper.hpp"

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    // TODO: cleanup

    namespace impl
    {
        template <typename TTag, typename TOptions>
        class data
        {
        public:
// TODO:
#define TEMP(x) mp::option_map::type_of<TOptions, decltype(x)>

            using tag_type = TTag;
            using parallelism_type = TEMP(keys::parallelism);
            using tag_dependency_list_type = TEMP(keys::dependencies);
            using read_component_tag_list_type = TEMP(keys::read_components);
            using write_component_tag_list_type = TEMP(keys::write_components);
            using output_type = TEMP(keys::output);

#undef TEMP


            // Assert system tag validity.
            ECST_S_ASSERT_DT(tag::system::valid(TTag{}));

            // TODO:
            // ECST_S_ASSERT(parallelism::is_valid<TParallelism>);

            // Assert dependency list validity.
            ECST_S_ASSERT_DT(tag::system::is_valid(tag_dependency_list_type{}));

            // Assert read component list validity.
            ECST_S_ASSERT_DT(
                tag::component::is_valid(read_component_tag_list_type{}));

            // Assert write component list validity.
            ECST_S_ASSERT_DT(
                tag::component::is_valid(write_component_tag_list_type{}));

            // Assert that no component tags are shared between the write and
            // read lists.
            ECST_S_ASSERT_DT(!mp::list::any_common_element( // .
                read_component_tag_list_type{},
                write_component_tag_list_type{}));

            ECST_S_ASSERT_DT(impl::is_valid_output<output_type>);

        private:
            template <typename TKey, typename T>
            constexpr auto change_self(const TKey& key, T&& x) noexcept
            {
                auto new_options = TOptions{}.set(key, FWD(x));
                return data<TTag, std::decay_t<decltype(new_options)>>{};
            }

        public:
            template <typename TNewParallelism>
            constexpr auto parallelism(TNewParallelism new_parallelism) noexcept
            {
                return change_self(keys::parallelism, new_parallelism);
            }

            template <typename... TSystemTags>
            constexpr auto dependencies(TSystemTags... sts) noexcept
            {
                return change_self(keys::dependencies, mp::list::make(sts...));
            }

            template <typename... TComponentTags>
            constexpr auto read(TComponentTags... cts) noexcept
            {
                return change_self(
                    keys::read_components, mp::list::make(cts...));
            }

            template <typename... TComponentTags>
            constexpr auto write(TComponentTags... cts) noexcept
            {
                return change_self(
                    keys::write_components, mp::list::make(cts...));
            }

            template <typename TNewOutput>
            constexpr auto output(TNewOutput new_output) noexcept
            {
                return change_self(keys::output, new_output);
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
