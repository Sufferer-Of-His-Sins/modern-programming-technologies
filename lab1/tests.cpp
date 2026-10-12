#include <gtest/gtest.h>
#include "order_calc.h"
#include <vector>

// applyDiscount
TEST(ApplyDiscountTest, NegativeTotal) {
    EXPECT_DOUBLE_EQ(applyDiscount(-5.5, false), -1.0);
}
TEST(ApplyDiscountTest, PremiumHigh) {
    EXPECT_DOUBLE_EQ(applyDiscount(12000, true), 9600);
}
TEST(ApplyDiscountTest, NotPremiumHigh) {
    EXPECT_DOUBLE_EQ(applyDiscount(12000, false), 10200);
}
TEST(ApplyDiscountTest, PremiumMedium) {
    EXPECT_DOUBLE_EQ(applyDiscount(7600, true), 6840);
}
TEST(ApplyDiscountTest, NotPremiumMedium) {
    EXPECT_DOUBLE_EQ(applyDiscount(7600, false), 7220);
}
TEST(ApplyDiscountTest, NoDiscount) {
    EXPECT_DOUBLE_EQ(applyDiscount(3000, false), 3000);
}

// calcShipping
TEST(CalcShippingTest, NegativeTotal) {
    EXPECT_DOUBLE_EQ(calcShipping(-5.5), -1.0);
}
TEST(CalcShippingTest, FreeShipping) {
    EXPECT_DOUBLE_EQ(calcShipping(4600), 0);
}
TEST(CalcShippingTest, PaidShipping) {
    EXPECT_DOUBLE_EQ(calcShipping(5001), 300);
}

// finalPrice
TEST(FinalPriceTest, NegativeTotal) {
    EXPECT_DOUBLE_EQ(finalPrice(-5.5, -5.0, -5.0), -1.0);
}
TEST(FinalPriceTest, DiscountGreaterThanTotal) {
    EXPECT_DOUBLE_EQ(finalPrice(100.0, 200.0, 0.0), 0.0);
}
TEST(FinalPriceTest, NormalCalculation) {
    EXPECT_DOUBLE_EQ(finalPrice(1000.0, 300.0, 40.0), 740.0);
}

// countExpensiveItems
TEST(CountExpensiveItemsTest, Empty) {
    vector<double> prices = {};
    EXPECT_EQ(countExpensiveItems(prices, 10.0), 0);
}
TEST(CountExpensiveItemsTest, OneExpensive) {
    vector<double> prices = {16.0};
    EXPECT_EQ(countExpensiveItems(prices, 10.0), 1);
}
TEST(CountExpensiveItemsTest, OneCheap) {
    vector<double> prices = {6.0};
    EXPECT_EQ(countExpensiveItems(prices, 10.0), 0);
}

// productByIndices
TEST(ProductByIndicesTest, EmptyIndices) {
    vector<double> value = {10.0, 20.0};
    vector<int> indices;
    EXPECT_DOUBLE_EQ(productByIndices(value, indices), 1.0);
}
TEST(ProductByIndicesTest, SingleIndex) {
    vector<double> value = {5.0};
    vector<int> indices = {0};
    EXPECT_DOUBLE_EQ(productByIndices(value, indices), 5.0);
}
TEST(ProductByIndicesTest, OutOfRangeIndex) {
    vector<double> value = {1.0, 2.0};
    vector<int> indices = {5};
    EXPECT_DOUBLE_EQ(productByIndices(value, indices), 1.0);
}

// sumEvenAboveSecondaryDiagonal
TEST(SumEvenAboveSecondaryDiagonalTest, Empty) {
    vector<vector<int>> A;
    EXPECT_EQ(sumEvenAboveSecondaryDiagonal(A), 0);
}
TEST(SumEvenAboveSecondaryDiagonalTest, OneElement) {
    vector<vector<int>> A = {{4}};
    EXPECT_EQ(sumEvenAboveSecondaryDiagonal(A), 0);
}
TEST(SumEvenAboveSecondaryDiagonalTest, TwoByTwo) {
    vector<vector<int>> A = {{2,4},{1,3}};
    EXPECT_EQ(sumEvenAboveSecondaryDiagonal(A), 2);
}
TEST(SumEvenAboveSecondaryDiagonalTest, ThreeByThree) {
    vector<vector<int>> A = {{1,2,3},{4,5,6},{7,8,9}};
    EXPECT_EQ(sumEvenAboveSecondaryDiagonal(A), 6);
}