// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include "./dependencies.hpp"
#include "./aliases.hpp"
#include "./vtable.hpp"
#include "./utils.hpp"

// TODO:
ECST_FUNCTION_QUEUE_NAMESPACE
{
    namespace impl
    {
        namespace storage
        {
            template <typename TSignature, typename TVTable, sz_t TBufferSize>
            class fixed
            {
            private:
                /// @brief Signature of the stored callable objects.
                using signature = TSignature;

                /// @brief Vtable that handles callable object operations.
                using vtable_type = TVTable;

                /// @brief Size of the fixed buffer.
                static constexpr auto buffer_size = TBufferSize;

                /// @brief Alignment of vtables and callable objects.
                static constexpr auto alignment = alignof(std::max_align_t);

                /// @brief Rounds `x` up to the next multiple of `alignment`.
                template <typename T>
                constexpr auto round_up_to_alignment(T x) const noexcept
                {
                    return multiple_round_up(x, alignment);
                }

                // TODO: use in place of `_vtable_ptrs`.
                static constexpr auto max_vtable_ptrs =
                    sizeof(vtable_type*) / buffer_size;

                /// @brief Fixed buffer that stores vtables and callable
                /// objects.
                std::aligned_storage_t<buffer_size, alignment> _buffer;

                /// @brief List of vtable pointers.
                /// @details The vtables are stored in `_buffer`.
                std::vector<vtable_type*> _vtable_ptrs;

                /// @brief Next free spot inside `_buffer`.
                char* _next;

                /// @brief Returns the address of `_buffer`.
                /// @details Non-`const` version.
                auto buffer_ptr() noexcept
                {
                    return reinterpret_cast<char*>(&_buffer);
                }

                /// @brief Returns the address of `_buffer`.
                /// @details `const` version.
                auto buffer_ptr() const noexcept
                {
                    return reinterpret_cast<const char*>(&_buffer);
                }

                /// @brief Given a `_buffer` pointer `ptr`, returns its offset
                /// from the beginning of the buffer.
                /// @details Non-`const` version.
                auto offset_from_beginning(char* ptr) noexcept
                {
                    return ptr - buffer_ptr();
                }

                /// @brief Given a `_buffer` pointer `ptr`, returns its offset
                /// from the beginning of the buffer.
                /// @details `const` version.
                auto offset_from_beginning(const char* ptr) const noexcept
                {
                    return ptr - buffer_ptr();
                }

                /// @brief Given an offset `x`, returns the pointer at that
                /// offset inside `_buffer`.
                /// @details Non-`const` version.
                template <typename T>
                auto buffer_ptr_from_offset(T x) noexcept
                {
                    ECST_S_ASSERT(std::is_arithmetic<T>{});
                    ECST_S_ASSERT(std::is_unsigned<T>{});

                    return static_cast<char*>(buffer_ptr() + x);
                }

                /// @brief Given an offset `x`, returns the pointer at that
                /// offset inside `_buffer`.
                /// @details `const` version.
                template <typename T>
                auto buffer_ptr_from_offset(T x) const noexcept
                {
                    ECST_S_ASSERT(std::is_arithmetic<T>{});
                    ECST_S_ASSERT(std::is_unsigned<T>{});

                    return static_cast<const char*>(buffer_ptr() + x);
                }

                /// @brief Given a `_buffer` pointer `ptr`, returns the next
                /// aligned pointer residing inside the buffer.
                /// @details Non-`const` version.
                auto get_next_aligned_ptr(char* ptr) noexcept
                {
                    // Get the offset of `ptr` inside `_buffer`.
                    const auto ofb = offset_from_beginning(ptr);

                    // Get the next offset (rounded up to `alignment`).
                    const auto next_ofb = round_up_to_alignment(ofb);

                    // Convert the new offset to a pointer and return it.
                    return buffer_ptr_from_offset(next_ofb);
                }

                /// @brief Given a `_buffer` pointer `ptr`, returns the next
                /// aligned pointer residing inside the buffer.
                /// @details `const` version.
                auto get_next_aligned_ptr(const char* ptr) const noexcept
                {
                    // Get the offset of `ptr` inside `_buffer`.
                    const auto ofb = offset_from_beginning(ptr);

                    // Get the next offset (rounded up to `alignment`).
                    const auto next_ofb = round_up_to_alignment(ofb);

                    // Convert the new offset to a pointer and return it.
                    return buffer_ptr_from_offset(next_ofb);
                }

                /// @brief Given a vtable pointer `vt_ptr` returns the next
                /// aligned pointer immediately after the vtable, suitable for
                /// callable object storage.
                /// @details Non-`const` version.
                auto get_fn_ptr_from_vtable(vtable_type* vt_ptr) noexcept
                {
                    return get_next_aligned_ptr(
                        (char*)vt_ptr + sizeof(vtable_type));
                }

                /// @brief Given a vtable pointer `vt_ptr` returns the next
                /// aligned pointer immediately after the vtable, suitable for
                /// callable object storage.
                /// @details `const` version.
                auto get_fn_ptr_from_vtable(const vtable_type* vt_ptr) const
                    noexcept
                {
                    return get_next_aligned_ptr(
                        (char*)vt_ptr + sizeof(vtable_type));
                }



                template <typename T, typename... TNewArgs>
                auto aligned_placement_new(char* ptr, TNewArgs&&... xs) // .
                    noexcept(std::is_nothrow_constructible<T, TNewArgs...>{})
                {
                    ptr = get_next_aligned_ptr(ptr);
                    new(ptr) T{FWD(xs)...};
                    return ptr;
                }

                auto emplace_vtable_at(char* ptr) noexcept
                {
                    return aligned_placement_new<vtable_type>(ptr);
                }

                template <typename TF>
                auto emplace_fn_at(char* ptr, TF&& f) // .
                    noexcept(
                        std::is_nothrow_constructible<TF, decltype(FWD(f))>{})
                {
                    return aligned_placement_new<TF>(ptr, FWD(f));
                }

                template <typename TF>
                void bind_vtable_to_fn(vtable_type& vt) noexcept
                {
                    vtable::template setup<TF, signature>(vt);
                }

                void subscribe_vtable(vtable_type& vt)
                {
                    _vtable_ptrs.emplace_back(&vt);
                }

                template <typename TF>
                auto emplace_starting_at(char* ptr, TF&& f)
                // TODO: noexcept
                {
                    VRM_CORE_ASSERT_OP(offset_from_beginning(ptr) +
                                           sizeof(vtable_type) + sizeof(TF),
                        <, buffer_size);

                    // Emplace the vtable and get a reference to it.
                    ptr = emplace_vtable_at(ptr);
                    auto& vt = *(reinterpret_cast<vtable_type*>(ptr));

                    // Get the aligned position where the callable object will
                    // be
                    // emplaced.
                    auto fn_start_ptr = ptr + sizeof(vtable_type);

                    // Emplace the callable object.
                    ptr = emplace_fn_at(fn_start_ptr, FWD(f));

                    // Bind the vtable to the callable object and add it to the
                    // vtable
                    // vector.
                    bind_vtable_to_fn<TF>(vt);
                    subscribe_vtable(vt);

                    // Move the "next emplacement pointer" forward.
                    // (`ptr` contains the address of the emplaced callable
                    // object.)
                    _next = ptr + sizeof(TF);
                }


                // TODO: docs, repetition, asserts

                template <typename TSelf, typename TVTablePtr, typename TF>
                static void call_fn_from_vt_ptr_impl(
                    TSelf&& self, TVTablePtr vt_ptr, TF&& f)
                {
                    auto fn_ptr = self.get_fn_ptr_from_vtable(vt_ptr);
                    f(*vt_ptr, fn_ptr);
                }

                template <typename TF>
                void call_fn_from_vt_ptr(vtable_type* vt_ptr, TF&& f)
                {
                    call_fn_from_vt_ptr_impl(*this, vt_ptr, FWD(f));
                }

                template <typename TF>
                void call_fn_from_vt_ptr(
                    const vtable_type* vt_ptr, TF&& f) const
                {
                    call_fn_from_vt_ptr_impl(*this, vt_ptr, FWD(f));
                }


                template <typename TSelf, typename TF>
                static void for_vts_impl(TSelf&& self, TF&& f)
                {
                    for(auto vt_ptr : self._vtable_ptrs)
                    {
                        f(vt_ptr);
                    }
                }

                template <typename TSelf, typename TF>
                static void for_fns_impl(TSelf&& self, TF&& f)
                {
                    for(auto vt_ptr : self._vtable_ptrs)
                    {
                        self.call_fn_from_vt_ptr(vt_ptr, FWD(f));
                    }
                }

                template <typename TF>
                void for_vts(TF&& f)
                // TODO: noexcept
                {
                    for_vts_impl(*this, FWD(f));
                }

                template <typename TF>
                void for_fns(TF&& f)
                // TODO: noexcept
                {
                    for_fns_impl(*this, FWD(f));
                }

                template <typename TF>
                void for_vts(TF&& f) const
                // TODO: noexcept
                {
                    for_vts_impl(*this, FWD(f));
                }

                template <typename TF>
                void for_fns(TF&& f) const
                // TODO: noexcept
                {
                    for_fns_impl(*this, FWD(f));
                }

                template <typename TF>
                void for_fns_reverse(TF&& f)
                // TODO: noexcept
                {
                    for(auto itr = std::rbegin(_vtable_ptrs);
                        itr != std::rend(_vtable_ptrs); ++itr)
                    {
                        call_fn_from_vt_ptr(*itr, FWD(f));
                    }
                }

                void destroy_all()
                {
                    for_fns_reverse([this](auto& vt, auto fn_ptr)
                        {
                            vtable::exec_fp(vtable::option::dtor, vt, fn_ptr);
                        });
                }

                template <typename TRhs, typename TOption>
                void move_copy_all_impl(TRhs&& rhs, TOption o)
                {
                    // TODO: optimize: traverse only once.
                    /*
                        // alternates between fn and vt
                        traverse(
                            [](auto& vt, auto fn_ptr){ },
                            [](auto vt_ptr){ }
                        );
                    */

                    VRM_CORE_STATIC_ASSERT_NM(
                        vtable::has_option(vtable_type{}, o));

                    rhs.for_fns([this, &rhs, o](auto& vt, auto fn_ptr)
                        {
                            auto fn_offset = rhs.offset_from_beginning(fn_ptr);
                            vtable::exec_fp(
                                o, vt, fn_ptr, buffer_ptr() + fn_offset);
                        });

                    rhs.for_vts([this, &rhs](auto vt_ptr)
                        {
                            auto vt_offset =
                                rhs.offset_from_beginning((char*)vt_ptr);

                            std::memcpy(buffer_ptr() + vt_offset, vt_ptr,
                                sizeof(vtable_type));
                        });
                }

                void copy_all(const fixed& rhs)
                {
                    move_copy_all_impl(rhs, vtable::option::copy);
                }

                void move_all(fixed& rhs)
                {
                    move_copy_all_impl(rhs, vtable::option::move);
                }

            public:
                fixed() noexcept : _next{buffer_ptr()}
                {
                }

                ~fixed()
                // TODO: noexcept
                {
                    destroy_all();
                }

                fixed(const fixed& rhs)
                    : _vtable_ptrs{rhs._vtable_ptrs},
                      _next{buffer_ptr() + offset_from_beginning(rhs._next)}
                {
                    copy_all(rhs);
                }

                auto& operator=(const fixed& rhs)
                {
                    _vtable_ptrs = rhs._vtable_ptrs;
                    _next = buffer_ptr() + offset_from_beginning(rhs._next);
                    copy_all(rhs);

                    return *this;
                }

                fixed(fixed&& rhs)
                    // Cannot move `_vtable_ptrs` as they contain dtor fn ptrs.
                    : _vtable_ptrs{rhs._vtable_ptrs},
                      _next{buffer_ptr() + offset_from_beginning(rhs._next)}
                {
                    move_all(rhs);
                }

                fixed& operator=(fixed&& rhs)
                {
                    // Cannot move `_vtable_ptrs` as they contain dtor fn ptrs.
                    _vtable_ptrs = rhs._vtable_ptrs;
                    _next = buffer_ptr() + offset_from_beginning(rhs._next);
                    move_all(rhs);

                    return *this;
                }

                template <typename TF>
                void emplace(TF&& f)
                // TODO: noexcept
                {
                    emplace_starting_at(_next, FWD(f));
                }

                template <typename... TArgs>
                auto call_all(TArgs&&... xs)
                // TODO: noexcept
                {
                    for_fns([&xs...](auto& vt, auto fn_ptr)
                        {
                            vtable::exec_fp(
                                vtable::option::call, vt, fn_ptr, FWD(xs)...);
                        });
                }

                void clear()
                {
                    destroy_all();
                    _vtable_ptrs.clear();
                    _next = buffer_ptr();
                }
            };
        }
    }
}
ECST_FUNCTION_QUEUE_NAMESPACE_END
