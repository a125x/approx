#pragma once
#include <vector>
#include <iostream>

using namespace std;

int find_coeff(int n, vector<double> x, vector<double> f, vector<double> fd, vector<double> fdd, vector<vector<double>>& a, vector<vector<double>>& b);

double calc_approx(double val, int n, vector<double> x, vector<vector<double>> coeff, double a, double b, bool is_arima);
