#include <stdio.h>
#include "menuStrip.h"

void testexecute1(void) { printf("Test1!\n"); }
void testexecute2(void) { printf("Test2!\n"); }
void testexecute3(void) { printf("Test3!\n"); }

void printMenu(Menu* menu)
{
    printf("i = %d\n", menu->index);

    int i;
    for (i = 0; i < menu->numOfItems; i++)
    {
        if (i == menu->index) printf("**");
        else printf("  ");
        printf("%s\n", MenuGetTextAt(menu, i));  
    }
}

int main(int argv, char* argc[0])
{
    Menu menu; 
    MenuReset(&menu);

    char userInput[255];
    
    MenuAddItem(&menu, "ITEM", testexecute1); 
    MenuAddItem(&menu, "ITE2", testexecute2);
    MenuAddItem(&menu, "ITE3", testexecute3);

    while (1)
    {
        scanf("%s", userInput);
        if (userInput[0] == 'u') menu.index = menu.index+1 >= menu.numOfItems ? 0 : menu.index+1;
        if (userInput[0] == 'd') menu.index = menu.index-1 < 0 ? menu.numOfItems - 1 : menu.index-1;
        if (userInput[0] == 'c') MenuItemClick(&menu)();
        
        printMenu(&menu);         
    }
    
    MenuItemClick(&menu)();

    printf("Done!\n");
    
    return 0;        
}
