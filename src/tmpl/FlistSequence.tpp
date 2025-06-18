#include <cstdlib>
#include <tuple>

template <typename T>
FlistSequence<T>::FlistSequence() : list() {}

template <typename T>
FlistSequence<T>::FlistSequence( const Flist<T>& src ) : list( src ) {}

template <typename T>
FlistSequence<T>::FlistSequence( const FlistSequence<T>& src ) : list( src.list ) {}

template <typename T>
FlistSequence<T>& FlistSequence<T>::operator=( const Flist<T>& src ) {
    if ( this != &src ) {
        this->list = src;
    }
    return *this;
}

template <typename T>
FlistSequence<T>& FlistSequence<T>::operator=( const FlistSequence<T>& src ) {
    if ( this != &src ) {
        this->list = src.list;
    }
    return *this;
}

template <typename T>
FlistSequence<T>::FlistSequence( Flist<T>&& src ) : list( std::move( src ) ) {}

template <typename T>
FlistSequence<T>::FlistSequence( FlistSequence<T>&& src ) : list( std::move( src.list ) ) {}

template <typename T>
FlistSequence<T>& FlistSequence<T>::operator=( Flist<T>&& src ) {
    if ( this != &src ) {
        this->list.~Flist();
        this->list = std::move( src );
    }
    return *this;
}

template <typename T>
FlistSequence<T>& FlistSequence<T>::operator=( FlistSequence<T>&& src ) {
    if ( this != &src ) {
        this->list.~Flist();
        this->list = std::move( src.list );
    }
    return *this;
}

template <typename T>
Sequence<T>* FlistSequence<T>::clone() const {
    try {
        return new FlistSequence( *this );
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void FlistSequence<T>::copy( const Sequence<T>& src ) {
    try {
        this->list.~Flist();
        this->list = *new Flist<T>();
        for ( int index = src.getSize() - 1; index >= 0; index-- ) {
            this->list.cons( src[index] );
        }
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
FlistSequence<T>::~FlistSequence() {}

template <typename T>
void FlistSequence<T>::clear() {
    this->list.~Flist();
    this->list = *new Flist<T>();
}

template <typename T>
void FlistSequence<T>::append( const T& value ) {
    try {
        this->list.reverse();
        this->list = *this->list.cons( value );
        this->list.reverse();
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void FlistSequence<T>::prepend( const T& value ) {
    try {
        // this->list = *this->list.cons( value );
        auto newew = this->list.cons( value );
        for ( int i = 0; i < newew->getSize(); i++ ) {
            std::cout << (*newew)[i] << std::endl;
        }
        std::cout << std::endl;
        this->list = *newew;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void FlistSequence<T>::insertAt( const T& value, const int pos ) {
    if ( pos < 0 || pos > this->list.getSize() ) {
        throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
    }

    try {
        if ( pos == 0 ) {
            this->list.cons( value );
        } else if ( pos == this->list.getSize() ) {
            this->prepend( value );
        } else {
            Flist<T>* temp1 = this->list.getSubFlist( 0, pos );
            Flist<T>* temp2 = this->list.getSubFlist( pos, this->list.getSize() );
            temp2->cons( value );
            this->clear();
            this->list = *temp1->concat( *temp2 );
            delete temp2;
        }
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void FlistSequence<T>::removeAt( const int pos ) {
    if ( pos < 0 || pos >= this->list.getSize() ) {
        throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
    }
    
    try {
        Flist<T>* temp1 = this->list.getSubFlist( 0, pos );
        Flist<T>* temp2 = this->list.getSubFlist( pos, this->list.getSize() );
        std::tuple<T, Flist<T>*> tup = temp2->uncons();
        auto& [val, rest] = tup;
        this->clear();
        this->list = *temp1->concat( *rest );
        delete temp2;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void FlistSequence<T>::setAt( const T& value, const int pos ) {
    if ( pos < 0 || pos >= this->list.getSize() ) {
        throw Exception( Exception::ErrorCode::INDEX_OUT_OF_BOUNDS );
    }
    
    try {
        Flist<T>* temp1 = this->list.getSubFlist( 0, pos );
        Flist<T>* temp2 = this->list.getSubFlist( pos, this->list.getSize() );
        std::tuple<T, Flist<T>*> tup = temp2->uncons();
        auto& [val, rest] = tup;
        rest->cons( value );
        this->clear();
        this->list = *temp1->concat( *rest );
        delete temp2;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void FlistSequence<T>::swap( const int pos1, const int pos2 ) {
    try {
        Flist<T>* res = this->list.swap( pos1, pos2 );
        this->list = *res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Sequence<T>* FlistSequence<T>::getSubSequence( const int startIndex, const int endIndex ) const {
    try {
        return new FlistSequence<T>( *this->list.getSubFlist( startIndex, endIndex ) );
    } catch ( Exception& ex ) {
        throw ex;
    } 
}

template <typename T>
Sequence<T>* FlistSequence<T>::concat( const Sequence<T>& other ) {
    try {
        for ( int index = 0; index < other.getSize(); index++ ) {
            this->append( other[index] );
        }
        return this;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
void FlistSequence<T>::map( T (*func)(const T& value) ) {
    try {
        Flist<T>* list = this->list.map( func );
        this->clear();
        this->list = *list;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
void FlistSequence<T>::where( bool (*func)( const T& value ) ) {
    try {
        Flist<T>* list = this->list.where( func );
        this->clear();
        this->list = *list;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
T FlistSequence<T>::foldl(  T (*func)( const T& arg1, const T& arg2 ), const T& base ) const {
    try {
        return this->list.foldl( func, base );
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
T FlistSequence<T>::foldr(  T (*func)( const T& arg1, const T& arg2 ), const T& base ) const {
    try {
        return this->list.foldr( func, base );
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
T FlistSequence<T>::foldl(  T (*func)( const T& arg1, const T& arg2 ) ) const {
    try {
        return this->list.foldl( func );
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
T FlistSequence<T>::foldr(  T (*func)( const T& arg1, const T& arg2 ) ) const {
    try {
        return this->list.foldr( func );
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
T& FlistSequence<T>::operator[]( const int pos ) {
    return this->list[pos];
}

template <typename T>
const T& FlistSequence<T>::operator[]( const int pos ) const {
    return this->list[pos];
}

template <typename T>
const bool FlistSequence<T>::isEmpty() const {
    return this->list.isEmpty();
}

template <typename T>
const int FlistSequence<T>::getSize() const {
    return this->list.getSize();
}

template <typename T>
Sequence<T>* FlistSequence<T>::appendImmutable( const T& value ) const {
    try {
        FlistSequence<T>* res = new FlistSequence<T>( *this );
        res->append( value );
        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Sequence<T>* FlistSequence<T>::prependImmutable( const T& value ) const {
    try {
        FlistSequence<T>* res = new FlistSequence<T>( *this );
        res->prepend( value );
        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Sequence<T>* FlistSequence<T>::insertAtImmutable( const T& value, const int pos ) const {
    try {
        FlistSequence<T>* res = new FlistSequence<T>( *this );
        res->insertAt( value, pos );
        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Sequence<T>* FlistSequence<T>::removeAtImmutable( const int pos ) const {
    try {
        FlistSequence<T>* res = new FlistSequence<T>( *this );
        res->removeAt( pos );
        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Sequence<T>* FlistSequence<T>::setAtImmutable(const T& value, const int pos) const {
    try {
        FlistSequence<T>* res = new FlistSequence<T>( *this );
        res->setAt(value, pos);
        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Sequence<T>* FlistSequence<T>::swapImmutable(const int pos1, const int pos2) const {
    try {
        FlistSequence<T>* res = new FlistSequence<T>( *this );
        res->swap(pos1, pos2);
        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Sequence<T>* FlistSequence<T>::concatImmutable( const Sequence<T>& other ) const {
    try {
        FlistSequence<T>* res = new FlistSequence<T>( *this );
        res->concat( other );
        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Sequence<T>* FlistSequence<T>::mapImmutable( T (*func)(const T& value) ) const {
    try {
        FlistSequence<T>* res = new FlistSequence<T>( *this );
        res->map( func );
        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Sequence<T>* FlistSequence<T>::whereImmutable( bool (*func)( const T& value ) ) const {
    try {
        FlistSequence<T>* res = new FlistSequence<T>( *this );
        res->where( func );
        return res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}