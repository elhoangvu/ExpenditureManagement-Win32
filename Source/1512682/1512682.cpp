// 1512682.cpp : Defines the entry point for the application.
// Thực hiện: Lê Hoàng Vũ - 1512682
// Đồ án giữa kỳ - Quản lý chi tiêu

#include "stdafx.h"
#include "1512682.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1512682, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512682));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
	auto bitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512682));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNHILIGHT + 1);
	//wcex.hbrBackground = CreatePatternBrush(bitmap);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1512682);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   auto widthWindow = GetSystemMetrics(SM_CXSCREEN);
   auto hightWindow = GetSystemMetrics(SM_CYSCREEN);

   HWND hWnd = CreateWindowW(szWindowClass, L"Quản lý chi tiêu", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	   widthWindow / 2 - 450, 20, 850, 650, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
		HANDLE_MSG(hWnd, WM_LBUTTONUP, OnLButtonUp);
		HANDLE_MSG(hWnd, WM_MOUSEMOVE, OnMouseMove);
		HANDLE_MSG(hWnd, WM_DRAWITEM, OnDrawItem);
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void OnDestroy(HWND hwnd)
{
	RestoreData();
	if (gItemList)
		delete gItemList;
	GdiplusShutdown(gdiplusToken);
	PostQuitMessage(0);
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{

	INITCOMMONCONTROLSEX icex;

	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_DATE_CLASSES;

	InitCommonControlsEx(&icex);

	HFONT hFont = CreateFont(17, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"Arial");

	SetWindowFont(hwnd, hFont, true);

	hItemBox = CreateWindow(L"combobox", L"", CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE, 10, 140, 120, 200, hwnd, (HMENU)ID_CMBCHOICE, hInst, NULL);
	ComboBox_AddString(hItemBox, L"Ăn uống");
	ComboBox_AddString(hItemBox, L"Di chuyển");
	ComboBox_AddString(hItemBox, L"Nhà cửa");
	ComboBox_AddString(hItemBox, L"Xe cộ");
	ComboBox_AddString(hItemBox, L"Nhu yếu phẩm");
	ComboBox_AddString(hItemBox, L"Dịch vụ");
	ComboBox_SetCurSel(hItemBox, 0);
	SetWindowFont(hItemBox, hFont, true);

	hDescription = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE, 10, 200, 120, 50, hwnd, (HMENU)ID_EDIT_DESCRIPTION, hInst, NULL);
	SetWindowFont(hDescription, hFont, true);

	hCost = CreateWindowEx(NULL, L"EDIT", L"", WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | SS_RIGHT | ES_NUMBER, 10, 290, 120, 20, hwnd, (HMENU)ID_EDIT_COST, hInst, NULL);
	SetWindowFont(hCost, hFont, true);

	hDateTime = CreateWindowEx(0, DATETIMEPICK_CLASS, TEXT("DateTime"), WS_CHILD | WS_VISIBLE | DTS_SHORTDATECENTURYFORMAT, 10, 350, 120, 25, hwnd, NULL, hInst, NULL);
	SetWindowFont(hDateTime, hFont, true);

	auto hBtnAdd = CreateWindowEx(NULL, L"button", L"Thêm", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 10, 390, 120, 25, hwnd, (HMENU)ID_BTN_ADD, hInst, NULL);
	SetWindowFont(hBtnAdd, hFont, true);

	HWND hNew = CreateWindowEx(NULL, L"button", L"Mới", WS_CHILD | WS_VISIBLE |  BS_OWNERDRAW, 750, 10, 30, 30, hwnd, (HMENU)ID_BTN_NEW, hInst, NULL);
	SetWindowFont(hNew, hFont, true);

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	gListView = ListView::create(hwnd, hInst, ID_LISTVIEW, CPoint(230, 370), CSize(550, 200));
	gListView->init();
	gListView->setFont(hFont);
	LoadData();
	gItemList = gListView->getItemList();
	gPieChart = PieChart::create(gItemList);
	gPieChart->setPosition(Gdiplus::Point(410, 120));
	gPieChart->setRadius(90);

	gTabControl = TabControl::create(hwnd, NULL, CPoint(230, 340), CSize(550, 30));


	gCurColor = Gdiplus::Color(255, 0, 174, 239);
	return true;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case ID_BTN_ADD:
	{
		WCHAR bufDesc[1000];
		WCHAR bufCost[33];
		WCHAR bufDate[20];
		GetWindowText(hDescription, bufDesc, 1000);
		GetWindowText(hCost, bufCost, 33);
		GetWindowText(hDateTime, bufDate, 20);

		int curIndex = ComboBox_GetCurSel(hItemBox);
		unsigned __int64 cost = wcstoll(bufCost, 0, 10);
		if (cost == 0)
		{
			break;
		}
		auto item = Item::create((ItemType)curIndex, bufDesc, bufDate, cost);
		gListView->addChild(item);
		SetWindowText(hDescription, L"");
		SetWindowText(hCost, L"");
		gTabControl->setCurSel(0);
		gCurColor = gTabControl->getCurColor();
		RECT rect = gPieChart->getBoudingBox();
		rect.bottom += 500;
		InvalidateRect(hwnd, &rect, FALSE);
		break;
	}
	case ID_BTN_NEW:
	{
		int id = MessageBox(hwnd, L"Nếu tạo mới thì dữ liệu hiện tại sẽ bị xóa. Bạn có muốn tiếp tục không?", L"Thông báo", MB_YESNO | MB_ICONWARNING);
		if (id == IDYES)
		{
			gListView->clear();
			gPieChart->clear();
			InvalidateRect(hwnd, &gPieChart->getBoudingBox(), FALSE);
		}
		break;
	}
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hwnd);
		break;
	}
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	gHDC = BeginPaint(hwnd, &ps);

	auto graphics = new Gdiplus::Graphics(gHDC);
	graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	if (!gIsDrew)
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 186, 188, 190));
		graphics->FillRectangle(&brush, 0, 0, rect.right, 53);
		Gdiplus::SolidBrush brush2(Gdiplus::Color(255, 0, 174, 239));
		graphics->FillRectangle(&brush2, 0, 0, rect.right, 50);


		gIsDrew = true;
	}
	gPieChart->setHDC(gHDC);
	gPieChart->draw(hwnd);

	gTabControl->setHDC(gHDC);
	gTabControl->draw();

	Gdiplus::Pen pen(Gdiplus::Color(255, 0, 174, 239));
	graphics->DrawRectangle(&pen, 9, 199, 121, 51);
	graphics->DrawRectangle(&pen, 9, 289, 121, 21);
	graphics->DrawRectangle(&pen, 9, 349, 121, 26);
	graphics->DrawRectangle(&pen, 9, 139, 121, 26);
	//Gdiplus::Pen pen2(gCurColor);
	//graphics->DrawRectangle(&pen2, 229, 370, 551, 200);

	Gdiplus::FontFamily  fontFamily(L"Arial");
	Gdiplus::Font        font(&fontFamily, 15, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(8.0f, 180.0f);
	Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 0, 174, 239));

	graphics->DrawString(L"Nội dung", -1, &font, pointF, &solidBrush);
	pointF = PointF(8.0f, 270.0f);
	graphics->DrawString(L"Chi phí (VNĐ)", -1, &font, pointF, &solidBrush);
	pointF = PointF(8.0f, 120.0f);
	graphics->DrawString(L"Loại chi tiêu", -1, &font, pointF, &solidBrush);
	pointF = PointF(8.0f, 330.0f);
	graphics->DrawString(L"Ngày", -1, &font, pointF, &solidBrush);

	pointF = PointF(8.0f, 8.0f);
	Gdiplus::FontFamily  fontFamily2(L"Arial");
	Gdiplus::Font        font2(&fontFamily2, 28, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 255, 255, 255));
	graphics->DrawString(L"Quản lý chi tiêu", -1, &font2, pointF, &solidBrush2);

	Gdiplus::Font        font3(&fontFamily2, 20, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	pointF = PointF(230.0f, 80.0f);
	graphics->DrawString(L"Biểu đồ", -1, &font3, pointF, &solidBrush);
	pointF = PointF(0.0f, 80.0f);
	graphics->DrawString(L"Thêm chi tiêu", -1, &font3, pointF, &solidBrush);
	delete graphics;
	EndPaint(hwnd, &ps);
}

void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}

void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
	static int lastIndexPie = -1;
	if (gPieChart->onMouseMove(Gdiplus::Point(x, y)))
	{
		if (lastIndexPie != gPieChart->getIndexPie())
		{
			InvalidateRect(hwnd, &gPieChart->getBoudingBox(), FALSE);
		}
	}
	else
	{
		if (lastIndexPie != -1)
		{
			InvalidateRect(hwnd, &gPieChart->getBoudingBox(), FALSE);
		}
	}
	lastIndexPie = gPieChart->getIndexPie();
}

void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
	static int lastIndexChoose = -1;
	if (gPieChart->onMouseClick(Gdiplus::Point(x, y)))
	{
		if (lastIndexChoose != gPieChart->getIndexChosen())
		{
			RECT rect = gPieChart->getBoudingBox();

			rect.bottom += 500;
			gTabControl->setCurSel(gPieChart->getIndexChosen() + 1);
			gCurColor = gTabControl->getCurColor();
			gListView->doFillter((ItemType)(gPieChart->getIndexChosen() + 1), gCurColor.ToCOLORREF());

			InvalidateRect(hwnd, &rect, FALSE);
			//InvalidateRect(hwnd, &gPieChart->getBoudingBox(), FALSE);
		}
	}
	else
	{
		if (lastIndexChoose != -1)
		{
			InvalidateRect(hwnd, &gPieChart->getBoudingBox(), FALSE);
		}
	}
	lastIndexChoose = gPieChart->getIndexChosen();

	int index = gTabControl->onMouseUp(CPoint(x, y));
	if (index >= 0)
	{
		if (gTabControl->getCurSel() != gTabControl->getPreSel())
		{
			RECT rect = gPieChart->getBoudingBox();

			rect.bottom += 500;
			gPieChart->setIndexChosen(index - 1);
			gCurColor = gTabControl->getCurColor();
			gListView->doFillter((ItemType)index, gCurColor.ToCOLORREF());

			InvalidateRect(hwnd, &rect, FALSE);
			//InvalidateRect(hwnd, &gPieChart->getBoudingBox(), FALSE);
		}
	}

}

void OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem)
{
	switch (lpDrawItem->CtlID)
	{
	case ID_BTN_ADD:
	{
		auto graphics = new Graphics(lpDrawItem->hDC);
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 174, 239));
		graphics->FillRectangle(&brush, 0, 0, 120, 25);

		Gdiplus::FontFamily  fontFamily(L"Arial");
		Gdiplus::Font        font(&fontFamily, 15, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		Gdiplus::PointF      pointF(37, 3);
		Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 255, 255, 255));
		graphics->DrawString(L"Thêm", -1, &font, pointF, &solidBrush);

		if (graphics)
			delete graphics;
		break;
	}
	case ID_BTN_NEW:
	{
		auto graphics = new Graphics(lpDrawItem->hDC);
		auto image = new Image(L"new.jpg");
		graphics->DrawImage(image, -1, -1, 32, 32);

		if (image)
			delete image;
		if (graphics)
			delete graphics;
		break;
	}
	}
}

void RestoreData()
{
	std::wofstream fos(L"data.txt");
	std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	fos.imbue(loc);
	if (!fos.is_open())
		return;
	for (int i = 0; i < gItemList->size(); i++)
	{
		WCHAR buffer[100];
		wcscpy(buffer, gItemList->at(i)->getName());
		fos << buffer << L";";
		wcscpy(buffer, gItemList->at(i)->getDescription());
		fos << buffer << L";";
		wcscpy(buffer, gItemList->at(i)->getDate());
		fos << buffer << L";";
		wsprintf(buffer, L"%lu", gItemList->at(i)->getCost());
		fos << buffer << L"\n";
	}
	fos.close();
}

void LoadData()
{
	std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	std::wifstream fin(L"data.txt");
	fin.imbue(loc);
	WCHAR buffer[200];
	WCHAR type[15];
	WCHAR description[100];
	WCHAR date[20];
	WCHAR cost[20];
	unsigned __int64 cost2;
	int i = 0;

	while (!fin.eof())
	{
		fin.getline(buffer, 200);
		if (buffer[0] == NULL)
		{
			break;
		}
		int len = wcslen(buffer);
		int j, k;
		for (j = 0; j < len; j++)
		{
			if (buffer[j] == L';')
			{
				j++;
				break;
			}
			type[j] = buffer[j];
		}
		type[j - 1] = NULL;

		for (k = j; k < len; k++)
		{
			if (buffer[k] == L';')
			{
				k++;
				break;
			}
			description[k - j] = buffer[k];
		}
		description[k - j - 1] = NULL;

		for (j = k; j < len; j++)
		{
			if (buffer[j] == L';')
			{
				j++;
				break;
			}
			date[j - k] = buffer[j];
		}
		date[j - k - 1] = NULL;

		for (k = j; k < len; k++)
		{
			cost[k - j] = buffer[k];
		}

		cost[k - j] = NULL;
		for (k = 0; k < 6; k++)
		{
			if (wcscmp(type, ItemName[k]) == 0)
			{
				break;
			}
		}

		cost2 = wcstoll(cost, 0, 10);
		if (cost == 0)
		{
			continue;
		}
		
		auto item = Item::create((ItemType)k, description, date, cost2);
		gListView->addChild(item);
	}

	fin.close();
}
