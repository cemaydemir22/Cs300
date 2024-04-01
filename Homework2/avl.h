#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct DocumentItem
{
    string documentName;
    int count;
};

struct WordItem
{
    vector<DocumentItem> mylist;
    string word;
};

template <typename Key, typename Value>
class AVLSearchTree
{
private:
    struct Node
    {
        Value word_item;
        Node* left;
        Node* right;
        int height;
        int bf; // Balance factor
    };

    Node* root;

    // Private helper functions
    Node* insert(Node* node, const Key& key, const Value& value);
    Node* remove(Node* node, const Key& key);
    Node* findMin(Node* node);
    Node* findMax(Node* node);
    int height(Node* node);
    int balanceFactor(Node* node);
    void updateHeight(Node* node);
    Node* balance(Node* node);
    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);
    void inorderTraversal(Node* node);
    void clear(Node*);

public:
    AVLSearchTree() : root(nullptr) {}
    ~AVLSearchTree();

    void insert(const Key& key, const Value& value);
    void remove(const Key& key);
    void inorderTraversal();

};
