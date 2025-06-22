#ifndef FUNCTIONAL_LIST_H
#define FUNCTIONAL_LIST_H

#include "util.hpp"
#include "Option.hpp"
#include <tuple>
#include <functional>

template <typename T>
class Flist 
{
private:
    Option<T> _head;
    Flist<T>* _tail;
    int size;
public:
    Flist();

    Flist( const T& head, Flist<T>* tail );

    Flist( const Flist<T>& other ) = delete;
    Flist<T>& operator=( const Flist<T>& other ) = delete;

    Flist( Flist<T>&& other );
    Flist<T>& operator=( Flist<T>&& other );

    Flist<T>* clone() const;

    ~Flist();
public:
    const T& head() const;
    const T& last() const;
    
    Flist<T>* init() const;
    Flist<T>* tail() const;

    std::tuple<T, Flist<T>*> uncons() const;
    std::tuple<Flist<T>*, T> unsnoc() const;

    Flist<T>* getSubList( const int startIndex, const int endIndex ) const;

    T& operator[]( const int index );
    const T& operator[]( const int index ) const;
public:
    T foldl( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const;
    T foldr( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const;

    T foldl( T (*func)( const T& arg1, const T& arg2 ) ) const;
    T foldr( T (*func)( const T& arg1, const T& arg2 ) ) const;
public:
    Flist<T>* cons( const T& value ) const;
    Flist<T>* swap( const int pos1, const int pos2 ) const;
    Flist<T>* reverse() const;
    Flist<T>* concat( const Flist<T>& other ) const;

    Flist<T>* map( T (*func)( const T& value ) ) const;
    Flist<T>* where( bool (*func)( const T& value ) ) const;
public:
    const bool isEmpty() const;
    const int getSize() const;
};

#include "../../src/tmpl/Flist.tpp"
#endif // FUNCTIONAL_LIST_H