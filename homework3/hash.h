// hash.h
#ifndef HASH_H
#define HASH_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

template<typename Key, typename Value>
class Hash;

struct Word_Info
{
    map<string, int> fileOccurrences; // Map stores filenames and frequencies
};


template<typename Key, typename Value>
struct item {
    Key name;
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
    int tablesize=10; // Make tablesize a constant member variable
    std::vector<item<Key, Value>*> Hashtable; // Use vector instead of static array

public:
    Hash();
    int Hash_function(std::string key) const ;
    void Add_item(std::string fileNamez, const std::string& name);
    int Num_items_index(int index) const;
    void printable();
    void print_items_index(int index);
    void find_file_occurrences(std::string name) const ;
    void remove(std::string name);
    float load_factor() const;
    int Hash_size();
    bool word_exist(std::string name) const ; // return true if the given name is in the table
    void resize(int newSize);
    int unique_words();
};

template<typename Key, typename Value>
float Hash<Key, Value>::load_factor() const {
    int total_items = 0;
    for (int i = 0; i < tablesize; ++i) {
        total_items += Num_items_index(i);
    }
    return static_cast<float>(total_items) / tablesize;
}

template<typename Key, typename Value>
int Hash<Key, Value>::unique_words()  {
    int count = 0;
    item<Key, Value>* traverse;
    for (int i = 0; i < tablesize; ++i) {
        traverse = Hashtable[i];
        while (traverse && traverse->name != "empty")
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
    std::vector<item<Key, Value>*> newHashtable(newSize);

    // Rehash all items into the new hash table
    for (int i = 0; i < tablesize; ++i) {
        item<Key, Value>* current = Hashtable[i];
        while (current) 
        {
            int newIndex = Hash_function(current->name) % newSize;
            item<Key, Value>* temp = current->next; // Save the next pointer
            current->next = newHashtable[newIndex]; // Insert at the beginning of the list
            newHashtable[newIndex] = current;
            current = temp; // Move to the next item
        }
    }

    // Update tablesize and Hashtable
    tablesize = newSize;
    Hashtable = std::move(newHashtable);
}


template<typename Key, typename Value>
int Hash<Key, Value>::Hash_size()  {
    int total_items = 0;
    for (int i = 0; i < tablesize; ++i) {
        total_items += Num_items_index(i);
    }
    return total_items;
}

template<typename Key, typename Value>
int Hash<Key, Value>::Hash_function(std::string key) const
{
    int hash = 0;
    int index = 0;

    for (int i = 0; i < key.length(); i++)
    {
        hash += key[i];
    }
    index = hash % tablesize;
    return index;
}

template<typename Key, typename Value>
Hash<Key, Value>::Hash() : Hashtable(tablesize) {
    // Other initialization code
    for (int i = 0; i < tablesize; i++) {
        Hashtable[i] = new item<Key, Value>;
        Hashtable[i]->name = "empty";
        Hashtable[i]->next = nullptr;
    }
}




template<typename Key, typename Value>
void Hash<Key, Value>::Add_item(string filename, const string& name) {
    int index = Hash_function(name);
    item<Key, Value>* current = Hashtable[index];

    // Search for the name in the linked list at the specified index
    while (current != nullptr) {
        if (current->name == name) {
            // Name found, update the count of filename occurrence
            current->info.fileOccurrences[filename]++;
            return; // Exit the function
        }
        current = current->next;
    }

    // If the name is not found, add it as a new item at the end of the linked list
    if (Hashtable[index]->name == "empty") {
        Hashtable[index]->name = name;
        Hashtable[index]->info.fileOccurrences[filename] = 1;
        Hashtable[index]->next = nullptr;
    }
    else {
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

    // Check the load factor after adding an item
    //float currentLoadFactor = load_factor();
    //if (currentLoadFactor > 0.9) {
    //    resize(tablesize * 2); // Double the size of the hash table
    //}
    //else if (currentLoadFactor < 0.25 && tablesize > 10) {
    //    resize(tablesize / 2); // Halve the size of the hash table (if it's larger than the initial size)
    //}
}

template<typename Key, typename Value>
int Hash<Key, Value>::Num_items_index(int index) const {
    int count = 0;
    item<Key, Value>* traverse = Hashtable[index];
    while (traverse)
    {
        if (traverse->name != "empty") {
            for (const auto& pair : traverse->info.fileOccurrences) {
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
    for (int i = 0; i < tablesize; i++) {
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
void Hash<Key, Value>::find_file_occurrences(string name) const
{
    int index = Hash_function(name);
    item<Key, Value>* traverse = Hashtable[index];
    while (traverse && traverse->name != name)
    {
        traverse = traverse->next;
    }
    if (traverse) {
        // Print map values
        for (const auto& pair : traverse->info.fileOccurrences) {
            cout << "(" << pair.first << ": " << pair.second << "), ";
        }
        cout << endl;
    }
}

template<typename Key, typename Value>
bool Hash<Key, Value>::word_exist(string name) const
{
    bool is_in = false;
    int index = Hash_function(name);
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
    if (word_exist(name))
    {
        int index = Hash_function(name);
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
        cout << "the word that you want to remove is not in the table." << endl;
    }
}

#endif
