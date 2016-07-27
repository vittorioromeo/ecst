// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include "./dependencies.hpp"
#include "./aliases.hpp"

ECST_FUNCTION_QUEUE_NAMESPACE
{
    /// @brief Namespace dealing with vtable options and construction.
    namespace vtable
    {
        namespace option
        {
            using call_t = bh::int_<0>;
            using dtor_t = bh::int_<1>;
            using copy_t = bh::int_<2>;
            using move_t = bh::int_<3>;

            /// @brief Enables the vtable to store a call operator ptr.
            constexpr call_t call{};

            /// @brief Enables the vtable to store a dtor ptr.
            constexpr dtor_t dtor{};

            /// @brief Enables the vtable to store a copy ctor ptr.
            constexpr copy_t copy{};

            /// @brief Enables the vtable to store a move ctor ptr.
            constexpr move_t move{};

            /// @brief Create a list of options that can be passed to
            /// `vtable::make`.
            template <typename... TOptions>
            auto make_list(TOptions... os) noexcept
            {
                // TODO: statically assert that `os...` are options.
                auto result = bh::make_basic_tuple(os...);

                VRM_CORE_STATIC_ASSERT_NM(decltype(
                    bh::equal(bh::unique(bh::sort(result)), result)){});

                return result;
            }
        }

        namespace impl
        {
            /// @brief Helper struct that creates the vtable.
            template <typename TSignature>
            class maker;

            template <typename TReturn, typename... TArgs>
            class maker<TReturn(TArgs...)>
            {
            private:
                // TODO: fix docs
                /// @brief Call function pointer type.
                /// @details The first parameter is the callable object.
                using call_fp = fn_ptr<TReturn(char*, TArgs...)>;

                /// @brief Dtor function pointer type.
                /// @details The first parameter is the callable object.
                using dtor_fp = fn_ptr<void(char*)>;

                /// @brief Copy ctor function pointer type.
                /// @details The first parameter is the vtable.
                /// The second parameter is the src callable object.
                /// The third parameter is the dst callable object.
                using copy_fp = fn_ptr<void(const char*, char*)>;

                /// @brief Move ctor function pointer type.
                /// @details The first parameter is the vtable.
                /// The second parameter is the src callable object.
                /// The third parameter is the dst callable object.
                using move_fp = fn_ptr<void(char*, char*)>;

            public:
                template <typename TOptionsList>
                static auto make(TOptionsList ol) noexcept
                {
                    // Map the options to their function pointer types.
                    auto fp_tuple = bh::make_basic_tuple(       // .
                        bh::make_pair(option::call, call_fp{}), // .
                        bh::make_pair(option::dtor, dtor_fp{}), // .
                        bh::make_pair(option::copy, copy_fp{}), // .
                        bh::make_pair(option::move, move_fp{})  // .
                        );

                    // Filter only the selected options (passed in `ol`).
                    auto fp_filtered_tuple = bh::filter(fp_tuple, [ol](auto x)
                        {
                            return bh::contains(ol, bh::first(x));
                        });

                    // Return the result as a `boost::hana::map`.
                    return bh::to_map(fp_filtered_tuple);
                }

                template <typename TF, typename TVTable>
                static void set_fp(option::call_t, TVTable& vt) noexcept
                {
                    bh::at_key(vt, option::call) = [](char* obj, TArgs... xs)
                    {
                        return reinterpret_cast<TF*>(obj)->operator()(xs...);
                    };
                }

                template <typename TF, typename TVTable>
                static void set_fp(option::dtor_t, TVTable& vt) noexcept
                {
                    bh::at_key(vt, option::dtor) = [](char* obj)
                    {
                        reinterpret_cast<TF*>(obj)->~TF();
                    };
                }

                template <typename TF, typename TVTable>
                static void set_fp(option::copy_t, TVTable& vt) noexcept
                {
                    bh::at_key(vt, option::copy) = [](
                        const char* src, char* dst)
                    {
                        // Copy-construct `src` into `dst`.
                        new(dst) TF(*(reinterpret_cast<const TF*>(src)));
                    };
                }

                template <typename TF, typename TVTable>
                static void set_fp(option::move_t, TVTable& vt) noexcept
                {
                    bh::at_key(vt, option::move) = [](char* src, char* dst)
                    {
                        // Move-construct `src` into `dst`.
                        new(dst) TF(std::move(*(reinterpret_cast<TF*>(src))));
                    };
                }
            };
        }

        /// @brief Creates a vtable for callable objects with signature
        /// `TSignature`, using the passed `ol` options.
        /// @details Option lists can be created using
        /// `vtable::option::make_list`.
        template <typename TSignature, typename TOptionsList>
        auto make(TOptionsList ol) noexcept
        {
            return impl::maker<TSignature>::make(ol);
        }

        template <typename TOption, typename TVTable, typename... Ts>
        void exec_fp(TOption o, TVTable& vt, Ts&&... xs)
        {
            auto& fp = bh::at_key(vt, o);
            (*fp)(FWD(xs)...);
        }

        template <typename TVTable, typename TOption>
        constexpr auto has_option(const TVTable& vt, TOption o) noexcept
        {
            return bh::contains(bh::keys(vt), o);
        }

        template <typename TF, typename TSignature, typename TVTable>
        void setup(TVTable& vt) noexcept
        {
            using sh = impl::maker<TSignature>;

            static_if(has_option(vt, option::call))
                .then([](auto& x_vt)
                    {
                        sh::template set_fp<TF>(option::call, x_vt);
                    })(vt);

            static_if(has_option(vt, option::dtor))
                .then([](auto& x_vt)
                    {
                        sh::template set_fp<TF>(option::dtor, x_vt);
                    })(vt);

            static_if(std::is_copy_constructible<TF>{} &&
                      has_option(vt, option::copy))
                .then([](auto& x_vt)
                    {
                        sh::template set_fp<TF>(option::copy, x_vt);
                    })(vt);

            static_if(std::is_move_constructible<TF>{} &&
                      has_option(vt, option::move))
                .then([](auto& x_vt)
                    {
                        sh::template set_fp<TF>(option::move, x_vt);
                    })(vt);
        }

        template <typename TSignature, typename... TOptions>
        using type =
            decltype(make<TSignature>(option::make_list(TOptions{}...)));
    }

    template <typename TSignature>
    using complete_vtable_type = vtable::type< // .
        TSignature,                            // .
        vtable::option::call_t,                // .
        vtable::option::dtor_t,                // .
        vtable::option::copy_t,                // .
        vtable::option::move_t                 // .
        >;
}
ECST_FUNCTION_QUEUE_NAMESPACE_END
