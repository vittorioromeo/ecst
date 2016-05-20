// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <sstream>
#include <ecst/config.hpp>
#include "./data.hpp"

ECST_SETTINGS_NAMESPACE
{
    namespace str
    {
        template <typename TSettings>
        auto entity_storage()
        {
            std::ostringstream oss;
            oss << "entity storage: ";

            static_if(settings::has_fixed_entity_storage<TSettings>)
                .then([&oss](auto ts)
                    {
                        oss << "fixed (capacity: "
                            << settings::fixed_capacity(ts) << ")";
                    })
                .else_([&oss](auto ts)
                    {
                        oss << "dynamic (initial: "
                            << settings::initial_capacity(ts) << ")";
                    })(TSettings{});

            return oss.str();
        }

        template <typename TSettings>
        auto multithreading()
        {
            std::ostringstream oss;
            oss << "multithreading: ";

            static_if(settings::inner_parallelism_allowed<TSettings>())
                .then([&oss](auto)
                    {
                        oss << "allows inner parallelism";
                    })
                .else_([&oss](auto)
                    {
                        oss << "disallows inner parallelism";
                    })(TSettings{});

            return oss.str();
        }
    }
}
ECST_SETTINGS_NAMESPACE_END
