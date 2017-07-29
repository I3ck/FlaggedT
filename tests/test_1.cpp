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

#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

#include <cmath>
#include <vector>

#include "flaggedT.h"

using namespace std;
using namespace flaggedT;

//examples (move these to example files) [also the example test case]
//generator
NonNull<std::unique_ptr<int> > safe_generator()
{
    return NonNull<std::unique_ptr<int> >(std::unique_ptr<int>(new int(5)));
}

int no_need_to_check_for_null(NonNull<std::unique_ptr<int> >&& in)
{
    return *(in.get().get()) + 1;
}

int add_one(int in)
{
    return in + 1;
}

//examples (move these to example files) [also the example test case]

TEST_CASE("FlaggedT")
{
    SECTION("Immutable")
    {
        auto im = Immutable<int>(3);

        int copied = im;

        //ref not allowed
        //int& ref = im;

        int const& cref = im;

        shared_im<int> safe_sharing;

        safe_sharing = make_shared<Immutable<int>>(4);
    }

    SECTION("NonNull")
    {
        //won't even compile, since nullptr_t overload is deleted
        //auto wontCompile = NonNull<int*>::make_non_null(nullptr);

        int* i = nullptr;
        REQUIRE_THROWS(NonNull<int*>(std::move(i)));

        int* i2 = new int(3);
        auto nn = NonNull<int*>(std::move(i2));
        int* i2s = std::move(nn);
        delete i2s;
    }

    SECTION("NonEmpty")
    {
        auto emptyVec = std::vector<int>();
        REQUIRE_THROWS(NonEmpty<std::vector<int> >(std::move(emptyVec)));

        auto goodVec = std::vector<int>({ 1, 2 });
        auto ne = NonEmpty<std::vector<int> >(std::move(goodVec));
        REQUIRE(ne.get().size() == 2);
    }

    SECTION("NonZero")
    {
        int ifail = 0;
        float ffail = 0.0f;
        double dfail = 0.0;

        int iworks = 1;
        float fworks = 1.0f;
        double dworks = 1.0;

        REQUIRE_THROWS(NonZero<int>(std::move(ifail)));
        REQUIRE_THROWS(NonZero<float>(std::move(ffail)));
        REQUIRE_THROWS(NonZero<double>(std::move(dfail)));

        auto i = NonZero<int>(std::move(iworks));
        auto f = NonZero<float>(std::move(fworks));
        auto d = NonZero<double>(std::move(dworks));

        REQUIRE(i.get() == 1);
        REQUIRE(f.get() == 1.0f);
        REQUIRE(d.get() == 1.0);
    }

    SECTION("Positive")
    {
        int ifail = -1;
        float ffail = -1.0f;
        double dfail = -1.0;

        int ifail2 = 0;
        float ffail2 = 0.0f;
        double dfail2 = 0.0;

        int iworks = 1;
        float fworks = 1.0f;
        double dworks = 1.0;

        REQUIRE_THROWS(Positive<int>(std::move(ifail)));
        REQUIRE_THROWS(Positive<float>(std::move(ffail)));
        REQUIRE_THROWS(Positive<double>(std::move(dfail)));

        REQUIRE_THROWS(Positive<int>(std::move(ifail2)));
        REQUIRE_THROWS(Positive<float>(std::move(ffail2)));
        REQUIRE_THROWS(Positive<double>(std::move(dfail2)));

        auto i = Positive<int>(std::move(iworks));
        auto f = Positive<float>(std::move(fworks));
        auto d = Positive<double>(std::move(dworks));

        REQUIRE(i.get() == 1);
        REQUIRE(f.get() == 1.0f);
        REQUIRE(d.get() == 1.0);
    }

    SECTION("NonPositive")
    {
        int ifail = 1;
        float ffail = 1.0f;
        double dfail = 1.0;

        int iworks = -1;
        float fworks = -1.0f;
        double dworks = -1.0;

        REQUIRE_THROWS(NonPositive<int>(std::move(ifail)));
        REQUIRE_THROWS(NonPositive<float>(std::move(ffail)));
        REQUIRE_THROWS(NonPositive<double>(std::move(dfail)));

        auto i = NonPositive<int>(std::move(iworks));
        auto f = NonPositive<float>(std::move(fworks));
        auto d = NonPositive<double>(std::move(dworks));

        REQUIRE(i.get() == -1);
        REQUIRE(f.get() == -1.0f);
        REQUIRE(d.get() == -1.0);
    }

    SECTION("Negative")
    {
        int ifail = 1;
        float ffail = 1.0f;
        double dfail = 1.0;

        int ifail2 = 0;
        float ffail2 = 0.0f;
        double dfail2 = 0.0;

        int iworks = -1;
        float fworks = -1.0f;
        double dworks = -1.0;

        REQUIRE_THROWS(Negative<int>(std::move(ifail)));
        REQUIRE_THROWS(Negative<float>(std::move(ffail)));
        REQUIRE_THROWS(Negative<double>(std::move(dfail)));

        REQUIRE_THROWS(Negative<int>(std::move(ifail2)));
        REQUIRE_THROWS(Negative<float>(std::move(ffail2)));
        REQUIRE_THROWS(Negative<double>(std::move(dfail2)));

        auto i = Negative<int>(std::move(iworks));
        auto f = Negative<float>(std::move(fworks));
        auto d = Negative<double>(std::move(dworks));

        REQUIRE(i.get() == -1);
        REQUIRE(f.get() == -1.0f);
        REQUIRE(d.get() == -1.0);
    }

    SECTION("NonNegative")
    {
        int ifail = -1;
        float ffail = -1.0f;
        double dfail = -1.0;

        int iworks = 1;
        float fworks = 1.0f;
        double dworks = 1.0;

        REQUIRE_THROWS(NonNegative<int>(std::move(ifail)));
        REQUIRE_THROWS(NonNegative<float>(std::move(ffail)));
        REQUIRE_THROWS(NonNegative<double>(std::move(dfail)));

        auto i = NonNegative<int>(std::move(iworks));
        auto f = NonNegative<float>(std::move(fworks));
        auto d = NonNegative<double>(std::move(dworks));

        REQUIRE(i.get() == 1);
        REQUIRE(f.get() == 1.0f);
        REQUIRE(d.get() == 1.0);
    }

    SECTION("Ceiled") {
        constexpr int iMax = 5;
        using iCeiledIncl = CeiledInclusive<int, iMax>;
        using iCeiledExcl = CeiledExclusive<int, iMax>;

        REQUIRE_THROWS(iCeiledIncl(6));
        auto incl = iCeiledIncl(5);
        REQUIRE(incl.get() == 5);

        REQUIRE_THROWS(iCeiledExcl(5));
        auto excl = iCeiledExcl(4);
        REQUIRE(excl.get() == 4);
    }

    SECTION("Floored") {
        constexpr int iMin = 2;
        using iFlooredIncl = FlooredInclusive<int, iMin>;
        using iFlooredExcl = FlooredExclusive<int, iMin>;

        REQUIRE_THROWS(iFlooredIncl(1));
        auto incl = iFlooredIncl(2);
        REQUIRE(incl.get() == 2);

        REQUIRE_THROWS(iFlooredExcl(2));
        auto excl = iFlooredExcl(3);
        REQUIRE(excl.get() == 3);
    }

    SECTION("Bounded") {
        constexpr int iMin = 3;
        constexpr int iMax = 5;
        using iBoundedIncl = BoundedInclusive<int, iMin, iMax>;
        using iBoundedExcl = BoundedExclusive<int, iMin, iMax>;

        REQUIRE_THROWS(iBoundedIncl(2));
        REQUIRE_THROWS(iBoundedIncl(6));

        auto incl1 = iBoundedIncl(3);
        REQUIRE(incl1.get() == 3);

        auto incl2 = iBoundedIncl(5);
        REQUIRE(incl2.get() == 5);

        REQUIRE_THROWS(iBoundedExcl(3));
        REQUIRE_THROWS(iBoundedExcl(5));

        auto excl = iBoundedExcl(4);
        REQUIRE(excl.get() == 4);
    }

    SECTION("MoreThan")
    {
        auto tooSmall = std::vector<int>({ 1, 2, 3 });
        using more3 = MoreThan<std::vector<int>, 3>;
        REQUIRE_THROWS(more3(std::move(tooSmall)));

        auto bigEnough = std::vector<int>({ 1, 2, 3, 4 });
        auto works = more3(std::move(bigEnough));
        REQUIRE(works.get().size() == 4);
    }

    SECTION("LessThan")
    {
        auto tooBig = std::vector<int>({ 1, 2, 3 });
        using less3 = LessThan<std::vector<int>, 3>;
        REQUIRE_THROWS(less3(std::move(tooBig)));

        auto smallEnough = std::vector<int>({ 1, 2 });
        auto works = less3(std::move(smallEnough));
        REQUIRE(works.get().size() == 2);
    }

    SECTION("FixedSized")
    {
        auto tooBig = std::vector<int>({ 1, 2, 3 });
        auto tooSmall = std::vector<int>({ 1 });
        auto correct = std::vector<int>({ 1, 2 });
        using fixed2 = FixedSized<std::vector<int>, 2>;
        REQUIRE_THROWS(fixed2(std::move(tooBig)));
        REQUIRE_THROWS(fixed2(std::move(tooSmall)));

        auto works = fixed2(std::move(correct));
        REQUIRE(works.get().size() == 2);
    }

    SECTION("FixedRange")
    {
        constexpr size_t nMin = 3;
        constexpr size_t nMax = 5;

        using rangedIncl = FixedRangeInclusive<std::vector<int>, nMin, nMax>;

        REQUIRE_THROWS(rangedIncl({1,2}));
        REQUIRE_THROWS(rangedIncl({1,2,3,4,5,6}));

        auto incl1 = rangedIncl({1,2,3});
        REQUIRE(incl1.get().size() == 3);

        auto incl2 = rangedIncl({1,2,3,4,5});
        REQUIRE(incl2.get().size() == 5);
    }

    SECTION("Sorted")
    {
        std::vector<int> unsorted({ 1, 7, 8, 14, 3 });

        REQUIRE(!std::is_sorted(unsorted.begin(), unsorted.end()));

        auto sorted = Sorted<std::vector<int> >(std::move(unsorted));
        REQUIRE(std::is_sorted(sorted.get().begin(), sorted.get().end()));
    }

    SECTION("UNIQUE")
    {
        std::vector<int> duped({ 1, 1, 2, 2, 3, 3, 7, 9, 11 });

        auto unduped = Unique<std::vector<int> >(std::move(duped));
        REQUIRE(unduped.get().size() == 6);
    }

    SECTION("UNIQUESORTED")
    {
        std::vector<int> dupedUnsorted({ 11, 9, 7, 3, 3, 2, 2, 1, 1 });

        auto fixed = UniqueAndSorted<std::vector<int> >(std::move(dupedUnsorted));
        REQUIRE(std::is_sorted(fixed.get().begin(), fixed.get().end()));
        REQUIRE(fixed.get().size() == 6);
    }

    SECTION("Move this to an example file")
    {
        auto tmp = safe_generator();
        auto res = no_need_to_check_for_null(std::move(tmp));

        auto autoUnwrap = NonNegative<int>(3);
        int result = add_one(autoUnwrap);
    }
}
