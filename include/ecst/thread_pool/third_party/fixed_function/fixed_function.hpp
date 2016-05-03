#pragma once

#include <cstddef>
#include <cassert>
#include <type_traits>

namespace etp
{
    /**
        * @brief The FixedFunction<R(ARGS...), storage_size> class implements
        * functional object.
        * This function is analog of 'std::function' with limited capabilities:
        *  - It supports only move semantics.
        *  - The size of functional objects is limited to storage size.
        * Due to limitations above it is much faster on creation and copying
     * than
        * std::function.
        */
    template <typename TSignature, std::size_t TStorageSize = 64>
    class FixedFunction;

    template <typename TR, typename... Ts, std::size_t TStorageSize>
    class FixedFunction<TR(Ts...), TStorageSize>
    {
    private:
        static constexpr auto storage_size = TStorageSize;

        using ret_type = TR;
        using storage_type =
            std::aligned_storage_t<storage_size, alignof(std::size_t)>;

        using fn_ptr_type = ret_type (*)(Ts...);
        using method_type = ret_type (*)(storage_type*, fn_ptr_type, Ts...);
        using alloc_type = void (*)(storage_type*, void* object_ptr);

        method_type _method_ptr;
        alloc_type _alloc_ptr;

        union
        {
            storage_type _storage;
            fn_ptr_type _function_ptr;
        };

    public:
        FixedFunction() noexcept : _function_ptr{nullptr},
                                   _method_ptr{nullptr},
                                   _alloc_ptr{nullptr}
        {
        }

        /**
         * @brief FixedFunction Constructor from functional object.
         * @param object Functor object will be stored in the internal storage
         * using move constructor. Unmovable objects are prohibited explicitly.
         */
        template <typename TFFwd>
        FixedFunction(TFFwd&& f)
            : FixedFunction()
        {
            using unref_type = std::remove_reference_t<TFFwd>;

            static_assert(sizeof(unref_type) < storage_size,
                "functional object doesn't fit into internal storage");
            static_assert(std::is_move_constructible<unref_type>{},
                "Should be of movable type");

            _method_ptr = [](
                storage_type* s, fn_ptr_type, Ts... args) -> ret_type
            {
                return reinterpret_cast<unref_type*>(s)->operator()(args...);
            };

            _alloc_ptr = [](storage_type* s, void* object_ptr)
            {
                if(object_ptr)
                {
                    unref_type* x_object = static_cast<unref_type*>(object_ptr);
                    new(s) unref_type(std::move(*x_object));
                }
                else
                {
                    reinterpret_cast<unref_type*>(s)->~unref_type();
                }
            };

            _alloc_ptr(&_storage, &f);
        }

        /**
         * @brief FixedFunction Constructor from free function or static member.
         */
        template <typename RET, typename... PARAMS>
        FixedFunction(RET (*func_ptr)(PARAMS...))
            : FixedFunction()
        {
            _function_ptr = func_ptr;
            _method_ptr = [](
                storage_type*, fn_ptr_type f_ptr, Ts... args) -> ret_type
            {
                return static_cast<RET (*)(PARAMS...)>(f_ptr)(args...);
            };
        }

        FixedFunction(FixedFunction&& o) noexcept : FixedFunction()
        {
            moveFromOther(o);
        }

        FixedFunction& operator=(FixedFunction&& o) noexcept
        {
            moveFromOther(o);
            return *this;
        }

        ~FixedFunction() noexcept
        {
            if(_alloc_ptr) _alloc_ptr(&_storage, nullptr);
        }

        /**
         * @brief operator () Execute stored functional object.
         */
        template <typename... TFwdTs>
        auto operator()(TFwdTs&&... xs)
        {
            assert(_method_ptr != nullptr);
            return _method_ptr(&_storage, _function_ptr, FWD(xs)...);
        }

    private:
        FixedFunction& operator=(const FixedFunction&) = delete;
        FixedFunction(const FixedFunction&) = delete;



        void moveFromOther(FixedFunction& o) noexcept
        {
            assert(this != &o);

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
    };
}
