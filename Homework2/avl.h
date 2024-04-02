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
    map<string, int> fileOccurrences; // Map to store filename -> frequency pairs
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

    AVLNode* rotateRight(AVLNode* node);
    AVLNode* rotateLeft(AVLNode* node);
    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);

    void makeEmpty(AVLNode*& t);
    AVLNode* insert(const Key& fileName, const Key& word, int frequency, AVLNode* t);
    AVLNode* remove(const Key& word, AVLNode* t);
    void print(AVLNode * root);


public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { makeEmpty(root); }
    
    void remove(const Key& word);
    AVLNode* getRoot() const { return root; } // Function to access the root
    void insert(const Key& fileName, const Key& word, int frequency); // Public insert function
    void searchWords(const vector<Key>& words);
    bool search(const Key& word, AVLNode* t, map<string, int>& occurrences);
    void print();
};
