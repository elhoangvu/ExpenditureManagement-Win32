#pragma once
#include "Space.h"
#include "Item.h"
#include <vector>
#include <Commctrl.h>
#include <windowsx.h>

class ListView
{
private:
	HWND _hParent;
	HINSTANCE _hIns;
	HWND _hwnd;
	HIMAGELIST _hImage;
	CPoint _position;
	CSize _size;
	int _id;
	std::vector<Item*>* _itemList;
public:
	static ListView* create(HWND hParent, HINSTANCE hIns, int id, CPoint position, CSize size);
	void init();
	void addChild(Item* item, bool isPushBack = true, int index = 0, bool isChangeColor = false);
	void doFillter(ItemType itemType, COLORREF color = RGB(0, 0, 0));
	std::vector<Item*>* getItemList();
	void setFont(HFONT hFont);
	void clear();
	ListView();
	~ListView();
};

