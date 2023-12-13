#ifndef MENUS_H
#define MENUS_H


int FIRST_MENU_LEN = 2;

struct choice
{
    char *phrase;
    void (*f)();
    menu *parent_menu;
    menu *sub_menu;
    menu *redirection;
};
typedef struct choice choice;

struct menu
{
    char *path;
    choice **choices;
    int len_choices;
    int len_max;
};
typedef struct menu menu;

menu *initMenus();
void navigateInMenu(menu *m);
void destroyMenu(menu *m);


#endif // MENUS_H
