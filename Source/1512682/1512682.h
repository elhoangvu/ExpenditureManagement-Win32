#pragma once

#include "resource.h"
#include "Windowsx.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#include "Item.h"
#include <vector>
#include "PieChart.h"
#include <Commctrl.h>
#include "ListView.h"
#include "TabControl.h"
#include <iostream>
#include <xlocale>
#include <xstring>
#include <codecvt>
#include <fstream>
#include <Shlwapi.h>
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Comctl32.lib")

using namespace Gdiplus;

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

void OnDestroy(HWND hwnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);
void OnSize(HWND hwnd, UINT state, int cx, int cy);
void OnPaint(HWND hwnd);
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
void OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem);
void RestoreData();
void LoadData();

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;
std::vector<Item*>* gItemList;
PieChart* gPieChart;
ListView* gListView;
TabControl* gTabControl;
bool gIsDrew = false;
HWND hCost;
HWND hDescription;
HWND hItemBox;
HWND hDateTime;
HDC gHDC;
Gdiplus::Color gCurColor;