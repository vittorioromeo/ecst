// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/core/make_array.hpp>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include "./elog.hpp"

ECST_DEBUG_NAMESPACE
{
    namespace impl
    {
        auto& ECST_CONST_FN last_log() noexcept
        {
            static int res = -1;
            return res;
        }

        const auto& ECST_CONST_FN tstrings() noexcept
        {
            static auto res = vrmc::make_array( // .
                /* 00 */ "      ENTITY",        // .
                /* 01 */ "   COMPONENT",        // .
                /* 02 */ "   ENTITY M.",        // .
                /* 03 */ "COMPONENT M.",        // .
                /* 04 */ " THREAD POOL",        // .
                /* 05 */ "SYS INSTANCE",        // .
                /* 06 */ "S.I. PARALL.",        // .
                /* 07 */ " CTX REFRESH",        // .
                /* 08 */ "  SYS BITSET",        // .
                /* 09 */ "   SYS MATCH",        // .
                /* 10 */ "MDATA BITSET"         // .
                );

            return res;
        }

        template <int TN>
        constexpr auto on = int_v<TN>;

        constexpr auto off = int_v<-1>;

        struct fake_cout_obj
        {
        };

        auto& ECST_CONST_FN fake_cout() noexcept
        {
            static fake_cout_obj res;
            return res;
        }

        template <typename T>
        auto& operator<<(fake_cout_obj& o, T&&) noexcept
        {
            return o;
        }

        /* 00 */ constexpr auto entity = off;
        /* 01 */ constexpr auto component = on<1>;
        /* 02 */ constexpr auto entity_memory = off;
        /* 03 */ constexpr auto component_memory = off;
        /* 04 */ constexpr auto thread_pool = off;
        /* 05 */ constexpr auto instance = off;
        /* 06 */ constexpr auto instance_parallelism = on<6>;
        /* 07 */ constexpr auto context_refresh = off;
        /* 08 */ constexpr auto system_bitset = on<8>;
        /* 09 */ constexpr auto system_match = on<9>;
        /* 10 */ constexpr auto metadata_bitset = on<10>;

        template <typename TType>
        auto& log(TType) noexcept
        {
            return static_if(bool_v<(TType::value >= 0)>)
                .then([]() -> auto&
                    {
                        constexpr auto logt = TType::value;

                        if(last_log() != logt)
                        {
                            std::cout << "\n";
                        }

                        last_log() = logt;

                        return std::cout << "[" << tstrings()[TType::value]
                                         << "]\t";
                    })
                .else_([]() -> auto&
                    {
                        return fake_cout();
                    })();
        }
    }
}
ECST_DEBUG_NAMESPACE_END
