// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <boost/hana.hpp>
#include <ecst/config.hpp>

namespace ecst
{
    /// @brief Alias for the `boost::hana` namespace.
    namespace bh = boost::hana;

    /// @brief Alias for `typename boost::hana::decay<...>::type`.
    template <typename... Ts>
    using decay_t = typename bh::detail::decay<Ts...>::type;
} // namespace ecst


/// @brief Alias for `decay_t<decltype(...)>`.
#define ECST_DECAY_DECLTYPE(...) ::ecst::decay_t<decltype(__VA_ARGS__)>
