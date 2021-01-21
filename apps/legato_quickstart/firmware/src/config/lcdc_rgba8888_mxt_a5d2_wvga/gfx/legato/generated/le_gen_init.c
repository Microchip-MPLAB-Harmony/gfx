#include "gfx/legato/generated/le_gen_init.h"

static uint32_t currentScreen;

void legato_initialize(void)
{
    leSetStringTable(&stringTable);

    initializeStrings();

    screenInit_Screen0();

    currentScreen = -1;

    legato_showScreen(screenID_Screen0);
}

uint32_t legato_getCurrentScreen(void)
{
    return currentScreen;
}

void legato_hideCurrentScreen()
{
    switch(currentScreen)
    {
        case screenID_Screen0:
        {
            screenHide_Screen0();
            currentScreen = 0;
            break;
        }
    }
}

void legato_showScreen(uint32_t id)
{
    legato_hideCurrentScreen(currentScreen);

    switch(id)
    {
        case screenID_Screen0:
        {
            screenShow_Screen0();
            currentScreen = id;
            break;
        }
    }
}

void legato_updateCurrentScreen(void)
{
    switch(currentScreen)
    {
        case screenID_Screen0:
        {
            screenUpdate_Screen0();
            break;
        }
    }
}

