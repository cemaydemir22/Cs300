#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "avl.cpp"
#include<cctype> 
#include <algorithm> 
using namespace std;


// If there is a punctuation removes and if there is a number remove it with word
bool check_word_format(std::string& word) {
    // Check if the word contains any digits
    if (std::any_of(word.begin(), word.end(), [](char c) { return std::isdigit(static_cast<unsigned char>(c)); })) {
        return false;
    }

    
    word.erase(std::remove_if(word.begin(), word.end(), [](char c) { return !std::isalpha(static_cast<unsigned char>(c)); }), word.end());

    // Check if the word is empty 
    if (word.empty() || std::all_of(word.begin(), word.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)); })) {
        return false;
    }

    return true;
}

template<typename Key, typename Value>
void searchWords(const vector<Key>& words, AVLTree<Key,Value> &tree) {
    map<string, map<string, vector<string>>> occurrencesByFile; // Stores words with filename
    vector<string> uniqueWords; // Stores unique words
    bool entered = true;

    for (int i = 0; i < words.size(); ++i) {
        AVLNode<Key,Value>* root = tree.getRoot();
        if (toLowercase(words[i]) == "remove" && i + 1 < words.size()) {
            map<string, int> occurrences;
            if (tree.Find(toLowercase(words[i + 1]), root, occurrences)) {
                cout << toLowercase(words[i + 1]) << " has been REMOVED" << endl;
            }
            tree.remove(words[i + 1]);
            ++i; // Skip the next word
            continue;
        }

        // Check if the word is already encountered
        bool found = false;
        for (int j = 0; j < uniqueWords.size(); ++j) {
            if (uniqueWords[j] == words[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            uniqueWords.push_back(words[i]); // If not encountered before, add it to the uniqueWords vector
        }

        map<string, int> occurrences;
        if (tree.Find(toLowercase(words[i]), root, occurrences)) {
            // Update occurrencesByFile with the word occurrences for each file
            for (const auto& pair : occurrences) {
                occurrencesByFile[pair.first][words[i]].push_back(to_string(pair.second) + " times");
            }
        }

        // If word is not in tree, print the message
        if (!tree.Find(toLowercase(words[i]), root, occurrences) && entered) {
            cout << "No document contains the given query" << endl;
            entered = false;
        }
    }

    // Output occurrences grouped by file name
    for (int i = 0; i < occurrencesByFile.size(); ++i) {
        auto filePair = occurrencesByFile.begin();
        advance(filePair, i);
        cout << "in Document " << filePair->first << ", ";
        bool firstWord = true; 
        for (int j = 0; j < uniqueWords.size(); ++j) {
            auto word = uniqueWords[j];
            auto& wordOccurrences = filePair->second[word];
            if (!wordOccurrences.empty()) {
                if (!firstWord) {
                    cout << ", "; // If it is not the first word, add a comma
                }
                cout << toLowercase(word) << " found ";
                for (int k = 0; k < wordOccurrences.size(); ++k) {
                    cout << wordOccurrences[k];
                    if (k != wordOccurrences.size() - 1) {
                        cout << ", ";
                    }
                }
                firstWord = false;
            }
        }
        cout <<"."<< endl;
    }
}


// Function to process files and build Avl tree
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
    cin.ignore(); 
    string query;
    do  {
            cout << "Enter queried words in one line: ";
            getline(cin, query);

            if (query != "ENDOFINPUT") {
                vector<string> words;
                istringstream iss(query);
                string word;
                while (iss >> word) {
                    words.push_back(word);
                }

                searchWords<string,WordInfo>(words,tree);
            }
        }while (query != "ENDOFINPUT");

    return 0;
}
