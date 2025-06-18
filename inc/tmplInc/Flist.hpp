#ifndef FUNCTIONAL_LIST_H
#define FUNCTIONAL_LIST_H

#include "util.hpp"
#include <tuple>
#include <functional>

template <typename T>
class Flist
{
public:
    Flist();

    Flist( const Flist<T>& other );
    Flist<T>& operator=( const Flist<T>& other );

    Flist( Flist<T>&& other );
    Flist<T>& operator=( Flist<T>&& other );

    Flist( const T& value ); // singleton

    Flist<T>* FlistOfSize( const int size ) const;

    ~Flist();
public:
    T& head() const;
    T& last() const;
    
    Flist<T>* tail() const;
    Flist<T>* init() const;

    std::tuple<T, Flist<T>*> uncons() const;
    std::tuple<Flist<T>*, T> unsnoc() const;
public:
    Flist<T>* cons( const T& value );
    Flist<T>* swap( const int pos1, const int pos2 ) const;
public:
    Flist<T>* concat( const Flist<T>& other ) const;
    void reverse();
    Flist<T>* getSubFlist( const int startIndex, const int endIndex ) const;
public:
    Flist<T>* map( T (*func)( const T& value ) ) const;
    Flist<T>* where( bool (*func)( const T& value ) ) const;

    T foldl( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const;
    T foldr( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const;

    T foldl( T (*func)( const T& arg1, const T& arg2 ) ) const;
    T foldr( T (*func)( const T& arg1, const T& arg2 ) ) const;
public:
    T& operator[]( const int pos );
    const T& operator[]( const int pos ) const;
    const bool isEmpty() const;
    const int getSize() const;
private:
    T* _head;
    Flist<T>* _tail;
    int size;
};

#include "../../src/tmpl/Flist.tpp"
#endif // FUNCTIONAL_LIST_H