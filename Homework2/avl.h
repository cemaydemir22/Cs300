#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm> // for std::transform
using namespace std;

struct WordInfo 
{
    string word;
    map<string, int> fileOccurrences; // Map stores filenames and frequencies
};


// AVL Node structure
template<typename Key, typename Value>
class AVLTree 
{
private:

    struct AVLNode 
    {
        Value info;
        AVLNode* left;
        AVLNode* right;
        int height;
        AVLNode(const Key& fileName, const Key& word, int frequency, AVLNode* lt = nullptr, AVLNode* rt = nullptr, int h = 1) : left(lt), right(rt), height(h) {
            info.fileOccurrences[fileName] = frequency;
            info.word = word;
        }
    };
    AVLNode* root;

    AVLNode* rotateRight(AVLNode* node); // Right rotation
    AVLNode* rotateLeft(AVLNode* node); // Left rotation
    int getHeight(AVLNode* node); // return height of a node
    int balance_Factor(AVLNode* node); // returns bf=L_height-R_height
    void makeEmpty(AVLNode*& t); // Deletes the tree
    AVLNode* insert(const Key& fileName, const Key& word, int frequency, AVLNode* t);
    AVLNode* remove(const Key& word, AVLNode* t);
    void print(AVLNode * root);
    AVLNode * Balance(AVLNode* node,const Key& word); // Balances the tree
    AVLNode * minvaluenode(AVLNode * node); // returns the node minimum key

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { makeEmpty(root); }
    
    void remove(const Key& word);
    AVLNode* getRoot() const { return root; } // Function to access the root
    void insert(const Key& fileName, const Key& word, int frequency); // Public insert function
    void searchWords(const vector<Key>& words);
    bool Find(const Key& word, AVLNode* t, map<string, int>& occurrences);
    void print();
};
