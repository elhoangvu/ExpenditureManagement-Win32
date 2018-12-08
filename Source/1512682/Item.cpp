#include "stdafx.h"
#include "Item.h"

WCHAR* ItemName[] = { L"Ăn uống", L"Di chuyển", L"Nhà cửa", L"Xe cộ", L"Nhu yếu phẩm",L"Dịch vụ" };

Item * Item::create(ItemType itemType, WCHAR * description, WCHAR* date, unsigned __int64 cost)
{
	Item* item = new (std::nothrow) Item;
	item->_itemType = itemType;
	int len = wcslen(description) + 1;
	item->_description = new WCHAR[len];
	wcscpy(item->_description, description);
	item->_description[len] = NULL;

	len = wcslen(date) + 1;
	item->_date = new WCHAR[len];
	wcscpy(item->_date, date);
	item->_date[len] = NULL;

	item->_cost = cost;

	return item;
}

ItemType Item::getItemType()
{
	return _itemType;
}

unsigned __int64 Item::getCost()
{
	return _cost;
}

WCHAR * Item::getName()
{
	return ITEMNAME[_itemType];

	/*switch (_itemType)
	{
	case AN_UONG:
		return L"Ăn uống";
	case DI_CHUYEN:
		return L"Di chuyển";
	case NHA_CUA:
		return L"Nhà cửa";
	case XE_CO:
		return L"Xe cộ";
	case NHU_YEU_PHAM:
		return L"Nhu yếu phẩm";
	}*/
		return L"Dịch vụ";
}

WCHAR * Item::getDescription()
{
	return _description;
}

WCHAR * Item::getDate()
{
	return _date;
}

Item::Item()
{
	_itemType = ItemType::AN_UONG;
	_cost = 0;
	_description = nullptr;
}


Item::~Item()
{
	if (_description)
		delete _description;
}
