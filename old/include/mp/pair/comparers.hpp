// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/pair/basic.hpp>

ECST_MP_PAIR_NAMESPACE
{
    namespace impl
    {
        template <sz_t TI>
        struct element_comparer
        {
            template <typename TPair, typename T>
            constexpr auto operator()(TPair p, T x) const
            {
                ECST_S_ASSERT_DT(valid(p));
                return mp::same_type_decay(list::at(p, sz_v<TI>), x);
            }
        };

        using fst_comparer = element_comparer<0>;
        using snd_comparer = element_comparer<1>;
    }

    constexpr impl::fst_comparer fst_comparer{};
    constexpr impl::snd_comparer snd_comparer{};

    /// @brief Returns true if the first element of a pair is equal to `x0`.
    template <typename TPair, typename T0>
    constexpr auto fst_is(TPair, T0 x0)
    {
        return bool_v<fst_comparer(TPair{}, x0)>;
    }

    /// @brief Returns true if the second element of a pair is equal to `x1`.
    template <typename TPair, typename T1>
    constexpr auto snd_is(TPair, T1 x1)
    {
        return bool_v<snd_comparer(TPair{}, x1)>;
    }
}

