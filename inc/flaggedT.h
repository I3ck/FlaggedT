/*
    Copyright (c) 2016 Martin Buck
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef FLAGGEDT_H
#define FLAGGEDT_H

namespace flaggedT {

//------------------------------------------------------------------------------

template<typename T>
class FlaggedTBase
{
    FlaggedTBase() = delete;

protected:
    T data;

    FlaggedTBase(T const& in)
        : data(in)
    {}

    FlaggedTBase(T&& in)
        : data(std::move(in))
    {}

public:
    T const& get() {
        return data;
    }

    static T unwrap(FlaggedTBase<T>&& in) {
        return in.data;
    }
};

//------------------------------------------------------------------------------

template <typename T>
class NonNull : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
public:
    NonNull(nullptr_t) = delete;

    ///THROWS
    NonNull(T&& in) :
        base(std::move(in))
    {
        if (nullptr == base::data)
            throw std::logic_error("Can't pass nullptr to constructor of NonNull");
    }

    NonNull(NonNull<T> const& in) :
        base(in)
    {}

    NonNull(NonNull<T>&& in) :
        base(std::move(in.data))
    {}
};

//------------------------------------------------------------------------------

template <typename T>
class Sorted : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
public:
    Sorted(T&& in) :
        base(std::move(in))
    {
        std::sort(std::begin(base::data), std::end(base::data));
    }

    Sorted(Sorted<T> const& in) :
        base(in.data)
    {}

    Sorted(Sorted<T>&& in) :
        base(std::move(in))
    {}
};

//------------------------------------------------------------------------------

template <typename T>
class Shuffled : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
public:
    Shuffled(T&& in) :
        base(std::move(in))
    {
        std::shuffle(std::begin(base::data), std::end(base::data));
    }

    Shuffled(Shuffled<T> const& in) :
        base(in.data)
    {}

    Shuffled(Shuffled<T>&& in) :
        base(std::move(in))
    {}
};

//------------------------------------------------------------------------------

template <typename T>
class Unique : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
public:
    Unique(T&& in) :
        base(std::move(in))
    {
        make_data_unique();
    }

    Unique(Unique<T> const& in) :
        base::data(in.data)
    {}

    Unique(Unique<T>&& in) :
        base(std::move(in.data))
    {}

private:
    void make_data_unique()
    {
        if (base::data.empty())
            return;
        base::data.erase(std::unique(std::begin(base::data), std::end(base::data)), std::end(base::data));
    }
};

//------------------------------------------------------------------------------

template <typename T>
class UniqueAndSorted : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
public:
    UniqueAndSorted(T&& in) :
        base(std::move(in))
    {
        std::sort(std::begin(base::data), std::end(base::data));
        make_data_unique();
    }

    UniqueAndSorted(UniqueAndSorted<T> const& in) :
        base(in.data)
    {}

    UniqueAndSorted(UniqueAndSorted<T>&& in) :
        base(std::move(in.data))
    {}

    UniqueAndSorted(Unique<T>&& in) :
        base(std::move(in.data))
    {
        std::sort(std::begin(base::data), std::end(base::data));
    }

    UniqueAndSorted(Sorted<T>&& in) :
        base(std::move(in.data))
    {
        make_data_unique();
    }
private:
    void make_data_unique() ///@todo duplicate definition
    {
        if (base::data.empty())
            return;
        base::data.erase(std::unique(std::begin(base::data), std::end(base::data)), std::end(base::data));
    }
};


//------------------------------------------------------------------------------

template <typename T>
class NonZero : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
public:
    ///THROWS
    NonZero(T&& in) :
        base(std::move(in))
    {
        if (0 == base::data)
            throw std::logic_error("Can't pass 0 to constructor of NonZero");
    }

    NonZero(NonZero<T> const& in) :
        base(in)
    {}

    NonZero(NonZero<T>&& in) :
        base(std::move(in.data))
    {}
};

//------------------------------------------------------------------------------

template <typename T>
class Positive : public NonZero<T>
{
    using base = NonZero<T>;
public:
    ///THROWS
    Positive(T&& in) :
        base(std::move(in))
    {
        if (base::data <= 0)
            throw std::logic_error("Can't pass <= 0 to constructor of Positive");
    }

    Positive(Positive<T> const& in) :
        base(in)
    {}

    Positive(Positive<T>&& in) :
        base(std::move(in.data))
    {}
};

template <typename T>
class NonPositive : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
public:
    ///THROWS
    NonPositive(T&& in) :
        base(std::move(in))
    {
        if (base::data > 0)
            throw std::logic_error("Can't pass > 0 to constructor of NonPositive");
    }

    NonPositive(NonPositive<T> const& in) :
        base(in)
    {}

    NonPositive(NonPositive<T>&& in) :
        base(std::move(in.data))
    {}
};

//------------------------------------------------------------------------------

template <typename T>
class Negative : public NonZero<T>
{
    using base = NonZero<T>;
public:
    ///THROWS
    Negative(T&& in) :
        base(std::move(in))
    {
        if (base::data >= 0)
            throw std::logic_error("Can't pass >= 0 to constructor of Negative");
    }

    Negative(Negative<T> const& in) :
        base(in)
    {}

    Negative(Negative<T>&& in) :
        base(std::move(in.data))
    {}
};

template <typename T>
class NonNegative : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
public:
    ///THROWS
    NonNegative(T&& in) :
        base(std::move(in))
    {
        if (base::data < 0)
            throw std::logic_error("Can't pass < 0 to constructor of NonNegative");
    }

    NonNegative(NonNegative<T> const& in) :
        base(in)
    {}

    NonNegative(NonNegative<T>&& in) :
        base(std::move(in.data))
    {}
};

//------------------------------------------------------------------------------

template <typename T>
class NonEmpty : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
public:
    ///THROWS
    NonEmpty(T&& in) :
        base(std::move(in))
    {
        if (base::data.empty())
            throw std::logic_error("Can't pass empty container to constructor of NonEmpty");
    }

    NonEmpty(NonEmpty<T> const& in) :
        base(in)
    {}

    NonEmpty(NonEmpty<T>&& in) :
        base(std::move(in.data))
    {}
};

//------------------------------------------------------------------------------

template <typename T, std::size_t SIZE>
class BiggerThan : public NonEmpty<T> ///@todo rename and smaller to make clear it's about containers
{
    using base = NonEmpty<T>;
public:
    ///THROWS
    BiggerThan(T&& in) :
        base(std::move(in))
    {
        if (base::data.size() <= SIZE)
            throw std::logic_error("Passed too small container to constructor of BiggerThan");
    }

    BiggerThan(BiggerThan<T, SIZE> const& in) :
        base(in)
    {}

    BiggerThan(BiggerThan<T, SIZE>&& in) :
        base(std::move(in.data))
    {}
};

//------------------------------------------------------------------------------

template <typename T, std::size_t SIZE>
class SmallerThan : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
public:
    ///THROWS
    SmallerThan(T&& in) :
        base(std::move(in))
    {
        if (base::data.size() >= SIZE)
            throw std::logic_error("Passed too big container to constructor of SmallerThan");
    }

    SmallerThan(SmallerThan<T, SIZE> const& in) :
        base(in)
    {}

    SmallerThan(SmallerThan<T, SIZE>&& in) :
        base(std::move(in.data))
    {}
};

//------------------------------------------------------------------------------

template <typename T, std::size_t SIZE>
class FixedSized : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
public:
    ///THROWS
    FixedSized(T&& in) :
        base(std::move(in))
    {
        if (base::data.size() != SIZE)
            throw std::logic_error("Passed container with wrong size to constructor of FixedSized");
    }

    FixedSized(FixedSized<T, SIZE> const& in) :
        base(in)
    {}

    FixedSized(FixedSized<T, SIZE>&& in) :
        base(std::move(in.data))
    {}
};
}


#endif // FLAGGEDT_H
