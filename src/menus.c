#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "menus.h"

#define KNRM "\x1B[0m"
#define KYEL "\x1B[33m"


const int FIRST_MENU_LEN = 2;

// Tool to flush stdin
void flushStdin()
{
    char c;
    do
        c = getchar();
    while (c != '\n' && c != EOF);
}

// Create a choice for a menu, on the heap
choice *createChoice(char *phrase, void (*f)(), menu *sub_menu, menu *redirection)
{
    choice *c = malloc(sizeof(choice));
    c->phrase = phrase;
    c->f = f;
    c->sub_menu = sub_menu;
    c->redirection = redirection;
    return c;
}

// Change redirection of a choice
void changeRedirection(choice *c, menu *redirection)
{
    c->redirection = redirection;
}

// Delete a choice from the heap
void destroyChoice(choice *c)
{
    free(c);
}

// Create a menu on the heap
menu *createMenu(char *path)
{
    menu *m = malloc(sizeof(menu));
    m->path = path;
    m->len_choices = 0;
    m->choices = malloc(sizeof(choice *) * FIRST_MENU_LEN);
    m->len_max = FIRST_MENU_LEN;
    return m;
}

// Delete a menu from the heap
void destroyMenu(menu *m)
{
    // Delete sub menus
    for (int i = 0; i < m->len_choices; i++)
    {
        if (m->choices[i]->sub_menu != NULL && m->choices[i]->sub_menu != m) // protect against infinity loop
            destroyMenu(m->choices[i]->sub_menu);
        destroyChoice(m->choices[i]);
    }

    free(m->choices);
    free(m);
}

// Double menu's choices' list size
void doubleSizeMenu(menu *m)
{
    m->len_max *= 2;
    choice **choices = malloc(sizeof(choice *) * m->len_max);
    for (int i = 0; i < m->len_choices; i++)
        choices[i] = m->choices[i];
}

// Add a choice to a menu
void addChoice(choice *c, menu *m)
{
    // Double size of necessary
    if (m->len_choices == m->len_max)
        doubleSizeMenu(m);

    // Add to menu
    m->choices[m->len_choices] = c;
    m->len_choices++;

    // Update choice
    c->parent_menu = m;
}

// Display a menu
void displayMenu(menu *m)
{
    system("clear");
    printf(" -~-~-~-~-~-~- [ %s%s%s ] -~-~-~-~-~-~- \n", KYEL, m->path, KNRM);
    for (int i = 0; i < m->len_choices; i++)
        printf("[%s%d%s] : %s\n", KYEL, i, KNRM, m->choices[i]->phrase);
}

// Ask the user's choice for a menu
int askMenuChoice(int len)
{
    int stop, n;
    int max_choice = len - 1;

    printf("> ");
    while (true)
    {
        stop = scanf("%d", &n);
        flushStdin();
        if ((stop == 0) || (n < 0) || (n > max_choice))
            printf("Please choose correctly\n> ");
        else
            break;
    }
    return n;
}

// Starts the interaction with a menu
menu *interactionMenu(menu *m)
{
    displayMenu(m);
    choice *c = m->choices[askMenuChoice(m->len_choices)];
    c->f(c); // effect
    return c->redirection; // return next destination
}

// Call interactionMenu until there is no more menu 
void navigateInMenu(menu *m)
{
    while (m != NULL)
        m = interactionMenu(m);
}
