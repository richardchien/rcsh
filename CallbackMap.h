//
//  CallbackMap.h
//  rcsh
//
//  Created by Richard Chien on 9/22/16.
//  Copyright (c) 2016 Richard Chien. All rights reserved.
//

#ifndef CLIMENU_CALLBACK_MAP_H
#define CLIMENU_CALLBACK_MAP_H

#include "Callback.h"

typedef struct _CallbackMap CallbackMap;

struct _CallbackMap {
    void *__root;

    void (*put)(CallbackMap *self, const char *key, Callback callback);

    Callback (*get)(const CallbackMap *self, const char *key);

    void (*remove)(CallbackMap *self, const char *key);

    void (*clear)(CallbackMap *self);
};

CallbackMap *newCallbackMap();

void deleteCallbackMap(CallbackMap **pself);

#endif //CLIMENU_CALLBACK_MAP_H
