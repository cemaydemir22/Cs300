#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm> // for std::transform
using namespace std;

// Structure to hold information about word occurrences in files
struct WordInfo {
    string word;
    map<string, int> fileOccurrences; // Map to store filename -> frequency pairs
};

// Binary Node structure modified to store 
template<typename Key, typename Value>
class BinarySearchTree {
private:

    struct BinaryNode {
        Value info;
        BinaryNode* left;
        BinaryNode* right;
        BinaryNode(const Key& fileName, const Key& word, int frequency, BinaryNode* lt = nullptr, BinaryNode* rt = nullptr) : left(lt), right(rt) {
            info.fileOccurrences[fileName] = frequency;
            info.word = word;
        }
    };
    BinaryNode* root;

    void makeEmpty(BinaryNode*& t);
    void insert(const Key& fileName, const Key& word, int frequency, BinaryNode*& t) const;
    bool search(const Key& word, BinaryNode* t, map<string, int>& occurrences) const;
    void remove(const Key& word, BinaryNode*& t) const;

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { makeEmpty(root); }

    BinaryNode* getRoot() const { return root; } // Function to access the root
    void insert(const Key& fileName, const Key& word, int frequency); // Public insert function
    void searchWords(const vector<Key>& words);
};


// Utility function to convert string to lowercase
string toLowercase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Function to insert word occurrences into the tree
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::insert(const Key& fileName, const Key& word, int frequency) {
    insert(fileName, word, frequency, root);
}
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::insert(const Key& fileName, const Key& word, int frequency, BinaryNode*& t) const {
    if (t == nullptr)
        t = new BinaryNode(fileName, word, frequency);
    else if (toLowercase(word) < toLowercase(t->info.word))
        insert(fileName, word, frequency, t->left);
    else if (toLowercase(t->info.word) < toLowercase(word))
        insert(fileName, word, frequency, t->right);
    else {
        t->info.fileOccurrences[fileName] += frequency; // Increment frequency if word already exists
    }
}


// Function to remove a word from the tree
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& word, BinaryNode*& t) const {
    if (t == nullptr)
        return;
    if (toLowercase(word) < toLowercase(t->info.word))
        remove(word, t->left);
    else if (toLowercase(t->info.word) < toLowercase(word))
        remove(word, t->right);
    else {
        if (t->left != nullptr && t->right != nullptr) {
            BinaryNode* minNode = t->right;
            while (minNode->left != nullptr) {
                minNode = minNode->left;
            }
            t->info = minNode->info;
            remove(t->info.word, t->right);
        } else {
            BinaryNode* oldNode = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete oldNode;
        }
    }
}

// Function to preprocess files and build the tree
template<typename Key, typename Value>
void preprocessFiles(BinarySearchTree<Key, Value>& tree, int numFiles) {
    for (int i = 0; i < numFiles; ++i) {
        Key fileName;
        cout << "Enter the name of file " << i + 1 << ": ";
        cin >> fileName;

        ifstream file(fileName);
        if (!file) {
            cerr << "Error: Unable to open file " << fileName << endl;
            continue;
        }

        // Read words from file and insert into tree
        Key word;
        while (file >> word) {
            // Convert word to lowercase
            word = toLowercase(word);

            // Insert word into tree with file occurrence
            tree.insert(fileName, word, 1);
        }

        file.close();
    }
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::searchWords(const vector<Key>& words) {
    map<string, map<string, vector<string>>> occurrencesByFile; // Map to store occurrences by file name

    // Iterate over each word in the query
    for (size_t i = 0; i < words.size(); ++i) {
        if (words[i] == "REMOVE" && i + 1 < words.size()) {
            remove(words[i + 1], root);
            ++i; // Skip the next word
            continue;
        }

        map<string, int> occurrences;

        // Search for the word in the tree and get its occurrences in each file
        if (search(toLowercase(words[i]), root, occurrences)) {
            // Update occurrencesByFile with the word occurrences for each file
            for (const auto& pair : occurrences) {
                occurrencesByFile[pair.first][words[i]].push_back(to_string(pair.second) + " times");
            }
        }
    }

    // Output occurrences grouped by file name
    for (const auto& filePair : occurrencesByFile) {
        cout << "in Document " << filePair.first << ", ";
        for (const auto& wordPair : filePair.second) {
            cout << wordPair.first << ": ";
            for (size_t i = 0; i < wordPair.second.size(); ++i) {
                cout << wordPair.second[i];
                if (i != wordPair.second.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "; ";
        }
        cout << endl;
    }
}

// Helper function to search for a word in the tree
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::search(const Key& word, BinaryNode* t, map<string, int>& occurrences) const {
    if (t == nullptr) {
        return false;
    }

    if (word < toLowercase(t->info.word))
        return search(word, t->left, occurrences);
    else if (toLowercase(t->info.word) < word)
        return search(word, t->right, occurrences);
    else {
        occurrences = t->info.fileOccurrences;
        return true;
    }
}

// Utility function to deallocate memory of the tree
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::makeEmpty(BinaryNode*& t) {
    if (t != nullptr) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}

int main() {
    BinarySearchTree<string, WordInfo> tree;

    int numFiles;
    cout << "Enter the number of input files: ";
    cin >> numFiles;

    preprocessFiles(tree, numFiles);
    cin.ignore(); // Clear

    string query;
    cout << "Enter queried words in one line (or type ENDOFINPUT to end): ";
    while (getline(cin, query) && query != "ENDOFINPUT") {
        istringstream iss(query);
        vector<string> words;
        string word;
        while (iss >> word) {
            words.push_back(word);
        }

        tree.searchWords(words);
        cout << "Enter queried words in one line (or type ENDOFINPUT to end): ";
    }

    return 0;
}
