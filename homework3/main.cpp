#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "BST.cpp"
#include <algorithm> // Include the algorithm header
#include "hash.h"    // Include your hash table implementation header file here
#include <set>
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

            // Insert the last word if not empty
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

// Function to search for a word in the hash table
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
            cout << "." << endl; // End the line with a dot
        }
    }

    // If no file contains all words, print appropriate message
    if (!found) {
        cout << "No document contains the given query." << endl;
    }
}



// Function to split a string into words based on punctuation or numbers
vector<string> splitWords(const string& input) {
    vector<string> words;
    string word;
    for (char c : input) {
        if (isalpha(c)) {
            word += c;
        }
        else {
            if (!word.empty()) {
                word=toLowercase1(word);
                words.push_back(word);
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        word=toLowercase1(word);
        words.push_back(word);
    }
    return words;
}


int main() 
{
    // Create a Hash object
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

    // Preprocess files and build the hash table
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
    cout << "Enter queried words in one line:"<<" ( to finish please write ENDOFINPUT) "<<": ";
    cin >> word;
    int k = 20;
    auto startHashTable = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
         vector<pair<string, int>> result_table=hashTable.queryDocuments(word);
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - startHashTable);
    std::cout << "\nAverage time for Hashtable: " << HTTime.count() / k << " nanoseconds\n";

    // Define a start time using high_resolution_clock for BST
    auto startBST = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        vector<pair<string, int>> result = tree.queryDocuments(word); // Perform query
    }
    auto endBST = std::chrono::high_resolution_clock::now();

    // Calculate duration using duration_cast for BST
    auto durationBST = std::chrono::duration_cast<std::chrono::nanoseconds>(endBST - startBST);

    // Print the duration for BST
    std::cout << "Time taken for BST: " << durationBST.count() << " nanoseconds" << std::endl;

   
  
    return 0;
}
