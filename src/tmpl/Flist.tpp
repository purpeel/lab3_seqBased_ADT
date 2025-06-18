#include <iostream>
#include <cstdlib>

template <typename T>
Flist<T>::Flist() {
    this->_head = nullptr;
    this->_tail = nullptr;
    this->size = 0;
}

template <typename T>
Flist<T>::Flist( const Flist<T>& other) {
    try {
        if ( other.isEmpty() ) {
            this->_head = nullptr;
            this->_tail = nullptr;
            this->size  = 0;
            return;
        }
        this->_head = new T( *other._head );
        this->_tail = new Flist<T>( *other._tail );
        this->size = other.size;
    } catch ( std::bad_alloc& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Flist<T>& Flist<T>::operator=( const Flist<T>& other ) {
    try {
        if ( this == &other ) return *this;

        if ( other.isEmpty() ) {
            this->_head = nullptr;
            this->_tail = nullptr;
            this->size  = 0;
            return *this;
        }
        this->_head = new T( *other._head );
        this->_tail = new Flist<T>( *other._tail );
        this->size = other.size;

        return *this;
    } catch ( std::bad_alloc& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Flist<T>::Flist( Flist<T>&& other ) {
    this->_head = other._head;
    this->_tail = other._tail;
    this->size  = other.size;

    other._head = nullptr;
    other._tail = nullptr;
    other.size  = 0;
}

template <typename T>
Flist<T>& Flist<T>::operator=( Flist<T>&& other ) {
    if ( this == &other ) return *this;

    this->_head = other._head;
    this->_tail = other._tail;
    this->size  = other.size;

    other._head = nullptr;
    other._tail = nullptr;
    other.size  = 0;

    return *this;
}

template <typename T>
Flist<T>::Flist( const T& value ) {
    try {
        this->_head = new T(value);
        this->_tail = new Flist<T>();
        this->size = 1;
    } catch ( std::bad_alloc& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Flist<T>* Flist<T>::FlistOfSize( const int size ) const {
    try {
        auto list = new Flist<T>();
        auto current = list;
        while ( list->getSize() < size ) {
            current->_head = new T();
            current->_tail = new Flist<T>();
            current = current->_tail;
            list->size++;
        }

        return list;
    } catch ( std::bad_alloc& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Flist<T>::~Flist() {
    delete this->_head;
    delete this->_tail;
}

template <typename T>
T& Flist<T>::head() const {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    }
    return *this->_head;
}

template <typename T>
T& Flist<T>::last() const {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    }

    auto current = this;
    while ( !current->_tail->isEmpty() ) { 
        current = current->_tail;
    }

    return *current->_head;
}

template <typename T>
Flist<T>* Flist<T>::tail() const {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    }
    return new Flist<T>( *this->_tail );
}

template <typename T>
Flist<T>* Flist<T>::init() const {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    }

    try {
        auto list = new Flist<T>();
        auto other = this;
        for ( int index = other->getSize() - 2; index >= 0; index-- ) {
            list = list->cons( (*other)[index] );
        }
        return list;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Flist<T>* Flist<T>::cons( const T& value ) {
    try {
        auto list = new Flist<T>( value );
        list->_tail = this;
        list->size = this->size + 1;
        return list;
    } catch ( std::bad_alloc& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void Flist<T>::reverse() {
    try { 
        if ( this->isEmpty() ) {
            return;
        }
        auto newHead = this->last();
        auto newTail = this->init();
        newTail->reverse();

        delete this->_head;
        delete this->_tail;

        this->_head = new T( newHead );
        this->_tail = newTail;

    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Flist<T>* Flist<T>::swap( const int pos1, const int pos2 ) const {
    if ( pos1 < 0 || pos1 >= this->getSize() ||
         pos2 < 0 || pos2 >= this->getSize() ) {
            throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
        }
    try {
        auto list = new Flist<T>();

        for ( int index = 0; index < this->getSize(); index++ ) {
            if ( index != pos1 && index != pos2 ) {
                list = list->cons( (*this)[index] );
            } else if ( index == pos1 ) {
                list = list->cons( (*this)[pos2] );
            } else if ( index == pos2 ) {
                list = list->cons( (*this)[pos1] );
            }
        }

        list->reverse();
        return list;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
std::tuple<T, Flist<T>*> Flist<T>::uncons() const {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    }

    return std::make_tuple( *this->_head, this->_tail );
}

template <typename T>
std::tuple<Flist<T>*, T> Flist<T>::unsnoc() const {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    }

    return std::make_tuple( this->init(), this->last() );
}

template <typename T>
Flist<T>* Flist<T>::concat( const Flist<T>& other ) const {
    try {
        auto res = new Flist<T>( *this );
        res->reverse();

        for ( int index = 0; index < other.getSize(); index++ ) {
            res = res->cons( other[index] );
        }

        res->reverse();
        return res;
    
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Flist<T>* Flist<T>::getSubFlist( const int startIndex, const int endIndex ) const {
    if ( startIndex < 0                || endIndex < 0 ||
         startIndex >= this->getSize() || endIndex >= this->getSize() ||
         startIndex > endIndex ) {
            throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
        }
    
    try {
        auto sub = new Flist<T>();
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
Flist<T>* Flist<T>::map( T (*func)( const T& value ) ) const {
    try {
        auto res = new Flist<T>( *this );
        if ( this->isEmpty() ) return res;
        *res->_head = func( *this->_head );
        res->_tail = this->_tail->map( func );
        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
Flist<T>* Flist<T>::where( bool (*func)( const T& value ) ) const {
    try {
        auto res = new Flist<T>();
        for ( int index = 0; index < this->getSize(); index++ ) {
            if ( func( (*this)[index] ) ) {
                res = res->cons( (*this)[index] );
            }
        }

        res->reverse();
        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
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
        for ( int index = this->getSize() - 1; index >= 0; index++ ) {
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
        for ( int index = this->getSize() - 2; index >= 0; index++ ) {
            res = func( res, (*this)[index] );
        }

        return res;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
T& Flist<T>::operator[]( const int pos ) {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    } else if ( pos < 0 || pos >= this->getSize() ) {
        throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
    }

    auto current = this;
    for ( int index = 0; index < pos; index++ ) {
        current = current->_tail;
    }

    return *current->_head;
}

template <typename T>
const T& Flist<T>::operator[]( const int pos ) const {
    if ( this->isEmpty() ) {
        throw Exception( Exception::ErrorCode::EMPTY_STRUCTURE );
    } else if ( pos < 0 || pos >= this->getSize() ) {
        throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
    }

    auto current = this;
    for ( int index = 0; index < pos; index++ ) {
        current = current->_tail;
    }

    return *current->_head;
}

template <typename T>
const bool Flist<T>::isEmpty() const {
    auto current = this;
    for ( int index = 0; index < this->getSize(); index++ ) {
        if ( current->_head != nullptr ) return false;
        current = current->_tail;
    }

    return true;
}

template <typename T>
const int Flist<T>::getSize() const {
    return this->size;
}