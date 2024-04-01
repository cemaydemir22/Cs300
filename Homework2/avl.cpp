#include "avl.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
// Destructor
template <typename Key, typename Value>
AVLSearchTree<Key, Value>::~AVLSearchTree() {
    clear(root);
}

// Helper method to recursively clear the tree
template <typename Key, typename Value>
void AVLSearchTree<Key, Value>::clear(Node* n) {
    if (n != nullptr) {
        clear(n->left);
        clear(n->right);
        delete n;
    }
}

// Insertion
template <typename Key, typename Value>
void AVLSearchTree<Key, Value>::insert(const Key& key, const Value& value)
{
    root = insert(root, key, value);
}

template <typename Key, typename Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::insert(Node* node, const Key& key, const Value& value)
{
    if (node == nullptr)
    {
        Node* newNode = new Node;
        newNode->word_item = value;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        newNode->bf = 0;
        return newNode;
    }

    if (key < node->word_item.word)
    {
        node->left = insert(node->left, key, value);
    }
    else if (key > node->word_item.word)
    {
        node->right = insert(node->right, key, value);
    }
    else
    {
        node->word_item = value;
    }

    return balance(node);
}

// Removal
template <typename Key, typename Value>
void AVLSearchTree<Key, Value>::remove(const Key& key)
{
    root = remove(root, key);
}

template <typename Key, typename Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::remove(Node* node, const Key& key)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (key < node->word_item.word)
    {
        node->left = remove(node->left, key);
    }
    else if (key > node->word_item.word)
    {
        node->right = remove(node->right, key);
    }
    else
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            return nullptr;
        }
        else if (node->left == nullptr)
        {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == nullptr)
        {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        else
        {
            Node* minRight = findMin(node->right);
            node->word_item = minRight->word_item;
            node->right = remove(node->right, minRight->word_item.word);
        }
    }

    return balance(node);
}

template <typename Key, typename Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::findMin(Node* node)
{
    while (node->left != nullptr)
    {
        node = node->left;
    }
    return node;
}

template <typename Key, typename Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::findMax(Node* node)
{
    while (node->right != nullptr)
    {
        node = node->right;
    }
    return node;
}

// Height and Balance Factor
template <typename Key, typename Value>
int AVLSearchTree<Key, Value>::height(Node* node)
{
    return (node == nullptr) ? 0 : node->height;
}

template <typename Key, typename Value>
int AVLSearchTree<Key, Value>::balanceFactor(Node* node)
{
    return (node == nullptr) ? 0 : height(node->right) - height(node->left);
}

// Update Height
template <typename Key, typename Value>
void AVLSearchTree<Key, Value>::updateHeight(Node* node)
{
    node->height = 1 + max(height(node->left), height(node->right));
}

// Balance
template <typename Key, typename Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::balance(Node* node)
{
    updateHeight(node);
    int bf = balanceFactor(node);

    if (bf == 2)
    {
        if (balanceFactor(node->right) < 0)
        {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    else if (bf == -2)
    {
        if (balanceFactor(node->left) > 0)
        {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    return node;
}

// Rotations
template <typename Key, typename Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::rotateLeft(Node* node)
{
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

template <typename Key, typename Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::rotateRight(Node* node)
{
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    updateHeight(node);
    updateHeight(newRoot);
    return newRoot;
}

// Traversal
template <typename Key, typename Value>
void AVLSearchTree<Key, Value>::inorderTraversal()
{
    inorderTraversal(root);
}

template <typename Key, typename Value>
void AVLSearchTree<Key, Value>::inorderTraversal(Node* node)
{
    if (node == nullptr)
    {
        return;
    }
    inorderTraversal(node->left);
    cout << node->word_item.word << " ";
    inorderTraversal(node->right);
}