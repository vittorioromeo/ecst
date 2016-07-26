// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/debug.hpp>
#include "./dependencies.hpp"
#include "./utils.hpp"
#include "./aliases.hpp"
#include "./vtable.hpp"

ECST_NAMESPACE
{
    template <typename TSignature, std::size_t TBufferSize>
    class fixed_function_queue;

    template <typename TReturn, typename... TArgs, std::size_t TBufferSize>
    class fixed_function_queue<TReturn(TArgs...), TBufferSize>
    {
    private:
        using signature = TReturn(TArgs...);
        static constexpr auto buffer_size = TBufferSize;
        static constexpr auto alignment = alignof(std::max_align_t);

        template <typename T>
        constexpr auto round_up_to_alignment(T x) const noexcept
        {
            return multiple_round_up(x, alignment);
        }

        using vtable_type = complete_vtable_type<signature>;

        // TODO: use a preallocated fixed buffer instead of `std::vector`, we
        // know the max vtable ptr count
        static constexpr auto max_vtable_ptrs =
            sizeof(vtable_type*) / buffer_size;

        std::aligned_storage_t<buffer_size, alignment> _buffer;
        std::vector<vtable_type*> _vtable_ptrs;
        char* _next;

        auto buffer_ptr() noexcept
        {
            return reinterpret_cast<char*>(&_buffer);
        }

        auto buffer_ptr() const noexcept
        {
            return reinterpret_cast<const char*>(&_buffer);
        }

        auto offset_from_beginning(char* ptr) const noexcept
        {
            return ptr - buffer_ptr();
        }

        template <typename T>
        auto buffer_ptr_from_offset(T x) const noexcept
        {
            return buffer_ptr() + x;
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
            ELOG( // .
                std::cout << "emplacing vtable... (size: "
                          << sizeof(vtable_type) << ")\n"; // .
                );

            return aligned_placement_new<vtable_type>(ptr);
        }

        template <typename TF>
        auto emplace_fn_at(char* ptr, TF&& f) // .
            noexcept(std::is_nothrow_constructible<TF, decltype(FWD(f))>{})
        {
            ELOG( // .
                std::cout << "emplacing fn... (size: " << sizeof(TF)
                          << ")\n"; // .
                );

            return aligned_placement_new<TF>(ptr, FWD(f));
        }

        template <typename TF>
        void bind_vtable_to_fn(vtable_type& vt) noexcept
        {
            ELOG(                                         // .
                std::cout << "binding vtable to fn...\n"; // .
                );

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
            VRM_CORE_ASSERT_OP(
                offset_from_beginning(ptr) + sizeof(vtable_type) + sizeof(TF),
                <, buffer_size);

            // Emplace the vtable and get a reference to it.
            ptr = emplace_vtable_at(ptr);
            auto& vt = *(reinterpret_cast<vtable_type*>(ptr));

            ELOG( // .
                std::cout << "vtable offset: " << offset_from_beginning(ptr)
                          << "\n"; // .
                );

            // Get the aligned position where the callable object will be
            // emplaced.
            auto fn_start_ptr = ptr + sizeof(vtable_type);

            ELOG(                                                         // .
                std::cout << "fn start offset: "                          // .
                          << offset_from_beginning(fn_start_ptr) << "\n"; // .
                );

            // Emplace the callable object.
            ptr = emplace_fn_at(fn_start_ptr, FWD(f));

            ELOG(                                                        // .
                std::cout << "fn offset: " << offset_from_beginning(ptr) // .
                          << "\n";                                       // .
                );

            // Bind the vtable to the callable object and add it to the vtable
            // vector.
            bind_vtable_to_fn<TF>(vt);
            subscribe_vtable(vt);

            // Move the "next emplacement pointer" forward.
            // (`ptr` contains the address of the emplaced callable object.)
            _next = ptr + sizeof(TF);

            ELOG( // .
                std::cout << "stored next offset: "
                          << offset_from_beginning(_next) << "\n"; // .
                );
        }

        auto get_next_aligned_ptr(char* ptr) const noexcept
        {
            auto ofb = offset_from_beginning(ptr);

            ELOG(                                    // .
                std::cout << "ofb: " << ofb << "\n"; // .
                );

            auto next_ofb = round_up_to_alignment(ofb);

            ELOG(                                              // .
                std::cout << "next_ofb: " << next_ofb << "\n"; // .
                );

            return (char*)buffer_ptr_from_offset(next_ofb);
        }

        auto get_fn_ptr_from_vtable(vtable_type* vt_ptr) const noexcept
        {
            return get_next_aligned_ptr((char*)vt_ptr + sizeof(vtable_type));
        }

        // TODO: docs, repetition, asserts

        template <typename TSelf, typename TF>
        static void call_fn_from_vt_ptr_impl(
            TSelf&& self, vtable_type* vt_ptr, TF&& f)
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
        void call_fn_from_vt_ptr(vtable_type* vt_ptr, TF&& f) const
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
            ELOG(                                             // .
                std::cout << "destroying all functions...\n"; // .
                for(auto vt_ptr
                    : _vtable_ptrs) // .
                {
                    std::cout << "    vt_ptr offset: " // .
                              << offset_from_beginning((char*)vt_ptr)
                              << "\n"; // .
                }                      // .
                );

            for_fns_reverse([this](auto& vt, auto fn_ptr)
                {
                    ELOG(                                              // .
                        std::cout << "    vt_ptr offset: "             // .
                                  << offset_from_beginning((char*)&vt) // .
                                  << "\n";                             // .
                        );

                    vtable::exec_fp(vtable::option::dtor, vt, fn_ptr);
                });
        }

        template <typename TRhs, typename TOption>
        void move_copy_all_impl(TRhs&& rhs, TOption o)
        {
            // TODO: optimize: traverse only once.

            VRM_CORE_STATIC_ASSERT_NM(vtable::has_option(vtable_type{}, o));


            rhs.for_fns([this, &rhs, o](auto& vt, auto fn_ptr)
                {
                    ELOG(                                              // .
                        std::cout << "    vt_ptr offset: "             // .
                                  << offset_from_beginning((char*)&vt) // .
                                  << "\n";                             // .
                        );

                    auto fn_offset = rhs.offset_from_beginning(fn_ptr);
                    vtable::exec_fp(o, vt, fn_ptr, buffer_ptr() + fn_offset);
                });

            rhs.for_vts([this, &rhs](auto vt_ptr)
                {
                    auto vt_offset = rhs.offset_from_beginning((char*)vt_ptr);
                    std::memcpy(
                        buffer_ptr() + vt_offset, vt_ptr, sizeof(vtable_type));
                });
        }

        void copy_all(const fixed_function_queue& rhs)
        {
            move_copy_all_impl(rhs, vtable::option::copy);
        }

        void move_all(fixed_function_queue& rhs)
        {
            move_copy_all_impl(rhs, vtable::option::move);
        }

    public:
        fixed_function_queue() noexcept : _next{buffer_ptr()}
        {
        }

        ~fixed_function_queue()
        // TODO: noexcept
        {
            destroy_all();
        }

        fixed_function_queue(const fixed_function_queue& rhs)
            : _vtable_ptrs{rhs._vtable_ptrs},
              _next{buffer_ptr() + offset_from_beginning(rhs._next)}
        {
            copy_all(rhs);
        }

        fixed_function_queue& operator=(const fixed_function_queue& rhs)
        {
            _vtable_ptrs = rhs._vtable_ptrs;
            _next = buffer_ptr() + offset_from_beginning(rhs._next);
            copy_all(rhs);

            return *this;
        }

        fixed_function_queue(fixed_function_queue&& rhs)
            // Cannot move `_vtable_ptrs` as they contains the dtor fn ptrs.
            : _vtable_ptrs{rhs._vtable_ptrs},
              _next{buffer_ptr() + offset_from_beginning(rhs._next)}
        {
            move_all(rhs);
        }

        fixed_function_queue& operator=(fixed_function_queue&& rhs)
        {
            // Cannot move `_vtable_ptrs` as they contains the dtor fn ptrs.
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

        auto call_all(TArgs... xs)
        // TODO: noexcept
        {
            ELOG(                                          // .
                std::cout << "calling all functions...\n"; // .
                // .
                for(auto vt_ptr
                    : _vtable_ptrs) // .
                {
                    std::cout << "    vt_ptr offset: " // .
                              << offset_from_beginning((char*)vt_ptr)
                              << "\n"; // .
                }                      // .
                );

            for_fns([&xs...](auto& vt, auto fn_ptr)
                {
                    vtable::exec_fp(vtable::option::call, vt, fn_ptr, xs...);
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
ECST_NAMESPACE_END
