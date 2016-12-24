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

TreeRoot *SearchTree_insert(TreeRoot *root, const char *key, Callback callback) {
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
        root->left = SearchTree_insert(root->left, key, callback);
    } else if (compareResult < 0) {
        root->right = SearchTree_insert(root->right, key, callback);
    } else {
        root->callback = callback;
    }
    return root;
}

TreeNode *SearchTree_find(TreeRoot *root, const char *key) {
    if (root) {
        int compareResult = strcmp(root->key, key);
        if (compareResult > 0) {
            return SearchTree_find(root->left, key);
        } else if (compareResult < 0) {
            return SearchTree_find(root->right, key);
        }
    }
    return root;  // Return root itself, or NULL.
}

TreeNode *SearchTree_findMin(TreeRoot *root) {
    if (!root) {
        return NULL;
    } else if (!root->left) {
        return root;
    } else {
        // Keep digging left sub tree.
        return SearchTree_findMin(root->left);
    }
}

TreeRoot *SearchTree_remove(TreeRoot *root, const char *key) {
    if (root) {
        int compareResult = strcmp(root->key, key);
        if (compareResult > 0) {
            root->left = SearchTree_remove(root->left, key);
        } else if (compareResult < 0) {
            root->right = SearchTree_remove(root->right, key);
        } else {
            // Remove current node.
            TreeNode *tmp;
            if (root->left && root->right) {
                // Has two children.
                // Replace current node with the min of right sub tree.
                tmp = SearchTree_findMin(root->right);
                free(root->key);
                root->key = strdup(tmp->key);
                root->callback = tmp->callback;
                root->right = SearchTree_remove(root->right, tmp->key);
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

void SearchTree_clear(TreeRoot *root) {
    if (root) {
        SearchTree_clear(root->left);
        SearchTree_clear(root->right);
        free(root->key);
        free(root);
    }
}

#pragma mark - Map

void CallbackMap_put(CallbackMap *self, const char *key, Callback callback) {
    if (!self) {
        return;
    }
    self->__root = SearchTree_insert(self->__root, key, callback);
}

Callback CallbackMap_get(const CallbackMap *self, const char *key) {
    if (!self) {
        return NULL;
    }
    TreeNode *node = SearchTree_find(self->__root, key);
    return node == NULL ? NULL : node->callback;
}

void CallbackMap_remove(CallbackMap *self, const char *key) {
    if (!self) {
        return;
    }
    self->__root = SearchTree_remove(self->__root, key);
}

void CallbackMap_clear(CallbackMap *self) {
    if (!self) {
        return;
    }
    SearchTree_clear(self->__root);
    self->__root = NULL;
}

CallbackMap *new_CallbackMap() {
    CallbackMap *map = malloc(sizeof(CallbackMap));
    map->__root = NULL;
    map->put = CallbackMap_put;
    map->get = CallbackMap_get;
    map->remove = CallbackMap_remove;
    map->clear = CallbackMap_clear;
    return map;
}

void delete_CallbackMap(CallbackMap **pself) {
    if (!pself) {
        return;
    }

    CallbackMap *self = *pself;
    *pself = NULL;
    if (self) {
        CallbackMap_clear(self);
        free(self);
    }
}
