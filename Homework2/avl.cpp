#include <iostream>
#include "avl.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm> // for std::transform
using namespace std;
// Destructor
// Function to insert word occurrences into the AVL tree


// Utility function to convert string to lowercase
string toLowercase(string str) 
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}
template<typename Key, typename Value>
typename AVLTree<Key,Value>::AVLNode* AVLTree<Key,Value>::minvaluenode(AVLNode * node)
{
    AVLNode * current=root;
    while(current && current->left){
        current=current->left;
    }
    return current;
}


// To balance the tree with 4 possible rotations
template<typename Key, typename Value>
typename AVLTree<Key,Value>::AVLNode* AVLTree<Key,Value>::Balance(AVLNode * t,const Key& word)
{
   
    // Update height
    t->height = max(getHeight(t->left), getHeight(t->right)) + 1;

    // Find Balance factor
    int balance = balance_Factor(t);

    // Left Left 
    if (balance > 1 && balance_Factor(t->left)>=0){
        return rotateRight(t);
    }
    // Left Right 
    if (balance > 1 && balance_Factor(t->left)<0) {
        t->left = rotateLeft(t->left);
        return rotateRight(t);
    }
    // Right Right 
    if (balance < -1 && balance_Factor(t->right)<=0){
        return rotateLeft(t);
    }
    // Right Left 
    if (balance < -1 && balance_Factor(t->right)>0) {
        t->right = rotateRight(t->right);
        return rotateLeft(t);
    }

    return t;

}


template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const Key& fileName, const Key& word, int frequency) 
{
    root = insert(fileName, word, frequency, root);
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::print()
{
    print(root);
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::print(AVLNode * t)
{
  if(t->left)
  {
     print(t->left);
  }
  cout<<t->info.word<<" ";
  if(t->right){
    print(t->right);
  }   
}

// To insert a node
template<typename Key, typename Value>
typename AVLTree<Key, Value>::AVLNode* AVLTree<Key, Value>::insert(const Key& fileName, const Key& word, int frequency, AVLNode* node) 
{
  
    if(!node){
        return new AVLNode(fileName,word,frequency);
    }
    else if(toLowercase(word) < toLowercase(node->info.word)){
        node->left=insert(fileName, word, frequency, node->left);
    }
    else if(toLowercase(node->info.word) < toLowercase(word)){
        node->right=insert(fileName, word, frequency, node->right);
    }
    else{
        node->info.fileOccurrences[fileName]+=frequency; // Increment frequency if word already exists else add and increment
        return node;
    }

    return Balance(node,word);
}

// Function to remove a word from the AVL tree
template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& word) {
    root = remove(word, root);
}

// Function to remove
template<typename Key, typename Value>
typename AVLTree<Key, Value>::AVLNode* AVLTree<Key, Value>::remove(const Key& word, AVLNode* node) {
    map<string, int> occurrences;
   if(!Find(toLowercase(word),node, occurrences)){// either there is no node or the number is not in the tree
      return node;
   }
   else{
     if(toLowercase(word)<node->info.word){
        node->left=remove(toLowercase(word),node->left);
     }
     else if(toLowercase(word)>node->info.word){
        node->right=remove(toLowercase(word),node->right);
     }
     else{
        //equal case
        // the cases are node has no children or node has one children or node has 2 children
        if(node->left==nullptr && node->right==nullptr){// no child
          delete node;
          node=nullptr;
          return node;
        }
        else if(node->left && !node->right){// only left child 
           AVLNode * temp;
           temp=node;
           node=node->left;
           delete node;
           return node;
        }
        else if(!node->left && node->right){// only right child 
           AVLNode * temp;
           temp=node;
           node=node->right;
           delete temp;
           return node;
        }
        else{// 2 child
          
           AVLNode * temp;
           temp=minvaluenode(node->right);
           node->info.word=temp->info.word;
           node->right=remove(temp->info.word,node->right);


        }
     }
   }

    if (node == nullptr){
        return node;
    }
    return Balance(node,toLowercase(word)); // call the function to balance the tree if necessary
}



// Function to perform a right rotation
template<typename Key, typename Value>
typename AVLTree<Key, Value>::AVLNode* AVLTree<Key, Value>::rotateRight(AVLNode* node) 
{
    AVLNode* new_node = node->left;
    node->left=new_node->right;
    new_node->right=node;
    // Update heigts (Only node and leftchild)
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    new_node->height = max(getHeight(new_node->left), getHeight(new_node->right)) + 1;

    return new_node;
}

// Function to perform a left rotation
template<typename Key, typename Value>
typename AVLTree<Key, Value>::AVLNode* AVLTree<Key, Value>::rotateLeft(AVLNode* node) 
{
    AVLNode* new_node = node->right;
    node->right=new_node->left;
    new_node->left=node;

    // Update heigts (Only node and rightchild)
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    new_node->height = max(getHeight(new_node->left), getHeight(new_node->right)) + 1;

    return new_node;
}

// Function to get the height of a node
template<typename Key, typename Value>
int AVLTree<Key, Value>::getHeight(AVLNode* node) 
{
    if(!node){
        return 0;
    }
    else{
        return node->height;
    }
}

// To find balance factor if >1 then left heavy if <-1 then right heavy
template<typename Key, typename Value>
int AVLTree<Key, Value>::balance_Factor(AVLNode* node) 
{
    if(!node)
    {
        return 0;
    }
    else
    {
        return getHeight(node->left)-getHeight(node->right);
    }
}

// Corrected searchWords function
template<typename Key, typename Value>
void AVLTree<Key, Value>::searchWords(const vector<Key>& words) 
{
    map<string, map<string, vector<string>>> occurrencesByFile; // Map to store occurrences by file name
    vector<string> uniqueWords; // Vector to store unique words encountered in the query
    bool found = false; // Flag to track if any word in the query is found
    bool removeRequested = false; // Flag to track if a removal is requested

    // Iterate over each word in the query
    for (size_t i = 0; i < words.size(); ++i) {
        if (toLowercase(words[i]) == "remove" && i + 1 < words.size()) 
        { // REMOVE or remove
            map<string, int> occurrences; 
            if(Find(toLowercase(words[i+1]), root, occurrences)){
               cout<<toLowercase(words[i+1])<<" has been REMOVED"<<endl;
            }
            remove(words[i + 1]); // Remove the specified word without converting to lowercase
            removeRequested = true; // Set the flag to true
            ++i; // Skip the next word
            continue;
        }

        // Check if the word is already encountered
        if (find(uniqueWords.begin(), uniqueWords.end(), words[i]) == uniqueWords.end()) {
            uniqueWords.push_back(words[i]); // If not encountered before, add it to the uniqueWords vector
        }

        map<string, int> occurrences;

        // Search for the word in the tree and get its occurrences in each file
        if (Find(toLowercase(words[i]), root, occurrences)) {
            found = true; // Set the flag to true if any word is found
            // Update occurrencesByFile with the word occurrences for each file
            for (const auto& pair : occurrences) {
                occurrencesByFile[pair.first][words[i]].push_back(to_string(pair.second) + " times");
            }
        }
    }

    // If no word in the query is found, print the message
    if (!found && !removeRequested) {
        cout << "No document contains the given query" << endl;
    }

    // Output occurrences grouped by file name
    for (const auto& filePair : occurrencesByFile) 
    {
        cout << "in Document " << filePair.first << ", ";
        bool firstWord = true; // Flag to track if it's the first word in the output
        for (const auto& word : uniqueWords) 
        {
            if (filePair.second.find(word) != filePair.second.end()) 
            {
                if (!firstWord) {
                    cout << ", "; // Add comma if it's not the first word
                }
                cout << toLowercase(word) << " found ";
                for (size_t i = 0; i < filePair.second.at(word).size(); ++i) 
                {
                    cout << filePair.second.at(word)[i];
                    if (i != filePair.second.at(word).size() - 1) 
                    {
                        cout << ", ";
                    }
                }
                firstWord = false;
            }
        }
        cout << endl;
    }
}



// Helper function to find for a word in the AVL tree
template<typename Key, typename Value>
bool AVLTree<Key, Value>::Find(const Key& word, AVLNode* node, map<string, int>& occurrences) 
{
    if (!node) {
        return false;
    }
    else{

        if (word < toLowercase(node->info.word))
        {
            return Find(word, node->left, occurrences);
        }
        else if (toLowercase(node->info.word) < word)
        {
            return Find(word, node->right, occurrences);
        }
        else 
        {
            occurrences = node->info.fileOccurrences;
            return true;
        }
    }
}

// Utility function to deallocate memory of the AVL tree
template<typename Key, typename Value>
void AVLTree<Key, Value>::makeEmpty(AVLNode*& t) 
{
    if (t != nullptr) 
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}

