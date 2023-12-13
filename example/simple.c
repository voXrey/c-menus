#include <stdio.h>

#include "menus.h"


void helloWorld()
{
	printf("Hello World");
}

void foo()
{
	printf("foo");
}

void bar()
{
	printf("bar");
}

void nothing(choice *c) {}

void leave(choice *c)
{
    printf("Good by!");
    changeRedirection(c, NULL);
}


// Init all menu and return the main menu
menu *initMenus()
{
    // Menus
    menu *main_menu = createMenu("~/main");
    menu *sub_menu_others = createMenu("~/main/others");

    // Choices main menu
    addChoice(createChoice("Say Hello", helloWorld, NULL, main_menu), main_menu);
    addChoice(createChoice("Others", nothing, sub_menu_others, sub_menu_others), main_menu);
    addChoice(createChoice("Exit game", leave, NULL, main_menu), main_menu);

    // Choices submenu others
    addChoice(createChoice("Foo", foo, NULL, sub_menu_others), sub_menu_others);
    addChoice(createChoice("Bar", bar, NULL, sub_menu_others), sub_menu_others);
    addChoice(createChoice("Back", nothing, NULL, main_menu), sub_menu_others);

    return main_menu;
}

int main()
{
    // Menus
    menu *main_menu = initMenus();
    navigateInMenu(main_menu); // start navigation and more
    destroyMenu(main_menu); // last action of the program
}
