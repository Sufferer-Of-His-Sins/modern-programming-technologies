#pragma once
#include <string>
#include <vector>
#include <cmath>

using namespace std;

double applyDiscount(double total, bool isPremium);
double calcShipping(double total);
double finalPrice(double total, double discount, double shipping);
int countExpensiveItems(const std::vector<double>& prices, double threshold); 

//___________добавил__________
double productByIndices(const std::vector<double>& values, const std::vector<int>& indices);
int sumEvenAboveSecondaryDiagonal(const std::vector<std::vector<int>>& A);
//____________________________