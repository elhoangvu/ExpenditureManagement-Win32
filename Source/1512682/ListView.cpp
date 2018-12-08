#include "stdafx.h"
#include "ListView.h"
#include <new>
#include <CommCtrl.h>
#include "Resource.h"

ListView * ListView::create(HWND hParent, HINSTANCE hIns, int id, CPoint position, CSize size)
{
	auto lv = new (std::nothrow) ListView();
	lv->_hParent = hParent;
	lv->_hIns = hIns;
	lv->_id = id;
	lv->_position = position;
	lv->_size = size;

	lv->_hwnd = CreateWindow(WC_LISTVIEWW, L"List View", WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_ICON, position.x, position.y, size.width, size.height, hParent, (HMENU)id, hIns, NULL);

	return lv;
}

void ListView::init()
{
	LVCOLUMN column;
	column.cx = _size.width * 0.25f;
	column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	column.fmt = LVCFMT_LEFT;
	column.pszText = L"Loại chi tiêu";
	ListView_InsertColumn(_hwnd, 0, &column);
	column.cx = _size.width * 0.4f;
	column.pszText = L"Nội dung";
	ListView_InsertColumn(_hwnd, 1, &column);
	column.cx = _size.width * 0.15f;
	column.pszText = L"Ngày";
	ListView_InsertColumn(_hwnd, 2, &column);
	column.cx = _size.width*0.2f;

	column.pszText = L"Chi phí (VNĐ)";
	ListView_InsertColumn(_hwnd, 3, &column);


	_hImage = ImageList_Create(14, 14, ILC_COLOR16 | ILC_MASK, 0, 0);
	ListView_SetImageList(_hwnd, _hImage, LVSIL_SMALL);
	HICON hIco = LoadIcon(_hIns, MAKEINTRESOURCE(IDI_ICON1));
	ImageList_AddIcon(_hImage, hIco);
	hIco = LoadIcon(_hIns, MAKEINTRESOURCE(IDI_ICON2));
	ImageList_AddIcon(_hImage, hIco);
	hIco = LoadIcon(_hIns, MAKEINTRESOURCE(IDI_ICON3));
	ImageList_AddIcon(_hImage, hIco);
	hIco = LoadIcon(_hIns, MAKEINTRESOURCE(IDI_ICON4));
	ImageList_AddIcon(_hImage, hIco);
	hIco = LoadIcon(_hIns, MAKEINTRESOURCE(IDI_ICON5));
	ImageList_AddIcon(_hImage, hIco);
	hIco = LoadIcon(_hIns, MAKEINTRESOURCE(IDI_ICON6));
	ImageList_AddIcon(_hImage, hIco);
	ListView_SetTextColor(_hwnd, RGB(0, 174, 239));
	ListView_SetExtendedListViewStyle(_hwnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void ListView::addChild(Item * item, bool isPushBack, int index, bool isChangeColor)
{
	if (!isChangeColor)
	{
		ListView_SetTextColor(_hwnd, RGB(0, 174, 239));
	}
	LVITEM i;
	i.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	i.iSubItem = 0;
	i.pszText = item->getName();
	i.iImage = item->getItemType();
	//i.iImage = ;
	//i.lParam = (LPARAM)path;
	i.cchTextMax = 255;
	if (isPushBack)
	{
		index = _itemList->size();
	}
	i.iItem = index;
	ListView_InsertItem(_hwnd, &i);
	ListView_SetItemText(_hwnd, index, 1, item->getDescription());
	ListView_SetItemText(_hwnd, index, 2, item->getDate());
	WCHAR buffer[15];
	wsprintf(buffer, L"%d", item->getCost());
	ListView_SetItemText(_hwnd, index, 3, buffer);

	if (isPushBack)
	{
		_itemList->push_back(item);
	}
}

void ListView::doFillter(ItemType itemType, COLORREF color)
{
	ListView_DeleteAllItems(_hwnd);
	ListView_SetTextColor(_hwnd, color);
	int count = 0;
	for (int j = 0; j < _itemList->size(); j++)
	{

		bool result = (*_itemList)[j]->getItemType() == itemType - 1;
		if (itemType == (ItemType)0)
		{
			result = true;
		}
		if (result)
		{
			this->addChild((*_itemList)[j], false, count, true);
			count++;
		}
	}
}

std::vector<Item*>* ListView::getItemList()
{
	return _itemList;
}

void ListView::setFont(HFONT hFont)
{
	SetWindowFont(_hwnd, hFont, true);
}

void ListView::clear()
{
	ListView_DeleteAllItems(_hwnd);
	_itemList->clear();
}

ListView::ListView()
{
	_itemList = new std::vector<Item*>;
}


ListView::~ListView()
{
	for (int i = 0; i < _itemList->size(); i++)
	{
		delete (*_itemList)[i];
	}
	delete _itemList;
}
