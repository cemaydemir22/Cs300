#include <vector>
#include <string>
#include <iostream>
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
 
 class node
 {
    Key myword;
    node *left;
    node * right;

 };
 node *root;
   



};
