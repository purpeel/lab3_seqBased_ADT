#ifndef STRING_ADDON
#define STRING_ADDON

#include <string>
#include "util.hpp"

inline std::string doubleUp( const std::string& other ) {
    return other + other;
}

inline std::string sign( const std::string& other ) {
    if ( other.empty() ) {
        return "";
    }
    return std::string( 1, other[0] );
}

inline bool isEven( const std::string& other ) {
    return other.length() % 2 == 0;
}

#endif // STRING_ADDON