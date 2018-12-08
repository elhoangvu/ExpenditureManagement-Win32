#pragma once
#ifndef __POINT_H__
#define __POINT_H__

class CPoint
{
public:
	int x;
	int y;
public:
	CPoint(int x, int y);
	CPoint();
	~CPoint();
};
#endif //__POINT_H__

#ifndef __SIZE_H__
#define __SIZE_H__

class CSize
{
public:
	int width;
	int height;
public:
	CSize(int width, int height);
	CSize();
	~CSize();
};
#endif //__SIZE_H__
