#include <iostream>
#include <cstdlib>

template <typename T>
Flist<T>::Flist() : _head(), _tail(), size(0) {}

template <typename T>
Flist<T>::Flist( const T& head, Flist<T>* tail ) 
    : _head( head ), 
      _tail( tail ), 
       size( tail->getSize() + 1) {}

template <typename T>
Flist<T>::Flist( Flist<T>&& other ) {
    this->_head = std::move( other._head );
    this->_tail = other._tail;
    this->size = other.size;
     
    other._tail = nullptr;
    other.size = 0;
}

template <typename T>
Flist<T>& Flist<T>::operator=( Flist<T>&& other ) {
    if ( this == &other ) { return *this; }
    this->_head = std::move( other._head );
    this->_tail = other._tail; 
    this->size = other.size;
     
    other._tail = nullptr;
    other.size = 0;

    return *this;
}

template <typename T>
Flist<T>* Flist<T>::clone() const {
    auto* res = new Flist<T>();
    for ( int index = this->getSize() - 1; index >= 0; index-- ) {
        res = res->cons( (*this)[index] );
    }
    
    return res;
}

template <typename T>
Flist<T>::~Flist() {
    this->_head.~Option();
    delete this->_tail;
}

template <typename T>
const T& Flist<T>::head() const {
    try {
        return this->_head.get();
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
const T& Flist<T>::last() const {
    try {
        if ( this->_tail->isEmpty() ) { return this->head(); }
        return this->_tail->last();
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Flist<T>* Flist<T>::tail() const {
    return this->_tail;
}

template <typename T>
Flist<T>* Flist<T>::init() const {
    try {
        auto* res = new Flist<T>();
        
        if ( this->getSize() == 1 ) { return res; }
        else if ( this->getSize() == 2 ) { return res->cons( this->head() ); }

        for ( int index = this->getSize() - 2; index >= 0; index-- ) {
            res = res->cons( (*this)[index] );
        }
        
        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
std::tuple<T, Flist<T>*> Flist<T>::uncons() const {
    try {
        return std::make_tuple( this->head(), this->tail() );
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
std::tuple<Flist<T>*, T> Flist<T>::unsnoc() const {
    try {
        return std::make_tuple( this->init(), this->last() );
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Flist<T>* Flist<T>::getSubList( const int startIndex, const int endIndex ) const {
    if ( startIndex < 0                || endIndex < 0 ||
         startIndex >= this->getSize() || endIndex > this->getSize() ||
         startIndex > endIndex ) {
            throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
        }
    
    try {
        Flist<T>* sub = new Flist<T>();
        for ( int index = this->getSize() - 1; index >= 0; index-- ) {
            if ( index >= startIndex && index < endIndex ) {
                sub = sub->cons( (*this)[index] );
            }
        }
        return sub;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
T& Flist<T>::operator[]( const int pos ) {
    if ( pos < 0 || pos >= this->getSize() ) {
        throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
    }

    auto* current = this;
    for ( int index = 0; index < pos; index++ ) {
        current = current->_tail;
    }

    return current->_head.get();
}

template <typename T>
const T& Flist<T>::operator[]( const int pos ) const {
    if ( pos < 0 || pos >= this->getSize() ) {
        throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
    }

    auto* current = this;
    for ( int index = 0; index < pos; index++ ) {
        current = current->_tail;
    }

    return current->_head.get();
}

template <typename T>
T Flist<T>::foldl( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const {
    try {
        T res = base;
        for ( int index = 0; index < this->getSize(); index++ ) {
            res = func( res, (*this)[index] );
        }

        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
T Flist<T>::foldr( T (*func)( const T& arg1, const T& arg2 ), const T& base ) const {
    try {
        T res = base;
        for ( int index = this->getSize() - 1; index >= 0; index-- ) {
            res = func( res, (*this)[index] );
        }

        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
T Flist<T>::foldl( T (*func)( const T& arg1, const T& arg2 ) ) const {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    }

    try {
        T res = (*this)[0];
        for ( int index = 1; index < this->getSize(); index++ ) {
            res = func( res, (*this)[index] );
        }

        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
T Flist<T>::foldr( T (*func)( const T& arg1, const T& arg2 ) ) const {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    }

    try {
        T res = (*this)[this->getSize() - 1];
        for ( int index = this->getSize() - 2; index >= 0; index-- ) {
            res = func( res, (*this)[index] );
        }

        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Flist<T>* Flist<T>::cons(const T& value) const {
    try {
        auto* newTail = this->clone();
        return new Flist<T>( value, newTail ); 
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Flist<T>* Flist<T>::swap( const int pos1, const int pos2 ) const {
    if ( pos1 < 0 || pos1 >= this->getSize() ||
         pos2 < 0 || pos2 >= this->getSize() ) {
            throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
        }
    try {
        Flist<T>* res = new Flist<T>();

        for ( int index = this->getSize() - 1; index >= 0; index-- ) {
            if ( index != pos1 && index != pos2 ) {
                res = res->cons( (*this)[index] );
            } else if ( index == pos1 ) {
                res = res->cons( (*this)[pos2] );
            } else if ( index == pos2 ) {
                res = res->cons( (*this)[pos1] );
            }
        }

        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Flist<T>* Flist<T>::reverse() const {
    try {
        Flist<T>* res = new Flist<T>();

        for ( int index = 0; index < this->getSize(); index++ ) {
            res = res->cons( (*this)[index] );
        }

        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Flist<T>* Flist<T>::concat( const Flist<T>& other ) const {
    try {
        Flist<T>* res = other.clone();

        for ( int index = this->getSize() - 1; index >= 0; index-- ) {
            res = res->cons( (*this)[index] );
        }

        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Flist<T>* Flist<T>::map( T (*func)( const T& value ) ) const {
    try {
        if ( this->isEmpty() ) { return new Flist<T>(); }

        auto* mappedTail = this->_tail->map(func);

        return new Flist<T>( func( this->head() ), mappedTail );
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Flist<T>* Flist<T>::where( bool (*func)( const T& value ) ) const {
    try {
        Flist<T>* res = new Flist<T>();

        for ( int index = this->getSize() - 1; index >= 0; index-- ) {
            if ( func( (*this)[index] ) ) {
                res = res->cons( (*this)[index] );
            }
        }

        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
const bool Flist<T>::isEmpty() const {
    return !this->_head.hasValue();
}

template <typename T>
const int Flist<T>::getSize() const {
    return this->size;
}