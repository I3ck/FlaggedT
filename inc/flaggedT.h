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

///@todo shared base class
///@todo move assign
///@todo reverse sorted
///@todo shuffled
///@todo rename get_data to get() (same as smart ptrs)
///@todo each class its own .h, but a merging .h file (as in cppOpt)

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

    FlaggedTBase(T&& in) ///@todo delete or hide other constructors
        : data(std::move(in))
    {}

public:
    T const& get() {
        return data;
    }

    static T own(FlaggedTBase<T>&& in) {
        return in.data;
    }
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
        std::sort(base::data.begin(), base::data.end());
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
        std::shuffle(base::data.begin(), base::data.end());
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
        auto it = std::unique(base::data.begin(), base::data.end());
        base::data.resize(std::distance(base::data.begin(), it)); ///@todo this might only work on vec find another method
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
        std::sort(base::data.begin(), base::data.end());
        make_data_unique();
    }

    UniqueAndSorted(UniqueAndSorted<T> const& in) :
        base(in.data) ///@todo all should use base directly instead of its data
    {}

    UniqueAndSorted(UniqueAndSorted<T>&& in) :
        base(std::move(in.data))
    {}

    UniqueAndSorted(Unique<T>&& in) :
        base(std::move(in.data))
    {
        std::sort(base::data.begin(), base::data.end());
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
        auto it = std::unique(base::data.begin(), base::data.end());
        base::data.resize(std::distance(base::data.begin(), it)); ///@todo this might only work on vec find another method
    }
};

//------------------------------------------------------------------------------

template <typename T>
class NonNull : public FlaggedTBase<T>
{
    using base = FlaggedTBase<T>;
private:
    NonNull(T&& in) :
        base(std::move(in))
    {}

public:
    NonNull(NonNull<T> const& in) :
        base(in)
    {}

    NonNull(NonNull<T>&& in) :
        base(std::move(in.data))
    {}

    ///THROWS
    static NonNull<T> make_non_null(T&& nonNull)
    {
        if (nullptr == nonNull)
            throw std::logic_error("Can't pass nullptr to make_non_null");
        return NonNull<T>(std::move(nonNull));
    }
};
}


#endif // FLAGGEDT_H
