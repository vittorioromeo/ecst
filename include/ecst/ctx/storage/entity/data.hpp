// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./container.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/ctx/entity.hpp>
#include <ecst/settings.hpp>

namespace ecst::ctx::storage::entity::impl
{
    using counter = ecst::impl::counter;
    using handle = ecst::ctx::entity::impl::handle;

    template <typename TContainer>
    class data
    {
    public:
        using container_type = TContainer;

    private:
        container_type _container;

        const auto& get_counter_from_eid(entity_id eid) const noexcept
        {
            return get_metadata(eid)._counter;
        }

        auto create_entity_and_get_eid()
        {
            return entity_id{_container.create_and_get_eid()};
        }

        auto create_entity_handle_from_eid(entity_id eid) noexcept
        {
            return handle{eid, get_counter_from_eid(eid)};
        }

    public:
        auto create_entity()
        {
            auto res(create_entity_and_get_eid());

            ELOG(                                                       // .
                debug::lo_entity() << "Created (eID: " << res << ")\n"; // .
            );

            ECST_ASSERT(alive(res));
            ECST_ASSERT(ctx::entity::is_valid_id(res));

            return res;
        }

        // TODO: interface to create null handle?

        auto create_handle(entity_id eid) noexcept
        {
            return create_entity_handle_from_eid(eid);
        }

        auto create_entity_and_handle()
        {
            ELOG(                                               // .
                debug::lo_entity() << "Creating handle for:\n"; // .
            );

            return create_handle(create_entity());
        }

        auto alive(entity_id eid) const noexcept
        {
            return _container.alive(eid);
        }

        auto alive(const handle& h) const noexcept
        {
            return alive(access(h));
        }

        auto valid_handle(const handle& h) const noexcept
        {
            return ctx::entity::is_valid_id(h._id);
        }

        auto access(const handle& h) const noexcept
        {
            ECST_ASSERT(valid_handle(h));
            return h._id;
        }

        auto reclaim(entity_id eid)
        {
            ELOG(                                                         // .
                debug::lo_entity() << "Reclaimed (eID: " << eid << ")\n"; // .
            );

#if defined(ECST_LOG_ENABLED)
            if(!alive(eid))
            {
                ELOG(                                                // .
                    debug::lo_entity() << "Reclaiming (eID: " << eid // .
                                       << ") twice\n";               // .
                );
            }
#endif

            // Increment validity counter and reset bitset.
            get_metadata(eid).reset();

            auto res = _container.reclaim(eid);
            ECST_ASSERT(!alive(eid));
            return res;
        }

        auto& get_metadata(entity_id eid)
        {
            return _container.get(eid);
        }

        const auto& get_metadata(entity_id eid) const
        {
            return _container.get(eid);
        }
    };

    template <typename TSettings, typename TEntityMetadata>
    using dynamic_entity_storage = data<               // .
        container::dynamic<TSettings, TEntityMetadata> // .
        >;

    template <typename TEntityMetadata, sz_t TCapacity>
    using fixed_entity_storage = data<               // .
        container::fixed<TEntityMetadata, TCapacity> // .
        >;
} // namespace ecst::ctx::storage::entity::impl
