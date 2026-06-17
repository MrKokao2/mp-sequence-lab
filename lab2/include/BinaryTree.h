#pragma once
#include <algorithm>

// Самобалансирующееся двоичное дерево поиска (AVL).
//
// Наивный BST при вставке отсортированной последовательности ключей
// вырождается в связный список высотой N, и поиск деградирует до O(n).
// AVL после каждой вставки восстанавливает баланс поворотами, поэтому
// высота всегда остаётся O(log n), а Find — O(log n) на любых данных,
// включая уже отсортированные.
template<typename Key, typename Value>
class BinaryTree {
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        int height;

        Node(Key k, Value v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root = nullptr;
    int count = 0;

    int height(Node* node) const {
        return node ? node->height : 0;
    }

    int balanceFactor(Node* node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(Node* node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }

    //      y               x
    //     / \             / \
    //    x   c    -->     a   y
    //   / \                  / \
    //  a   b                b   c
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* b = x->right;

        x->right = y;
        y->left = b;

        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* b = y->left;

        y->left = x;
        x->right = b;

        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* balance(Node* node) {
        updateHeight(node);
        int bf = balanceFactor(node);

        if (bf > 1) {                                // перекос влево
            if (balanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left); // случай Left-Right
            return rotateRight(node);
        }

        if (bf < -1) {                               // перекос вправо
            if (balanceFactor(node->right) > 0)
                node->right = rotateRight(node->right); // случай Right-Left
            return rotateLeft(node);
        }

        return node;
    }

    Node* insert(Node* node, Key key, Value value) {
        if (!node) {
            ++count;
            return new Node(key, value);
        }

        if (key < node->key)
            node->left = insert(node->left, key, value);
        else if (node->key < key)
            node->right = insert(node->right, key, value);
        else
            node->value = value;                     // ключ уже есть — обновляем

        return balance(node);
    }

    Value find(Node* node, Key key) const {
        while (node) {
            if (key < node->key)
                node = node->left;
            else if (node->key < key)
                node = node->right;
            else
                return node->value;
        }
        return -1;                                   // не найдено
    }

    void destroy(Node* node) {
        if (!node)
            return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BinaryTree() = default;
    ~BinaryTree() { destroy(root); }

    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;

    void Insert(Key key, Value value) {
        root = insert(root, key, value);
    }

    Value Find(Key key) const {
        return find(root, key);
    }

    // Текущая высота дерева — для проверки балансировки в тестах.
    int Height() const { return height(root); }

    // Количество узлов.
    int Count() const { return count; }
};
