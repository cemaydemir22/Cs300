#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "BST.cpp"
#include <algorithm> 
#include "hash.h"    
#include <set>
using namespace std;


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

        
        Key word;
        while (file >> word)

        {
            word = toLowercase(word);
            // Convert word to lowercase and split by punctuations
            Key filteredWord;
            for (char ch : word) {
                if (std::isalpha(ch)) {  // Check if character is a letter
                    filteredWord += std::tolower(ch);
                }
                else if (std::isdigit(ch)) {  // Check digit or not
                    if (!filteredWord.empty()) {
                        hashtable.Add_item(filenames[i], filteredWord);
                        tree.insert(filenames[i],filteredWord,1);
                        filteredWord.clear();
                    }
                }
                else {
                    // If it's a punctuation, add the current word to the hash table
                    if (!filteredWord.empty()) {
                        hashtable.Add_item(filenames[i], filteredWord);
                        tree.insert(filenames[i],filteredWord,1);
                        filteredWord.clear();
                    }
                }
            }

            // Insert if not empty
            if (!filteredWord.empty()) {
                hashtable.Add_item(filenames[i], filteredWord);
                tree.insert(filenames[i],filteredWord,1);
            }
        }


        file.close();
    } 
    cout<<endl;
    cout<<"After preprocessing, the unique word count is "<<hashtable.unique_words()<<". Current load ratio is "<< hashtable.load_factor()<<endl;
}


template<typename Key, typename Value>
void searchWord(const vector<Key>& words, const Hash<Key, Value>& hashtable) 
{
    hashtable.find_file_occurrences(words);
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
    map<string, set<string>> matchingFiles; // Stores files containing all words
    set<string> allWords(words.begin(), words.end()); // Convert vector of words to a set for efficient lookup
    map<string, map<string, int>> wordCounts; // Stores the count of each word in each file

    for (const auto& word : allWords) 
    {
        map<string, int> occurrences;
        tree.Find(word, tree.getRoot(), occurrences);

        for (const auto& pair : occurrences) 
        {
            if (pair.second > 0) 
            {
                matchingFiles[pair.first].insert(word); // Add file to matchingFiles for each word found
                wordCounts[pair.first][word] += pair.second; // Increment the count of the word in the file
            }
        }
    }

    // Print out files containing all words along with their occurrences
    bool found = false;
    for (const auto& pair : matchingFiles) {
        if (pair.second == allWords) { // If the set of words in the file matches allWords
            found = true;
            cout << "in Document " << pair.first << ", ";
            for(int i=0;i<words.size();i++)
            {
                if(wordCounts[pair.first].count(words[i])>0)
                {
                    cout<<words[i]<<" found ";
                    cout<<wordCounts[pair.first][words[i]]<<" times";
                }
                if(i<words.size()-1){
                    cout<<", ";
                }
            }
            cout << "." << endl;
        }
    }

    if (!found) {
        cout << "No document contains the given query." << endl;
    }
}
template<typename Key, typename Value>
vector<string> hash_find(const vector<string> words, Hash<Key, Value>& hashtable)
{
    vector<string> word;
    for(int i=0;i<words.size();i++)
    {
       if(hashtable.word_exist(words[i]))
       {
         word.push_back(words[i]);
       }
     
    }
    return word;
}
template<typename Key, typename Value>
vector<string> tree_find(const vector<string> words, BST<Key, WordInfo>& tree)
{
    vector<string> word;
    map<string, int> occurrences;
    for(int i=0;i<words.size();i++)
    {
        if(tree.word_exist(words[i])){
            word.push_back(words[i]);
        }
      
    }
    return word;
}

// Split the query 
vector<string> splitWords(const string& input) {
    vector<string> words;
    string word;
    for (char c : input) {
        if (isalpha(c)) {
            word += c;
        }
        else {
            if (!word.empty()) {
                word=toLowercase(word);
                words.push_back(word);
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        word=toLowercase(word);
        words.push_back(word);
    }
    return words;
}


int main() 
{
    
    Hash<string, item<string, Word_Info>> hashTable;
    BST<string,WordInfo> tree;

    // Get the number of files from the user
    int numFiles;
    cout << "Enter number of input files:  ";
    cin >> numFiles;

    vector<string> filenames(numFiles);
    for (int i = 0; i < numFiles; ++i) 
    {
        cout << "Enter file name " << i + 1 << ": ";
        cin >> filenames[i];
    }

    // Preprocess files and build the hash table and BST
    preprocessFiles(hashTable,tree, numFiles, filenames);
    cin.ignore();
    string input;
    vector<string> words;
    do 
    {
        cout << "Enter queried words in one line (to finish, please write 'ENDOFINPUT'): ";
        getline(cin, input);

        if (input != "ENDOFINPUT") 
        {
            // Split the input into individual words
            words = splitWords(input);
            searchWords<string,WordInfo>(words, tree);
            searchWord(words,hashTable);
        }
    }while (input != "ENDOFINPUT");
    
    int k = 20;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; ++i) {
        vector<string> find;
        find = tree_find<string, item<string, Word_Info>>(words, tree); // CHECKS FOR ONLY THE LAST QUERY 
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - start);
    cout << "BST Average Time: " << BSTTime.count() / k << " nanoseconds\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; ++i) {
        vector<string> find;
        find = hash_find<string, item<string, Word_Info>>(words, hashTable);// CHECKS FOR ONLY THE LAST QUER
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - start);
    cout << "Hash Table Average Time: " << HTTime.count() / k << " nanoseconds\n"; 
    double ratio = static_cast<double>(BSTTime.count()) / HTTime.count();
    cout << "BST Time is " << ratio << " times the Hash Table Time\n";



   
  
    return 0;
}
