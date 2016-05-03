// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/context/types.hpp>
#include <ecst/utils/sparse_int_set.hpp>

ECST_NAMESPACE
{
    namespace impl
    {
        template <typename TSettings>
        class to_kill_set_wrapper
        {
        private:
            using set_type = dispatch_set<TSettings>;

            set_type _to_kill_ids;

        public:
            void clear() noexcept
            {
                _to_kill_ids.clear();
            }

            void add_to_kill(entity_id eid);

            template <typename TF>
            void for_to_kill(TF&& f);
        };

        template <typename TSettings>
        void to_kill_set_wrapper<TSettings>::add_to_kill(entity_id eid)
        {
            _to_kill_ids.add(eid);
        }

        template <typename TSettings>
        template <typename TF>
        void to_kill_set_wrapper<TSettings>::for_to_kill(TF&& f)
        {
            _to_kill_ids.for_each(FWD(f));
        }
    }
}
ECST_NAMESPACE_END
