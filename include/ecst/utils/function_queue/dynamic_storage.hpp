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
                typename TAllocator>
            class dynamic_storage
            {
            private:
                using signature = TSignature;
                using vtable_type = TVTable;


                static constexpr auto alignment = alignof(std::max_align_t);

                template <typename T>
                constexpr auto round_up_to_alignment(T x) const noexcept
                {
                    return multiple_round_up(x, alignment);
                }

            public:
            };
        }
    }
}
ECST_NAMESPACE_END
