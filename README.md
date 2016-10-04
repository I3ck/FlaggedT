FlaggedT
==========
A C++ library for type level flagging.  
`Flagged<T>` offers multiple wrapper types which allow you to add properties to your variables at type level.    


Examples, Tutorials
---------------------
```cpp
using namespace flaggedT;
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

### `NonNull<T>`

```cpp
auto wontCompile = NonNull<int*>::make_non_null(nullptr); //won't compile

int* i = nullptr;
auto throwsException = NonNull<int*>::make_non_null(std::move(i)); //exception

int* i2 = new int(3);
auto nn = NonNull<int*>::make_non_null(std::move(i2)); //works

//This is really useful for code which creates smart pointers
NonNull<shared_ptr<int>> generate() {
    return NonNull<shared_ptr<int>>::make_non_null(make_shared(new int(3)));
}

//Methods using a NonNull won't have to check for nullptr anymore
void no_fear(NonNull<shared_ptr<int>> const& in) {
    int x = *(in.get().get());
}

```

### `NonEmpty<T>`
```cpp
auto emptyVec = std::vector<int>();
auto ne = NonEmpty<std::vector<int>>::make_non_empty(std::move(emptyVec))); //EXCEPTION

auto vec = std::vector<int>({1,2,3});
auto ne2 = NonEmpty<std::vector<int>>::make_non_empty(std::move(vec))); //works just fine

//Methods now can access the first element without checking the size
void access_first(NonEmpty<std::vector<int>> const& in) {
    auto first = in.get()[0]; //this will always work
}
```

### `BiggerThan<T,SIZE>`
```cpp
auto tooSmall = std::vector<int>({1,2,3});
auto throwsException = BiggerThan<std::vector<int>,3>::make_bigger_than(std::move(tooSmall)); //Exception

auto bigEnough = std::vector<int>({1,2,3,4});
auto works = BiggerThan<std::vector<int>,3>::make_bigger_than(std::move(bigEnough));

//Methods now can access the first SIZE-elements without checking the size
void access_four(BiggerThan<std::vector<int>,3> const& in) {
    auto first = in.get()[0]; //this will always work
    auto second = in.get()[1]; //this will always work
    auto third = in.get()[2]; //this will always work
    auto fourth = in.get()[3]; //this will always work
}
```

### `SmallerThan<T,SIZE>`
```cpp
//analog to BiggerThan, but forcing the size to be smaller than SIZE
```

### `FixedSized<T,SIZE>`
```cpp
//analog to BiggerThan and SmallerThan, but enforcing an exact size
```

### Accessing the wrapped data

To ensure that the type reflects the state of the wrapped data, there's only immutable access to it.  
```cpp
auto wrappedExample = Sorted<...>(...); //some flagged type
wrappedExample.get() // returns a const& to the wrapped data
//moves ownership out of the container and destroys it
auto inner = FlaggedTBase<...>::unwrap(std::move(wrappedExample));
```

Version
-------
0.2.0

License
------
MIT (See LICENSE)
