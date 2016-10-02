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

#define DELTA 0.01

TEST_CASE("TMP") {
    SECTION("TMP") {
        int* i;
        REQUIRE_THROWS(NonNull<int*>::make_non_null(std::move(i)));

        int* i2 = new int(3);
        auto nn = NonNull<int*>::make_non_null(std::move(i2));
        auto i2s = NonNull<int*>::own(std::move(nn));
        delete i2s;
    }

    SECTION("TMP2") {
        std::vector<int> i({1,7,8,14,3});
        auto sortedMove = Sorted<std::vector<int>>(std::move(i));
    }
}
