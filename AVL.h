#pragma once
#include <iostream>
#include <queue>
#include "AVLInterface.h"

using namespace std;

class AVL : public AVLInterface {
private:
    Node* root;
    int size_tree = 0;

    bool _contains(Node* node, int item) const {
        if (node == nullptr) {
            return false;
        }
        if (node->data == item) {
            return true;
        }
        if (node->data > item) {
            return _contains(node->left, item);
        }
        else {
            return _contains(node->right, item);
        }
    }

    bool _insert(Node*& node, int item){
        if (node == nullptr) {
            node = new Node(item);
            return true;
        }
        else if (item == node->data) {
            return false;
        }
        else if (item < node->data) {
            bool isAdded = _insert(node->left, item);
            if (isAdded) {
                rebalance(node);
            }
            node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
            return isAdded;
        }
        else {
            bool isAdded = _insert(node->right, item);
            if (isAdded) {
                rebalance(node);
            }
            node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
            return isAdded;
        }
    }

    Node* getInorderPredecessor(Node*& node) {
        Node* iop = node->left;
        while (iop->right != nullptr) {
            iop = iop->right;
        }
        return iop;
    }

    bool _remove(Node*& node, int item) {
        if (node == nullptr) {
            return false;
        }
        if (item == node->data) {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            }
            else if (node->left == nullptr && node->right != nullptr) {
                Node* tmp = node->right;
                delete node;
                node = tmp;
            }
            else if (node->right == nullptr && node->left != nullptr) {
                Node* tmp = node->left;
                delete node;
                node = tmp;
            }
            else {
                Node* iop = getInorderPredecessor(node);
                node->data = iop->data;
                _remove(node->left, iop->data);
            }
            rebalance(node);
            if (node != nullptr) {
                node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
            }
            return true;
        }
        else if (item < node->data) {
            bool isRemoved = _remove(node->left, item);
            if (isRemoved) {
                rebalance(node);
                node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
            }
            return isRemoved;
        }
        else {
            bool isRemoved = _remove(node->right, item);
            if (isRemoved) {
                rebalance(node);
                node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
            }
            return isRemoved;
        }
    }

    int getBalanceFactor(Node*& node) {
        if (node == nullptr) {
            return 0;
        }
        return getHeight(node->right) - getHeight(node->left);
    }

    void rotateRight(Node*& node) {
        Node* tmp = node;
        node = node->left;
        tmp->left = node->right;
        node->right = tmp;
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
        tmp->height = max(getHeight(tmp->left), getHeight(tmp->right)) + 1;
        if (tmp->left != nullptr) {
            tmp->left->height = max(getHeight(tmp->left->left), getHeight(tmp->left->right)) + 1;
        }
        if (tmp == root) {
            root = node;
        }
    }

    void rotateLeft(Node*& node) {
        Node* tmp = node;
        node = node->right;
        tmp->right = node->left;
        node->left = tmp;
        tmp->height = max(getHeight(tmp->left), getHeight(tmp->right)) + 1;
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
        if (tmp->right != nullptr) {
            tmp->right->height = max(getHeight(tmp->right->left), getHeight(tmp->right->right)) + 1;
        }
        if (tmp == root) {
            root = node;
        }
    }

    void rebalance(Node*& node) {
        int balance = getBalanceFactor(node);
        if (balance < -1) {
            if (getBalanceFactor(node->left) <= 0) {
                rotateRight(node);
            }
            else if (getBalanceFactor(node->left) > 0) {
                rotateLeft(node->left);
                rotateRight(node);
            }
        }
        else if (balance > 1) {
            if (getBalanceFactor(node->right) >= 0) {
                rotateLeft(node);
            }
            else if (getBalanceFactor(node->right) < 0) {
                rotateRight(node->right);
                rotateLeft(node);
            }
        }
        else {
            return;
        }
    }

    int getHeight(Node*& node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    void _clear(Node*& node) {
        if (node == nullptr) {
            return;
        }
        _clear(node->left);
        _clear(node->right);
        delete node;
        node = nullptr;
    }

public:
    AVL() {
        root = nullptr;
    }
    ~AVL() {
        clear();
    }

    Node* getRootNode() const {
        return root;
    }

    bool insert(int item) {
        if (_insert(root, item)) {
            root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
            size_tree++;
            return true;
        }
        return false;
    }

    bool contains(int item) const {
        if (_contains(root, item)) {
            return true;
        }
        return false;
    }

    bool remove(int item) {
        if (_remove(root, item)) {
            if (root != nullptr) {
                root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
            }
            size_tree--;
            return true;
        }
        return false;
    }

    void clear() {
        _clear(root);
        size_tree = 0;
    }

    int size() const {
        return size_tree;
    }

};