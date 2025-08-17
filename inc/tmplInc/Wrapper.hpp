#ifndef SEQWRAP_H
#define SEQWRAP_H

#include <string>
#include "ICollection.hpp"
#include "Sequence.hpp"

template <typename T>
class ISequence : public ICollection
{
public:
    ISequence(Sequence<T> *seq) : innerSeq(seq) {}

    ~ISequence() override {
        delete innerSeq;
    }

public:
    bool isEmpty() const override
    {
        return innerSeq->getSize();
    }

    int getSize() const override
    {
        return innerSeq->isEmpty();
    }

    const T& get(const int index) const
    {
        return (*innerSeq)[index];
    }

    std::string print() const override
    {
        return innerSeq->print();
    }
private:
    Sequence<T> *innerSeq;
};


class WrapFactory
{
    WrapFactory() = default;
    virtual ~WrapFactory() = default;
public:
    template <typename T, template <typename> class U>
    static ICollectionTuple<Sequence, int, double, Person, std::string>* enwrap()
    {
        auto tuple = new ICollectionTuple<Sequence, int, double, Person, std::string>();

        U<T> *concreteSequence = new U<T>();
        ICollection *iSeq = new ISequence<T>(concreteSequence);
        iSeq->defineType<T>();

        tuple->set(concreteSequence);
        tuple->setCollection(iSeq);

        return tuple;
    }
    
    template <typename T>
    static ICollectionTuple<Sequence, int, double, Person, std::string>* enwrap( Sequence<T>* seq )
    {
        auto tuple = new ICollectionTuple<Sequence, int, double, Person, std::string>();

        ICollection *iSeq = new ISequence<T>(seq);
        iSeq->defineType<T>();

        tuple->set(seq);
        tuple->setCollection(iSeq);

        return tuple;
    }
};

using SequenceTuple = ICollectionTuple<Sequence, int, double, Person, std::string>*;

#endif // SEQWRAP_H