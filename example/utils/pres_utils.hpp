// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./dependencies.hpp"

/*
    struct VRM_CORE_UNUSED_FN VRM_PP_CAT(ss, __LINE__, semicolon_suppressor) \
    {                                                                       \
    }
*/

#define EXAMPLE_COMPONENT_TAG(x)                                      \
                                                                      \
    namespace example                                                 \
    {                                                                 \
        namespace c                                                   \
        {                                                             \
            struct x;                                                 \
        }                                                             \
        namespace ct                                                  \
        {                                                             \
            constexpr auto x = ecst::signature::component::tag<c::x>; \
            void VRM_CORE_UNUSED_FN ECST_CONST_FN VRM_PP_CAT(         \
                x, __LINE__, warning_suppressor)()                    \
            {                                                         \
                (void) x;                                             \
            }                                                         \
        }                                                             \
    }                                                                 \
    ECST_SPECIALIZE_COMPONENT_NAME(example::c::x)

#define EXAMPLE_SYSTEM_TAG(x)                                      \
                                                                   \
    namespace example                                              \
    {                                                              \
        namespace s                                                \
        {                                                          \
            struct x;                                              \
        }                                                          \
        namespace st                                               \
        {                                                          \
            constexpr auto x = ecst::signature::system::tag<s::x>; \
            void VRM_CORE_UNUSED_FN ECST_CONST_FN VRM_PP_CAT(      \
                x, __LINE__, warning_suppressor)()                 \
            {                                                      \
                (void) x;                                          \
            }                                                      \
        }                                                          \
    }                                                              \
    ECST_SPECIALIZE_SYSTEM_NAME(example::s::x)

namespace example
{
    std::random_device rnd_device;
    std::default_random_engine rnd_gen{rnd_device()};

    auto rndf = [](float min, float max)
    {
        using dist_t = std::uniform_real_distribution<float>;
        return dist_t(min, max)(rnd_gen);
    };

    auto rndvec2f = [](float min, float max)
    {
        return vec2f{rndf(min, max), rndf(min, max)};
    };

    template <typename T>
    ECST_ALWAYS_INLINE auto ECST_CONST_FN square(T x) noexcept
    {
        return x * x;
    }

    template <typename TVec2>
    ECST_ALWAYS_INLINE auto ECST_CONST_FN squared_distance(
        const TVec2& a, const TVec2& b) noexcept
    {
        return square(b.x - a.x) + square(b.y - a.y);
    }

    template <typename TVec2>
    ECST_ALWAYS_INLINE auto ECST_CONST_FN distance(
        const TVec2& a, const TVec2& b) noexcept
    {
        return std::sqrt(squared_distance(a, b));
    }

    void solve_penetration(float dist, vec2f& p0, vec2f& v0, float r0,
        vec2f& p1, vec2f& v1, float r1) noexcept
    {
        std::swap(v0, v1);

        auto dir = p1 - p0;
        auto dir_len = std::sqrt(square(dir.x) + square(dir.y));

        auto norm_dir = dir / dir_len;
        auto depth = (r0 + r1) - dist;

        constexpr auto softness = 0.6f;
        auto softned_depth = norm_dir * depth * softness;

        p0 -= softned_depth;
        p1 += softned_depth;
    }


    template <typename TCell, typename TF>
    void for_unique_pairs(TCell& cell, ecst::entity_id eid, TF&& f)
    {
        for(auto eid2 : cell)
        {
            if(eid <= eid2)
            {
                continue;
            }

            f(eid2);
        }
    }
}
