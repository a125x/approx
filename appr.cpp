#include "appr.h"

int find_coeff(int n, vector<double> x, vector<double> f, vector<double> fder, vector<vector<double>>& a)
{
    //Checking sizes
    if (n != f.size() || n != fder.size()) {
		cout << "f size " << f.size() << " der size " << fder.size() << endl;
        return -1; 
    }

	for (int i = 1; i < n; ++i)
	{
		vector<double> abc = {1.0, 2.0, 3.0, 4.0};
		abc[0] = (fder[i] - fder[i-1]) / (6*(x[i] - x[i-1]));
		abc[1] = (fder[i]*x[i-1] - fder[i-1]*x[i]) / (2*(x[i] - x[i-1]));
		double alpha = 0.0, beta = 0.0;
		alpha = f[i-1] - abc[0]*pow(x[i-1], 3) - abc[1]*pow(x[i-1], 2);
		beta = f[i] - abc[0]*pow(x[i], 3) - abc[1]*pow(x[i], 2);
		abc[2] = (beta - alpha) / (x[i] - x[i-1]); 
		abc[3] = (alpha * x[i] - beta * x[i-1]) / (x[i] - x[i-1]);
		a.push_back(abc);
	}

	return 0;
}

double calc_approx(double val, int n, vector<double> x, vector<vector<double>> coeff, double a, double b)
{
    //int i = int((val-a) / ((b-a)/(n-1)));
    for (int i = 1; i < n; ++i)
	{
		if (val > x[i-1] and val < x[i])
			return pow(val, 3) * coeff[i-1][0] + pow(val, 2) * coeff[i-1][1] + val * coeff[i-1][2] + coeff[i-1][3];
	}

	return -1;
}
