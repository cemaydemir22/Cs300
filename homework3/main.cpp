#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "BST.cpp"
#include <algorithm> // Include the algorithm header
#include "hash.h"    // Include your hash table implementation header file here

using namespace std;

// To make the words lowercase
string toLowercase1(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Function to preprocess files and insert words into the hash table
template<typename Key, typename Value>
void preprocessFiles(Hash<Key, Value>& hashtable, BST<Key, WordInfo>& tree, int numFiles, const vector<string>& filenames) 
{
    for (int i = 0; i < numFiles; ++i) {
        ifstream file(filenames[i]);
        if (!file) {
            cerr << "Error: Unable to open file " << filenames[i] << endl;
            continue;
        }

        // Read words from file and insert into hash table
        // Read words from file and insert into hash table
        Key word;
        while (file >> word)

        {
            word = toLowercase1(word);
            // Convert word to lowercase and split by punctuations
            Key filteredWord;
            for (char ch : word) {
                if (std::isalpha(ch)) {  // Check if character is a letter
                    filteredWord += std::tolower(ch);
                }
                else if (std::isdigit(ch)) {  // Check if character is a digit
                    // Add code here to split the word and insert both parts into the hash table
                    // For example, if the word is "word123", split it into "word" and "123" and insert both
                    // You can keep track of letters before and after the number in separate strings and then insert them
                }
                else {
                    // If it's a punctuation, add the current word to the hash table
                    if (!filteredWord.empty()) {
                        hashtable.Add_item(filenames[i], filteredWord);
                        //tree.insert(filenames[i],filteredWord,1);
                        filteredWord.clear();
                    }
                }
            }

            // Insert the last word if not empty
            if (!filteredWord.empty()) {
                hashtable.Add_item(filenames[i], filteredWord);
                //tree.insert(filenames[i],filteredWord,1);
            }
        }


        file.close();
    }
}

// Function to search for a word in the hash table
template<typename Key, typename Value>
void searchWord(string word,const Hash<Key, Value>& hashtable) 
{
    // Ask the user to enter a word to search
   
    word = toLowercase1(word);

    // Search for the word in the hash table
    if (hashtable.word_exist(word)) 
    {
        cout << "Word '" << word << "' found in the hash table:" << endl;
        hashtable.find_file_occurrences(word); // Display file occurrences for the word
    }
    else 
    {
        cout << "No occurrences of '" << word << "' in the hash table." << endl;
    }
}

void BST_time()
{
    int k = 20;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
    // QueryDocuments(with BST);
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
    (std::chrono::high_resolution_clock::now() -start);
    cout << "\nTime: " << BSTTime.count() / k << "\n";
 
}

template<typename Key,typename Value>
float HASH_time(string word,Hash<Key, Value>& hashtable)
{
    int k = 20;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
       searchWord(word,hashtable);
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
    (std::chrono::high_resolution_clock::now() - start);
    cout << "\nTime: " << HTTime.count() / k << "\n";
    return (HTTime.count() / k);
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
void searchWords(const vector<Key>& words, BST<Key,Value>& tree) {
    bool entered = true;
    map<string, map<string, vector<string>>> occurrencesByFile; // Stores words with filename
    vector<string> uniqueWords; // Stores unique words
    for (int i = 0; i < words.size(); ++i) {
        BSTNode<Key,Value>* root = tree.getRoot();
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
        if (!tree.Find(toLowercase(words[i]), root, occurrences) && entered) 
        {
            cout << "No document contains the given query" << endl;
            entered = false;
        }
    }

    outputOccurrences<string,WordInfo>(occurrencesByFile,uniqueWords);
}



int main() 
{
    // Create a Hash object
    Hash<string, item<string, Word_Info>> hashTable;
    BST<string,WordInfo> tree;

    // Get the number of files from the user
    int numFiles;
    cout << "Enter the number of files: ";
    cin >> numFiles;

    vector<string> filenames(numFiles);
    for (int i = 0; i < numFiles; ++i) {
        cout << "Enter file name " << i + 1 << ": ";
        cin >> filenames[i];
    }

    // Preprocess files and build the hash table
    preprocessFiles(hashTable,tree, numFiles, filenames);
    string word;
    do
    {
        cout << "Enter a word to search in the hash table: ";
        cin >> word;
        vector<string> words;
        words.push_back(word);
        string remove;
        if(word!="finish")
        {
            // Search for a word in the hash table
            searchWord(word,hashTable);
            //searchWords(words,tree);
            //searchWord(word,hashTable);
            word.clear();
            words.clear();
            //cout<<"enter a word to remove: ";
            //cin>>remove;
            //hashTable.remove(remove);
        }
       
    }
    while(word!="finish");
    cout<<"unique count: "<<hashTable.unique_words();
  
    return 0;
}
