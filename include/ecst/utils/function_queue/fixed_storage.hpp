// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include "./dependencies.hpp"
#include "./aliases.hpp"
#include "./vtable.hpp"

// TODO:
ECST_NAMESPACE
{
    namespace impl
    {
        namespace storage
        {
            template <typename TSignature, typename TVTable,
                std::size_t TBufferSize>
            class fixed_storage
            {
            private:
                using signature = TSignature;
                using vtable_type = TVTable;

                static constexpr auto buffer_size = TBufferSize;
                static constexpr auto alignment = alignof(std::max_align_t);

                template <typename T>
                constexpr auto round_up_to_alignment(T x) const noexcept
                {
                    return multiple_round_up(x, alignment);
                }

                static constexpr auto max_vtable_ptrs =
                    sizeof(vtable_type*) / buffer_size;

                std::aligned_storage_t<buffer_size, alignment> _buffer;
                std::vector<vtable_type*> _vtable_ptrs;
                char* _next;

                auto get_next_aligned_ptr(char* ptr) const noexcept
                {
                    auto ofb = offset_from_beginning(ptr);
                    auto next_ofb = round_up_to_alignment(ofb);
                    return (char*)buffer_ptr_from_offset(next_ofb);
                }

                auto get_fn_ptr_from_vtable(vtable_type* vt_ptr) const noexcept
                {
                    return get_next_aligned_ptr(
                        (char*)vt_ptr + sizeof(vtable_type));
                }

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

            public:
                fixed_storage()
                {
                }

                fixed_storage(const fixed_storage&)
                {
                }
                fixed_storage& operator=(const fixed_storage&)
                {
                }

                fixed_storage(fixed_storage&&)
                {
                }
                fixed_storage& operator=(fixed_storage&&)
                {
                }
            };
        }
    }
}
ECST_NAMESPACE_END
