#include "menuStrip.h"

void MenuReset(Menu* this)
{
    this->index = 0;
    this->numOfItems = 0;
}

const char* MenuGetText(Menu* this)
{
    return this->items[this->index].text;
}

const char* MenuGetTextAt(Menu* this, int index)
{
    return this->items[index].text;
}

int MenuLength(Menu* this)
{
	return this->numOfItems;
}

int MenuSetIndex(Menu* this, int index)
{
	if (index >= this->numOfItems) return -1;

	// Return 1 if the index was actually changed.
	int returnValue = 0;
	if (index != this->index) returnValue = 1;

	this->index = index;

	return returnValue;
}

int MenuAddItem(Menu* this, const char* text, menuCallback callback)
{
    if (this->numOfItems >= NUMBER_OF_MENU_ITEMS) return 0;
    
    this->items[this->numOfItems].text = text;
    this->items[this->numOfItems].callback = callback;

    this->numOfItems++;

    return 1;
}

int MenuGetIndex(Menu* this)
{
    return this->index;
}

menuCallback MenuItemClick(Menu* this)
{
    return this->items[this->index].callback;
}

