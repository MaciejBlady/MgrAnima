#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <random>
#include "spline.h"

class InterpolationPoints
{
public:
	InterpolationPoints();
	~InterpolationPoints();
	std::vector<double> points;
	void SetBounds(float min, float max);
	void GeneratePoints(int count, FbxAnimCurve* curve);
	double GetSplineValue(double t);

private:
	void GenerateSpline();

	//float min = -100;
	//float max = 100;

	tk::spline spline;

	std::random_device rd;     // only used once to initialise (seed) engine
};

InterpolationPoints::InterpolationPoints()
{

}

InterpolationPoints::~InterpolationPoints()
{
}

void InterpolationPoints::SetBounds(float min, float max)
{
	//this->min = min;
	//this->max = max;
}

void InterpolationPoints::GeneratePoints(int count, FbxAnimCurve* curve)
{
	double max = -10000;
	double min = 10000;
	double mean = 0;

	int keyCount = curve->KeyGetCount();

	for (size_t i = 0; i < keyCount; i++)
	{
		double value = curve->EvaluateIndex((double)i);
		mean += value;
		if (value > max)
		{
			max = value;
		}
	    if (value < min)
		{
			min = value;
		}
	}
	mean /= curve->KeyGetCount();
	max = max - mean;
	min = mean - min;

	//mean *= 0.25;
	points.clear();

	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_real_distribution<> uni(-std::abs(mean), std::abs(mean)); // guaranteed unbiased

	points.push_back(0.0);
	for (size_t i = 0; i < count; i++)
	{		
		points.push_back(uni(rng));
	}	
	points.push_back(0.0);

	GenerateSpline();
}

void InterpolationPoints::GenerateSpline()
{
	float timestep = 1.0f / (float)(points.size() - 1);
	std::vector<double> time;

	for (size_t i = 0; i < points.size()-1; i++)
	{
		time.push_back(timestep * (float)i);
	}
	time.push_back(1.0f);
	tk::spline s;
	s.set_points(time, points);
	spline = s;
}

double InterpolationPoints::GetSplineValue(double t)
{
	return spline(t);
}