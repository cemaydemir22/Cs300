#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include <vector>
#include <map>
#include "avl.cpp"
#include<cctype> 
#include <algorithm> 
using namespace std;

bool check_word_format(string & word) {
    bool contain_digit = false;
    bool contain_punctuation = false;

    // contain digit or punc
    for (size_t i = 0; i < word.size(); ++i) {
        if (std::isdigit(static_cast<unsigned char>(word[i]))) {
            contain_digit = true;
        }
        else if (std::ispunct(static_cast<unsigned char>(word[i]))) {
            contain_punctuation = true;
            word.erase(i, 1); //if there is a punc remove
            --i; 
        }
    }

    // if there is digit return false (do not insert)
    if (contain_digit) {
        return false;
    }

    // If punctuation return true (punc already deleted word can be added)
    if (contain_punctuation) {
        return true;
    }

    // if there is no digit and punc return directly true (insert)
    return true;
}

template<typename Key,typename Value>
void outputOccurrences(const map<string, map<string, vector<string>>> occurrencesByFile, const vector<string> uniqueWords) {
    for (const auto& filePair : occurrencesByFile) {
        cout << "in Document " << filePair.first << ", ";
        bool firstWord = true;
        for (const auto& word : uniqueWords) {
            auto wordOccurrences = filePair.second.find(word);
            if (wordOccurrences != filePair.second.end() && !wordOccurrences->second.empty()) {
                if (!firstWord) {
                    cout << ", "; 
                }
                cout << toLowercase(word) << " found ";
                for (int k = 0; k < wordOccurrences->second.size(); ++k) {
                    cout << wordOccurrences->second[k];
                    if (k != wordOccurrences->second.size() - 1) {
                        cout << ", ";
                    }
                }
                firstWord = false;
            }
        }
        cout << "." << endl;
    }
}// Prints searched words and number of occurences


template<typename Key, typename Value>
void searchWords(const vector<Key>& words, AVLTree<Key,Value>& tree) {
    bool entered = true;
    map<string, map<string, vector<string>>> occurrencesByFile; // Stores words with filename
    vector<string> uniqueWords; // Stores unique words
    for (int i = 0; i < words.size(); ++i) {
        AVLNode<Key,Value>* root = tree.getRoot();
        if (toLowercase(words[i]) == "remove" && i + 1 < words.size()) {
            map<string, int> occurrences;
            if (tree.Find(toLowercase(words[i + 1]), root, occurrences)) {
                cout << toLowercase(words[i + 1]) << " has been REMOVED" << endl;
            }
            tree.remove(words[i + 1]);
            ++i; 
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
            uniqueWords.push_back(words[i]); // If not encountered  add it to the  vector
        }

        map<string, int> occurrences;
        if (tree.Find(toLowercase(words[i]), root, occurrences)) {
           
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

    outputOccurrences<string,WordInfo>(occurrencesByFile,uniqueWords);
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

                  searchWords<string,WordInfo>(words, tree);
                
            }
        }while (query != "ENDOFINPUT");

    return 0;
}
