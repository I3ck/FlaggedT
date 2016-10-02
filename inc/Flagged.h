#ifndef FLAGGEDT_H
#define FLAGGEDT_H

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

    static T own(SortedVector<T>&& in)
    {
        return in.datax;
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


#endif // FLAGGEDT_H
