// hash.h
#ifndef HASH_H
#define HASH_H
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <set>
#include <algorithm>
#include <string>
using namespace std;

template<typename Key, typename Value>
class Hash;

struct Word_Info
{
    map<string, int> fileOccurrences; // Map stores filenames and frequencies ("a.txt,2"),(b.txt,3)
};


template<typename Key, typename Value>
struct item
{
    Key name; // (cem,(a.txt,2))
    item* next;
    Word_Info info;

    // Constructor to initialize next pointer
    item(item* next = nullptr) : next(next) {}

    // Allow Hash class to access private members
    friend class Hash<Key, Value>;
};



template<typename Key, typename Value>
class Hash
{
private:
    int tablesize; // Make tablesize a constant member variable
    std::vector<item<Key, Value>*> Hashtable; // Use vector instead of static array

public:
    Hash();
    int Hash_function(std::string key,int Tablesize) const;
    void Add_item(std::string fileName, const std::string& name);
    int Num_items_index(int index) const;
    void printable();
    int get_table_size(){return tablesize;}
    void print_items_index(int index);
    void find_file_occurrences(const vector<string>& names) const;
    void remove(std::string name);
    float load_factor() const;
    int Hash_size();
    bool word_exist(std::string name) const; // return true if the given name is in the table
    void resize(int newSize);
    int unique_words();
    int find_prime(int number) ;
    bool prime_number(int number);
    
};

template<typename Key, typename Value>
float Hash<Key, Value>::load_factor() const 
{
    int count = 0;
    item<Key, Value>* traverse;
    for (int i = 0; i < tablesize; ++i)
    {
        traverse = Hashtable[i];
        while (traverse && traverse->name != "_file_is_empty")
        {
            count++;
            traverse = traverse->next;
        }
    }
    
    return static_cast<float>(count)/(tablesize);
}

template<typename Key, typename Value>
bool Hash<Key, Value>::prime_number(int number)
{
    if (number <= 1)
        return false;
    if (number <= 3)
        return true;

    if (number % 2 == 0 || number % 3 == 0)
        return false;

    for (int i = 5; i * i <= number; i += 6) {
        if (number % i == 0 || number % (i + 2) == 0)
            return false;
    }

    return true;
}

template<typename Key, typename Value>
int Hash<Key, Value>::find_prime(int number) 
{
    while (!prime_number(number))
    {
        number++;
    }
    return number;
}

template<typename Key, typename Value>
int Hash<Key, Value>::unique_words() {
    int count = 0;
    item<Key, Value>* traverse;
    for (int i = 0; i < tablesize; ++i)
    {
        traverse = Hashtable[i];
        while (traverse && traverse->name != "_file_is_empty")
        {
            count++;
            traverse = traverse->next;
        }
    }
    return count;
}

template<typename Key, typename Value>
void Hash<Key, Value>::resize(int newSize)
{
    std::vector<item<Key, Value>*> newHashtable(newSize, nullptr);

    // Rehash all items into the new hash table
    for (int i = 0; i < tablesize; ++i)
    {
        item<Key, Value>* current = Hashtable[i];
        while (current && current->name!="_file_is_empty")
        {
            // Rehash the key using the new table size
            int newIndex = Hash_function(current->name,newSize);
            item<Key, Value>* temp = current->next; // Save the next pointer

            // Insert the current item at the beginning of the list in the new hash table
            current->next = newHashtable[newIndex];
            newHashtable[newIndex] = current;

            // Move to the next item
            current = temp;
        }
    }
    for(int i=0;i<newHashtable.size();i++)
    {
        if (newHashtable[i] == nullptr)
        {
            newHashtable[i] = new item< Key, Value>;
            newHashtable[i]->name = "_file_is_empty";
            newHashtable[i]->next=nullptr;

        }
    }

    // Update tablesize and Hashtable
    tablesize = newSize;
    Hashtable = newHashtable;
}


template<typename Key, typename Value>
int Hash<Key, Value>::Hash_size()
{
    int total_items = 0;
    for (int i = 0; i < tablesize; ++i) {
        total_items += Num_items_index(i);
    }
    return total_items;
}

template<typename Key, typename Value>
int Hash<Key, Value>::Hash_function(std::string key,int Tablesize) const
{
    int hash = 0;
    int index = 0;

    for (int i = 0; i < key.length(); i++)
    {
        hash += key[i];
    }
    index = hash % Tablesize;
    return index;
}

template<typename Key, typename Value>
Hash<Key, Value>::Hash() : Hashtable(tablesize),tablesize(101)
{
    // Other initialization code
    for (int i = 0; i < tablesize; i++) 
    {
        Hashtable[i] = new item<Key, Value>;
        Hashtable[i]->name = "_file_is_empty";
        Hashtable[i]->next = nullptr;
        
    }
}

template<typename Key, typename Value>
void Hash<Key, Value>::Add_item(string filename, const string& name)
{
    int index = Hash_function(name,tablesize);
    item<Key, Value>* current = Hashtable[index];

    // Search for the name in the linked list at the specified index
    while (current != nullptr) 
    {
        if (current->name == name) 
        {
            // Name found, update the count of filename occurrence
            current->info.fileOccurrences[filename]++;
            return; // Exit the function
        }
        current = current->next;
    }


    if (Hashtable[index]->name == "_file_is_empty")
    {
        Hashtable[index]->name = name;
        Hashtable[index]->info.fileOccurrences[filename] = 1;
        Hashtable[index]->next = nullptr;
    }
    else
    {
        current = Hashtable[index];
        while (current->next) {
            current = current->next;
        }
        current->next = new item<Key, Value>;
        current = current->next;
        current->name = name;
        current->info.fileOccurrences[filename] = 1;
        current->next = nullptr;
    }




    float currentLoadFactor = load_factor();
    int size=tablesize;
    if ((load_factor()) >= 0.90) 
    {
        resize(find_prime(tablesize * 2)); // Double the size of the hash table
        cout<<"rehashed..."<<endl;
        cout<<"previous table size: "<<size<<", new table size: "<<tablesize<<", current unique word count: "<<unique_words()<<", current load factor: "<< load_factor()<<endl;

    }
    else if(currentLoadFactor<=0.25 && tablesize>101)
    {
        resize(find_prime(tablesize/2)); // Double the size of the hash table
        cout<<"previous table size: "<<size<<", new table size: "<<tablesize<<", current unique word count: "<<unique_words()<<", current load factor: "<< load_factor()<<endl<<endl;
    }
    
}

template<typename Key, typename Value>
int Hash<Key, Value>::Num_items_index(int index) const {
    int count = 0;
    item<Key, Value>* traverse = Hashtable[index];
    while (traverse)
    {
        if (traverse && traverse->name != "_file_is_empty")
        {
            for (const auto& pair : traverse->info.fileOccurrences)
            {
                count += pair.second; // Add the frequency of occurrences
            }
        }
        traverse = traverse->next;
    }

    return count;
}


template<typename Key, typename Value>
void Hash<Key, Value>::print_items_index(int index)
{

    int number = Num_items_index(index);
    item<Key, Value>* traverse;
    traverse = Hashtable[index];
    while (traverse)
    {
        cout << traverse->name << " ";
        // Print map values
        for (const auto& pair : traverse->info.fileOccurrences) {
            cout << "(" << pair.first << ": " << pair.second << "), ";
        }
        cout << endl;
        traverse = traverse->next;
    }

}

template<typename Key, typename Value>
void Hash<Key, Value>::printable()
{
    int number;
    for (int i = 0; i < tablesize; i++)
    {
        number = Num_items_index(i);
        cout << "-----------\n";
        cout << "index: " << i << endl;
        cout << Hashtable[i]->name << endl;
        // Print map values
        for (const auto& pair : Hashtable[i]->info.fileOccurrences) {
            cout << "(" << pair.first << ": " << pair.second << "), ";
        }
        cout << endl;
        cout << "-----------\n";
        cout << "# of items in this index is " << number << endl;
    }
}

template<typename Key, typename Value>
void Hash<Key, Value>::find_file_occurrences(const vector<string>& query) const {
    // Collect the files that contain all the words in the query
    set<string> filesContainingAllWords;

    // Check if each word exists in the hash table
    for (const auto& word : query) {
        if (!word_exist(word)) 
        {
            cout << "No document contains the given query." << endl;
            return; // Exit if any word is not found
        }
    }

    // Collect the files that contain all words in the query
    for (const auto& word : query) {
        int index = Hash_function(word, tablesize);
        item<Key, Value>* traverse = Hashtable[index];

        // Traverse the linked list at the index to find the word
        while (traverse && traverse->name != word) {
            traverse = traverse->next;
        }

        // Add the files containing the word to the set
        if (traverse) {
            for (const auto& pair : traverse->info.fileOccurrences) {
                filesContainingAllWords.insert(pair.first);
            }
        }
    }

    // Print occurrences for files that contain all words
    bool foundFiles = false; // Flag to track if any files were found
    for (const auto& file : filesContainingAllWords) {
        bool fileContainsAllWords = true;
        for (const auto& word : query) {
            int index = Hash_function(word, tablesize);
            item<Key, Value>* traverse = Hashtable[index];

            // Traverse the linked list at the index to find the word
            while (traverse && traverse->name != word) {
                traverse = traverse->next;
            }

            // Check if the file contains the current word
            if (traverse && traverse->info.fileOccurrences.find(file) == traverse->info.fileOccurrences.end()) {
                fileContainsAllWords = false;
                break; // Exit loop if the file does not contain the word
            }
        }

        // Print occurrences only if the file contains all words in the query
        if (fileContainsAllWords) {
            if (foundFiles) {
                cout << endl;
            }
            cout << "in Document " << file << ", ";
            bool isFirstWord = true;
            for (const auto& word : query) {
                int index = Hash_function(word, tablesize);
                item<Key, Value>* traverse = Hashtable[index];

                // Traverse the linked list at the index to find the word
                while (traverse && traverse->name != word) {
                    traverse = traverse->next;
                }

                // Print the occurrence of the word in the file
                if (traverse && traverse->info.fileOccurrences.find(file) != traverse->info.fileOccurrences.end()) {
                    if (!isFirstWord) {
                        cout << ", ";
                    }
                    cout << word << " found " << traverse->info.fileOccurrences.at(file) << " times";
                    isFirstWord = false;
                }
            }
            foundFiles = true;
        }
    }

    // If no files containing all words were found, print a message
    if (!foundFiles) {
        cout << "No document contains the given query." << endl;
    }
    cout << "." << endl;
}


template<typename Key, typename Value>
bool Hash<Key, Value>::word_exist(string name) const
{
    bool is_in = false;
    int index = Hash_function(name,tablesize);
    item<Key, Value>* traverse = Hashtable[index];
    while (traverse)
    {
        if (traverse->name == name)
        {
            return true;
        }

        traverse = traverse->next;
    }

    return false;
}

template<typename Key, typename Value>
void Hash<Key, Value>::remove(string name)
{
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    if (word_exist(name))
    {
        int index = Hash_function(name,tablesize);
        item<Key, Value>* traverse = Hashtable[index];
        if (traverse->name == name)
        {
            Hashtable[index] = Hashtable[index]->next;
            delete traverse;
            traverse = nullptr;
        }
        else
        {
            while (traverse->next && traverse->next->name != name)
            {
                traverse = traverse->next;
            }
            item<Key, Value>* remove = traverse->next;
            traverse->next = traverse->next->next;
            delete remove;
            remove = nullptr;
        }
    }
    else
    {
        cout << "No document contains the given query" << endl;
    }
}

#endif
