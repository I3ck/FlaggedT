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

template <typename T>
class Positive;

template <typename T>
class Negative;

template <typename T, int64_t MAX>
class CeiledInclusive;

template <typename T, int64_t MAX>
class CeiledExclusive;

template <typename T, int64_t MIN>
class FlooredInclusive;

template <typename T, int64_t MIN>
class FlooredExclusive;

template <typename T, int64_t MIN, int64_t MAX>
class BoundedInclusive;

template <typename T, int64_t MIN, int64_t MAX>
class BoundedExclusive;

//------------------------------------------------------------------------------

template <typename T>
class FlaggedTBase {
protected:
    FlaggedTBase() = delete;

    FlaggedTBase(FlaggedTBase const&) = default;
    FlaggedTBase(FlaggedTBase&&) = default;
    FlaggedTBase& operator=(FlaggedTBase const&) = default;
    FlaggedTBase& operator=(FlaggedTBase&&) = default;

    T data;

    FlaggedTBase(T const& in)
        : data(in)
    {
    }

    FlaggedTBase(T&& in)
        : data(std::move(in))
    {
    }

public:
    T const& get() const
    {
        return data;
    }

    operator T const& () const
    {
        return data;
    }

    operator T&& ()
    {
        return std::move(data);
    }
};

//------------------------------------------------------------------------------

template <typename T>
class Immutable final : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;
public:

    Immutable(T const& in)
        : base(in)
    {
    }

    Immutable(T&& in)
        : base(std::move(in))
    {
    }
    Immutable() = delete;

    Immutable(Immutable const&) = default;
    Immutable(Immutable &&) = default;
    Immutable& operator=(Immutable const&) = default;
    Immutable& operator=(Immutable &&) = default;
};

//------------------------------------------------------------------------------

template <typename T>
class shared_im final {
    std::shared_ptr<Immutable<T>> ptr;

public:

    shared_im(T const& in)
    {
        ptr = std::make_shared<Immutable<T>>(in);
    }

    shared_im(T&& in)
    {
        ptr = std::make_shared<Immutable<T>>(std::move(in));
    }

    shared_im() = delete;
    shared_im(shared_im const&) = default;
    shared_im(shared_im &&) = default;
    shared_im& operator=(shared_im const&) = default;
    shared_im& operator=(shared_im &&) = default;


    T const& get() const
    {
        return ptr->get();
    }

    operator T const& () const
    {
        return ptr->get();
    }

    operator T&& ()
    {
        return std::move(ptr->get());
    }
};

//------------------------------------------------------------------------------

template <typename T>
class NonNull : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    NonNull() = delete;
    NonNull(nullptr_t) = delete;

    NonNull(NonNull const&) = default;
    NonNull(NonNull&&) = default;
    NonNull& operator=(NonNull const&) = default;
    NonNull& operator=(NonNull&&) = default;

    ///THROWS
    NonNull(T&& in)
        : base(std::move(in))
    {
        if (nullptr == base::data)
            throw std::logic_error("Can't pass nullptr to constructor of NonNull");
    }
};

//------------------------------------------------------------------------------

template <typename T>
class Sorted : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    Sorted() = delete;

    Sorted(Sorted const&) = default;
    Sorted(Sorted&&) = default;
    Sorted& operator=(Sorted const&) = default;
    Sorted& operator=(Sorted&&) = default;

    Sorted(T&& in)
        : base(std::move(in))
    {
        std::sort(std::begin(base::data), std::end(base::data));
    }
};

//------------------------------------------------------------------------------

template <typename T>
class Shuffled : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    Shuffled() = delete;

    Shuffled(Shuffled const&) = default;
    Shuffled(Shuffled&&) = default;
    Shuffled& operator=(Shuffled const&) = default;
    Shuffled& operator=(Shuffled&&) = default;

    Shuffled(T&& in)
        : base(std::move(in))
    {
        std::shuffle(std::begin(base::data), std::end(base::data));
    }
};

//------------------------------------------------------------------------------

template <typename T>
class Unique : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    Unique() = delete;

    Unique(Unique const&) = default;
    Unique(Unique&&) = default;
    Unique& operator=(Unique const&) = default;
    Unique& operator=(Unique&&) = default;

    Unique(T&& in)
        : base(std::move(in))
    {
        make_data_unique();
    }

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
class UniqueAndSorted : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    UniqueAndSorted() = delete;

    UniqueAndSorted(UniqueAndSorted const&) = default;
    UniqueAndSorted(UniqueAndSorted&&) = default;
    UniqueAndSorted& operator=(UniqueAndSorted const&) = default;
    UniqueAndSorted& operator=(UniqueAndSorted&&) = default;

    UniqueAndSorted(T&& in)
        : base(std::move(in))
    {
        std::sort(std::begin(base::data), std::end(base::data));
        make_data_unique();
    }

    UniqueAndSorted(Unique<T>&& in)
        : base(std::move(in.data))
    {
        std::sort(std::begin(base::data), std::end(base::data));
    }

    UniqueAndSorted(Sorted<T>&& in)
        : base(std::move(in.data))
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
class NonZero : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    NonZero() = delete;

    NonZero(NonZero const&) = default;
    NonZero(NonZero&&) = default;
    NonZero& operator=(NonZero const&) = default;
    NonZero& operator=(NonZero&&) = default;

    ///THROWS
    NonZero(T&& in)
        : base(std::move(in))
    {
        if (0 == base::data)
            throw std::logic_error("Can't pass 0 to constructor of NonZero");
    }

    NonZero(Positive<T>&& in)
        : base(std::move(in.data))
    {}

    NonZero(Negative<T>&& in)
        : base(std::move(in.get()))
    {}

    template <typename U = T, int64_t MIN>
    NonZero(FlooredInclusive<U, MIN>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN > 0, "NonZero can only be constructed by a FlooredInclusive if MIN > 0");
    }

    template <typename U = T, int64_t MIN>
    NonZero(FlooredExclusive<U, MIN>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN >= 0, "NonZero can only be constructed by a FlooredExclusive if MIN >= 0");
    }

    template <typename U = T, int64_t MAX>
    NonZero(CeiledInclusive<U, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MAX < 0, "NonZero can only be constructed by a CeiledInclusive if MAX < 0");
    }

    template <typename U = T, int64_t MAX>
    NonZero(CeiledExclusive<U, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MAX <= 0, "NonZero can only be constructed by a CeiledExclusive if MAX <= 0");
    }

    template <typename U = T, int64_t MIN, int64_t MAX>
    NonZero(BoundedInclusive<U, MIN, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN < 0 && MAX < 0 || MIN > 0 && MAX > 0, "NonZero can only be constructed by a BoundedInclusive if MIN and MAX don't span over 0");
    }

    template <typename U = T, int64_t MIN, int64_t MAX>
    NonZero(BoundedExclusive<U, MIN, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN <= 0 && MAX <= 0 || MIN >= 0 && MAX >= 0, "NonZero can only be constructed by a BoundedExclusive if MIN and MAX don't span over 0");
    }
};

//------------------------------------------------------------------------------

template <typename T>
class Positive : public NonZero<T> {
    using base = NonZero<T>;

public:
    Positive() = delete;

    Positive(Positive const&) = default;
    Positive(Positive&&) = default;
    Positive& operator=(Positive const&) = default;
    Positive& operator=(Positive&&) = default;

    ///THROWS
    Positive(T&& in)
        : base(std::move(in))
    {
        if (base::data <= 0)
            throw std::logic_error("Can't pass <= 0 to constructor of Positive");
    }

    template <typename U = T, int64_t MIN>
    Positive(FlooredInclusive<U, MIN>&& in)
        : base(std::move(in))
    {
        static_assert(MIN > 0, "Positive can only be constructed by a FlooredInclusive if MIN > 0");
    }

    template <typename U = T, int64_t MIN>
    Positive(FlooredExclusive<U, MIN>&& in)
        : base(std::move(in))
    {
        static_assert(MIN >= 0, "Positive can only be constructed by a FlooredExclusive if MIN >= 0");
    }

    template <typename U = T, int64_t MIN, int64_t MAX>
    Positive(BoundedInclusive<U, MIN, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN > 0 && MAX > 0, "Positive can only be constructed by a BoundedInclusive if MIN and MAX > 0");
    }

    template <typename U = T, int64_t MIN, int64_t MAX>
    Positive(BoundedExclusive<U, MIN, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN >= 0 && MAX >= 0, "Positive can only be constructed by a BoundedExclusive if MIN and MAX >= 0");
    }
};

template <typename T>
class NonPositive : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    NonPositive() = delete;

    NonPositive(NonPositive const&) = default;
    NonPositive(NonPositive&&) = default;
    NonPositive& operator=(NonPositive const&) = default;
    NonPositive& operator=(NonPositive&&) = default;

    ///THROWS
    NonPositive(T&& in)
        : base(std::move(in))
    {
        if (base::data > 0)
            throw std::logic_error("Can't pass > 0 to constructor of NonPositive");
    }

    NonPositive(Negative<T>&& in)
        : base(std::move(in.get()))
    {}

    template <typename U = T, int64_t MAX>
    NonPositive(CeiledInclusive<U, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MAX <= 0, "NonPositive can only be constructed by a CeiledInclusive if MAX <= 0");
    }

    template <typename U = T, int64_t MAX>
    NonPositive(CeiledExclusive<U, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MAX <= 0, "NonPositive can only be constructed by a CeiledExclusive if MAX <= 0");
    }

    template <typename U = T, int64_t MIN, int64_t MAX>
    NonPositive(BoundedInclusive<U, MIN, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN <= 0 && MAX <= 0, "NonPositive can only be constructed by a BoundedInclusive if MIN and MAX <= 0");
    }

    template <typename U = T, int64_t MIN, int64_t MAX>
    NonPositive(BoundedExclusive<U, MIN, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN <= 0 && MAX <= 0, "NonPositive can only be constructed by a BoundedExclusive if MIN and MAX <= 0");
    }
};

//------------------------------------------------------------------------------

template <typename T>
class Negative : public NonZero<T> {
    using base = NonZero<T>;

public:
    Negative() = delete;

    Negative(Negative const&) = default;
    Negative(Negative&&) = default;
    Negative& operator=(Negative const&) = default;
    Negative& operator=(Negative&&) = default;

    ///THROWS
    Negative(T&& in)
        : base(std::move(in))
    {
        if (base::data >= 0)
            throw std::logic_error("Can't pass >= 0 to constructor of Negative");
    }

    template <typename U = T, int64_t MAX>
    Negative(CeiledInclusive<U, MAX>&& in)
        : base(std::move(in))
    {
        static_assert(MAX < 0, "Negative can only be constructed by a CeiledInclusive if MAX < 0");
    }

    template <typename U = T, int64_t MAX>
    Negative(CeiledExclusive<U, MAX>&& in)
        : base(std::move(in))
    {
        static_assert(MAX <= 0, "Negative can only be constructed by a CeiledExclusive if MAX <= 0");
    }

    template <typename U = T, int64_t MIN, int64_t MAX>
    Negative(BoundedInclusive<U, MIN, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN < 0 && MAX < 0, "Negative can only be constructed by a BoundedInclusive if MIN and MAX < 0");
    }

    template <typename U = T, int64_t MIN, int64_t MAX>
    Negative(BoundedExclusive<U, MIN, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN <= 0 && MAX <= 0, "Negative can only be constructed by a BoundedExclusive if MIN and MAX <= 0");
    }
};

template <typename T>
class NonNegative : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    NonNegative() = delete;

    NonNegative(NonNegative const&) = default;
    NonNegative(NonNegative&&) = default;
    NonNegative& operator=(NonNegative const&) = default;
    NonNegative& operator=(NonNegative&&) = default;

    ///THROWS
    NonNegative(T&& in)
        : base(std::move(in))
    {
        if (base::data < 0)
            throw std::logic_error("Can't pass < 0 to constructor of NonNegative");
    }

    NonNegative(Positive<T>&& in)
        : base(std::move(in.get()))
    {}

    template <typename U = T, int64_t MIN>
    NonNegative(FlooredInclusive<U, MIN>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN > 0, "NonNegative can only be constructed by a FlooredInclusive if MIN > 0");
    }

    template <typename U = T, int64_t MIN>
    NonNegative(FlooredExclusive<U, MIN>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN >= 0, "NonNegative can only be constructed by a FlooredExclusive if MIN >= 0");
    }

    template <typename U = T, int64_t MIN, int64_t MAX>
    NonNegative(BoundedInclusive<U, MIN, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN >= 0 && MAX >= 0, "NonNegative can only be constructed by a BoundedInclusive if MIN and MAX >= 0");
    }

    template <typename U = T, int64_t MIN, int64_t MAX>
    NonNegative(BoundedExclusive<U, MIN, MAX>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN >= 0 && MAX >= 0, "NonNegative can only be constructed by a BoundedExclusive if MIN and MAX >= 0");
    }
};

//------------------------------------------------------------------------------

template <typename T, int64_t MAX>
class CeiledInclusive : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    CeiledInclusive() = delete;

    CeiledInclusive(CeiledInclusive const&) = default;
    CeiledInclusive(CeiledInclusive&&) = default;
    CeiledInclusive& operator=(CeiledInclusive const&) = default;
    CeiledInclusive& operator=(CeiledInclusive&&) = default;

    ///THROWS
    CeiledInclusive(T&& in)
        : base(std::move(in))
    {
        if (base::data > MAX)
            throw std::logic_error("Passed value to constructor of CeiledInclusive is too big");
    }

    CeiledInclusive(Negative<T>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MAX >= 0, "CeiledInclusive can only be built from a Negative if MAX >= 0");
    }

    CeiledInclusive(NonPositive<T>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MAX >= 0, "CeiledInclusive can only be built from a NonPositive if MAX >= 0");
    }
};

template <typename T, int64_t MAX>
class CeiledExclusive : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    CeiledExclusive() = delete;

    CeiledExclusive(CeiledExclusive const&) = default;
    CeiledExclusive(CeiledExclusive&&) = default;
    CeiledExclusive& operator=(CeiledExclusive const&) = default;
    CeiledExclusive& operator=(CeiledExclusive&&) = default;

    ///THROWS
    CeiledExclusive(T&& in)
        : base(std::move(in))
    {
        if (base::data >= MAX)
            throw std::logic_error("Passed value to constructor of CeiledExclusive is too big");
    }

    CeiledExclusive(Negative<T>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MAX >= 0, "CeiledExclusive can only be built from a Negative if MAX >= 0");
    }

    CeiledExclusive(NonPositive<T>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MAX > 0, "CeiledExclusive can only be built from a NonPositive if MAX > 0");
    }
};

//------------------------------------------------------------------------------

template <typename T, int64_t MIN>
class FlooredInclusive : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    FlooredInclusive() = delete;

    FlooredInclusive(FlooredInclusive const&) = default;
    FlooredInclusive(FlooredInclusive&&) = default;
    FlooredInclusive& operator=(FlooredInclusive const&) = default;
    FlooredInclusive& operator=(FlooredInclusive&&) = default;

    ///THROWS
    FlooredInclusive(T&& in)
        : base(std::move(in))
    {
        if (base::data < MIN)
            throw std::logic_error("Passed value to constructor of FlooredInclusive is too small");
    }

    FlooredInclusive(Positive<T>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN <= 0, "FlooredInclusive can only be built from a Positive if MIN <= 0");
    }

    FlooredInclusive(NonNegative<T>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN <= 0, "FlooredInclusive can only be built from a NonNegative if MIN <= 0");
    }
};

template <typename T, int64_t MIN>
class FlooredExclusive : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    FlooredExclusive() = delete;

    FlooredExclusive(FlooredExclusive const&) = default;
    FlooredExclusive(FlooredExclusive&&) = default;
    FlooredExclusive& operator=(FlooredExclusive const&) = default;
    FlooredExclusive& operator=(FlooredExclusive&&) = default;

    ///THROWS
    FlooredExclusive(T&& in)
        : base(std::move(in))
    {
        if (base::data <= MIN)
            throw std::logic_error("Passed value to constructor of FlooredExclusive is too small");
    }

    FlooredExclusive(Positive<T>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN <= 0, "FlooredExclusive can only be built from a Positive if MIN <= 0");
    }

    FlooredExclusive(NonNegative<T>&& in)
        : base(std::move(in.get()))
    {
        static_assert(MIN < 0, "FlooredExclusive can only be built from a NonNegative if MIN < 0");
    }
};

//------------------------------------------------------------------------------

template <typename T, int64_t MIN, int64_t MAX>
class BoundedInclusive : public FlaggedTBase<T> {
    static_assert(MIN <= MAX, "MIN not <= MAX");

    using base = FlaggedTBase<T>;
public:
    BoundedInclusive() = delete;

    BoundedInclusive(BoundedInclusive const&) = default;
    BoundedInclusive(BoundedInclusive&&) = default;
    BoundedInclusive& operator=(BoundedInclusive const&) = default;
    BoundedInclusive& operator=(BoundedInclusive&&) = default;

    ///THROWS
    BoundedInclusive(T&& in)
        : base(std::move(in))
    {
        if (base::data < MIN)
            throw std::logic_error("Passed value to constructor of BoundedInclusive is too small");
        if (base::data > MAX)
            throw std::logic_error("Passed value to constructor of BoundedInclusive is too big");
    }
};

template <typename T, int64_t MIN, int64_t MAX>
class BoundedExclusive : public FlaggedTBase<T> {
    static_assert(MIN <= MAX, "MIN not <= MAX");

    using base = FlaggedTBase<T>;
public:
    BoundedExclusive() = delete;

    BoundedExclusive(BoundedExclusive const&) = default;
    BoundedExclusive(BoundedExclusive&&) = default;
    BoundedExclusive& operator=(BoundedExclusive const&) = default;
    BoundedExclusive& operator=(BoundedExclusive&&) = default;

    ///THROWS
    BoundedExclusive(T&& in)
        : base(std::move(in))
    {
        if (base::data <= MIN)
            throw std::logic_error("Passed value to constructor of BoundedExclusive is too small");
        if (base::data >= MAX)
            throw std::logic_error("Passed value to constructor of BoundedExclusive is too big");
    }
};

//------------------------------------------------------------------------------

template <typename T>
class NonEmpty : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    NonEmpty() = delete;

    NonEmpty(NonEmpty const&) = default;
    NonEmpty(NonEmpty&&) = default;
    NonEmpty& operator=(NonEmpty const&) = default;
    NonEmpty& operator=(NonEmpty&&) = default;

    ///THROWS
    NonEmpty(T&& in)
        : base(std::move(in))
    {
        if (base::data.empty())
            throw std::logic_error("Can't pass empty container to constructor of NonEmpty");
    }
};

//------------------------------------------------------------------------------

template <typename T, std::size_t SIZE>
class MoreThan : public NonEmpty<T>
{
    using base = NonEmpty<T>;

public:
    MoreThan() = delete;

    MoreThan(MoreThan const&) = default;
    MoreThan(MoreThan&&) = default;
    MoreThan& operator=(MoreThan const&) = default;
    MoreThan& operator=(MoreThan&&) = default;

    ///THROWS
    MoreThan(T&& in)
        : base(std::move(in))
    {
        if (base::data.size() <= SIZE)
            throw std::logic_error("Passed too small container to constructor of MoreThan");
    }
};

//------------------------------------------------------------------------------

template <typename T, std::size_t SIZE>
class LessThan : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    LessThan() = delete;

    LessThan(LessThan const&) = default;
    LessThan(LessThan&&) = default;
    LessThan& operator=(LessThan const&) = default;
    LessThan& operator=(LessThan&&) = default;

    ///THROWS
    LessThan(T&& in)
        : base(std::move(in))
    {
        if (base::data.size() >= SIZE)
            throw std::logic_error("Passed too big container to constructor of LessThan");
    }
};

//------------------------------------------------------------------------------

template <typename T, std::size_t SIZE>
class FixedSized : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    FixedSized() = delete;

    FixedSized(FixedSized const&) = default;
    FixedSized(FixedSized&&) = default;
    FixedSized& operator=(FixedSized const&) = default;
    FixedSized& operator=(FixedSized&&) = default;

    ///THROWS
    FixedSized(T&& in)
        : base(std::move(in))
    {
        if (base::data.size() != SIZE)
            throw std::logic_error("Passed container with wrong size to constructor of FixedSized");
    }
};

//------------------------------------------------------------------------------

template <typename T, std::size_t MINSIZE, std::size_t MAXSIZE>
class FixedRangeInclusive : public FlaggedTBase<T> {
    static_assert(MINSIZE <= MAXSIZE, "MINSIZE not <= MAXSIZE");

    using base = FlaggedTBase<T>;
public:
    FixedRangeInclusive() = delete;

    FixedRangeInclusive(FixedRangeInclusive const&) = default;
    FixedRangeInclusive(FixedRangeInclusive&&) = default;
    FixedRangeInclusive& operator=(FixedRangeInclusive const&) = default;
    FixedRangeInclusive& operator=(FixedRangeInclusive&&) = default;

    ///THROWS
    FixedRangeInclusive(T&& in)
        : base(std::move(in))
    {
        const auto n = base::data.size();
        if (n < MINSIZE)
            throw std::logic_error("Passed too small container to constructor of FixedRangeInclusive");
        if (n > MAXSIZE)
            throw std::logic_error("Passed too big container to constructor of FixedRangeInclusive");
    }
};
}

#endif // FLAGGEDT_H
