#include "appr.h"

int find_coeff(int n, vector<double> x, vector<double> f, vector<double> fd, vector<double> fder, vector<vector<double>>& a, vector<vector<double>>& b)
{
    //Checking sizes
    if (n != f.size() || n != fder.size()) {
		cout << "f size " << f.size() << " der size " << fder.size() << endl;
        return -1; 
    }

	for (int i = 1; i < n; ++i)
	{
		vector<double> abc = {0.0, 0.0, 0.0, -1.0};
		abc[0] = (fder[i] - fder[i-1]) / (6*(x[i] - x[i-1]));
		abc[1] = (fder[i]*x[i-1] - fder[i-1]*x[i]) / (2*(x[i] - x[i-1]));
		double alpha = 0.0, beta = 0.0;
		alpha = f[i-1] - abc[0]*pow(x[i-1], 3) - abc[1]*pow(x[i-1], 2);
		beta = f[i] - abc[0]*pow(x[i], 3) - abc[1]*pow(x[i], 2);
		abc[2] = (beta - alpha) / (x[i] - x[i-1]); 
		abc[3] = (alpha * x[i] - beta * x[i-1]) / (x[i] - x[i-1]);
		a.push_back(abc);
	}

	vector<double> temp = {0.0, 0.0, 0.0, -1.0};
	b.push_back(temp);
	b.push_back(temp);

	for (int i = 2; i < n-3; ++i)
	{
		//works from uaaaaa
		vector<double> abc = {0.0, 0.0, 0.0, -1.0};
		double a = (f[i+1] - f[i]) / (x[i+1] - x[i]);
		double a_p = (f[i] - f[i-1]) / (x[i] - x[i-1]);
		double a_pp = (f[i-1] - f[i-2]) / (x[i-1] - x[i-2]);
		double a_n = (f[i+2] - f[i+1]) / (x[i+2] - x[i+1]);
		double a_nn = (f[i+3] - f[i+2]) / (x[i+3] - x[i+2]);
		double w = abs(a - a_p);
		double w_n = abs(a_n - a);
		double w_nn = abs(a_nn - a_n);
		double w_p = abs(a_p - a_pp);
		double d = 0.0, d_n = 0.0;

		if (w_n == 0 and w_p == 0)
		{
			d = ((x[i+1] - x[i]) * a_p + (x[i] - x[i-1])) * a / (x[i+1] - x[i-1]);
			d_n = ((x[i+2] - x[i+1]) * a + (x[i+1] - x[i])) * a_n / (x[i+2] - x[i]);
		}
		else
		{
			d = (w_n * a_p + w_p * a) / (w_n - w_p);
			d_n = (w_nn * a + w + a_n) / (w_nn - w);
		}

		abc[3] = f[i]; 
		abc[2] = d;
		abc[1] = (3*a - 2*d - d_n) / (x[i+1] - x[i]); 
		abc[0] = (d + d_n - 2*a) / pow(x[i+1] - x[i], 2);

		b.push_back(abc);
	}
	
	b.push_back(temp);
	b.push_back(temp);

	return 0;
}

double calc_approx(double val, int n, vector<double> x, vector<vector<double>> coeff, double a, double b, bool is_arima)
{
    int i = int((val-a) / ((b-a)/double(n-1)));
	
	if (i > 0 and i < n and val > x[i] and val <= x[i+1])
		if (is_arima)
			return pow(val-x[i], 3) * coeff[i][0] + pow(val-x[i], 2) * coeff[i][1] + (val-x[i]) * coeff[i][2] + coeff[i][3];
		else
			return pow(val, 3) * coeff[i][0] + pow(val, 2) * coeff[i][1] + val * coeff[i][2] + coeff[i][3];
	
	return -1;
}
