// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <boost/hana.hpp>

ECST_NAMESPACE
{
    namespace bh = boost::hana;

    template <typename... Ts>
    using decay_t = typename bh::detail::decay<Ts...>::type;
}
ECST_NAMESPACE_END

/// @brief Alias for `decay_t<decltype(...)>`.
#define ECST_DECAY_DECLTYPE(...) ::ecst::decay_t<decltype(__VA_ARGS__)>
