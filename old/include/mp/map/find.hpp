// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/pair.hpp>
#include <ecst/mp/map/basic.hpp>
#include <ecst/mp/map/picker.hpp>

ECST_MP_MAP_NAMESPACE
{
    namespace impl
    {
        template <typename TMap, typename TComparer>
        constexpr auto find_all_indices_by_comparer_impl(TMap m, TComparer c)
        {
            (void)c;

            return list::fold_l_with_index(list::empty_v,
                [=](auto i, auto acc, auto xp)
                {
                    ECST_S_ASSERT_DT(pair::valid(xp));

                    // Pass both index and current value to comparer.
                    return static_if(c(xp))
                        .then([=](auto xi, auto xacc)
                            {
                                return list::append(xacc, xi);
                            })
                        .else_([=](auto, auto xacc)
                            {
                                return xacc;
                            })(i, acc);
                },
                m);
        }

        template <typename TMap, typename TKey>
        constexpr auto find_all_indices_by_key_impl(TMap m, TKey k)
        {
            return find_all_indices_by_comparer_impl(m, by_key(k));
        }

        template <typename TMap, typename TValue>
        constexpr auto find_all_indices_by_value_impl(TMap m, TValue v)
        {
            return find_all_indices_by_comparer_impl(m, by_value(v));
        }

        template <typename TMap, typename TComparer>
        constexpr auto count_by_comparer_impl(TMap m, TComparer c)
        {
            return list::size(find_all_indices_by_comparer_impl(m, c));
        }

        template <typename TMap, typename TKey>
        constexpr auto count_by_key_impl(TMap m, TKey k)
        {
            return count_by_comparer_impl(m, by_key(k));
        }

        template <typename TMap, typename TValue>
        constexpr auto count_by_value_impl(TMap m, TValue v)
        {
            return count_by_comparer_impl(m, by_value(v));
        }
    }

    template <typename TMap, typename TKey>
    constexpr auto find_all_indices_by_key(TMap m, TKey k)
    {
        return decltype(impl::find_all_indices_by_key_impl(m, k)){};
    }


    template <typename TMap, typename TValue>
    constexpr auto find_all_indices_by_value(TMap m, TValue v)
    {
        return decltype(impl::find_all_indices_by_value_impl(m, v)){};
    }

    namespace impl
    {
        template <typename TMap, typename TComparer>
        constexpr auto find_first_index_by_comparer_impl(TMap m, TComparer c)
        {
            (void)m;
            (void)c;

            auto indices(find_all_indices_by_comparer_impl(TMap{}, c));
            return list::null_if_empty_or_head(indices);
        }

        template <typename TMap, typename TKey>
        constexpr auto find_first_index_by_key_impl(TMap m, TKey k)
        {
            return find_first_index_by_comparer_impl(m, by_key(k));
        }

        template <typename TMap, typename TValue>
        constexpr auto find_first_index_by_value_impl(TMap m, TValue v)
        {
            return find_first_index_by_comparer_impl(m, by_value(v));
        }
    }

    template <typename TMap, typename TKey>
    constexpr auto find_first_index_by_key(TMap m, TKey k)
    {
        return decltype(impl::find_first_index_by_key_impl(m, k)){};
    }

    template <typename TMap, typename TValue>
    constexpr auto find_first_index_by_value(TMap m, TValue v)
    {
        return decltype(impl::find_first_index_by_value_impl(m, v)){};
    }

    namespace impl
    {
        template <typename TMap, typename TComparer, typename TPicker>
        constexpr auto find_all_by_comparer_impl(
            TMap m, TComparer c, TPicker pk)
        {
            (void)m;
            (void)c;

            auto indices(find_all_indices_by_comparer_impl(TMap{}, c));
            return list::transform(
                [=](auto xi)
                {
                    return pk(list::at(m, xi));
                },
                decltype(indices){});
        }

        template <typename TMap, typename TKey>
        constexpr auto find_all_by_key_impl(TMap m, TKey k)
        {
            return find_all_by_comparer_impl(m, by_key(k), picker_value());
        }

        template <typename TMap, typename TValue>
        constexpr auto find_all_by_value_impl(TMap m, TValue v)
        {
            return find_all_by_comparer_impl(m, by_value(v), picker_key());
        }

        template <typename TMap, typename TKey>
        constexpr auto find_first_by_key_impl(TMap m, TKey k)
        {
            return list::null_if_empty_or_head(find_all_by_key_impl(m, k));
        }

        template <typename TMap, typename TValue>
        constexpr auto find_first_by_value_impl(TMap m, TValue v)
        {
            return list::null_if_empty_or_head(find_all_by_value_impl(m, v));
        }
    }

    template <typename TMap, typename TKey>
    constexpr auto find_all_by_key(TMap m, TKey k)
    {
        return decltype(impl::find_all_by_key_impl(m, k)){};
    }

    template <typename TMap, typename TValue>
    constexpr auto find_all_by_value(TMap m, TValue v)
    {
        return decltype(impl::find_all_by_value_impl(m, v)){};
    }

    template <typename TMap, typename TKey>
    constexpr auto find_first_by_key(TMap m, TKey k)
    {
        return decltype(impl::find_first_by_key_impl(m, k)){};
    }

    template <typename TMap, typename TValue>
    constexpr auto find_first_by_value(TMap m, TValue v)
    {
        return decltype(impl::find_first_by_value_impl(m, v)){};
    }

    template <typename TMap, typename TKey>
    constexpr auto count_by_key(TMap m, TKey k)
    {
        return decltype(impl::count_by_key_impl(m, k)){};
    }

    template <typename TMap, typename TValue>
    constexpr auto count_by_value(TMap m, TValue v)
    {
        return decltype(impl::count_by_value_impl(m, v)){};
    }

    template <typename TMap, typename TKey>
    constexpr auto contains_key(TMap m, TKey k)
    {
        return bool_v<(count_by_key(m, k) > 0)>;
    }

    template <typename TMap, typename TValue>
    constexpr auto contains_value(TMap m, TValue v)
    {
        return bool_v<(count_by_value(m, v) > 0)>;
    }
}
ECST_MP_MAP_NAMESPACE_END
