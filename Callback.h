//
//  Callback.h
//  rcsh
//
//  Created by Richard Chien on 9/22/16.
//  Copyright (c) 2016 Richard Chien. All rights reserved.
//

#ifndef CLIMENU_CALLBACK_H
#define CLIMENU_CALLBACK_H

typedef struct _CLIMenu CLIMenu;

typedef int (*Callback)(CLIMenu *menu, int argc, char **argv);

typedef void (*SimpleCallback)(CLIMenu *menu);

#endif //CLIMENU_CALLBACK_H
