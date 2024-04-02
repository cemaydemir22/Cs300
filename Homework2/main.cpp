#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "avl.cpp"
#include<cctype> // yo check whether it constains any punctuation
#include <algorithm> // for std::transform
using namespace std;
// sayı içerenleri at, eğer noktala varsa onu at ama kelimeyi al
// Structure to hold information about word occurrences in files
bool check_word_format(string & word) 
{
    // Check if the word contains any digits
    if (any_of(word.begin(), word.end(), ::isdigit)) {
        return false;
    }

    // Remove punctuation from the word
    word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());

    // Check if the word is empty after removing punctuation or contains only whitespace
    if (word.empty() || all_of(word.begin(), word.end(), ::isspace)){
        return false;
    }

    return true;
}


// Function to preprocess files and build the AVL tree
template<typename Key, typename Value>
void preprocessFiles(AVLTree<Key, Value>& tree, int numFiles) {
    for (int i = 0; i < numFiles; ++i) {
        Key fileName;
        cout << "Enter "<<i+1<<". file name: ";
        cin >> fileName;

        ifstream file(fileName);
        if (!file) {
            continue;
        }

        // Read words from file and insert into tree
        Key word;
        while (file >> word) 
        {
            // Convert word to lowercase
            word = toLowercase(word);

            
            if(check_word_format(word))
            {
                tree.insert(fileName, word, 1);
            }
            
        }

        file.close();
    }
}


int main() 
{
    AVLTree<string, WordInfo> tree;

    int numFiles;
    cout << "Enter number of input files: ";
    cin >> numFiles;

    preprocessFiles(tree, numFiles);
    cin.ignore(); // Clear
    tree.print();
    string query;
    do  {
            cout << "Enter queried words in one line (or type ENDOFINPUT to end): ";
            getline(cin, query);

            if (query != "ENDOFINPUT") {
                vector<string> words;
                istringstream iss(query);
                string word;
                while (iss >> word) {
                    words.push_back(word);
                }

                tree.searchWords(words);
            }
        }while (query != "ENDOFINPUT");

    return 0;
}
