#pragma once

struct Node {
    Node(int data) : data(data), height(1), left(nullptr), right(nullptr) {}

    int data;
    int height;
    Node *left;
    Node *right;
};
