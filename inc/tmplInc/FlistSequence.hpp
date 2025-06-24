#ifndef FLIST_SEQUENCE_H
#define FLIST_SEQUENCE_H

#include "Sequence.hpp"
#include "Flist.hpp"

template <typename T>
class FlistSequence : public Sequence<T>
{
public:
    FlistSequence();

    FlistSequence( const Flist<T>& src );
    FlistSequence( const FlistSequence<T>& src );
    FlistSequence<T>& operator=( const Flist<T>& src );
    FlistSequence<T>& operator=( const FlistSequence<T>& src );

    FlistSequence( Flist<T>&& src );
    FlistSequence( FlistSequence<T>&& src );
    FlistSequence<T>& operator=( Flist<T>&& src );
    FlistSequence<T>& operator=( FlistSequence<T>&& src );

    Sequence<T>* clone() const override;

    void copy( const Sequence<T>& src ) override;
    void clear() override;
    virtual ~FlistSequence();
public:
    void append( const T& value ) override;
    void prepend( const T& value ) override;
    void insertAt( const T& value, const int pos ) override;
    void setAt( const T& value, const int pos ) override;
    void removeAt( const int pos ) override;
    void swap( const int pos1, const int pos2 ) override;
    Sequence<T>* getSubSequence( const int startIndex, const int endIndex ) const override;
    Sequence<T>* concat( const Sequence<T>& other ) override;
public:
    void map( T (*func)(const T& value) ) override;
    void where( bool (*func)(const T& value) ) override;

    T foldl( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const override;
    T foldr( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const override;
    T foldl( T (*func)( const T& arg1, const T& arg2 ) ) const override;
    T foldr( T (*func)( const T& arg1, const T& arg2 ) ) const override;
public:
    T& operator[]( const int pos ) override;
    const T& operator[]( const int pos ) const override;
public:
    const bool isEmpty() const override;
    const int getSize() const override;
public:
    Sequence<T>* appendImmutable( const T& value ) const override;
    Sequence<T>* prependImmutable( const T& value ) const override;
    Sequence<T>* insertAtImmutable( const T& value, const int pos ) const override;
    Sequence<T>* setAtImmutable( const T& value, const int pos ) const override;
    Sequence<T>* removeAtImmutable( const int pos ) const override;
    Sequence<T>* swapImmutable( const int pos1, const int pos2 ) const override;
    Sequence<T>* concatImmutable( const Sequence<T>& other ) const override;
public:
    Sequence<T>* mapImmutable( T (*func)(const T& value) ) const override;
    Sequence<T>* whereImmutable( bool (*func)(const T& value) ) const override;
public:
    const std::string print() const override;
private:
    Flist<T> list;
};

#include "../../src/tmpl/FlistSequence.tpp"
#endif // FLIST_SEQUENCE_H