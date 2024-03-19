#pragma once
#include <QtWidgets>
#include <cmath>

class PlotWidget : public QWidget
{
public:
    PlotWidget(QWidget *parent = nullptr) : QWidget(parent) {}

    void setFunction(double (*func)(double, int), int k, double a, double b)
    {
        this->function = func;
		this->mode = k;
        this->lowerBound = a;
        this->upperBound = b;
    }

	void setApprox(double (*func)(double, int, vector<double>, vector<vector<double>>, double, double), int n, vector<double> x, vector<vector<double>> coeff, double a, double b)
	{
		this->approx = func;
		this->n = n;
		this->dots = x;
		this->coeff = coeff;
		this->lowerBound = a;
        this->upperBound = b;
	}

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);

        // Find the maximum value of the function in the given range
        double maxAbsValue = 0.0;
        for (double x = lowerBound; x <= upperBound; x += 0.1)
        {
            double fx = function(x, mode);
            if (std::abs(fx) > maxAbsValue)
            {
                maxAbsValue = std::abs(fx);
            }
        }

        // Scale the plot to fit within the window
        double scale = height() / (2.0 * maxAbsValue);
        double xOffset = width() * 0.1;
        double yOffset = height() / 2.0;
/*
        // Draw the function
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::blue);
        QPainterPath path;
        for (double x = lowerBound; x <= upperBound; x += 0.01)
        {
            double y = function(x, mode);
            QPointF point(xOffset + scale * x, yOffset - scale * y);
            if (x == lowerBound)
            {
                path.moveTo(point);
            }
            else
            {
                path.lineTo(point);
            }
        }
        painter.drawPath(path);
		*/
		// Draw the function
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::blue);
        QPainterPath path;
        for (double x = lowerBound; x <= upperBound; x += 0.01)
        {
            double y = approx(x, n, dots, coeff, lowerBound, upperBound);
            QPointF point(xOffset + scale * x, yOffset - scale * y);
            if (x == lowerBound)
            {
                path.moveTo(point);
            }
            else
            {
                path.lineTo(point);
            }
        }
        painter.drawPath(path);
        // Output the max absolute value to the console
        qDebug() << "Maximum absolute value of the function: " << maxAbsValue;
    }

private:
    double (*function)(double, int) = nullptr;
    double lowerBound = 0.0;
    double upperBound = 1.0;
	int mode = 0;
	int n = 0;
	double (*approx)(double, int, vector<double>, vector<vector<double>>, double, double) = nullptr;
	vector<double> dots = {};
	vector<vector<double>> coeff = {};
};
