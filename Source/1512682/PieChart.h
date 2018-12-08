#pragma once
#ifndef __PIE_CHART__
#define __PIE_CHART__

#include "Item.h"
#include <vector>
//#include "Point.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#include <Windows.h>
#pragma comment(lib, "gdiplus.lib")

class PieChart
{
private:
	std::vector<Item*>* _itemList;
	HDC _hdc;
	HDC _hMemDC;
	HDC _hMergingMemDC;
	HBITMAP _background;
	bool _isDrew;
	Gdiplus::Point _position;
	int _radius;
	float _percent[6] = { 0 };
	float _angle[7] = { 0 };
	
	Gdiplus::Color _color[6];
	RECT _lastRect;
	int _indexPie;
	int _indexChosen;
	unsigned __int64 _totalCost;
	unsigned __int64 _partialCost[6] = { 0 };
	float distance(Gdiplus::Point a, Gdiplus::Point b);
	Gdiplus::Point findPointOfPie(Gdiplus::Point center, float deltaAngle);
public:
	static PieChart* create(std::vector<Item*>* itemList);
	void setHDC(HDC hdc);
	void setPosition(Gdiplus::Point pos);
	void setRadius(int radius);
	void setIndexChosen(int index);
	RECT getBoudingBox();
	Gdiplus::Point getPosition();
	int getRadius();
	void update();
	void draw(HWND hwnd);
	bool onMouseMove(Gdiplus::Point pos);
	bool onMouseClick(Gdiplus::Point pos);
	int getIndexPie();
	int getIndexChosen();
	void clear();
	PieChart();
	~PieChart();
};
#endif // __PIE_CHART__
