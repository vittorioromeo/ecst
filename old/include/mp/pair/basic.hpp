// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/pair/types.hpp>
#include <ecst/mp/pair/is_pair.hpp>

ECST_MP_PAIR_NAMESPACE
{
    /// @brief Returns a new pair `(x0, x1)`.
    template <typename T0, typename T1>
    constexpr auto make(T0 x0, T1 x1)
    {
        return list::make(x0, x1);
    }

    namespace impl
    {
        template <typename TPair>
        constexpr auto valid_impl(TPair p)
        {
            return static_if(is_pair(p))
                .then([](auto xp)
                    {
                        return bool_v<list::size(decltype(xp){}) == sz_v<2>>;
                    })
                .else_([](auto)
                    {
                        return bool_v<false>;
                    })(p);
        }
    }

    /// @brief Returns true if `p` is a valid pair.
    template <typename TPair>
    constexpr auto valid(TPair p)
    {
        return decltype(impl::valid_impl(p)){};
    }

    /// @brief Returns the first element of a pair.
    template <typename TPair>
    constexpr auto fst(TPair p)
    {
        ECST_S_ASSERT_DT(valid(p));
        return list::at(p, sz_v<0>);
    }

    /// @brief Returns the second element of a pair.
    template <typename TPair>
    constexpr auto snd(TPair p)
    {
        ECST_S_ASSERT_DT(valid(p));
        return list::at(p, sz_v<1>);
    }

    /// @brief Replaces the first element of a pair.
    template <typename TPair, typename T0>
    constexpr auto replace_fst(TPair p, T0 x0)
    {
        ECST_S_ASSERT_DT(valid(p));
        return list::replace_at(p, sz_v<0>, x0);
    }

    /// @brief Replaces the second element of a pair.
    template <typename TPair, typename T1>
    constexpr auto replace_snd(TPair p, T1 x1)
    {
        ECST_S_ASSERT_DT(valid(p));
        return list::replace_at(p, sz_v<1>, x1);
    }
}
ECST_MP_PAIR_NAMESPACE_END
