#ifndef HANOI_H
#define HANOI_H

#include "Stack.hpp"

struct Disk 
{
private:
    int diameter;
public:
    Disk() : diameter() {}
    Disk( const int diameter ) : diameter( diameter ) {}
    ~Disk() = default;
};

using pole = Stack<Disk>;
using disk = Disk;

class Hanoi 
{
private:
    pole* _a;
    pole* _b;
    pole* _c;
public:
    Hanoi( const int disksQty = 3 );

    ~Hanoi() = default;
public:
    void solve( const int pyramidHeight, const int from, const int to );
    pole* getPole( const int num ) const;
};

#endif // HANOI_H
