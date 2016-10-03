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

template <typename T>
class Sorted
{
private:
    T data;

public:
    Sorted(T&& in) :
        data(in)
    {
        std::sort(data.begin(), data.end());
    }

    Sorted(Sorted<T> const& in) :
        data(in.data)
    {}

    Sorted(Sorted<T>&& in) :
        data(std::move(in.data))
    {}

    T const& get_data()
    {
        return data;
    }

    static T own(Sorted<T>&& in)
    {
        return in.data;
    }
};

template <typename T>
class Shuffled
{
private:
    T data;

public:
    Shuffled(T&& in) :
        data(in)
    {
        std::shuffle(data.begin(), data.end());
    }

    Shuffled(Shuffled<T> const& in) :
        data(in.data)
    {}

    Shuffled(Shuffled<T>&& in) :
        data(std::move(in.data))
    {}

    T const& get_data()
    {
        return data;
    }

    static T own(Shuffled<T>&& in)
    {
        return in.data;
    }
};

template <typename T>
class Unique
{
private:
    T data;

public:
    Unique(T&& in) :
        data(in) ///@todo all these should use move too!!!!
    {
        make_data_unique();
    }

    Unique(Unique<T> const& in) :
        data(in.data)
    {}

    Unique(Unique<T>&& in) :
        data(std::move(in.data))
    {}

    T const& get_data() ///@todo const, above as well ///@todo rename to read and put all into some abstract base class
    {
        return data;
    }

    static T own(Unique<T>&& in)
    {
        return in.data;
    }

private:
    void make_data_unique()
    {
        if (data.empty())
            return;
        auto it = std::unique(data.begin(), data.end());
        data.resize(std::distance(data.begin(), it)); ///@todo this might only work on vec find another method
    }
};

template <typename T>
class UniqueAndSorted
{
private:
    T data;

public:
    UniqueAndSorted(T&& in) :
        data(std::move(in))
    {
        std::sort(data.begin(), data.end());
        make_data_unique();
    }

    UniqueAndSorted(UniqueAndSorted<T> const& in) :
        data(in.data)
    {}

    UniqueAndSorted(UniqueAndSorted<T>&& in) :
        data(std::move(in.data))
    {}

    UniqueAndSorted(Unique<T>&& in) :
        data(std::move(in.data))
    {
        std::sort(data.begin(), data.end());
    }

    UniqueAndSorted(Sorted<T>&& in) :
        data(std::move(in.data))
    {
        make_data_unique();
    }

    T const& get_data() ///@todo const, above as well ///@todo rename to read and put all into some abstract base class
    {
        return data;
    }

    static T own(Unique<T>&& in)
    {
        return in.data;
    }



private:
    void make_data_unique() ///@todo duplicate definition
    {
        if (data.empty())
            return;
        auto it = std::unique(data.begin(), data.end());
        data.resize(std::distance(data.begin(), it)); ///@todo this might only work on vec find another method
    }


};

template <typename T>
class NonNull
{
private:
    T data;

    NonNull(T&& in) :
        data(std::move(in))
    {}

public:

    NonNull(NonNull<T> const& in) :
        data(in)
    {}

    NonNull(NonNull<T>&& in) :
        data(std::move(in.data))
    {}


    T const& get_data() const
    {
        return data;
    }

    ///THROWS
    static NonNull<T> make_non_null(T&& nonNull)
    {
        if (nullptr == nonNull)
            throw std::logic_error("Can't pass nullptr to make_non_null");
        return NonNull<T>(std::move(nonNull));
    }

    static T own(NonNull<T>&& in)
    {
        return in.data;
    }
};
}


#endif // FLAGGEDT_H
