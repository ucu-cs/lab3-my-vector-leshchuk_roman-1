#ifndef TEST_ARRAY_H
#define TEST_ARRAY_H

#include <string>
#include <cassert>
#include <algorithm>
#include <numeric>

#include "my_array.h"

void test_array()
{
    // test comparisons
    assert((my_array<int, 3>{ 1, 2, 3 } == my_array<int, 3>{ 1, 2, 3 }));
    assert((my_array<int, 3>{ 1, 2, 3 } == my_array<double, 3>{ 1.0, 2.0, 3.0 }));
    assert((my_array<int, 3>{ 1, 3, 3 } != my_array<int, 3>{ 1, 2, 3 }));
    assert((my_array<int, 3>{ 1, 2 } != my_array<int, 3>{ 1, 2, 3 }));
    assert((my_array<int, 3>{ 1, 2, 2 } < my_array<int, 3>{ 1, 2, 3 }));
    assert((my_array<int, 3>{ 1, 2 } < my_array<int, 3>{ 1, 2, 3 }));
    assert((my_array<int, 3>{ 1, 2, 3 } > my_array<int, 3>{ 1, 2 }));
    assert((my_array<int, 3>{ 1, 2, 3 } > my_array<int, 3>{ 1, 2 }));
    assert((my_array<int, 3>{ 1, 2, 3 } >= my_array<int, 3>{ 1, 2 }));
    assert((my_array<int, 3>{ 1, 2, 3 } >= my_array<int, 3>{ 1, 2, 3 }));
    assert((my_array<int, 0>{} == my_array<double, 0>{}));

    // test iterators
    my_array<int, 3> arr{};
    assert(arr.begin() < arr.end());
    assert(arr.begin() + 1 > arr.begin());
    std::iota(arr.begin(), arr.end(), 0);
    assert((arr == my_array<int, 3>{ 0, 1, 2 }));
    std::reverse(arr.begin(), arr.end());
    assert((arr == my_array<int, 3>{ 2, 1, 0 }));
    const my_array<int, 3> constArr = arr;
    std::transform(constArr.cbegin(), constArr.cend(), arr.begin(), [](int i) { return i * 2; });
    assert((arr == my_array<int, 3>{ 4, 2, 0 }));

    int repr = 0;
    for (int i : arr)
    {
        repr *= 10;
        repr += i;
    }
    assert(repr == 420);

    repr = 0;
    for (auto i = arr.rbegin(); i != arr.rend(); ++i)
    {
        repr *= 10;
        repr += *i;
    }
    assert(repr == 24);

    repr = 0;
    for (auto i = constArr.crbegin(); i != constArr.crend(); ++i)
    {
        repr *= 10;
        repr += *i;
    }
    assert(repr == 12);

    my_array<int, 3>::iterator it = arr.begin();
    my_array<int, 3>::const_iterator constIt = it;
    assert(*constIt == 4);

    my_array<int, 3>::reverse_iterator reverseIt = arr.rbegin();
    my_array<int, 3>::const_reverse_iterator constReverseIt = reverseIt;
    assert(*constReverseIt == 0);

    assert(arr.begin()[1] == arr[1]);
    assert(arr.end()[-1] == arr[2]);
    assert(arr.rbegin()[1] == arr.end()[-2]);
    assert(arr.rend()[-1] == *arr.begin());

    const my_array<std::string, 3> strArr{ "aaa", "bbb", "ccc" };
    assert(strArr.cbegin()->size() == 3);
    assert(*(strArr.cbegin() + 2) == "ccc");
    assert(strArr.cbegin() + 2 == strArr.cend() - 1);

    // test methods
    bool caughtError = false;
    try
    {
        my_array<int, 3>{}.at(3);
    }
    catch (const my_array_out_of_range&)
    {
        caughtError = true;
    }
    assert(caughtError);

    assert(arr.at(0) == 4);
    assert(arr.at(1) == 2);
    assert(arr.at(2) == 0);
    assert(arr[0] == 4);
    assert(arr[1] == 2);
    assert(arr[2] == 0);

    arr.front() *= 2;
    arr.back() = 42;
    assert((arr == my_array<int, 3>{ 8, 2, 42 }));
    assert(arr.data()[0] == 8);
    assert(arr.data()[1] == 2);
    assert(arr.data()[2] == 42);

    constexpr my_array<int, 2> otherConstArr{ 12, 34 };
    static_assert(otherConstArr.front() == 12);
    static_assert(otherConstArr.back() == 34);

    assert(!strArr.is_empty());
    assert((my_array<int, 0>{}.is_empty()));

    assert((my_array<int, 42>{}.size() == 42));

    arr.fill(42);
    assert((arr == my_array<int, 3>{ 42, 42, 42 }));
    my_array<int, 3> otherArr{};
    otherArr.fill(7);
    arr.swap(otherArr);
    assert((arr == my_array<int, 3>{ 7, 7, 7 }));
    assert((otherArr == my_array<int, 3>{ 42, 42, 42 }));

    // test complicated types
    my_array<my_array<std::string, 2>, 3> arrTwoDim{
        my_array<std::string, 2>{ "1", "2" },
        my_array<std::string, 2>{ "3", "4" },
        my_array<std::string, 2>{ "5", "6" }
    };
    assert(arrTwoDim[0][0] == "1");
    assert(arrTwoDim[0][1] == "2");
    assert(arrTwoDim[1][0] == "3");
    assert(arrTwoDim[1][1] == "4");
    assert(arrTwoDim[2][0] == "5");
    assert(arrTwoDim[2][1] == "6");
}

#endif
