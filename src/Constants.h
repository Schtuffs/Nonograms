#pragma once

#if defined(__GNUC__) || defined(__clang__)
    #define MACRO_FUNCTION __PRETTY_FUNCTION__
#else
    #define MACRO_FUNCTION __func__
#endif

