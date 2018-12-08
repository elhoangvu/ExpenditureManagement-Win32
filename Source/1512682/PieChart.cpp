#include "stdafx.h"
#include "PieChart.h"
#include <corecrt_math_defines.h>

float PieChart::distance(Gdiplus::Point a, Gdiplus::Point b)
{
	return sqrt((a.X - b.X)*(a.X - b.X) + (a.Y - b.Y)*(a.Y - b.Y));
}

Gdiplus::Point PieChart::findPointOfPie(Gdiplus::Point center, float deltaAngle)
{
	float deltaAngle2 = deltaAngle*M_PI / 180.0f;
	float x = _radius*cos(deltaAngle2) + center.X;

	float y;
	float delta = -x*x + 2 * x*center.X - center.X*center.X + _radius*_radius;
	float y1 = center.Y + sqrt(delta);
	float y2 = center.Y - sqrt(delta);
	if (deltaAngle > 180)
	{
		y = y2;
	}
	else
	{
		y = y1;
	}

	auto midPoint = Gdiplus::Point(x, y);
	return midPoint;
}

PieChart * PieChart::create(std::vector<Item*>* itemList)
{
	auto pieChart = new (std::nothrow) PieChart;
	pieChart->_itemList = itemList;

	return pieChart;
}

void PieChart::setHDC(HDC hdc)
{
	_hdc = hdc;
}

void PieChart::setPosition(Gdiplus::Point pos)
{
	_position = pos;
}

void PieChart::setRadius(int radius)
{
	_radius = radius;
}

void PieChart::setIndexChosen(int index)
{
	_indexChosen = index;
}

RECT PieChart::getBoudingBox()
{
	RECT rect;
	rect.top = _position.Y - _radius * 0.2f;
	rect.left = _position.X - _radius * 2.8;
	rect.right = rect.left + _radius * 7.8;
	rect.bottom = rect.top + _radius*2.4;
	return rect;
}

Gdiplus::Point PieChart::getPosition()
{
	return _position;
}

int PieChart::getRadius()
{
	return _radius;
}


void PieChart::update()
{
	bool count[6] = { false };
	unsigned __int64 sum = 0;
	for (int i = 0; i < 6; i++)
	{
		_partialCost[i] = 0;
	}
	for (int i = 0; i < _itemList->size(); i++)
	{
		count[(*_itemList)[i]->getItemType()] = true;
		_partialCost[(*_itemList)[i]->getItemType()] += (*_itemList)[i]->getCost();
		sum += (*_itemList)[i]->getCost();
	}
	if (_itemList->size() == 0)
	{
		return;
	}

	for (int i = 0; i < 6; i++)
	{
		_percent[i] = ((double)count[i] * (double)_partialCost[i] / (double)sum) * 100;
		_angle[i + 1] = _percent[i] * 3.6 + _angle[i];
	}
	_totalCost = sum;
}

void PieChart::draw(HWND hwnd)
{
	if (_hdc == NULL)
	{
		return;
	}

	if (!_isDrew)
	{
		GetClientRect(hwnd, &_lastRect);
		_hMemDC = CreateCompatibleDC(_hdc);
		_background = CreateCompatibleBitmap(_hdc, _lastRect.right, _lastRect.bottom);
		FillRect(_hMemDC, &_lastRect, HBRUSH(RGB(255,255,255)));
		///_background = (HBITMAP)LoadImage(NULL, L"background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		(HBITMAP)SelectObject(_hMemDC, _background);

		_hMergingMemDC = CreateCompatibleDC(_hdc);
		_background = CreateCompatibleBitmap(_hdc, _lastRect.right, _lastRect.bottom);
		FillRect(_hMergingMemDC, &_lastRect, HBRUSH(RGB(255, 255, 255)));
		///_background = (HBITMAP)LoadImage(NULL, L"background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		(HBITMAP)SelectObject(_hMergingMemDC, _background);
		_background = CreateCompatibleBitmap(_hdc, _lastRect.right, _lastRect.bottom);
		FillRect(_hMergingMemDC, &_lastRect, HBRUSH(RGB(255, 255, 255)));
		///_background = (HBITMAP)LoadImage(NULL, L"background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		_isDrew = true;
	}

	this->update();
	if (_isDrew)
	{
		//_background = (HBITMAP)LoadImage(NULL, L"background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		FillRect(_hMergingMemDC, &_lastRect, HBRUSH(RGB(255, 255, 255)));
		FillRect(_hMemDC, &_lastRect, HBRUSH(RGB(255, 255, 255)));
		///FillRect(_hMemDC, &_lastRect, CreatePatternBrush(_background));
		///FillRect(_hMergingMemDC, &_lastRect, CreatePatternBrush(_background));
	}
	auto graphics = new Gdiplus::Graphics(_hMemDC);
	graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	Gdiplus::Rect ellipseRect(_position.X, _position.Y, _radius * 2, _radius * 2);

	for (int i = 0; i < 6; i++)
	{
		Gdiplus::SolidBrush brush(_color[i]);
		graphics->FillPie(&brush, ellipseRect, _angle[i], _angle[i + 1] - _angle[i] + 0.5f);
	}
	Gdiplus::SolidBrush brush3(Gdiplus::Color(255, 54, 60, 58));
	Gdiplus::Rect ellipseRect3(_position.X + _radius*0.55f, _position.Y + _radius*0.55f, _radius*0.9f, _radius*0.9f);
	graphics->FillEllipse(&brush3, ellipseRect3);

	if (!BitBlt(_hMergingMemDC, ellipseRect.X - _radius * 0.2f, ellipseRect.Y - _radius * 0.16f, ellipseRect.Width + _radius * 0.32f, ellipseRect.Height + _radius * 0.32f, _hMemDC, ellipseRect.X - _radius * 0.16f, ellipseRect.Y - _radius * 0.16f, SRCCOPY))
	{
		MessageBox(hwnd, L"Failed to transfer bit block", L"Error", MB_OK);
	}

	//if (!BitBlt(_hMergingMemDC, ellipseRect.X - 10, ellipseRect.Y - 10, ellipseRect.Width + 20, ellipseRect.Height + 20, _hBorderMemDC, ellipseRect.X - 10, ellipseRect.Y - 10, SRCAND))
	//{
	//	MessageBox(hwnd, L"Failed to transfer bit block", L"Error", MB_OK);
	//}
	auto graphics2 = new Gdiplus::Graphics(_hMergingMemDC);
	graphics2->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	
	WCHAR bufString[33];
	float delta = 0;
	
	//if (bufString)
	//	delete[] bufString;

	//auto pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 255, 255), 20);
	////Gdiplus::SolidBrush brush(_color[i]);
	////graphics->FillPie(&brush, ellipseRect, _angle[i], _angle[i + 1] - _angle[i]);
	//graphics2->DrawEllipse(pen, (int)ellipseRect2.X, (int)ellipseRect2.Y, (int)ellipseRect2.Width, (int)ellipseRect2.Height);
	///
	auto midPos = Gdiplus::Point(_position.X + _radius, _position.Y + _radius);


	std::vector<Gdiplus::Point> leftPos;
	std::vector<Gdiplus::Point> rightPos;
	int begin = 0;
	bool isHalfCir = false;
	for (int i = 0; i < 6; i++)
	{
		Gdiplus::SolidBrush brush3(_color[i]);
		auto pos = findPointOfPie(midPos, (_angle[i + 1] + _angle[i]) / 2.0f);
		Gdiplus::Rect ellipseRect2(pos.X - 4, pos.Y - 4, 8, 8);
		graphics2->FillEllipse(&brush3, (int)ellipseRect2.X, (int)ellipseRect2.Y, (int)ellipseRect2.Width, (int)ellipseRect2.Height);

		if (pos.X < midPos.X)
		{
			leftPos.push_back(pos);
			isHalfCir = true;
		}
		else
		{
			if (i == 0)
			{
				rightPos.push_back(pos);
				continue;
			}
			if (isHalfCir)
			{
				rightPos.insert(rightPos.begin() + begin++, pos);
			}
			else
			{
				rightPos.push_back(pos);
			}
			/*if (pos.Y < midPos.Y)
			{
				rightPos.insert(rightPos.begin() + begin++, pos);
			}
			else
			{
				rightPos.push_back(pos);
			}*/
		}
	}

	int leftCount = leftPos.size();
	int rightCount = rightPos.size();

	int leftDelta = _radius * 2 / (leftCount + 1);
	int rightDelta = _radius * 2 / (rightCount + 1);

	int bottom = _position.Y + _radius * 2;

	int downCount = 0;
	int upCount = 0;

	delta = 0;

	for (int i = 0; i < leftCount; i++)
	{
		auto pos1 = Gdiplus::Point(_position.X - _radius*0.5f, leftPos[i].Y);
		auto pos2 = Gdiplus::Point(pos1.X, bottom - leftDelta*(i + 1));

		int upDownCount = 0;
		if (leftPos[i].Y < pos2.Y)
		{
			pos1 = Gdiplus::Point(pos1.X - _radius*0.15f*upCount, pos1.Y);
			upDownCount = ++upCount;
		}
		else
		{
			pos1 = Gdiplus::Point(pos1.X + _radius*0.15f*downCount, pos1.Y);
			upDownCount = ++downCount;
		}
		auto pos3 = Gdiplus::Point(pos1.X, pos2.Y);
		int index = i + rightCount - begin;
		Gdiplus::SolidBrush brush3(_color[index]);
		//auto pos4 = Gdiplus::Point(pos3.X - _radius*0.2f*(i + 1), pos3.Y);
		if (leftCount > 2 && (i == leftCount - 1 || i == 0))
		{
			delta = _radius*0.25;
		}
		else
		{
			delta = 0;
		}
		Gdiplus::Pen pen(_color[index], 2);
		auto pos4 = Gdiplus::Point(_position.X - _radius - delta, pos3.Y);
		graphics2->DrawLine(&pen, pos1, leftPos[i]);
		graphics2->DrawLine(&pen, pos1, pos3);
		graphics2->DrawLine(&pen, pos3, pos4);
		graphics2->FillRectangle(&brush3, pos4.X - _radius*0.5f, pos4.Y - _radius*0.15f, _radius*0.5f, _radius*0.3f);

		wsprintf(bufString, L"%d.%d%%", (int)_percent[index], ((int)floorf(_percent[index] * 10)) % 10);
		Gdiplus::FontFamily  fontFamily(L"Arial");
		float size = _radius / 6;
		if (wcslen(bufString) == 6)
		{
			size = _radius / 7;
		}
		Gdiplus::Font        font(&fontFamily, size, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		float posXString = _radius*0.5;
		if (wcslen(bufString) == 4)
		{
			posXString = _radius*0.45f;
		}
		Gdiplus::PointF      pointF(pos4.X - posXString, pos4.Y - _radius*0.1f);
		Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 255, 255, 255));

		graphics2->DrawString(bufString, -1, &font, pointF, &solidBrush);
		float dt = 0;
		int len = wcslen(ITEMNAME[index]);
		if (len > 5)
		{
			dt = (len - 5)*_radius*0.07f;
		}
		pointF = Gdiplus::PointF(pos4.X - posXString - _radius*0.5f - dt, pos4.Y - _radius*0.1f);
		Gdiplus::SolidBrush  solidBrush2(_color[index]);
		Gdiplus::Font        font2(&fontFamily, _radius / 7, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		graphics2->DrawString(ITEMNAME[index], -1, &font2, pointF, &solidBrush2);
		//if (bufString)
		//	delete[] bufString;
	}

	downCount = 0;
	upCount = 0;
	for (int i = rightCount - 1; i >= 0; i--)
	{
		auto pos1 = Gdiplus::Point(_position.X + _radius*2.5f, rightPos[i].Y);
		auto pos2 = Gdiplus::Point(pos1.X, bottom - rightDelta*(rightCount - i));
		int upDownCount = 0;
		if (rightPos[i].Y < pos2.Y)
		{
			pos1 = Gdiplus::Point(pos1.X + _radius*0.15f*upCount, pos1.Y);
			upDownCount = ++upCount;
		}
		else
		{
			pos1 = Gdiplus::Point(pos1.X - _radius*0.15f*downCount, pos1.Y);
			upDownCount = ++downCount;
		}

		auto pos3 = Gdiplus::Point(pos1.X, pos2.Y);
		int index = 6 + i - begin;
		if (i >= begin)
		{
			index = i - begin;
		}
		Gdiplus::SolidBrush brush3(_color[index]);
		//auto pos4 = Gdiplus::Point(pos3.X + _radius*0.2f*(i + 1), pos3.Y);
		if (rightCount > 2 && (i == rightCount - 1 || i == 0))
		{
			delta = _radius*0.25;
		}
		else
		{
			delta = 0;
		}
		Gdiplus::Pen pen(_color[index], 2);
		auto pos4 = Gdiplus::Point(_position.X + _radius*3.0f + delta, pos3.Y);
		graphics2->DrawLine(&pen, pos1, rightPos[i]);
		graphics2->DrawLine(&pen, pos1, pos3);
		graphics2->DrawLine(&pen, pos3, pos4);
		graphics2->FillRectangle(&brush3, (Gdiplus::REAL)pos4.X, pos4.Y - _radius*0.15f, _radius*0.5f, _radius*0.3f);

		wsprintf(bufString, L"%d.%d%%", (int)_percent[index], ((int)floorf(_percent[index] * 10)) % 10);
		Gdiplus::FontFamily  fontFamily(L"Arial");
		float size = _radius / 6;
		if (wcslen(bufString) == 6)
		{
			size = _radius / 7;
		}
		Gdiplus::Font        font(&fontFamily, size, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		float posXString = 0;
		if (wcslen(bufString) == 4)
		{
			posXString = _radius*0.05f;
		}
		Gdiplus::PointF      pointF(pos4.X + posXString, pos4.Y - _radius*0.1f);
		Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 255, 255, 255));

		graphics2->DrawString(bufString, -1, &font, pointF, &solidBrush);

		pointF = Gdiplus::PointF(pos4.X + posXString + _radius*0.62f, pos4.Y - _radius*0.1f);
		Gdiplus::SolidBrush  solidBrush2(_color[index]);
		Gdiplus::Font        font2(&fontFamily, _radius / 7, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		graphics2->DrawString(ITEMNAME[index], -1, &font2, pointF, &solidBrush2);
	}
	/*if (pos.X < midPos.X)
	{
		graphics2->DrawLine(&pen, Gdiplus::Point(_position.X - _radius*0.25f, pos.Y), pos);
	}
	else
	{
		graphics2->DrawLine(&pen, Gdiplus::Point(_position.X + _radius*2.25f, pos.Y), pos);
	}*/
	///
	if (_indexPie != -1 || _indexChosen != -1)
	{
		int index = _indexChosen == -1 ? _indexPie : _indexChosen;
		Gdiplus::Rect ellipseRect3(_position.X - _radius * 0.15f, _position.Y - _radius * 0.15f, _radius*2.3f, _radius*2.3f);
		Gdiplus::SolidBrush brush(_color[index]);
		graphics2->FillPie(&brush, ellipseRect3, _angle[index] - 2, _angle[index + 1] - _angle[index] + 4);

	}
	Gdiplus::SolidBrush brush2(Gdiplus::Color(255, 54, 60, 58));
	Gdiplus::Rect ellipseRect2(_position.X + _radius / 2.0f, _position.Y + _radius / 2.0f, _radius, _radius);
	graphics2->FillEllipse(&brush2, ellipseRect2);
	//////////////////////
	unsigned __int64 par1;
	int par2;
	Gdiplus::FontFamily  fontFamily(L"Arial");
	Gdiplus::Font        font(&fontFamily, _radius / 6, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF;
	Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 255, 255, 255));
	

	Gdiplus::Font        font2(&fontFamily, _radius / 10, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	if (_indexPie == -1 && _indexChosen == -1)
	{
		par1 = _totalCost / 1000;
		par2 = (_totalCost % 1000);
		par2 = par2 / 100;
		pointF = Gdiplus::PointF(_position.X + _radius*0.7f, _position.Y + _radius*0.75f);
		graphics2->DrawString(L"Tổng chi phí", -1, &font2, pointF, &solidBrush);
	}
	else
	{
		int index = _indexPie == -1 ? _indexChosen : _indexPie;
		par1 = _partialCost[index] / 1000;
		par2 = (_partialCost[index] % 1000);
		par2 = par2 / 100;

		delta = 0;
		if (_percent[index] < 10)
		{
			delta = _radius*0.055f;
		}
		pointF = Gdiplus::PointF(_position.X + _radius*0.84f + delta, _position.Y + _radius*1.1f);
		wsprintf(bufString, L"%d.%d%%", (int)_percent[index], ((int)floorf(_percent[index] * 10)) % 10);
		graphics2->DrawString(bufString, -1, &font2, pointF, &solidBrush);
	
		delta = 0;
		auto len = wcslen(ITEMNAME[index]);
		delta = len*_radius*0.027;

		pointF = Gdiplus::PointF(_position.X + _radius - delta, _position.Y + _radius*0.75f);
		graphics2->DrawString(ITEMNAME[index], -1, &font2, pointF, &solidBrush);
	}
	
	wsprintf(bufString, L"%lu.%d K", par1,  (int)par2);

	auto len = wcslen(bufString);
	delta = 0;
	if (len > 4)
	{
		delta = (len - 4) *_radius*0.045f;
	}
	pointF = Gdiplus::PointF(_position.X + _radius*0.83f - delta, _position.Y + _radius*0.9f);
	graphics2->DrawString(bufString, -1, &font, pointF, &solidBrush);
	

	if (!BitBlt(_hdc, ellipseRect.X - _radius*2.8, ellipseRect.Y - _radius * 0.2f, _radius *7.8, _radius*2.4, _hMergingMemDC, ellipseRect.X - _radius * 2.8, ellipseRect.Y - _radius * 0.2f, SRCCOPY))
	{
		MessageBox(hwnd, L"Failed to transfer bit block", L"Error", MB_OK);
	}

	if (graphics2)
		delete graphics2;
	if (graphics)
		delete graphics;
}

bool PieChart::onMouseMove(Gdiplus::Point pos)
{
	COLORREF color = GetPixel(_hMemDC, pos.X, pos.Y);
	for (int i = 0; i < 6; i++)
	{
		if (color == _color[i].ToCOLORREF())
		{
			_indexPie = i;
			return true;
		}
	}
	_indexPie = -1;
	return false;
}

bool PieChart::onMouseClick(Gdiplus::Point pos)
{
	COLORREF color = GetPixel(_hMemDC, pos.X, pos.Y);
	for (int i = 0; i < 6; i++)
	{
		if (color == _color[i].ToCOLORREF())
		{
			_indexChosen = i;
			return true;
		}
	}
	_indexChosen = -1;
	return false;
}

int PieChart::getIndexPie()
{
	return _indexPie;
}

int PieChart::getIndexChosen()
{
	return _indexChosen;
}

void PieChart::clear()
{
	_indexPie = -1;
	_indexChosen = -1;
	_totalCost = 0;
	_angle[0] = 0;
	for (int i = 0; i < 6; i++)
	{
		_percent[i] = 100.0f / 6.0f;
		_angle[i + 1] = _angle[i] + 60;
		_partialCost[i] = 0;
	}
	_totalCost = 0;
}

PieChart::PieChart()
{
	_hdc = NULL;
	_radius = 100;
	_isDrew = false;
	_indexPie = -1;
	_indexChosen = -1;
	_totalCost = 0;
	_position = Gdiplus::Point(0, 0);
	_color[0] = Gdiplus::Color(255, 238, 53, 91);
	_color[1] = Gdiplus::Color(255, 217, 191, 173);
	_color[2] = Gdiplus::Color(255, 100, 201, 213);
	_color[3] = Gdiplus::Color(255, 50, 169, 146);
	_color[4] = Gdiplus::Color(255, 238, 167, 58);
	_color[5] = Gdiplus::Color(255, 156, 107, 179);
	_angle[0] = 0;
	for (int i = 0; i < 6; i++)
	{
		_percent[i] = 100.0f / 6.0f;
		_angle[i + 1] = _angle[i] + 60;
	}
}


PieChart::~PieChart()
{
}
