#ifndef PERSON_TYPE_H
#define PERSON_TYPE_H

#include <string>
#include <iostream>
#include <regex>
#include "util.hpp"

class Person 
{
public:
    Person() : _name( "John doe" ), _age( 0 ) {}
    Person( std::string& name, unsigned age ): _name{name}, _age{age} {}
public:
    const std::string& getName() const { return _name; }
    const unsigned getAge() const { return _age; }
 
    void setName( const std::string& name )    { _name = name; }
    void setAge( unsigned age )            { _age = age; }


    bool operator==( const Person& other ) const {
        return _name == other._name && _age == other._age;
    }
private:
    std::string _name;
    unsigned _age;
};

inline std::ostream& operator<<( std::ostream &os, const Person &person )
{
    return os << person.getName() << " " << person.getAge();
}

inline std::istream& operator>>( std::istream& in, Person& person )
{
    std::regex r(R"(([A-Za-z]+(?:\s+[A-Za-z]+)*)\s+([0-9]+))");   
    std::string line;
    std::getline(in, line);
    
    std::smatch matches;
    if ( std::regex_match(line, matches, r) )
    {
        person.setName(matches[1].str());
        person.setAge(std::stoul(matches[2].str()));
    }
    else
    {
        throw Exception( Exception::ErrorCode::INVALID_INPUT );
    }
    return in;
}


#endif // PERSON_TYPE_H