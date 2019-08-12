// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <utility>

ECST_NAMESPACE
{
    /// @brief Wrapper around `std::atomic<T>` that allows move operations.
    /// @details Moves are handled by using `store(rhs.load())`.
    template <typename T>
    class movable_atomic final : public ecst::atomic<T>
    {
    private:
        using base_type = ecst::atomic<T>;

    public:
        using base_type::base_type;

        movable_atomic() = default;

        movable_atomic(const movable_atomic&) = delete;
        movable_atomic& operator=(const movable_atomic&) = delete;

        movable_atomic(movable_atomic&& rhs) noexcept : base_type{rhs.load()}
        {
        }

        movable_atomic& operator=(movable_atomic&& rhs) noexcept
        {
            this->store(rhs.load());
            return *this;
        }

        template <T TV>
        movable_atomic& operator=(std::integral_constant<T, TV> x) noexcept
        {
            this->store(decltype(x)::value);
            return *this;
        }
    };
}
ECST_NAMESPACE_END
