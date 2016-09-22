//
// Created by Richard Chien on 9/22/16.
//

#include "CLIMenu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CallbackMap.h"

void menuAdd(CLIMenu *self, const char *command, Callback callback);

void menuRemove(CLIMenu *self, const char *command);

void menuClear(CLIMenu *self);

void menuRun(CLIMenu *self, const char *exitCommand);

int defaultNoSuchCommandCallback(int argc, char **argv) {
    printf("command not found: %s\n", argv[0]);
    return 0;
}

CLIMenu *newCLIMenu(const char *name) {
    CLIMenu *menu = malloc(sizeof(CLIMenu));
    menu->__callbackMap = newCallbackMap();
    menu->noSuchCommandCallback = defaultNoSuchCommandCallback;
    menu->name = strdup(name);
    menu->add = menuAdd;
    menu->remove = menuRemove;
    menu->clear = menuClear;
    menu->run = menuRun;
    return menu;
}

void deleteCLIMenu(CLIMenu **pself) {
    if (!pself) {
        return;
    }

    CLIMenu *self = *pself;
    *pself = NULL;
    if (self) {
        deleteCallbackMap((CallbackMap **) &self->__callbackMap);
        free(self->name);
        free(self);
    }
}

void menuAdd(CLIMenu *self, const char *command, Callback callback) {
    if (!self) {
        return;
    }
    CallbackMap *map = self->__callbackMap;
    map->put(map, command, callback);
}

void menuRemove(CLIMenu *self, const char *command) {
    if (!self) {
        return;
    }
    CallbackMap *map = self->__callbackMap;
    map->remove(map, command);
}

void menuClear(CLIMenu *self) {
    if (!self) {
        return;
    }
    CallbackMap *map = self->__callbackMap;
    map->clear(map);
}

typedef struct {
    int argc;
    char **argv;
} Command;

char *strAppendChar(char *str, int *len, int *maxLen, int allocStep, char ch) {
    while (*len >= *maxLen) {
        str = realloc(str, sizeof(char) + allocStep);
        if (str) {
            *maxLen += allocStep;
        } else {
            exit(1);
        }
    }
    str[(*len)++] = ch;
    str[*len] = '\0';
    return str;
}

Command readCommand() {
    Command command;
    command.argc = 0;
    command.argv = NULL;
    char leftQuoteMark = '\0';
    int backslash = 0;
    int argStart = 1;
    int argLen = 0;
    int argMaxLen = 0;
    char *argStr = NULL;
    for (char ch; (ch = (char) getchar()) != '\n' && ch != EOF;) {
        if (leftQuoteMark == '\0' && (ch == ' ' || ch == '\t')) {
            // Skip blank characters not in quote marks.
            argStart = 1;
            continue;
        }

        if (argStart) {
            command.argc++;
            command.argv = realloc(command.argv, sizeof(char *) * command.argc);
            argMaxLen = 20;
            argLen = 0;
            argStr = command.argv[command.argc - 1] = malloc(sizeof(char) * (argMaxLen + 1));
            argStart = 0;
        }

        if (backslash) {
            strAppendChar(argStr, &argLen, &argMaxLen, 20, ch);
            backslash = 0;
        } else {
            switch (ch) {
                case '\\':
                    backslash = 1;
                    break;
                case '\"':
                case '\'':
                    if (leftQuoteMark == ch) {
                        leftQuoteMark = '\0';
                    } else if (leftQuoteMark == '\0') {
                        leftQuoteMark = ch;
                    } else {
                        strAppendChar(argStr, &argLen, &argMaxLen, 20, ch);
                    }
                    break;
                default:
                    strAppendChar(argStr, &argLen, &argMaxLen, 20, ch);
                    break;
            }
        }
    }
    return command;
}

void freeCommand(Command command) {
    for (int i = 0; i < command.argc; i++) {
        free(command.argv[i]);
    }
    free(command.argv);
}

void menuRun(CLIMenu *self, const char *exitCommand) {
    CallbackMap *map = self->__callbackMap;
    for (;;) {
        printf("%s> ", self->name);
        Command command = readCommand();
        if (command.argc > 0) {
            if (strcmp(exitCommand, command.argv[0]) == 0) {
                freeCommand(command);
                break;
            } else {
                Callback callback = map->get(map, command.argv[0]);
                if (callback) {
                    callback(command.argc, command.argv);
                } else {
                    if (self->noSuchCommandCallback) {
                        self->noSuchCommandCallback(command.argc, command.argv);
                    }
                }
                freeCommand(command);
            }
        }
    }
}
