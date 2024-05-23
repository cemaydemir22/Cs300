#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "algorithms.h"
#include <map>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace std::chrono;


vector<Contact> readPhoneBook(const string& filename) {
    vector<Contact> phoneBook;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        Contact contact;
        iss >> contact.name >> contact.surname >> contact.number >> contact.city;
        contact.name = toUpperCase(contact.name);
        contact.surname = toUpperCase(contact.surname);
        contact.number = toUpperCase(contact.number);
        contact.city = toUpperCase(contact.city);


        phoneBook.push_back(contact);
    }
    return phoneBook;
}

int main() {
    string filename, query;
    cout << "Please enter the contact file name: "<<endl;
    cin >> filename;
    cout << "Please enter the word to be queried: "<<endl;
    cin.ignore();
    getline(cin, query);
    vector<Contact> phoneBook = readPhoneBook(filename);
    algorithms<Contact> algo;
    vector<Contact> phoneBookQuick = phoneBook;
    vector<Contact> phoneBookInsert = phoneBook;
    vector<Contact> phoneBookMerge = phoneBook;
    vector<Contact> phoneBookHeap = phoneBook;
    algo.insertion_sort(phoneBookQuick,0,phoneBookQuick.size()-1);
    cout << endl;
    cout << "Sorting the vector copies" << endl;
    cout << "======================================" << endl;

    const int repetitions = 20; // Number of times to run the sort

    chrono::duration<double, nano> total_duration_quick_sort(0);
    chrono::duration<double, nano> total_duration_insertion_sort(0);
    chrono::duration<double, nano> total_duration_merge_sort(0);
    chrono::duration<double, nano> total_duration_heap_sort(0);
    chrono::duration<double, nano> total_duration_seqeunce_search(0);
    chrono::duration<double, nano> total_duration_binary_search(0);
    for (int i = 0; i < repetitions; ++i) {
        // Create a copy of the phoneBook for each sort to ensure each run starts with the same data
        vector<Contact> phoneBookCopy = phoneBook;

        auto start = chrono::high_resolution_clock::now();
        algo.quick_sort(phoneBookCopy, 0, phoneBookCopy.size() - 1);  // Calling the member function
        auto end = chrono::high_resolution_clock::now();

        total_duration_quick_sort += end - start;
    }

    chrono::duration<double, nano> average_duration_quick_sort = total_duration_quick_sort / repetitions;
    cout << "Quick Sort Time: " << average_duration_quick_sort.count() << " Nanoseconds" << endl;

    for (int i = 0; i < repetitions; ++i) {
        // Create a copy of the phoneBook for each sort to ensure each run starts with the same data
        vector<Contact> phoneBookCopy = phoneBook;

        auto start = chrono::high_resolution_clock::now();
        algo.insertion_sort(phoneBookCopy, 0, phoneBookCopy.size() - 1);  // Calling the member function
        auto end = chrono::high_resolution_clock::now();

        total_duration_insertion_sort += end - start;
    }

    chrono::duration<double, nano> average_duration_insertion_sort = total_duration_insertion_sort / repetitions;
    cout << "Insertion Sort Time: " << average_duration_insertion_sort.count() << " Nanoseconds" << endl;

    for (int i = 0; i < repetitions; ++i) {
        // Create a copy of the phoneBook for each sort to ensure each run starts with the same data
        vector<Contact> phoneBookCopy = phoneBook;

        auto start = chrono::high_resolution_clock::now();
        algo.merge_sort(phoneBookCopy, 0, phoneBookCopy.size() - 1);  // Calling the member function
        auto end = chrono::high_resolution_clock::now();

        total_duration_merge_sort += end - start;
    }

    chrono::duration<double, nano> average_duration_merge_sort = total_duration_merge_sort / repetitions;
    cout << "Merge Sort Time: " << average_duration_merge_sort.count() << " Nanoseconds" << endl;

    for (int i = 0; i < repetitions; ++i) {
        // Create a copy of the phoneBook for each sort to ensure each run starts with the same data
        vector<Contact> phoneBookCopy = phoneBook;

        auto start = chrono::high_resolution_clock::now();
        algo.heapsort(phoneBookCopy);  // Calling the member function
        auto end = chrono::high_resolution_clock::now();

        total_duration_heap_sort += end - start;
    }

    chrono::duration<double, nano> average_duration_heap_sort = total_duration_heap_sort / repetitions;
    cout << "Heap Sort Time: " << average_duration_heap_sort.count() << " Nanoseconds" << endl<<endl;








    cout << "Searching for " << query << endl;
    cout << "======================================" << endl;
    query=toUpperCase(query);
    algo.binary_search(phoneBookQuick,query,true);
    for (int i = 0; i < repetitions; ++i) {
        // Create a copy of the phoneBook for each sort to ensure each run starts with the same data
       

        auto start = chrono::high_resolution_clock::now();
        algo.binary_search(phoneBookQuick,query,false);  // Calling the member function
        auto end = chrono::high_resolution_clock::now();

        total_duration_binary_search += end - start;
    }
    
    chrono::duration<double, nano> average_duration_binary_search = total_duration_binary_search / repetitions;
    cout << "Binary  Search Time: " << average_duration_binary_search.count() << " Nanoseconds" << endl << endl;
    cout << "Search results for Sequential Search:"<<endl;
    algo.sequential_search(phoneBookQuick, query, true);
    for (int i = 0; i < repetitions; ++i) {
        // Create a copy of the phoneBook for each sort to ensure each run starts with the same data
        vector<Contact> phoneBookCopy = phoneBook;

        auto start = chrono::high_resolution_clock::now();
        algo.sequential_search(phoneBookQuick,query,false);  // Calling the member function
        auto end = chrono::high_resolution_clock::now();

        total_duration_seqeunce_search += end - start;
    }

    chrono::duration<double, nano> average_duration_sequence_search = total_duration_seqeunce_search / repetitions;
   
    cout << "Sequential Search Time: " << average_duration_sequence_search.count() << " Nanoseconds" << endl << endl;

    cout << "SpeedUps between Search Algorithms" << endl;
    cout << "======================================" << endl;
    cout << "(Sequential Search/ Binary Search) SpeedUp = " << static_cast<double> (average_duration_sequence_search.count() / average_duration_binary_search.count());
    cout << endl << endl;
    

    cout << "SpeedUps between Sorting Algorithms"<<endl;
    cout << "======================================"<<endl;
    cout << "(Insertion Sort/ Quick Sort) SpeedUp = "<< static_cast<double> (average_duration_insertion_sort.count()/average_duration_quick_sort.count())<<endl;
    cout << "(Merge Sort / Quick Sort) SpeedUp = "<< static_cast<double>(average_duration_merge_sort.count()/average_duration_quick_sort.count())<<endl;
    cout << "(Heap Sort / Quick Sort) SpeedUp = " << static_cast<double>(average_duration_heap_sort.count() / average_duration_quick_sort.count())<<endl;

    return 0;
}