#include <cstdlib>
#include <tuple>

template <typename T>
FlistSequence<T>::FlistSequence() : list() {}

template <typename T>
FlistSequence<T>::FlistSequence( const Flist<T>& src ) {
    auto* res = src.clone();
    this->list = std::move(*res);
    delete res;
}

template <typename T>
FlistSequence<T>::FlistSequence( const FlistSequence<T>& src ) {
    auto* res = src.list.clone();
    this->list = std::move(*res);
    delete res;
}

template <typename T>
FlistSequence<T>& FlistSequence<T>::operator=( const Flist<T>& src ) {
    if ( this->list != &src ) {
        auto* res = src.clone();
        this->list = std::move(*res);
        delete res;
    }
    return *this;
}

template <typename T>
FlistSequence<T>& FlistSequence<T>::operator=( const FlistSequence<T>& src ) {
    if ( this != &src ) {
        auto* res = src.list.clone();
        this->list = std::move(*res);
        delete res;
    }
    return *this;
}

template <typename T>
FlistSequence<T>::FlistSequence( Flist<T>&& src ) {
    this->list = std::move(src);
}

template <typename T>
FlistSequence<T>::FlistSequence( FlistSequence<T>&& src ) {
    this->list = std::move(src.list);
}

template <typename T>
FlistSequence<T>& FlistSequence<T>::operator=( Flist<T>&& src ) {
    this->clear();
    this->list = std::move(src);
    return *this;
}

template <typename T>
FlistSequence<T>& FlistSequence<T>::operator=( FlistSequence<T>&& src ) {
    if ( this != &src ) {
        this->clear();
        this->list = std::move(src.list);
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
        this->clear();
        for ( int index = 0; index < src.getSize(); index++ ) {
            this->append( src[index] );
        }
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
FlistSequence<T>::~FlistSequence() {}

template <typename T>
void FlistSequence<T>::clear() {
    list.~Flist();
    auto* empty = new Flist<T>();
    list = std::move(*empty);
    delete empty;
}

template <typename T>
void FlistSequence<T>::append( const T& value ) {
    try {
        auto* singleton = new Flist<T>( value, new Flist<T>() );
        auto* temp = this->list.concat( *singleton );
        this->clear();
        this->list = std::move( *temp );
        delete temp;
        delete singleton;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void FlistSequence<T>::prepend( const T& value ) {
    try {
        auto* temp = this->list.cons( value );
        this->clear();
        this->list = std::move( *temp );
        delete temp;
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
            this->append( value );
        } else if ( pos == this->list.getSize() ) {
            this->append( value );
        } else {
            auto* tail = list.getSubList(pos, list.getSize());
            auto* newTail = tail->cons(value);
            
            auto* head = list.getSubList(0, pos); 
            auto* result = head->concat(*newTail);
            
            this->list = std::move( *result );
            
            delete newTail;
            delete head;
            delete result;
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
        auto* temp1 = this->list.getSubList( 0, pos );
        auto* temp2 = this->list.getSubList( pos, this->list.getSize() );
        std::tuple<T, Flist<T>*> tup = temp2->uncons();
        auto& [val, rest] = tup;
        this->clear();
        auto* res = temp1->concat( *rest );
        this->list = std::move( *res );
        delete res;
        delete temp1;
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
        auto* temp1 = this->list.getSubList( 0, pos );
        auto* temp2 = this->list.getSubList( pos, this->list.getSize() );
        std::tuple<T, Flist<T>*> tup = temp2->uncons();
        auto& [val, rest] = tup;
        rest = rest->cons(value);
        this->clear();
        auto* res = temp1->concat( *rest );
        this->list = std::move( *res );
        delete res;
        delete temp1;
        delete temp2;
    } catch ( Exception& ex ) {
        throw Exception(ex);
    }
}

template <typename T>
void FlistSequence<T>::swap( const int pos1, const int pos2 ) {
    try {
        auto* res = this->list.swap( pos1, pos2 );
        this->clear();
        this->list = std::move( *res );
        delete res;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
Sequence<T>* FlistSequence<T>::getSubSequence( const int startIndex, const int endIndex ) const {
    try {
        auto* subFlist = this->list.getSubList(startIndex, endIndex);
        auto* res = new FlistSequence<T>( *subFlist );
        delete subFlist;
        return res;
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
        auto* list = this->list.map( func );
        this->clear();
        this->list = std::move( *list );
        delete list;
    } catch ( Exception& ex ) {
        throw ex;
    }
}

template <typename T>
void FlistSequence<T>::where( bool (*func)( const T& value ) ) {
    try {
        auto* res = this->list.where( func );
        this->clear();
        this->list = std::move( *res );
        delete res;
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
    return this->list.operator[](pos);
}

template <typename T>
const T& FlistSequence<T>::operator[]( const int pos ) const {
    return this->list.operator[](pos);
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

template <typename T>
const std::string FlistSequence<T>::print() const {
    std::ostringstream out;
    out << "{";
    for ( int index = 0; index < this->getSize(); index++ ) {
        out << this->operator[](index) << " ";
    }
    out << "}";
    return out.str();
}