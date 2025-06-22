#ifndef OPTION_H
#define OPTION_H

#include "util.hpp"

template <typename T>
class Option
{
private:
    bool _hasValue;
    union 
    {
        T _value;
        int _dummy;
    };
public:
    Option() : _hasValue(false), _dummy(0) {}

    Option( const T& value ) : _hasValue(true) {
        new(&_value) T(value);
    }
    Option<T>& operator=( const T& value ) {
        if ( _hasValue ) _value.~T();
        _hasValue = true;
        new(&_value) T(value);
        return *this;
    }

    Option( T&& value ) : _hasValue(true) {
        new(&_value) T(std::move(value));
    }
    Option<T>& operator=( T&& value ) {
        if (_hasValue) _value.~T();
        _hasValue = true;
        new(&_value) T(std::move(value));
        return *this;
    }

    Option( const Option<T>& other ) : _hasValue( other._hasValue ) {
        if (_hasValue) {
            new(&_value) T( other._value );
        }
    }
    Option<T>& operator=( const Option& other ) {
        if ( this != &other ) {
            if (_hasValue) _value.~T();
            _hasValue = other._hasValue;
            if (_hasValue) {
                new(&_value) T( other._value );
            }
        }
        return *this;
    }

    Option(Option&& other) : _hasValue(other._hasValue) {
        if (_hasValue) {
            new(&_value) T(std::move(other._value));
            other._value.~T();
            other._hasValue = false;
        }
    }
    Option<T>& operator=(Option&& other) {
        if (this != &other) {
            if (_hasValue) _value.~T();
            _hasValue = other._hasValue;
            if (_hasValue) {
                new(&_value) T(std::move(other._value));
                other._value.~T();
                other._hasValue = false;
            }
        }
        return *this;
    }

    ~Option() {
        if ( _hasValue ) _value.~T();
    } 
public:
    T& get() {
        if ( !_hasValue ) throw Exception( Exception::ErrorCode::EMPTY_OPTION );
        return _value;
    }
    const T& get() const {
        if ( !_hasValue ) throw Exception( Exception::ErrorCode::EMPTY_OPTION );
        return _value;
    }
    
    operator bool() const { return _hasValue; }
    bool hasValue() const { return _hasValue; }

    template <typename F>
    Option<F> map( F (*func)( const T& value ) ) const {
        if ( !_hasValue ) return Option<F>();
        else {
            try {
                return Option<F>( func( _value ) );
            } catch ( Exception& ex ) {
                throw Exception( ex );
            }
        }
    }
};

#endif // OPTION_H