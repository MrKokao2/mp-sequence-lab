#pragma once
#include <functional>

template<typename Key, typename Value>
class BinaryTree {
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;

        Node(Key k, Value v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;

    Node* insert(Node* node, Key key, Value value) {
        if (!node)
            return new Node(key, value);

        if (key < node->key)
            node->left = insert(node->left, key, value);
        else
            node->right = insert(node->right, key, value);

        return node;
    }

    Value find(Node* node, Key key) {
        if (!node)
            return -1;

        if (key == node->key)
            return node->value;

        if (key < node->key)
            return find(node->left, key);
        else
            return find(node->right, key);
    }

public:
    void Insert(Key key, Value value) {
        root = insert(root, key, value);
    }

    Value Find(Key key) {
        return find(root, key);
    }
};