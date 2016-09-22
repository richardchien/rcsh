//
//  CallbackMap.c
//  rcsh
//
//  Created by Richard Chien on 9/22/16.
//  Copyright (c) 2016 Richard Chien. All rights reserved.
//

#include "CallbackMap.h"

#include <stdlib.h>
#include <string.h>

#pragma mark - Search Tree

typedef struct _TreeNode TreeNode;
typedef struct _TreeNode TreeRoot;

struct _TreeNode {
    char *key;
    Callback callback;
    TreeNode *left;
    TreeNode *right;
};

TreeRoot *treeInsert(TreeRoot *root, const char *key, Callback callback) {
    if (!root) {
        // First insertion, create a node as root.
        TreeNode *node = malloc(sizeof(TreeNode));
        node->key = strdup(key);
        node->callback = callback;
        node->left = node->right = NULL;
        return node;
    }

    int compareResult = strcmp(root->key, key);
    if (compareResult > 0) {
        root->left = treeInsert(root->left, key, callback);
    } else if (compareResult < 0) {
        root->right = treeInsert(root->right, key, callback);
    } else {
        root->callback = callback;
    }
    return root;
}

TreeNode *treeFind(TreeRoot *root, const char *key) {
    if (root) {
        int compareResult = strcmp(root->key, key);
        if (compareResult > 0) {
            return treeFind(root->left, key);
        } else if (compareResult < 0) {
            return treeFind(root->right, key);
        }
    }
    return root;  // Return root itself, or NULL.
}

TreeNode *treeFindMin(TreeRoot *root) {
    if (!root) {
        return NULL;
    } else if (!root->left) {
        return root;
    } else {
        // Keep digging left sub tree.
        return treeFindMin(root->left);
    }
}

TreeRoot *treeRemove(TreeRoot *root, const char *key) {
    if (root) {
        int compareResult = strcmp(root->key, key);
        if (compareResult > 0) {
            root->left = treeRemove(root->left, key);
        } else if (compareResult < 0) {
            root->right = treeRemove(root->right, key);
        } else {
            // Remove current node.
            TreeNode *tmp;
            if (root->left && root->right) {
                // Has two children.
                // Replace current node with the min of right sub tree.
                tmp = treeFindMin(root->right);
                free(root->key);
                root->key = strdup(tmp->key);
                root->callback = tmp->callback;
                root->right = treeRemove(root->right, tmp->key);
            } else {
                // Has one child.
                tmp = root;
                if (root->right) {
                    root = root->right;
                } else {
                    root = root->left;
                }
                free(tmp->key);
                free(tmp);
            }
        }
    }
    return root;
}

void treeClear(TreeRoot *root) {
    if (root) {
        treeClear(root->left);
        treeClear(root->right);
        free(root->key);
        free(root);
    }
}

#pragma mark - Map

void mapPut(CallbackMap *self, const char *key, Callback callback) {
    if (!self) {
        return;
    }
    self->__root = treeInsert(self->__root, key, callback);
}

Callback mapGet(const CallbackMap *self, const char *key) {
    if (!self) {
        return NULL;
    }
    TreeNode *node = treeFind(self->__root, key);
    return node == NULL ? NULL : node->callback;
}

void mapRemove(CallbackMap *self, const char *key) {
    if (!self) {
        return;
    }
    self->__root = treeRemove(self->__root, key);
}

void mapClear(CallbackMap *self) {
    if (!self) {
        return;
    }
    treeClear(self->__root);
    self->__root = NULL;
}

CallbackMap *newCallbackMap() {
    CallbackMap *map = malloc(sizeof(CallbackMap));
    map->__root = NULL;
    map->put = mapPut;
    map->get = mapGet;
    map->remove = mapRemove;
    map->clear = mapClear;
    return map;
}

void deleteCallbackMap(CallbackMap **pself) {
    if (!pself) {
        return;
    }

    CallbackMap *self = *pself;
    *pself = NULL;
    if (self) {
        mapClear(self);
        free(self);
    }
}
