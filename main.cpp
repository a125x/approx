#include "main.h"
#define delta 0.00000001

double f(double x, int k)
{
	switch (k)
	{
		case 0:
			return 1.0;
		case 1:
			return x;
		case 2:
			return pow(x, 2);
		case 3:
			return pow(x, 3);
		case 4:
			return pow(x, 4);
		case 5:
			return exp(x);
		case 6:
			return 1.0 / (25.0 * pow(x, 2) + 1.0);
	}
}

double fd(double x, int k)
{
	return 0.5 * (f(x+delta, k) - f(x-delta, k)) / delta;
}

double fdd(double x, int k)
{
	return 0.5 * (fd(x+delta, k) - fd(x-delta, k)) / delta;
    /*switch (k)
	{
		case 0:
			return 0;
		case 1:
			return 0;
		case 2:
			return 2;
		case 3:
			return 6*x;
		case 4:
			return 12*pow(x, 2);
		case 5:
			return exp(x);
		case 6:
			return (5000*pow(x, 2)/pow(25*pow(x, 2)+1, 3)) - (50/pow(25*pow(x, 2)+1, 2));
	}*/
}

int main(int argc, char *argv[])
{
	if (argc < 5) {
        cerr << "Usage: " << argv[0] << " <a> <b> <n> <k>\n";
        return 1;
    }

    double a = strtod(argv[1], nullptr);
    double b = strtod(argv[2], nullptr);
    int n = atoi(argv[3]);
    int k = atoi(argv[4]);

    if (k < 0 || k > 6) {
        cerr << "Error: k must be between 0 and 6.\n";
        return 1;
    }

    // Check if the conversions were successful
    if (a == 0 && *argv[1] != '0') {
        cerr << "Error: Invalid value for a.\n";
        return 1;
    }
    if (b == 0 && *argv[2] != '0' || b < a) {
        cerr << "Error: Invalid value for b.\n";
        return 1;
    }
    if (n == 0 && *argv[3] != '0') {
        cerr << "Error: Invalid value for n.\n";
        return 1;
    }

	vector<double> x = {}, fval = {}, fder = {};
	vector<vector<double>> coeff = {};
	double curval = a;

	for (int i = 0; i < n; ++i)
	{
		x.push_back(curval);
		fval.push_back(f(curval, k));
		fder.push_back(fdd(curval, k));
		curval += (b-a) / double(n-1);
	}

	if (find_coeff(n, x, fval, fder, coeff) != 0)
		cerr << "Error: Something's wrong with finding coefficients";

    QApplication app(argc, argv);

    PlotWidget plotWidget;
    plotWidget.setFunction(&f, &fdd, k, a, b); 
    plotWidget.setApprox(&calc_approx, n, x, coeff, a, b); 

    plotWidget.setWindowTitle("Function Plot");
    plotWidget.resize(800, 600);
    plotWidget.show();

    return app.exec();
}
