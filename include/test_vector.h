#ifndef TEST_VECTOR_H
#define TEST_VECTOR_H

#include <string>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <numeric>

#include "my_vector.h"

struct Test
{
    int a;
    std::string b;
};

void test_vector()
{
    // test constructors/assignment operators
    my_vector<int> firstVec{ 1, 2, 3 };
    my_vector<int> secondVec({ 5, 6, 7 });
    assert((firstVec == my_vector<int>{ 1, 2, 3 }));
    assert((secondVec == my_vector<int>{ 5, 6, 7 }));
    firstVec = my_vector<int>(secondVec.begin() + 1, secondVec.end());
    assert((firstVec == my_vector<int>{ 6, 7 }));
    firstVec = my_vector<int>(firstVec.rbegin(), firstVec.rend());
    assert((firstVec == my_vector<int>{ 7, 6 }));
    assert((my_vector<int>(firstVec.begin(), firstVec.end()) == firstVec));
    firstVec = std::move(secondVec);
    assert((firstVec == my_vector<int>{ 5, 6, 7 }));
    assert(secondVec.is_empty());
    assert(secondVec.size() == 0);
    assert(secondVec.capacity() == 0);
    secondVec = firstVec;
    assert(secondVec == firstVec);
    secondVec = { 1, 2, 3 };
    assert((secondVec == my_vector<int>{ 1, 2, 3 }));

    // test comparison operators
    assert((my_vector<int>{ 1, 2, 3 } == my_vector<int>{ 1, 2, 3 }));
    assert((my_vector<int>{ 1, 3, 3 } != my_vector<int>{ 1, 2, 3 }));
    assert((my_vector<int>{ 1, 2 } != my_vector<int>{ 1, 2, 3 }));
    assert((my_vector<int>{ 1, 2, 2 } < my_vector<int>{ 1, 2, 3 }));
    assert((my_vector<int>{ 1, 2 } < my_vector<int>{ 1, 2, 3 }));
    assert((my_vector<int>{ 1, 2, 3 } > my_vector<int>{ 1, 2 }));
    assert((my_vector<int>{ 1, 2, 3 } > my_vector<int>{ 1, 2 }));
    assert((my_vector<int>{ 1, 2, 3 } >= my_vector<int>{ 1, 2 }));
    assert((my_vector<int>{ 1, 2, 3 } >= my_vector<int>{ 1, 2, 3 }));

    // test access
    assert(firstVec[0] == 5);
    assert(firstVec[1] == 6);
    assert(firstVec[2] == 7);
    assert(firstVec.at(0) == 5);
    assert(firstVec.at(1) == 6);
    assert(firstVec.at(2) == 7);

    bool caughtError = false;
    try
    {
        firstVec.at(3);
    }
    catch (const my_vector_out_of_range&)
    {
        caughtError = true;
    }
    assert(caughtError);

    // test iterators
    my_vector<int> vec{};
    assert(vec.begin() == vec.end());
    vec.resize(1);
    assert(vec.begin() + 1 > vec.begin());
    vec.resize(3);
    std::iota(vec.begin(), vec.end(), 0);
    assert((vec == my_vector<int>{ 0, 1, 2 }));
    std::reverse(vec.begin(), vec.end());
    assert((vec == my_vector<int>{ 2, 1, 0 }));
    const my_vector<int> constVec = vec;
    std::transform(constVec.cbegin(), constVec.cend(), vec.begin(), [](int i) { return i * 2; });
    assert((vec == my_vector<int>{ 4, 2, 0 }));

    int repr = 0;
    for (int i : vec)
    {
        repr *= 10;
        repr += i;
    }
    assert(repr == 420);

    repr = 0;
    for (auto i = vec.rbegin(); i != vec.rend(); ++i)
    {
        repr *= 10;
        repr += *i;
    }
    assert(repr == 24);

    repr = 0;
    for (auto i = constVec.crbegin(); i != constVec.crend(); ++i)
    {
        repr *= 10;
        repr += *i;
    }
    assert(repr == 12);

    my_vector<int>::iterator it = vec.begin();
    my_vector<int>::const_iterator constIt = it;
    assert(*constIt == 4);

    my_vector<int>::reverse_iterator reverseIt = vec.rbegin();
    my_vector<int>::const_reverse_iterator constReverseIt = reverseIt;
    assert(*constReverseIt == 0);

    assert(vec.begin()[1] == vec[1]);
    assert(vec.end()[-1] == vec[2]);
    assert(vec.rbegin()[1] == vec.end()[-2]);
    assert(vec.rend()[-1] == *vec.begin());

    const my_vector<std::string> strArr{ "aaa", "bbb", "ccc" };
    assert(strArr.cbegin()->size() == 3);
    assert(*(strArr.cbegin() + 2) == "ccc");
    assert(strArr.cbegin() + 2 == strArr.cend() - 1);

    // test with back_inserter
    vec = { 1, 2, 3, 4, 5 };
    firstVec.clear();
    std::copy(vec.cbegin(), vec.cend(), std::back_inserter(firstVec));
    assert((firstVec == my_vector<int>{ 1, 2, 3, 4, 5 }));

    // test push_back reference with possible reallocation
    int lastVecElem = vec.back();
    my_vector<int> checkVec = vec;
    for (std::size_t i = 0; i < 100; ++i)
    {
        checkVec.push_back(lastVecElem);
        vec.push_back(vec.back());
    }
    assert(vec == checkVec);

    // test emplace_back, resize, constructor with multiple objects
    my_vector<Test> testVec(10, { 42, "content" });
    assert(testVec.size() == 10);
    assert(testVec[7].a == 42);
    assert(testVec[4].b == "content");

    testVec.emplace_back(123, "another");
    assert(testVec.back().a == 123);
    assert(testVec.back().b == "another");

    vec.clear();
    vec.resize(5, 337);
    assert((vec == my_vector<int>{ 337, 337, 337, 337, 337 }));
    vec.resize(3, 234);
    assert((vec == my_vector<int>{ 337, 337, 337 }));
    vec.resize(5, 234);
    assert((vec == my_vector<int>{ 337, 337, 337, 234, 234 }));
    vec.resize(7);
    assert((vec == my_vector<int>{ 337, 337, 337, 234, 234, 0, 0 }));
    vec.resize(4);
    assert((vec == my_vector<int>{ 337, 337, 337, 234 }));
    vec.resize(0);
    assert(vec.is_empty());

    // test methods
    vec = { 1, 2, 3, 42 };
    vec.clear();
    assert(vec.size() == 0);
    assert(vec.capacity() == 0);
    assert(vec.is_empty());
    vec.push_back(1);
    assert(vec.size() == 1);
    assert(vec.capacity() == 1);
    assert(!vec.is_empty());
    vec.push_back(2);
    vec.push_back(3);
    assert(vec.size() == 3);
    assert(vec.capacity() == 4);
    vec.shrink_to_fit();
    assert(vec.capacity() == vec.size());

    vec.clear();
    for (int i = 0; i < 129; ++i)
    {
        vec.push_back(i);
    }
    assert(vec.size() == 129);
    assert(vec.capacity() == 256);
    assert(!vec.is_empty());
    for (int i = 0; i < 64; ++i)
    {
        vec.pop_back();
    }
    assert(vec.size() == 65);
    assert(vec.capacity() == 256);
    assert(!vec.is_empty());
    vec.pop_back();
    assert(vec.size() == 64);
    assert(vec.capacity() == 128);
    assert(!vec.is_empty());
    for (int i = 0; i < 31; ++i)
    {
        vec.pop_back();
    }
    assert(vec.size() == 33);
    assert(vec.capacity() == 128);
    assert(!vec.is_empty());
    vec.pop_back();
    assert(vec.size() == 32);
    assert(vec.capacity() == 64);
    assert(!vec.is_empty());
    for (int i = 0; i < 32; ++i)
    {
        vec.pop_back();
    }
    assert(vec.size() == 0);
    assert(vec.capacity() == 0);
    assert(vec.is_empty());

    firstVec = { 1, 2, 3 };
    auto iter = vec.insert(vec.end(), firstVec.begin(), firstVec.end());
    assert(vec == firstVec);
    assert(iter - vec.begin() == 0);
    iter = vec.insert(vec.begin() + 1, firstVec.begin(), firstVec.end());
    assert((vec == my_vector<int>{ 1, 1, 2, 3, 2, 3 }));
    assert(iter - vec.begin() == 1);
    iter = vec.insert(vec.begin(), firstVec.rbegin(), firstVec.rend());
    assert((vec == my_vector<int>{ 3, 2, 1, 1, 1, 2, 3, 2, 3 }));
    assert(iter - vec.begin() == 0);

    iter = vec.insert(vec.begin() + 2, 42);
    assert((vec == my_vector<int>{ 3, 2, 42, 1, 1, 1, 2, 3, 2, 3 }));
    assert(iter - vec.begin() == 2);
    iter = vec.insert(vec.begin(), 42);
    assert((vec == my_vector<int>{ 42, 3, 2, 42, 1, 1, 1, 2, 3, 2, 3 }));
    assert(iter - vec.begin() == 0);
    iter = vec.insert(vec.end(), 42);
    assert((vec == my_vector<int>{ 42, 3, 2, 42, 1, 1, 1, 2, 3, 2, 3, 42 }));
    assert(iter - vec.begin() == vec.size() - 1);

    vec.front() *= 2;
    assert(vec.front() == 84);
    vec.back() /= 2;
    assert(vec.back() == 21);
    const my_vector<int> otherConstVec{ 12, 34 };
    assert(otherConstVec.front() == 12);
    assert(otherConstVec.back() == 34);

    iter = vec.erase(vec.begin() + 3, vec.end() - 2);
    assert((vec == my_vector<int>{ 84, 3, 2, 3, 21 }));
    assert(iter - vec.begin() == vec.size() - 2);

    iter = vec.erase(vec.begin() + 1);
    assert((vec == my_vector<int>{ 84, 2, 3, 21 }));
    assert(iter - vec.begin() == 1);
    iter = vec.erase(vec.begin());
    assert((vec == my_vector<int>{ 2, 3, 21 }));
    assert(iter - vec.begin() == 0);
    iter = vec.erase(vec.end() - 1);
    assert((vec == my_vector<int>{ 2, 3 }));
    assert(iter == vec.end());

    // test with complex types
    my_vector<my_vector<std::string>> twoDimVec{
        { "1", "2" },
        { "3", "4", "5" },
        { "6" }
    };

    std::string concatenatedString = std::accumulate(twoDimVec[1].begin(), twoDimVec[1].end(), std::string(""));
    assert(concatenatedString == "345");
    twoDimVec[0][1] = "string";
    auto strVecIter = twoDimVec.erase(twoDimVec.begin() + 1, twoDimVec.end());
    assert(strVecIter == twoDimVec.end());
    concatenatedString = std::accumulate(twoDimVec[0].begin(), twoDimVec[0].end(), std::string(""));
    assert(concatenatedString == "1string");
}

#endif
