#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>

ECST_NAMESPACE
{
    /**
            * @brief The fixed_function<R(ARGS...), storage_size> class
     * implements
            * functional object.
            * This function is analog of 'std::function' with limited
     * capabilities:
            *  - It supports only move semantics.
            *  - The size of functional objects is limited to storage size.
            * Due to limitations above it is much faster on creation and copying
         * than
            * std::function.
            */
    template <typename TSignature, sz_t TStorageSize = 64>
    class fixed_function;

    template <typename TReturn, typename... Ts, sz_t TStorageSize>
    class fixed_function<TReturn(Ts...), TStorageSize>
    {
    private:
        static constexpr auto storage_size = TStorageSize;

        using ret_type = TReturn;
        using storage_type =
            std::aligned_storage_t<storage_size, alignof(sz_t)>;

        using fn_ptr_type = ret_type (*)(Ts...);
        using method_type = ret_type (*)(storage_type*, fn_ptr_type, Ts...);
        using alloc_type = void (*)(storage_type*, void* object_ptr);

        union
        {
            storage_type _storage;
            fn_ptr_type _function_ptr;
        };

        method_type _method_ptr;
        alloc_type _alloc_ptr;

        void moveFromOther(fixed_function& o) noexcept
        {
            ECST_ASSERT(this != &o);

            if(_alloc_ptr)
            {
                _alloc_ptr(&_storage, nullptr);
                _alloc_ptr = nullptr;
            }
            else
            {
                _function_ptr = nullptr;
            }

            _method_ptr = o._method_ptr;
            o._method_ptr = nullptr;

            if(o._alloc_ptr)
            {
                _alloc_ptr = o._alloc_ptr;
                _alloc_ptr(&_storage, &o._storage);
            }
            else
            {
                _function_ptr = o._function_ptr;
            }
        }

    public:
        fixed_function() noexcept : _function_ptr{nullptr},
                                    _method_ptr{nullptr},
                                    _alloc_ptr{nullptr}
        {
        }

        /**
         * @brief fixed_function Constructor from functional object.
         * @param f Functor object will be stored in the internal storage
         * using move constructor. Unmovable objects are prohibited explicitly.
         */
        template <typename TFFwd>
        fixed_function(TFFwd&& f) noexcept : fixed_function()
        {
            using unref_type = std::remove_reference_t<TFFwd>;

            static_assert(sizeof(unref_type) < storage_size,
                "functional object doesn't fit into internal storage");

            static_assert(std::is_move_constructible<unref_type>{},
                "Should be of movable type");

            _method_ptr = [](storage_type* s, fn_ptr_type, Ts... xs)
            {
                return vrmc::storage_cast<unref_type>(s)->operator()(xs...);
            };

            _alloc_ptr = [](storage_type* s, void* o)
            {
                if(o)
                {
                    new(s) unref_type(std::move(*static_cast<unref_type*>(o)));
                }
                else
                {
                    vrmc::storage_cast<unref_type>(s)->~unref_type();
                }
            };

            _alloc_ptr(&_storage, &f);
        }

        /**
         * @brief fixed_function Constructor from free function or static
         * member.
         */
        template <typename TFReturn, typename... TFs>
        fixed_function(TFReturn (*f)(TFs...)) noexcept : fixed_function()
        {
            _function_ptr = f;
            _method_ptr = [](storage_type*, fn_ptr_type xf, Ts... xs)
            {
                return static_cast<decltype(f)>(xf)(xs...);
            };
        }

        fixed_function& operator=(const fixed_function&) = delete;
        fixed_function(const fixed_function&) = delete;

        fixed_function(fixed_function&& o) noexcept : fixed_function()
        {
            moveFromOther(o);
        }

        fixed_function& operator=(fixed_function&& o) noexcept
        {
            moveFromOther(o);
            return *this;
        }

        ~fixed_function() noexcept
        {
            if(_alloc_ptr) _alloc_ptr(&_storage, nullptr);
        }

        /**
         * @brief operator () Execute stored functional object.
         */
        template <typename... TFwdTs>
        auto operator()(TFwdTs&&... xs) noexcept(noexcept(    // .
            _method_ptr(&_storage, _function_ptr, FWD(xs)...) // .
            ))
        {
            ECST_ASSERT(_method_ptr != nullptr);
            return _method_ptr(&_storage, _function_ptr, FWD(xs)...);
        }
    };
}
ECST_NAMESPACE_END
