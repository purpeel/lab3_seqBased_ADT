#ifndef ICOLLECTION_H
#define ICOLLECTION_H

#include "Sequence.hpp"
#include "util.hpp"
#include "../types/person.hpp"
#include "../types/int.hpp"
#include "../types/double.hpp"
#include "../types/string.hpp"
#include <tuple>
#include <type_traits>

class ICollection 
{
public:
    virtual ~ICollection() = default;
public:
    VarType typeId;
public:
    virtual bool isEmpty() const = 0;
    virtual int getSize() const = 0;
    virtual std::string print() const = 0;

    template <typename T>
    void defineType() {
        if constexpr (std::is_same_v<T, int>) {
            typeId = VarType::INT;
        } else if constexpr (std::is_same_v<T, double>) {
            typeId = VarType::DOUBLE;
        } else if constexpr (std::is_same_v<T, std::string>) {
            typeId = VarType::STRING;
        } else if constexpr (std::is_same_v<T, Person>) {
            typeId = VarType::PERSON;
        }
    }
};

// non-defined
template <template <typename> class C, typename... Types>
class ICollectionTuple;

//recursion base
template <template <typename> class C>
class ICollectionTuple<C>
{
protected:
    ICollection* clct = nullptr;
public:
    ~ICollectionTuple() { delete clct; }

    ICollection* getCollection() { return clct; }
    void setCollection(ICollection* ptr) { clct = ptr; }

    template <typename T>
    C<T>* get() {
        throw Exception( Exception::ErrorCode::INVALID_TYPE );
    }

    template <typename T>
    void set(C<T>* ptr) {
        throw Exception( Exception::ErrorCode::INVALID_TYPE );
    }
};

//somewhere in recursion
template <template <typename> class C, typename First, typename ...Rest>
class ICollectionTuple<C, First, Rest...> : public ICollectionTuple<C, Rest...>
{
protected:

    using Base = ICollectionTuple<C, Rest...>;
    C<First>* first;

public:

    ICollectionTuple() : first(nullptr) {}
    ICollectionTuple(C<First>* ptr) : first(ptr) {}

    ~ICollectionTuple() {}

public:
    template <typename T>
    C<T>* get() {
        if constexpr (std::is_same_v<T, First>) {
            return first;
        } else {
            return Base::template get<T>();
        }
    }

    template<typename T>
    void set(C<T>* ptr) {
        if constexpr (std::is_same_v<T, First>)
            first = ptr;
        else
            Base::template set<T>(ptr);
    }
};

#endif // ICOLLECTION_H