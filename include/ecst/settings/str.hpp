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

            settings::dispatch_on_storage_type(TSettings{},
                [&oss](auto fixed_capacity)
                {
                    oss << "fixed (capacity: " << fixed_capacity << ")";
                },
                [&oss](auto initial_capacity)
                {
                    oss << "dynamic (initial: " << initial_capacity << ")";
                });

            return oss.str();
        }

        template <typename TSettings>
        auto multithreading()
        {
            std::ostringstream oss;
            oss << "multithreading: ";

            static_if(settings::inner_parallelism_allowed(TSettings{}))
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
