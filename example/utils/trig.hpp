// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <array>
#include <vrm/core/type_aliases.hpp>
#include <vrm/core/assert.hpp>

namespace example
{
    namespace vrmc = vrm::core;
    using vrmc::sz_t;

    constexpr float pi{3.14159265f};
    constexpr float tau{pi * 2.f};

    template <sz_t TPrecision>
    struct trig_table
    {
    private:
        static constexpr sz_t count{TPrecision};
        static constexpr float ratio{TPrecision / tau};
        std::array<float, count> arr;

    public:
        template <typename TF>
        trig_table(TF&& f) noexcept
        {
            for(auto i(0u); i < count; ++i) arr[i] = f(i / ratio);
        }

        auto get(float x) const noexcept
        {
            auto idx(vrmc::to_sz_t(x * ratio));
            VRM_CORE_ASSERT(idx < count);

            return arr[idx];
        }
    };

    namespace impl
    {
        static constexpr sz_t trig_table_precision{628};

        const auto& sin_table() noexcept
        {
            static trig_table<trig_table_precision> result{[](auto x)
                {
                    return std::sin(x);
                }};

            return result;
        }

        const auto& cos_table() noexcept
        {
            static trig_table<trig_table_precision> result{[](auto x)
                {
                    return std::cos(x);
                }};

            return result;
        }
    }

    auto tbl_sin(float mX) noexcept
    {
        return impl::sin_table().get(mX);
    }

    auto tbl_cos(float mX) noexcept
    {
        return impl::cos_table().get(mX);
    }
}
