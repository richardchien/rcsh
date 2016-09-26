//
//  CLIMenu.h
//  rcsh
//
//  Created by Richard Chien on 9/22/16.
//  Copyright (c) 2016 Richard Chien. All rights reserved.
//

#ifndef CLIMENU_CLIMENU_H
#define CLIMENU_CLIMENU_H

#include "Callback.h"

typedef struct _CLIMenu CLIMenu;

struct _CLIMenu {
    void *__callbackMap;
    void *data;
    SimpleCallback initializeCallback;
    SimpleCallback finalizeCallback;
    SimpleCallback promptCallback;
    Callback noSuchCommandCallback;

    void (*add)(CLIMenu *self, const char *command, Callback callback);

    void (*remove)(CLIMenu *self, const char *command);

    void (*clear)(CLIMenu *self);

    void (*run)(CLIMenu *self, const char *exitCommand);
};

CLIMenu *newCLIMenu();

void deleteCLIMenu(CLIMenu **pself);

#endif //CLIMENU_CLIMENU_H
