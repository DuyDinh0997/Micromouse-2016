#ifndef __MENU_STRIP_H__
#define __MENU_STRIP_H__

#define NUMBER_OF_MENU_ITEMS 10

// The standard callback type. 
typedef void (*menuCallback)();

typedef struct MenuItem
{
    menuCallback callback;
    const char* text;
} MenuItem;

typedef struct Menu
{
    // Each Menu Item
    MenuItem items[NUMBER_OF_MENU_ITEMS];

    // Which item is currently selected.
    int index;
    
    // Number of items 
    int numOfItems;
} Menu;

void MenuReset(Menu* this);
const char* MenuGetTextAt(Menu* this, int index);
menuCallback MenuItemClick(Menu* this);
int MenuGetIndex(Menu* this);
int MenuAddItem(Menu* this, const char* text, menuCallback callback);
const char* MenuGetText(Menu* this);
int MenuSetIndex(Menu* this, int index);
int MenuLength(Menu* this);
#endif
