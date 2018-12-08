#pragma once
#ifndef __TAB_CONTROL_H__
#define __TAB_CONTROL_H__

#include "Item.h"
#include "Space.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#include <Windows.h>
#pragma comment(lib, "gdiplus.lib")
#define ITEM_COUNT 7

class TabControl
{
private:
	CSize _size;
	CPoint _position;
	HDC _hdc;
	int _curSel;
	int _preSel;
	HDC _hMemDC;
	HBITMAP _background;
	HWND _hwnd;
	Gdiplus::Color _color[7];
	Gdiplus::Color _subColor[7];
	bool _isDrew;
public:
	static TabControl* create(HWND hwnd, HDC hdc, CPoint position, CSize size);
	int onMouseUp(CPoint pos);
	void draw();
	void setHDC(HDC hdc);
	int getCurSel();
	int getPreSel();
	CPoint getPosition();
	CSize getSize();
	void setCurSel(int curSel);
	Gdiplus::Color getCurColor();
	TabControl();
	~TabControl();
};
#endif //__TAB_CONTROL_H__
