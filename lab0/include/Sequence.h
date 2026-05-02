#pragma once

template<typename TElement>
class Sequence {
public:
    virtual ~Sequence() = default;

    virtual int getLength() const = 0;
    virtual bool getIsEmpty() const = 0;

    virtual TElement Get(int index) const = 0;
    virtual TElement GetFirst() const = 0;
    virtual TElement GetLast() const = 0;

    virtual void Append(TElement item) = 0;
    virtual void Prepend(TElement item) = 0;
    virtual void InsertAt(int index, TElement item) = 0;
    virtual void Remove(TElement item) = 0;
};