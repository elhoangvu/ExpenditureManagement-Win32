#pragma once
#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>
#include <new>

static enum ItemType
{
	AN_UONG,
	DI_CHUYEN,
	NHA_CUA,
	XE_CO,
	NHU_YEU_PHAM,
	DICH_VU
};

extern WCHAR* ItemName[];
#define ITEMNAME ItemName

class Item
{
private:
	enum ItemType _itemType;
	unsigned __int64 _cost;
	WCHAR* _description;
	WCHAR* _date;
public:
	
	static Item* create(ItemType itemType, WCHAR* description, WCHAR* date, unsigned __int64 cost);
	ItemType getItemType();
	unsigned __int64 getCost();
	WCHAR* getName();
	WCHAR* getDescription();
	WCHAR* getDate();
	Item();
	~Item();
};

#endif // __ITEM__