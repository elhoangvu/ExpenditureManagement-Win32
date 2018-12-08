#include "stdafx.h"
#include "TabControl.h"
#include <new>

TabControl * TabControl::create(HWND hwnd, HDC hdc, CPoint position, CSize size)
{
	auto tc = new (std::nothrow) TabControl();
	tc->_position = position;
	tc->_size = size;
	tc->_hdc = hdc;
	tc->_hwnd = hwnd;
	return tc;
}


int TabControl::onMouseUp(CPoint pos)
{
	if (pos.x > _position.x && pos.x < _position.x + _size.width &&
		pos.y>_position.y && pos.y < _position.y + _size.height)
	{
		int delta = _size.width / ITEM_COUNT;
		int ptr = _position.x;
		for (int i = 0; i < ITEM_COUNT; i++)
		{
			if (pos.x > ptr && pos.x < ptr + delta)
			{
				_preSel = _curSel;
				_curSel = i;
				return i;
			}
			ptr += delta;
		}
	}
	return -1;
}

void TabControl::draw()
{
	RECT rect;
	if (!_isDrew)
	{
		GetClientRect(_hwnd, &rect);
		_hMemDC = CreateCompatibleDC(_hdc);
		_background = CreateCompatibleBitmap(_hdc, rect.right, rect.bottom);
		FillRect(_hMemDC, &rect, HBRUSH(RGB(255, 255, 255)));
		(HBITMAP)SelectObject(_hMemDC, _background);


		_background = CreateCompatibleBitmap(_hdc, rect.right, rect.bottom);
		_isDrew = true;
	}

	GetClientRect(_hwnd, &rect);
	FillRect(_hMemDC, &rect, HBRUSH(RGB(255, 255, 255)));
	auto graphics = new Gdiplus::Graphics(_hMemDC);
	graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	float delta = (float)_size.width / (float)ITEM_COUNT;
	float ptr = _position.x;
	Gdiplus::RectF rect2;
	for (int i = 0; i < ITEM_COUNT; i++)
	{
		Gdiplus::Color color;
		if (_curSel != -1)
		{
			if (_curSel == i)
			{
				color = _color[i];
			}
			else
			{
				color = _subColor[i];
			}
		}

		Gdiplus::SolidBrush brush(color);
		rect2.X = ptr;
		rect2.Y = _position.y;
		rect2.Width = delta;
		rect2.Height = _size.height;
		graphics->FillRectangle(&brush, rect2);

		Gdiplus::FontFamily  fontFamily(L"Arial");
		Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 255, 255, 255));
		if (i == 0)
		{
			WCHAR buffer[] = L"Tất cả";
			Gdiplus::Font        font(&fontFamily, delta / 5, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
			Gdiplus::PointF      pointF(_position.x + delta / 6.5, _position.y + _size.height / 5);
			graphics->DrawString(buffer, -1, &font, pointF, &solidBrush);
		}
		else
		{
			float dt = 0;
			float size = delta / 6;
			int len = wcslen(ITEMNAME[i - 1]);
			if (len > 6)
			{
				dt = (len - 6)*delta*0.02;
			}
			if (i == 5)
			{
				size = delta / 8;
			}
			Gdiplus::Font        font(&fontFamily, size, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
			Gdiplus::PointF      pointF(ptr + delta / 6 - dt, _position.y + _size.height / 5);
			graphics->DrawString(ITEMNAME[i - 1], -1, &font, pointF, &solidBrush);
		}
		ptr += delta;
	}

	Gdiplus::Pen pen(_color[_curSel], 2);
	graphics->DrawLine(&pen, Gdiplus::Point(_position.x, _position.y + _size.height - 1), Gdiplus::Point(_position.x + _size.width, _position.y + _size.height - 1));

	if (!BitBlt(_hdc, _position.x, _position.y, _size.width, _size.height, _hMemDC, _position.x, _position.y, SRCCOPY))
	{
		MessageBox(_hwnd, L"Failed to transfer bit block", L"Error", MB_OK);
	}

}

void TabControl::setHDC(HDC hdc)
{
	_hdc = hdc;
}

int TabControl::getCurSel()
{
	return _curSel;
}

int TabControl::getPreSel()
{
	return _preSel;
}

CPoint TabControl::getPosition()
{
	return _position;
}

CSize TabControl::getSize()
{
	return _size;
}

void TabControl::setCurSel(int curSel)
{
	_preSel = _curSel;
	_curSel = curSel;
}

Gdiplus::Color TabControl::getCurColor()
{
	return _color[_curSel];
}


TabControl::TabControl()
{
	_curSel = 0;
	_preSel = -1;
	_isDrew = false;
	_color[0] = Gdiplus::Color(255, 7, 146, 201);
	_color[1] = Gdiplus::Color(255, 238, 53, 91);
	_color[2] = Gdiplus::Color(255, 217, 191, 173);
	_color[3] = Gdiplus::Color(255, 100, 201, 213);
	_color[4] = Gdiplus::Color(255, 50, 169, 146);
	_color[5] = Gdiplus::Color(255, 238, 167, 58);
	_color[6] = Gdiplus::Color(255, 156, 107, 179);


	_subColor[0] = Gdiplus::Color(255, 227, 244, 253);
	_subColor[1] = Gdiplus::Color(255, 254, 235, 231);
	_subColor[2] = Gdiplus::Color(255, 251, 249, 248);
	_subColor[3] = Gdiplus::Color(255, 239, 248, 250);
	_subColor[4] = Gdiplus::Color(255, 234, 243, 242);
	_subColor[5] = Gdiplus::Color(255, 254, 245, 233);
	_subColor[6] = Gdiplus::Color(255, 240, 235, 245);

}


TabControl::~TabControl()
{
}
