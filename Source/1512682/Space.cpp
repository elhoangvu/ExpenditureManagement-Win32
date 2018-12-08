#include "stdafx.h"
#include "Space.h"

CPoint::CPoint(int x, int y)
{
	this->x = x;
	this->y = y;
}

CPoint::CPoint() :x(0), y(0)
{
}

CPoint::~CPoint()
{
}

CSize::CSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

CSize::CSize() :width(0), height(0)
{
}

CSize::~CSize()
{
}
