#pragma once
#include "main.h"
#include <QApplication>
#include <QtWidgets>
#include <QPainter>
#include <QRect>
#include <QColor>
#include <QFont>
#include <cmath>

class PlotWidget : public QWidget
{
public:
    PlotWidget(QWidget *parent = nullptr) : QWidget(parent) {}

    void setFunction(double (*func)(double, int), double (*fdd)(double, int), int k, double a, double b)
    {
        this->function = func;
		this->fdd = fdd;
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
	void recalc()
	{
		vector<double> x = {}, fval = {}, fder = {};
		vector<vector<double>> coeff = {};
		double curval = this->lowerBound;

		for (int i = 0; i < this->n; ++i)
		{
			if (i == n/2)
				fval.push_back(this->function(curval, this->mode)-this->p*this->maxValf/10.0);
			else
				fval.push_back(this->function(curval, this->mode));
			x.push_back(curval);
			fder.push_back(fdd(curval, this->mode));
			curval += (this->upperBound-this->lowerBound) / double(n-1);
		}

		if (find_coeff(this->n, x, fval, fder, coeff) != 0)
			cerr << "Error: Something's wrong with finding coefficients";
			
		this->dots = x;
		this->coeff = coeff;
	}

	void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_0) {
            this->mode += 1;
			this->mode %= 7;
			this->recalc();
			update();
		} else if (event->key() == Qt::Key_2) {
            this->scale *= 2.0; // Increase scale
			this->lowerBound /= 2.0;
			this->upperBound /= 2.0;
			this->recalc();
            update();
		} else if (event->key() == Qt::Key_3) {
            this->scale /= 2.0; // Decrease scale
			this->lowerBound *= 2.0;
			this->upperBound *= 2.0;
			this->recalc();
            update();
		} else if (event->key() == Qt::Key_4) {
			this->n *= 2;
			this->recalc();
			update();
		} else if (event->key() == Qt::Key_5) {
			this->n = this->n / 2 > 1 ? this->n / 2 : 1;
			this->recalc();
			update();
		} else if (event->key() == Qt::Key_6) {
			this->p += 1;
			this->recalc();
			update();
		} else if (event->key() == Qt::Key_7) {
			this->p -= 1;
			this->recalc();
			update();
		}

    }
    
	void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.fillRect(rect(), Qt::black);

        // Find the maximum value of the function in the given range
        this->maxValf = this->function(this->lowerBound, this->mode);
		this->minValf = this->function(this->lowerBound, this->mode);
		double curval = this->lowerBound;
        for (int i = 0; i < this->n; i++)
		{
			if (this->function(curval, this->mode) < minValf)
				minValf = this->function(curval, this->mode);
			
			if (this->function(curval, this->mode) > maxValf)
				maxValf = this->function(curval, this->mode);

			curval += (this->upperBound - this->lowerBound) / double(n-1);
		}
        
		// Scale the plot to fit within the window
        double xFactor = width() / (this->upperBound - this->lowerBound);
        double yFactor = height() / (maxValf - minValf);

		// Draw the coordinate axes
        painter.drawLine(0, height() / 2, width(), height() / 2);
        painter.drawLine(width() / 2, 0, width() / 2, height());
    	
		// Draw the function graphs
        painter.setPen(Qt::green);
		QPainterPath pathf, patha;
        for (int x = 0; x < width(); ++x) {
            double y = function((x - width() / 2) / xFactor, this->mode);
        	if (x == 0)
                pathf.moveTo(x, height() / 2 - y * yFactor);
            else 
                pathf.lineTo(x, height() / 2 - y * yFactor);
		}
		painter.drawPath(pathf);
		
		painter.setPen(Qt::blue);
        for (int x = 0; x < width(); ++x) {
            double y = approx((x - width() / 2) / xFactor, n, dots, coeff, lowerBound, upperBound);
        	if (x == 0)
                patha.moveTo(x, height() / 2 - y * yFactor);
            else 
                patha.lineTo(x, height() / 2 - y * yFactor);
		}
		painter.drawPath(patha);

		// Display information about the scale, interval [a, b], f min, and f max
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 14));
        painter.drawText(20, 20, QString("Scale: %1").arg(this->scale));
        painter.drawText(20, 40, QString("[a, b]: [%1, %2]").arg(this->lowerBound).arg(this->upperBound));
        painter.drawText(20, 60, QString("f min: %1").arg(minValf));
        painter.drawText(20, 80, QString("f max: %1").arg(maxValf));
        painter.drawText(20, 100, QString("k: %1").arg(mode));
        painter.drawText(20, 120, QString("n: %1").arg(n));
        painter.drawText(20, 140, QString("p: %1").arg(p));
	}

private:
    double (*function)(double, int) = nullptr;
    double (*fdd)(double, int) = nullptr;
    double lowerBound = 0.0;
    double upperBound = 1.0;
    double scale = 1.0;
	int mode = 0;
	int n = 1;
	int p = 0;
	double (*approx)(double, int, vector<double>, vector<vector<double>>, double, double) = nullptr;
	vector<double> dots = {};
	vector<vector<double>> coeff = {};
	double maxValf = 0;
	double minValf = 0;
};
