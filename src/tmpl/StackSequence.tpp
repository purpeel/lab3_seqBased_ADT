#include <cstdlib>

template <typename T>
StackSequence<T>::StackSequence() : stack() {}

template <typename T>
StackSequence<T>::StackSequence( const Stack<T>& src ) : stack( src ) {}

template <typename T>
StackSequence<T>::StackSequence( const StackSequence<T>& src ) : stack( src.stack ) {}

template <typename T>
StackSequence<T>& StackSequence<T>::operator=( const Stack<T>& src ) {
    if ( this != &src ) {
        this->stack = src;
    }
    return *this;
}

template <typename T>
StackSequence<T>& StackSequence<T>::operator=( const StackSequence<T>& src ) {
    if ( this != &src ) {
        this->stack = src.stack;
    }
    return *this;
}

template <typename T>
StackSequence<T>::StackSequence( Stack<T>&& src ) : stack( std::move( src ) ) {}

template <typename T>
StackSequence<T>::StackSequence( StackSequence<T>&& src ) : stack( std::move( src.stack ) ) {}

template <typename T>
StackSequence<T>& StackSequence<T>::operator=( Stack<T>&& src ) {
    if ( this != &src ) {
        this->stack.~Stack();
        this->stack = std::move( src );
    }
    return *this;
}

template <typename T>
StackSequence<T>& StackSequence<T>::operator=( StackSequence<T>&& src ) {
    if ( this != &src ) {
        this->stack.~Stack();
        this->stack = std::move( src.stack );
    }
    return *this;
}

template <typename T>
Sequence<T>* StackSequence<T>::clone() const {
    return new StackSequence<T>( *this );
}

template <typename T>
void StackSequence<T>::copy( const Sequence<T>& src ) {
    try {
        this->stack.~Stack();
        this->stack = *new Stack<T>();
        for ( int index = src.getSize() - 1; index >= 0; index-- ) {
            this->stack.push( src[index] );
        }
    } catch ( std::bad_alloc& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void StackSequence<T>::clear() {
    this->stack.clear();
}

template <typename T>
StackSequence<T>::~StackSequence() {}

template <typename T>
void StackSequence<T>::append( const T& value ) {
    this->stack.reverse();;
    this->stack.push( value );
    this->stack.reverse();;
}

template <typename T>
void StackSequence<T>::prepend( const T& value ) {
    this->stack.push( value );
}

template <typename T>
void StackSequence<T>::insertAt( const T& value, const int pos ) {
    if ( pos < 0 || pos > this->stack.getSize() ) {
        throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
    }
    
    try {
        if ( pos == 0 ) { 
            this->prepend( value ); 
            return; 
        } else if ( pos == this->stack.getSize() ) { 
            this->append( value ); 
            return;
        } else {
            Stack<T>* temp1 = this->stack.getSubStack( 0, pos );
            Stack<T>* temp2 = this->stack.getSubStack( pos, this->stack.getSize() );
            temp2->push( value );
            this->stack.clear();
            this->stack = *temp1->concat( *temp2 );
            delete temp2;
        }
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void StackSequence<T>::setAt( const T& value, const int pos ) {
    if ( pos < 0 || pos >= this->stack.getSize() ) {
        throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
    }
    
    try {
        if ( pos == 0 ) { 
            this->stack.pop();
            this->stack.push( value );
            return; 
        } else if ( pos == this->stack.getSize() - 1 ) { 
            this->stack.reverse();

            this->stack.pop();
            this->stack.push( value );

            this->stack.reverse();
            return;
        } else {
            Stack<T>* temp1 = this->stack.getSubStack( 0, pos );
            Stack<T>* temp2 = this->stack.getSubStack( pos, this->stack.getSize() );
            temp2->pop();
            temp2->push( value );
            this->stack.clear();
            this->stack = *temp1->concat( *temp2 );
            delete temp2;
        }
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void StackSequence<T>::removeAt( const int pos ) {
    if ( pos < 0 || pos >= this->stack.getSize() ) {
        throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
    }

    try {
        if ( pos == 0 ) {
            this->stack.pop();
            return;
        } else if ( pos == this->stack.getSize() - 1 ) {
            this->stack.reverse();
            this->stack.pop();
            this->stack.reverse();
            return;
        } else {
            Stack<T>* temp1 = this->stack.getSubStack( 0, pos );
            Stack<T>* temp2 = this->stack.getSubStack( pos + 1, this->stack.getSize() );
            this->stack.clear();
            this->stack = *temp1->concat( *temp2 );
            delete temp2;
        }
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void StackSequence<T>::swap( const int pos1, const int pos2 ) {
    if ( pos1 < 0 || pos1 >= this->stack.getSize() ||
         pos2 < 0 || pos2 >= this->stack.getSize()  ) {
        throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
    }

    try {
        Stack<T>* temp1 = this->stack.getSubStack( 0, pos1 );
        Stack<T>* temp2 = this->stack.getSubStack( pos1, pos2 );
        Stack<T>* temp3 = this->stack.getSubStack( pos2, this->stack.getSize() );

        T elem1 = temp2->getTop();
        T elem2 = temp3->getTop();
        temp2->pop();
        temp3->pop();
        temp2->push( elem2 );
        temp3->push( elem1 );
        this->stack.clear();
        this->stack = *temp1->concat( *temp2 )->concat( *temp3 );
        delete temp2;
        delete temp3;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Sequence<T>* StackSequence<T>::getSubSequence( const int startIndex, const int endIndex ) const {
    try {
        auto newSequence = new StackSequence<T>( *this->stack.getSubStack( startIndex, endIndex ) );
        return newSequence;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Sequence<T>* StackSequence<T>::concat( const Sequence<T>& other ) {
    try {
        for ( int index = 0; index < other.getSize(); index++ ) {
            this->stack.reverse();
            this->stack.push( other[index] );
            this->stack.reverse();
        }
        return this;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void StackSequence<T>::map( T (*func)( const T& value ) ) {
    this->stack.map( func );
}

template <typename T>
void StackSequence<T>::where( bool (*func)( const T& value ) ) {
    this->stack.where( func );
}

template <typename T>
T StackSequence<T>::foldl( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const {
    return this->stack.foldl( func, base );
}

template <typename T>
T StackSequence<T>::foldr(T (*func)(const T& arg1, const T& arg2), const T& base) const {
    return this->stack.foldr(func, base);
}

template <typename T>
T StackSequence<T>::foldl( T (*func)( const T& arg1, const T& arg2 ) ) const {
    return this->stack.foldl( func );
}

template <typename T>
T StackSequence<T>::foldr( T (*func)( const T& arg1, const T& arg2 ) ) const {
    return this->stack.foldr( func );
}

template <typename T>
T& StackSequence<T>::operator[]( const int pos ) {
    try {
        Stack<T>* temp = this->stack.getSubStack( pos, pos + 1 );
        return temp->getTop();
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
const T& StackSequence<T>::operator[]( const int pos ) const {
    try {
        Stack<T>* temp = this->stack.getSubStack(pos, pos + 1);
        return temp->getTop();
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
const bool StackSequence<T>::isEmpty() const {
    return this->stack.isEmpty();
}

template <typename T>
const int StackSequence<T>::getSize() const {
    return this->stack.getSize();
}

template <typename T>
Sequence<T>* StackSequence<T>::appendImmutable( const T& value ) const {
    try {
        StackSequence<T>* res = new StackSequence<T>( *this );
        res->append( value );
        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Sequence<T>* StackSequence<T>::prependImmutable( const T& value ) const {
    try {
        StackSequence<T>* res = new StackSequence<T>( *this );
        res->prepend( value );
        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Sequence<T>* StackSequence<T>::insertAtImmutable( const T& value, const int pos ) const {
    try {
        StackSequence<T>* res = new StackSequence<T>( *this );
        res->insertAt( value, pos );
        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Sequence<T>* StackSequence<T>::removeAtImmutable(const int pos) const {
    try {
        StackSequence<T>* res = new StackSequence<T>( *this );
        res->removeAt(pos);
        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Sequence<T>* StackSequence<T>::swapImmutable(const int pos1, const int pos2) const {
    try {
        StackSequence<T>* res = new StackSequence<T>( *this );
        res->swap(pos1, pos2);
        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Sequence<T>* StackSequence<T>::setAtImmutable( const T& value, const int pos ) const {
    try {
        StackSequence<T>* res = new StackSequence<T>( *this );
        res->setAt(value, pos);
        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Sequence<T>* StackSequence<T>::concatImmutable( const Sequence<T>& other ) const {
    try {
        StackSequence<T>* res = new StackSequence<T>( *this );
        res->concat( other );
        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Sequence<T>* StackSequence<T>::mapImmutable( T (*func)( const T& value ) ) const {
    try {
        StackSequence<T>* res = new StackSequence<T>( *this );
        res->map( func );
        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Sequence<T>* StackSequence<T>::whereImmutable( bool (*func)( const T& value ) ) const {
    try {
        StackSequence<T>* res = new StackSequence<T>( *this );
        res->where( func );
        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
const std::string StackSequence<T>::print() const {
    std::ostringstream* out = new std::ostringstream();
    (*out) << "{";
    for ( int index = 0; index < this->getSize(); index++ ) {
        (*out) << (*this)[index] << " ";
    }
    (*out) << "}";
    return (*out).str();
}
