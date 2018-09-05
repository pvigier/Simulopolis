#pragma once

#define DEBUG_BUILD // Comment for release build

#ifdef DEBUG_BUILD
    #include <iostream>
    #define DEBUG(msg) do { std::cout << msg << std::flush; } while (0)
    #define DEBUG_IF(cond, msg) do { if (cond) std::cout << msg << std::flush; } while (0)
#else
    #define DEBUG(msg) do {} while (0)
    #define DEBUG_IF(cond, msg) do {} while (0)
#endif
