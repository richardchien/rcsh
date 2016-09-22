//
//  main.c
//  rcsh
//
//  Created by Richard Chien on 9/22/16.
//  Copyright (c) 2016 Richard Chien. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "CLIMenu.h"

int hello(int argc, char **argv) {
    printf("Hello! Welcome to rcsh!\n");
    return 0;
}

int echo(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (i > 1) {
            putchar(' ');
        }
        printf("%s", argv[i]);
    }
    putchar('\n');
    return 0;
}

char *currDir;

char *concatPath(const char *old, const char *new) {
    char *tmp;
    if (new[0] == '/') {
        // Absolute path, just overwrite it.
        tmp = strdup(new);
    } else {
        char *curr = strdup(old);
        char *path = strdup(new);
        int currLen = (int) strlen(curr);
        int pathLen = (int) strlen(path);
        tmp = malloc(sizeof(char) * (currLen + pathLen + 1));
        strcpy(tmp, curr);
        int index = currLen;
        char *part = strtok(path, "/");
        while (part != NULL) {
            if (strcmp(".", part) == 0) {
                // .
            } else if (strcmp("..", part) == 0) {
                // ..
                for (; index > 0 && tmp[index] != '/'; index--);
                tmp[index] = '\0';
            } else {
                tmp[index++] = '/';
                tmp[index] = '\0';
                index += strlen(part);
                strcat(tmp, part);
            }
            part = strtok(NULL, "/");
        }
        if (index == 0) {
            tmp[index++] = '/';
        }
        tmp[index] = '\0';
        tmp = realloc(tmp, sizeof(char) * (strlen(tmp) + 1));
        free(curr);
        free(part);
    }
    return tmp;
}

int cd(int argc, char **argv) {
    if (argc == 1) {
        if (currDir) {
            free(currDir);
        }
        currDir = strdup(getenv("HOME"));
    } else if (argc == 2) {
        char *tmp = concatPath(currDir, argv[1]);
        DIR *dir;
        if ((dir = opendir(tmp)) != NULL) {
            // Directory exists.
            closedir(dir);
            if (currDir) {
                free(currDir);
            }
            currDir = tmp;
        } else {
            // Failed to open directory.
            printf("cd: %s: no such directory or permission denied\n", argv[1]);
        }
    } else {
        printf("cd: too many arguments\n");
    }
    return 0;
}

int pwd(int argc, char **argv) {
    if (argc == 1) {
        if (currDir) {
            printf("%s", currDir);
            putchar('\n');
        }
    } else {
        printf("pwd: too many arguments\n");
    }
    return 0;
}

int ls(int argc, char **argv) {
    if (argc <= 2) {
        char *path;
        if (argc == 1) {
            path = currDir;
        } else {
            path = concatPath(currDir, argv[1]);
        }
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(path)) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (ent->d_name[0] != '.') {
                    printf("%s\n", ent->d_name);
                }
            }
            closedir(dir);
        } else {
            // Failed to open directory.
            if (argc == 1) {
                printf("ls: unknown error\n");
            } else if (argc == 2) {
                printf("ls: %s: no such directory or permission denied\n", argv[1]);
            }
        }
        if (argc == 2) {
            // Free memory.
            free(path);
        }
    } else {
        printf("ls: too many arguments\n");
    }
    return 0;
}

int exec(int argc, char **argv) {
    int cmdLen = argc - 1 - 1;  // number of space
    for (int i = 1; i < argc; i++) {
        cmdLen += strlen(argv[i]);
    }
    char *cmd = malloc(sizeof(char) * (cmdLen + 1));
    cmd[0] = '\0';
    for (int i = 1; i < argc; i++) {
        if (i > 1) {
            strcat(cmd, " ");
        }
        strcat(cmd, argv[i]);
    }
    int code = system(cmd);
    free(cmd);
    return code;
}

int noSuchCommand(int argc, char **argv) {
    printf("rcsh: %s: no such command, using system shell\n", argv[0]);
    int cmdLen = argc - 1;  // number of space
    for (int i = 0; i < argc; i++) {
        cmdLen += strlen(argv[i]);
    }
    char *cmd = malloc(sizeof(char) * (cmdLen + 1));
    cmd[0] = '\0';
    for (int i = 0; i < argc; i++) {
        if (i > 0) {
            strcat(cmd, " ");
        }
        strcat(cmd, argv[i]);
    }
    int code = system(cmd);
    free(cmd);
    return code;
}

int help(int argc, char **argv) {
    printf("Commands may be abbreviated. Commands are:\n"
                   "help\t\tprint help information\n"
                   "hello\t\tprint hello message\n"
                   "echo\t\techo a string\n"
                   "cd\t\tenter a directory\n"
                   "pwd\t\tprint current directory\n"
                   "ls\t\tlist items in current directory\n"
                   "exec\t\trun command in system shell\n"
                   "exit\t\texit the shell\n");
    return 0;
}

int main(int argc, char **argv) {
    currDir = strdup(getenv("HOME"));
    CLIMenu *menu = newCLIMenu("rcsh");
    menu->add(menu, "hello", hello);
    menu->add(menu, "echo", echo);
    menu->add(menu, "cd", cd);
    menu->add(menu, "pwd", pwd);
    menu->add(menu, "ls", ls);
    menu->add(menu, "exec", exec);
    menu->noSuchCommandCallback = noSuchCommand;
    menu->add(menu, "help", help);
    menu->run(menu, "exit");
    deleteCLIMenu(&menu);
    return 0;
}
