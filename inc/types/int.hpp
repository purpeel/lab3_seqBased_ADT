#ifndef INTEGER_ADDON
#define INTEGER_ADDON


inline int doubleUp( const int& arg ) {
    return arg + arg;
}

inline int sign( const int& arg ) {
    return arg > 0 ? 1 : -1;
}

inline bool isEven( const int& arg ) {
    return (int) arg % 2 == 0;
}


#endif // INTEGER_ADDON