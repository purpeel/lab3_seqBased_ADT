#ifndef STACKSEQUENCE_H
#define STACKSEQUENCE_H

#include "Sequence.hpp"
#include "Stack.hpp"
#include "util.hpp"

template <typename T>
class StackSequence : public Sequence<T>
{
public:
    StackSequence();

    StackSequence( const Stack<T>& src );
    StackSequence( const StackSequence<T>& src );
    StackSequence<T>& operator=( const Stack<T>& src );
    StackSequence<T>& operator=( const StackSequence<T>& src );

    StackSequence( Stack<T>&& src );
    StackSequence( StackSequence<T>&& src );
    StackSequence<T>& operator=( Stack<T>&& src );
    StackSequence<T>& operator=( StackSequence<T>&& src );

    Sequence<T>* clone() const override;

    void copy( const Sequence<T>& src ) override;
    void clear() override;
    virtual ~StackSequence();
public:
    void append( const T& value ) override;
    void prepend( const T& value ) override;
    void insertAt( const T& value, const int pos ) override;
    void removeAt( const int pos ) override;
    void setAt( const T& value, const int pos ) override;
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
    Sequence<T>* removeAtImmutable( const int pos ) const override;
    Sequence<T>* setAtImmutable( const T& value, const int pos ) const override;
    Sequence<T>* swapImmutable( const int pos1, const int pos2 ) const override;
    Sequence<T>* concatImmutable( const Sequence<T>& other ) const override;
public:
    Sequence<T>* mapImmutable( T (*func)(const T& value) ) const override;
    Sequence<T>* whereImmutable( bool (*func)(const T& value) ) const override;
public:
    // const std::string print() const override;
private:
    Stack<T> stack;
};

#include "../../src/tmpl/StackSequence.tpp"
#endif // STACKSEQUENCE_H