#ifndef __POINT_H__
#define __POINT_H__

#include "Globals.h"
#include <math.h>

template<class TYPE>
class Point
{
public:

	TYPE x, y;

	Point() : x(0), y(0)
	{}

	Point(const Point& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	Point(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;
	}

	Point& create(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;

		return(*this);
	}

	// Math ------------------------------------------------
	Point operator -(const Point& v) const
	{
		Point r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	Point operator + (const Point& v) const
	{
		Point r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	Point operator /(const Point& v) const
	{
		Point r;

		r.x = x / v.x;
		r.y = y / v.y;

		return(r);
	}

	Point operator /(const float v) const
	{
		Point r;

		r.x = x / v;
		r.y = y / v;

		return(r);
	}

	Point operator *(const float num)
	{
		Point result;
		result.x = this->x * num;
		result.y = this->y * num;

		return(result);
	}

	Point operator *(const Point& p)
	{
		Point result;
		result.x = this->x * p.x;
		result.y = this->y * p.y;

		return(result);
	}

	Point Normalize()
	{

		//DEBUG
		TYPE m = Module();

		Point n;

		if (m == 0)
		{
			n = { 0,0 };
		}
		else
		{
			n = { x / m, y / m };
		}

		return n;
	}

	const Point& operator -=(const Point& v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const Point& operator +=(const Point& v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	const Point& operator +=(int num)
	{
		x += num;
		y += num;

		return(*this);
	}

	const Point& operator *=(const int num)
	{
		x *= num;
		y *= num;

			return(*this);
	}

	const Point& operator *=(const float num)
	{
		x *= num;
		y *= num;

			return(*this);
	}

	const Point& operator *=(const Point& num)
	{
		x *= num.x;
		y *= num.y;

		return(*this);
	}

	bool operator ==(const Point& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const Point& v) const
	{
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	Point& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	Point& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	TYPE Module()
	{
		TYPE m;

		m = sqrt(x * x + y * y);

		return m;
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (TYPE)sqrtf(float(fx * fx) + float(fy * fy));
	}

	TYPE DistanceNoSqrt(const Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx * fx) + (fy * fy);
	}

	TYPE DistanceManhattan(const Point& v) const
	{
		return abs(v.x - x) + abs(v.y - y);
	}

	TYPE DistanceManhattan(const Point& v, const Point& v2) const
	{
		return abs(v.x - v2.x) + abs(v.y - v2.y);
	}
};

typedef Point<int> iPoint;
typedef Point<float> fPoint;

#endif // __POINT_H__
