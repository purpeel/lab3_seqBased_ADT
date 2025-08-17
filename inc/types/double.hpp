#ifndef DOUBLE_ADDON
#define DOUBLE_ADDON

inline double doubleUp( const double& arg ) {
    return arg + arg;
}

inline double sign( const double& arg ) {
    return arg > 0 ? 1.0 : -1.0;
}

inline bool isEven( const double& arg ) {
    return (int) arg % 2 == 0;
}


#endif // DOUBLE_ADDON