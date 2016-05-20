// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/context/types.hpp>
#include <ecst/settings.hpp>
#include <ecst/debug.hpp>
#include "./bits.hpp"

ECST_CONTEXT_BITSET_NAMESPACE
{
    namespace impl
    {
        /// @brief Bitset wrapper class.
        template <typename TSettings>
        class data
        {
        private:
            using my_bits = bits<TSettings>;
            using my_bitset = typename my_bits::bitset_type;
            my_bitset _bitset;

        public:
            void clear() noexcept
            {
                _bitset.reset();
            }

            template <typename TComponent>
            auto has_component() const noexcept
            {
                return _bitset.test( // .
                    my_bits::template component_bit<TComponent>());
            }

            template <typename TID>
            void set_component_by_id(TID id, bool x) noexcept
            {
                ECST_ASSERT(my_bits::valid_component_id(id));
                _bitset.set(my_bits::component_id_to_bit(id), x);
            }

            template <typename TBit>
            void set_component_by_bit(TBit bit, bool x) noexcept
            {
                ECST_ASSERT(my_bits::valid_component_bit(bit));
                _bitset.set(bit, x);
            }

            template <typename TComponent>
            void set_component(bool x) noexcept
            {
                auto bit = my_bits::template component_bit<TComponent>();

                ELOG(                                                     // .
                    debug::lo_metadata_bitset() << "Setting bit #" << bit // .
                                                << " to " << x << "\n";   // .
                    );

                _bitset.set(bit, x);
            }

            template <typename TBitset>
            auto contains(const TBitset& b) const noexcept
            {
                return (_bitset & b._bitset) == _bitset;
            }

            auto str() const
            {
                return _bitset.to_string();
            }
        };
    }

    template <typename TSettings>
    using dispatch = impl::data<TSettings>;
}
ECST_CONTEXT_BITSET_NAMESPACE_END
