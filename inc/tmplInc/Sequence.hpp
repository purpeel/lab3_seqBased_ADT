#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "util.hpp"
#include <variant>
#include <concepts>

template <typename T>
class Sequence 
{
public:
    virtual Sequence<T>* clone() const = 0;
    
    virtual void copy( const Sequence<T>& src ) = 0;

    virtual ~Sequence() = default;
    virtual void clear() = 0;
public:
    virtual void append( const T& value ) = 0;
    virtual void prepend( const T& value ) = 0;
    virtual void insertAt( const T& value, const int pos ) = 0;
    virtual void removeAt( const int pos ) = 0;
    virtual void setAt( const T& value, const int pos ) = 0;
    virtual void swap( const int pos1, const int pos2 ) = 0;
    virtual Sequence<T>* getSubSequence( const int startIndex, const int endIndex ) const = 0;
    virtual Sequence<T>* concat( const Sequence<T>& other ) = 0;
public:
    virtual T& operator[]( const int pos ) = 0;
    virtual const T& operator[]( const int pos ) const = 0;
public:
    virtual const bool isEmpty() const = 0;
    virtual const int getSize() const = 0;
public: 
    virtual const std::string print() const = 0;
public: // immutable functions
    virtual Sequence<T>* appendImmutable( const T& value ) const = 0;
    virtual Sequence<T>* prependImmutable( const T& value ) const = 0;
    virtual Sequence<T>* insertAtImmutable( const T& value, const int pos ) const = 0;
    virtual Sequence<T>* removeAtImmutable( const int pos ) const = 0;
    virtual Sequence<T>* setAtImmutable( const T& value, const int pos ) const = 0;
    virtual Sequence<T>* swapImmutable( const int pos1, const int pos2 ) const = 0;
    virtual Sequence<T>* concatImmutable( const Sequence<T>& other ) const = 0;
};

enum SequenceType
{
    LIST,
    ARRAY
};

enum VarType
{
    INT,
    DOUBLE
};

#endif // SEQUENCE_H