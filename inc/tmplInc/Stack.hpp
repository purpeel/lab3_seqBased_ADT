#ifndef STACK_H
#define STACK_H

#include "util.hpp"

template <typename T>
class Stack 
{
public:
    Stack();

    Stack( const Stack<T>& other );
    Stack<T>& operator=( const Stack<T>& other );

    Stack( Stack<T>&& other );
    Stack<T>& operator=( Stack<T>&& other );

    ~Stack();
    void clear();
public:
    void push( const T& value );
    void pop();
    T& getTop() const;
public:
    Stack<T>* concat( const Stack<T>& other );
    Stack<T>* getSubStack( const int startIndex, const int endIndex ) const;
    void reverse();
public:
    Stack<T>* pushImmutable( const T& value ) const;
    Stack<T>* popImmutable() const;
    Stack<T>* concatImmutable( const Stack<T>& other ) const;
    Stack<T>* mapImmutable( T (*func)(const T& value) ) const;
    Stack<T>* whereImmutable( bool (*func)(const T& value) ) const;
    Stack<T>* reverseImmutable() const;
public:
    const int getSize() const;
    const bool isEmpty() const;
public: 
    void map( T (*func)(const T& value) );
    void where( bool (*func)(const T& value) );

    T foldl( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const;
    T foldr( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const;
    T foldl( T (*func)( const T& arg1, const T& arg2 ) ) const;
    T foldr( T (*func)( const T& arg1, const T& arg2 ) ) const;
private:
    struct Node
    {
        T data;
        Node* next;
    };
    Node* top;
    int size;
};

#include "../../src/tmpl/Stack.tpp"
#endif // STACK_H