#include "../../inc/tmplInc/Hanoi.hpp"

Hanoi::Hanoi( const int DisksQty ) {
    this->_a = new pole();
    this->_b = new pole();
    this->_c = new pole();

    for ( int i = DisksQty; i > 0; i-- ) {
        
        this->_a->push( disk(i) );
    }
}

void Hanoi::solve( const int pyramidHeight, int from, int to ) {
    if ( pyramidHeight == 1 ) {
        this->getPole(to)->push( this->getPole(from)->getTop() );
        this->getPole(from)->pop();
        return;
    } else {

        auto tmp = 6 - from - to;
        solve( pyramidHeight - 1, from, tmp );
        this->getPole(to)->push( this->getPole(from)->getTop() );
        this->getPole(from)->pop();
        solve( pyramidHeight - 1, tmp, to );
    }
}

pole* Hanoi::getPole( const int num ) const {
    switch (num) {
        case 1:
            return this->_a;
        case 2:
            return this->_b;
        case 3:
            return this->_c;
        default:
            return this->_a;
    }
}