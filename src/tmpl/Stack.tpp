#include <iostream>
#include <cstdlib>

template <typename T>
Stack<T>::Stack() {
    this->top = nullptr;
    this->size = 0;
}

template <typename T>
Stack<T>::Stack( const Stack<T>& other ) {
    this->top = nullptr;
    this->size = 0;

    auto current = other.top;
    auto temp = new Stack<T>();
    while ( current ) {
        this->push( current->data );
        if ( !current->next ) break;
        current = current->next;
    }
    this->reverse();
}

template <typename T>
Stack<T>& Stack<T>::operator=( const Stack<T>& other ) {
    if ( this != &other ) {
        this->clear();

        this->top = nullptr;

        auto current = other.top;
        while ( current ) {
            this->push( current->data );
            if ( !current->next ) break;
            current = current->next;
        }
        this->reverse();
    }
    return *this;
}

template <typename T>
Stack<T>::Stack( Stack<T>&& other ) {
    this->top = other.top;
    this->size = other.size;

    other.top = nullptr;
    other.size = 0;
}

template <typename T>
Stack<T>& Stack<T>::operator=( Stack<T>&& other ) {
    if ( this != &other ) {
        this->clear();

        this->top = other.top;
        this->size = other.size;

        other.clear();
    }
    return *this;
}

template <typename T>
Stack<T>::~Stack() {
    this->clear();
}

template <typename T>
void Stack<T>::push( const T& value ) {
    auto newNode = new Node;
    newNode->data = value;
    newNode->next = this->top;
    this->top = newNode;
    this->size++;
}

template <typename T>
T& Stack<T>::getTop() const {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    }
        return this->top->data;
}

template <typename T>
void Stack<T>::pop() {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    }

    auto temp = this->top;
    this->top = this->top->next;
    this->size--;
    delete temp;
}

template <typename T>
Stack<T>* Stack<T>::concat( const Stack<T>& other ) {
    auto current = other.top;
    this->reverse();
    while ( current ) {
        this->push( current->data );
        if ( !current->next ) break;
        current = current->next;
    }
    this->reverse();
    return this;
}

template <typename T>
void Stack<T>::reverse() {
    if ( this->isEmpty() ) return;

    auto current = this->top;
    Node* next = nullptr;
    Node* prev = nullptr;

    while ( current ) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    this->top = prev;

}

template <typename T>
void Stack<T>::map( T (*func)( const T& value ) ) {
    auto current = this->top;
    while ( current ) {
        current->data = func( current->data );
        current = current->next;
    }
}

template <typename T>
void Stack<T>::where( bool (*func)( const T& value ) ) {
    auto res = new Stack<T>();
    auto current = this->top;
    while ( current ) {
        if ( func( current->data ) ) {
            res->push( current->data );
        }
        current = current->next;
    }
    *this = *res;
}

template <typename T>
T Stack<T>::foldl( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const {
    auto current = this->top;
    T res = base;
    while ( current ) {
        res = func( res, current->data );
        current = current->next;
    }
    return res;
}

template <typename T>
T Stack<T>::foldl( T (*func)( const T& arg1, const T& arg2 ) ) const {
    auto current = this->top;
    T res = current->data;
    while ( current ) {
        current = current->next;
        res = func( res, current->data );
    }
    return res;
}

template <typename T>
T Stack<T>::foldr( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const {
    Stack<T>* tmp = this->reverseImmutable();
    T res = tmp->foldl( func, base );
    return res;
}

template <typename T>
T Stack<T>::foldr( T (*func)( const T& arg1, const T& arg2 ) ) const {
    auto copy = Stack<T>( *this );
    copy.reverse();
    auto current = copy.top;
    T res = current->data;
    while ( current ) {
        current = current->next;
        res = func( res, current->data );
    }
    return res;
}

template <typename T>
Stack<T>* Stack<T>::getSubStack( const int startIndex, const int endIndex ) const {
    if ( startIndex < 0                || endIndex < 0                ||
         startIndex >= this->getSize() || endIndex > this->getSize() ||
         startIndex > endIndex ) {
        throw Exception(Exception::ErrorCode::INDEX_OUT_OF_BOUNDS);
    }
    if ( startIndex == endIndex ) {
        return new Stack<T>();
    }
    auto subStack = new Stack<T>();
    auto current = this->top;
    for ( int index = 0; index < endIndex; index++ ) {
        if ( index >= startIndex ) {
            subStack->push( current->data );
        }
        if ( !current->next ) break;
        current = current->next;
    }
    
    subStack->reverse();
    return subStack;
}

template <typename T>
Stack<T>* Stack<T>::pushImmutable( const T& value ) const {
    Stack<T> *res = new Stack<T>(*this);
    res->push( value );
    return res;
}

template <typename T>
Stack<T>* Stack<T>::popImmutable() const {
    Stack<T> *res = new Stack<T>(*this);
    res->pop();
    return res;
}

template <typename T>
Stack<T>* Stack<T>::concatImmutable( const Stack<T> &other ) const {
    Stack<T> *res = new Stack<T>(*this);
    res.concat( other );
    return res;
}

template <typename T>
Stack<T>* Stack<T>::reverseImmutable() const {
    Stack<T>* res = new Stack<T>( *this );
    res->reverse();
    return res;
}

template <typename T>
Stack<T>* Stack<T>::mapImmutable( T (*func)( const T& value ) ) const {
    Stack<T>* res = new Stack<T>( *this );
    res->map( func );
    return res;
}

template <typename T>
Stack<T>* Stack<T>::whereImmutable( bool (*func)( const T& value ) ) const {
    Stack<T>* res = new Stack<T>( *this );
    res->where( func );
    return res;
}

template <typename T>
const int Stack<T>::getSize() const {
    return this->size;
}

template <typename T>
const bool Stack<T>::isEmpty() const {
    return this->size == 0;
}

template <typename T>
void Stack<T>::clear() {
    while ( this->top ) {
        Node *temp = this->top;
        this->top = this->top->next;
        delete temp;
    }
    this->size = 0;
}