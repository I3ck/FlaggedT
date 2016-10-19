FlaggedT
==========
A C++ library for type level flagging.  
`Flagged<T>` offers multiple wrapper types which allow you to add properties to your variables at type level. The wrapped types can still be used as the inner type, thanks to operator overloading.  


Examples, Tutorials
---------------------
```cpp
using namespace flaggedT;
```


### Accessing the wrapped data

To ensure that the type reflects the state of the wrapped data, there's only immutable access to it. Both the moving and const reference conversion operator to T are overloaded.
This makes it possible to move the wrapped data out, or to use them via const reference.

```cpp
auto wrapped = NonNegative<int>(3); //if no exception is thrown, wrapped is now guarenteed >= 0
int abs = easy_abs(wrapped); //using a method defined for the wrapped type
int result = add_one(wrapped); //still able to use methods defined for the inner type

int unwrapped = std::move(wrapped);

int add_one(int in)
{
    return in + 1;
}

int easy_abs(NonNegative<int> in)
{
    return in.get();
}

```

### `NonNull<T>`

```cpp
auto wontCompile = NonNull<int*>(nullptr); //won't compile

int* i = nullptr;
auto throwsException = NonNull<int*>(std::move(i)); //exception

int* i2 = new int(3);
auto nn = NonNull<int*>(std::move(i2)); //works

//This is really useful for code which creates smart pointers
NonNull<shared_ptr<int>> generate() {
    return NonNull<shared_ptr<int>>(make_shared(new int(3)));
}

//Methods using a NonNull won't have to check for nullptr anymore
void no_fear(NonNull<shared_ptr<int>> const& in) {
    int x = *(in.get().get());
}
```

### `Sorted<T>`

```cpp
//creating any Sorted<T> will directly sort its data and keep it that way
auto alwaysSorted = Sorted<std::vector<int>>(std::vector<int>({4,9,2,13,15,17}));
//alwaysSorted.get() == {2,4,9,13,15,17}

int smallest(Sorted<std::vector<int>> const& sorted) {
    //no need to check whether the input is already sorted
    //or even sort it just to retrieve the smallest value

    return sorted.get()[0]; //this might fail if sorted is empty, see NonEmpty
}
```

### `Unique<T>`

```cpp
//creating any Unqique<T> will directly make its data unique and keep it that way
auto alwaysUnique = Unique<std::vector<int>>(std::vector<int>({1,1,2,2,3,3}));
//alwaysUnique.get == {1,2,3}

void algorithm_not_allowing_duplicate_data(Unique<std::vector<int>> const& unique) {
    //no need to check, already ensured by its type
}
```

### `UniqueAndSorted<T>`

```cpp
//always unique and sorted
```

### `Shuffled<T>`

```cpp
//data is randomly shuffled
```

### `NonZero<T>`
```cpp
auto zero = 0;
auto nz = NonZero<int>(std::move(zero))); //EXCEPTION

auto i = 2;
auto nz2 = NonZero<int>(i); //works just fine

//Division by zero? No problem!
void safe_div(int nominator, NonZero<int> const& denominator) {
    return nominator / denominator.get();
}
```

### `Positive<T> : NonZero<T>`
```cpp
//T > 0
```

### `NonPositive<T>`
```cpp
//T <= 0
```

### `Negative<T> : NonZero<T>`
```cpp
//T < 0
```

### `NonNegative<T>`
```cpp
//T >= 0
```

### `CeiledInclusive<T, T MAX>`
```cpp
//T <= MAX
```

### `CeiledExclusive<T, T MAX>`
```cpp
//T < MAX
```

### `FlooredInclusive<T, T MIN>`
```cpp
//T >= MIN
```

### `FlooredExclusive<T, T MIN>`
```cpp
//T > MIN
```

### `BoundedInclusive<T, T MIN, T MAX>`
```cpp
//T >= MIN && T <= MAX
```

### `BoundedExclusive<T, T MIN, T MAX>`
```cpp
//T > MIN && T < MAX
```

### `NonEmpty<T>`
```cpp
auto emptyVec = std::vector<int>();
auto ne = NonEmpty<std::vector<int>>(std::move(emptyVec))); //EXCEPTION

auto vec = std::vector<int>({1,2,3});
auto ne2 = NonEmpty<std::vector<int>>(std::move(vec))); //works just fine

//Methods now can access the first element without checking the size
void access_first(NonEmpty<std::vector<int>> const& in) {
    auto first = in.get()[0]; //this will always work
}
```

### `MoreThan<T,SIZE> : NonEmpty<T>`
```cpp
auto tooSmall = std::vector<int>({1,2,3});
auto throwsException = MoreThan<std::vector<int>,3>(std::move(tooSmall)); //Exception

auto bigEnough = std::vector<int>({1,2,3,4});
auto works = MoreThan<std::vector<int>,3>(std::move(bigEnough));

//Methods now can access the first SIZE-elements without checking the size
void access_four(MoreThan<std::vector<int>,3> const& in) {
    auto first = in.get()[0]; //this will always work
    auto second = in.get()[1]; //this will always work
    auto third = in.get()[2]; //this will always work
    auto fourth = in.get()[3]; //this will always work
}
```

### `LessThan<T,SIZE>`
```cpp
//analog to MoreThan
//T.size() < SIZE
```

### `FixedSized<T,SIZE>`
```cpp
//analog to MoreThan and LessThan, but enforcing an exact size
//T.size() == SIZE
```

### `FixedRangeInclusive<T,MINSIZE,MAXSIZE>`
```cpp
//analog to MoreThan and LessThan, but enforcing a size range
//T.size() >= MINSIZE && T.size() <= MAXSIZE
```

Version
-------
2.3.1

License
------
MIT (See LICENSE)
