// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

// #define ECST_LOG_ENABLED 1

#if defined(ECST_LOG_ENABLED)
#define ELOG(...)   \
    do              \
    {               \
        __VA_ARGS__ \
    } while(false)
#else
#define ELOG(...) \
    do            \
    {             \
    } while(false)
#endif
