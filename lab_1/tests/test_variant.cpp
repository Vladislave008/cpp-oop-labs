#include <gtest/gtest.h>
#include "array_ops.h"

TEST(ArrayOps, CreateAndDelete)
{
    int *a = array_create(5);
    ASSERT_NE(a, nullptr);
    for (std::size_t i = 0; i < 5; ++i)
        EXPECT_EQ(a[i], 0);
    array_delete(a);
    EXPECT_EQ(a, nullptr);
}

TEST(ArrayOps, CreateZero)
{
    int *a = array_create(0);
    EXPECT_EQ(a, nullptr);
    array_delete(a);
    EXPECT_EQ(a, nullptr);
}

TEST(ArrayOps, InsertMiddle)
{
    std::size_t size = 3;
    int *a = array_create(3);
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a = array_insert(a, size, 1, 99);
    ASSERT_EQ(size, 4u);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 99);
    EXPECT_EQ(a[2], 2);
    EXPECT_EQ(a[3], 3);
    array_delete(a);
}

TEST(ArrayOps, InsertEnd)
{
    std::size_t size = 2;
    int *a = array_create(2);
    a[0] = 7;
    a[1] = 8;
    a = array_insert(a, size, 2, 9);
    ASSERT_EQ(size, 3u);
    EXPECT_EQ(a[2], 9);
    array_delete(a);
}

TEST(ArrayOps, RemoveMiddle)
{
    std::size_t size = 4;
    int *a = array_create(4);
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a[3] = 4;
    a = array_remove(a, size, 1);
    ASSERT_EQ(size, 3u);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 3);
    EXPECT_EQ(a[2], 4);
    array_delete(a);
}

TEST(ArrayOps, RemoveLast)
{
    std::size_t size = 3;
    int *a = array_create(3);
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a = array_remove(a, size, 2);
    ASSERT_EQ(size, 2u);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    array_delete(a);
}

TEST(ArrayOps, InsertFront)
{
    std::size_t size = 2;
    int *a = array_create(2);
    a[0] = 1;
    a[1] = 2;
    a = array_insert(a, size, 0, 99);
    ASSERT_EQ(size, 3u);
    EXPECT_EQ(a[0], 99);
    EXPECT_EQ(a[1], 1);
    EXPECT_EQ(a[2], 2);
    array_delete(a);
}

TEST(ArrayOps, InsertBeyondEndAppends)
{
    std::size_t size = 2;
    int *a = array_create(2);
    a[0] = 1;
    a[1] = 2;
    a = array_insert(a, size, 100, 9);
    ASSERT_EQ(size, 3u);
    EXPECT_EQ(a[2], 9);
    array_delete(a);
}

TEST(ArrayOps, InsertIntoEmpty)
{
    std::size_t size = 0;
    int *a = array_create(0);
    a = array_insert(a, size, 0, 42);
    ASSERT_EQ(size, 1u);
    ASSERT_NE(a, nullptr);
    EXPECT_EQ(a[0], 42);
    array_delete(a);
}

TEST(ArrayOps, RemoveOutOfRangeKeepsArray)
{
    std::size_t size = 3;
    int *a = array_create(3);
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    int *same = array_remove(a, size, 7);
    EXPECT_EQ(same, a);
    EXPECT_EQ(size, 3u);
    array_delete(a);
}

TEST(ArrayOps, ResizeGrowZeroFills)
{
    int *a = array_create(2);
    a[0] = 5;
    a[1] = 6;
    a = array_resize(a, 2, 5);
    ASSERT_NE(a, nullptr);
    EXPECT_EQ(a[0], 5);
    EXPECT_EQ(a[1], 6);
    EXPECT_EQ(a[2], 0);
    EXPECT_EQ(a[3], 0);
    EXPECT_EQ(a[4], 0);
    array_delete(a);
}

TEST(ArrayOps, ResizeShrinkTruncates)
{
    int *a = array_create(4);
    for (std::size_t i = 0; i < 4; ++i)
        a[i] = static_cast<int>(i) + 1;
    a = array_resize(a, 4, 2);
    ASSERT_NE(a, nullptr);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    array_delete(a);
}

TEST(ArrayOps, ResizeToZeroReturnsNull)
{
    int *a = array_create(3);
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a = array_resize(a, 3, 0);
    EXPECT_EQ(a, nullptr);
}

TEST(ArrayOps, ResizeFromNull)
{
    int *a = array_resize(nullptr, 0, 3);
    ASSERT_NE(a, nullptr);
    EXPECT_EQ(a[0], 0);
    EXPECT_EQ(a[1], 0);
    EXPECT_EQ(a[2], 0);
    array_delete(a);
}

TEST(Variant2, SelectionSort)
{
    int a[] = {3, 1, 4, 1, 5};
    array_selection_sort(a, 5);
    int expected[] = {1, 1, 3, 4, 5};
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(a[i], expected[i]);
}

TEST(Variant2, SortEdgeCases)
{
    array_selection_sort(nullptr, 0);
    int one[] = {42};
    array_selection_sort(one, 1);
    EXPECT_EQ(one[0], 42);
}

TEST(Variant2, Reverse)
{
    int a[] = {1, 2, 3, 4, 5};
    array_reverse(a, 5);
    int expected[] = {5, 4, 3, 2, 1};
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(a[i], expected[i]);
}

TEST(Variant2, ReverseEvenAndOne)
{
    int a[] = {1, 2, 3, 4};
    array_reverse(a, 4);
    EXPECT_EQ(a[0], 4);
    EXPECT_EQ(a[1], 3);
    EXPECT_EQ(a[2], 2);
    EXPECT_EQ(a[3], 1);

    int one[] = {7};
    array_reverse(one, 1);
    EXPECT_EQ(one[0], 7);
}

TEST(Variant2, FullAlgorithm)
{
    int a[] = {3, 1, 4, 1, 5};
    array_selection_sort(a, 5);
    array_reverse(a, 5);
    int expected[] = {5, 4, 3, 1, 1};
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(a[i], expected[i]);
}

TEST(Variant2, EmptyAlgorithm)
{
    array_selection_sort(nullptr, 0);
    array_reverse(nullptr, 0);
    SUCCEED();
}

TEST(BinarySearch, FoundMiddle)
{
    int a[] = {1, 3, 5, 7, 9};
    std::size_t idx = 0;
    ASSERT_TRUE(array_binary_search(a, 5, 5, idx));
    EXPECT_EQ(idx, 2u);
}

TEST(BinarySearch, FoundLast)
{
    int a[] = {1, 3, 5, 7, 9};
    std::size_t idx = 0;
    ASSERT_TRUE(array_binary_search(a, 5, 9, idx));
    EXPECT_EQ(idx, 4u);
}

TEST(BinarySearch, DuplicatesFindAnOccurrence)
{
    int a[] = {1, 1, 3, 4, 5};
    std::size_t idx = 0;
    ASSERT_TRUE(array_binary_search(a, 5, 1, idx));
    EXPECT_EQ(a[idx], 1);
}

TEST(BinarySearch, NotFoundBelowAndAbove)
{
    int a[] = {1, 3, 5, 7, 9};
    std::size_t idx = 0;
    EXPECT_FALSE(array_binary_search(a, 5, 0, idx));
    EXPECT_FALSE(array_binary_search(a, 5, 10, idx));
}

TEST(BinarySearch, NotFoundBetween)
{
    int a[] = {1, 3, 5, 7, 9};
    std::size_t idx = 0;
    EXPECT_FALSE(array_binary_search(a, 5, 4, idx));
}

TEST(BinarySearch, EmptyArray)
{
    std::size_t idx = 0;
    EXPECT_FALSE(array_binary_search(nullptr, 0, 42, idx));
}