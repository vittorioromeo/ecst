// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/tag/system.hpp>

namespace ecst::signature::system
{
    namespace impl
    {
        /// @brief Empty struct representing the lack of an output.
        struct empty_output_type
        {
        };

        /// @brief Tag type for the system output.
        template <typename T>
        struct output_impl : bh::type<T>
        {
        };

        /// @brief Evaluates to true if `T` is a valid wrapped output type.
        template <typename T>
        constexpr auto is_valid_output = is_specialization_of_v<T, output_impl>;
    } // namespace impl

    /// @brief Wrapper for the system's output type `T`.
    template <typename T>
    constexpr impl::output_impl<T> output{};

    /// @brief Wrapper for the system's empty output type.
    constexpr auto no_output = output<impl::empty_output_type>;
} // namespace ecst::signature::system
