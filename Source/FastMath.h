/*
  ==============================================================================

    FastMath.h
    Created: 16 Jul 2019 8:48:20pm
    Author:  matth

  ==============================================================================
*/

#pragma once
namespace FastMath
{
	typedef int             Int32;  // Must be 32 bits
	typedef __int64 Int64;  // Must be 64 bits


	const double Pi = 3.1415926535897932; //PI
	const double Pi_2 = 1.5707963267948966; //PI/2
	const double PiSqr = 9.8696044010893571; //Pi^2
	const double Ln2 = 0.6931471805599453; //LN(2)
	const double Ln10 = 2.3025850929940456; //LN(10)
	const double Sqrt2 = 1.4142135623730949; //SQRT(2)

	//Trig approximations
	inline void fastersincos(double x, double* sn, double* cs)
	{
		//always wrap input angle to -PI..PI
		if (x < -Pi) x += 2 * Pi;
		else if (x > Pi) x -= 2 * Pi;
		//compute sine
		if (x < 0)* sn = 1.27323954 * x + 0.405284735 * x * x;
		else            *sn = 1.27323954 * x - 0.405284735 * x * x;
		//compute cosine: sin(x + PI/2) = cos(x)
		x += Pi_2;
		if (x > Pi) x -= 2 * Pi;
		if (x < 0)* cs = 1.27323954 * x + 0.405284735 * x * x;
		else            *cs = 1.27323954 * x - 0.405284735 * x * x;
	}

	inline void fastsincos(double x, double* sn, double* cs)
	{
		double s, c;
		//always wrap input angle to -PI..PI
		if (x < -Pi) x += 2 * Pi;
		else if (x > Pi) x -= 2 * Pi;
		//compute sine
		if (x < 0)
		{
			s = 1.27323954 * x + .405284735 * x * x;
			if (s < 0)      s = .225 * (s * -s - s) + s;
			else            s = .225 * (s * s - s) + s;
		}
		else
		{
			s = 1.27323954 * x - 0.405284735 * x * x;
			if (s < 0)      s = .225 * (s * -s - s) + s;
			else            s = .225 * (s * s - s) + s;
		}
		*sn = s;
		//compute cosine: sin(x + PI/2) = cos(x)
		x += Pi_2;
		if (x > Pi) x -= 2 * Pi;
		if (x < 0)
		{
			c = 1.27323954 * x + 0.405284735 * x * x;
			if (c < 0)      c = .225 * (c * -c - c) + c;
			else            c = .225 * (c * c - c) + c;
		}
		else
		{
			c = 1.27323954 * x - 0.405284735 * x * x;
			if (c < 0)      c = .225 * (c * -c - c) + c;
			else            c = .225 * (c * c - c) + c;
		}
		*cs = c;
	}

	//Square root approximations
	inline float fastsqrt1(float x)
	{
		union { Int32 i; float x; } u;
		u.x = x;
		u.i = (Int32(1) << 29) + (u.i >> 1) - (Int32(1) << 22);
		return u.x;
	}

	inline double fastsqrt1(double x)
	{
		union { Int64 i; double x; } u;
		u.x = x;
		u.i = (Int64(1) << 61) + (u.i >> 1) - (Int64(1) << 51);
		return u.x;
	}

	// Log Base 2 Approximation with one extra Babylonian Step
	// 2 times faster than sqrt()

	inline float fastsqrt2(float x)
	{
		float v = fastsqrt1(x);
		v = 0.5f * (v + x / v); // One Babylonian step
		return v;
	}

	inline double fastsqrt2(const double x)
	{
		double v = fastsqrt1(x);
		v = 0.5f * (v + x / v); // One Babylonian step
		return v;
	}

	// Log Base 2 Approximation with two extra Babylonian Steps
	// 50% faster than sqrt()

	inline float fastsqrt3(float x)
	{
		float v = fastsqrt1(x);
		v = v + x / v;
		v = 0.25f * v + x / v; // Two Babylonian steps
		return v;
	}

	inline double fastsqrt3(const double x)
	{
		double v = fastsqrt1(x);
		v = v + x / v;
		v = 0.25 * v + x / v; // Two Babylonian steps
		return v;
	}


}
