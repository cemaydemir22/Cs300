#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

string toUpperCase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

struct Contact {
public:
    string name;
    string surname;
    string number;
    string city;

    bool operator<(const Contact& contact) const {
        if (name != contact.name) {
            return name < contact.name;
        }
        if (surname != contact.surname) {
            return surname < contact.surname;
        }
        return number < contact.number;
    }

    bool operator<=(const Contact& contact) const {
        if (name != contact.name) {
            return name <= contact.name;
        }
        if (surname != contact.surname) {
            return surname <= contact.surname;
        }
        
        return number <= contact.number;
    }


    bool operator>(const Contact& contact) const {
        return !(*this <= contact);
    }

    bool operator==(const string& query) const {
        string fullName = name + " " + surname;

        // Check if the query contains a space character
        if (query.find(' ') != string::npos) {
            
            if (fullName == query) {
                return true;
            }
        } else {
            
            if (fullName.find(query) == 0) {
                return true;
            }
        }

        // No match found
        return false;
    }


    bool operator<(const string& query) const {
        string fullName = name + " " + surname;
        return fullName < query;
    }
};

template<typename T>
class algorithms {

private:



public:
    void insertion_sort(vector<T>& contacts, int left, int right);
    //int partition(vector<T>& contacts, int low, int high);
    void quick_sort(vector<T>& contacts, int low, int high);
    void merge(vector<T>& contacts, int left, int mid, int right);
    void merge_sort(vector<T>& contacts, int left, int right);
    void percDown(vector<T>& contacts, int n, int i);
    void heapsort(vector<T>& contacts);
    void binary_search(vector<T>& contacts, const string& query, bool check);
    inline int leftChild(int i);
    void sequential_search(vector<T>& contacts, string query, bool check);
    inline void Swap(T& value1, T& value2);
    const T& median3(vector<T>& contacts, int left, int right);
    void print_info(T a);
};


template<typename T>
inline void algorithms<T>::Swap(T& value1, T& value2)
{
    T temp;
    temp = value1;
    value1 = value2;
    value2 = temp;
}

template<typename T>
const T& algorithms<T>::median3(vector<T>& contacts, int left, int right)// I took it from sorting3 slide
{
    int center = (left + right) / 2;

    // Order left, center, and right
    if (contacts[center] < contacts[left])
        Swap(contacts[left], contacts[center]);
    if (contacts[right] < contacts[left])
        Swap(contacts[left], contacts[right]);
    if (contacts[right] < contacts[center])
        Swap(contacts[center], contacts[right]);

    // Place pivot at position right - 1
    Swap(contacts[center], contacts[right - 1]);
    return contacts[right - 1];
}


template<typename T>
inline int algorithms<T>::leftChild(int i)
{
    return 2 * i + 1;
}


template<typename T>
void algorithms<T>::insertion_sort(vector<T>& contacts, int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        T key = contacts[i];
        int j = i - 1;

        // Move elements of contacts[0..i-1], that are greater than key, to one position ahead of their current position
        while (j >= left && contacts[j] > key)
        {
            contacts[j + 1] = contacts[j];
            j--;
        }
        contacts[j + 1] = key;
    }
}

template<typename T>
void algorithms<T>::quick_sort(vector<T>& a, int left, int right)// I took it from sorting3 slide
{
    if (left + 10 <= right)
    {
        T pivot = median3(a, left, right);
        int i = left, j = right - 1;
        for (; ; )
        {
            while (a[++i] < pivot) {}

            while (pivot < a[--j]) {}

            if (i < j)
                Swap(a[i], a[j]);
            else
                break;
        }
        Swap(a[i], a[right - 1]);   // Restore pivot

        quick_sort(a, left, i - 1);       // Sort small elements
        quick_sort(a, i + 1, right);    // Sort large elements
    }
    else  // Do an insertion sort on the subarray
        insertion_sort(a, left, right);



}

template <typename T>
void algorithms<T>::merge(vector<T>& arr, int left, int mid, int right) {
    int start2 = mid + 1;

    // If the direct merge is already sorted
    if (arr[mid] <= arr[start2]) {
        return;
    }

    // Two pointers to maintain start of both arrays to merge
    while (left <= mid && start2 <= right) {
        if (arr[left] <= arr[start2]) {
            left++;
        }
        else {
            T value = arr[start2];
            int index = start2;

            // Shift all the elements between element 1 and element 2, right by 1.
            while (index != left) {
                arr[index] = arr[index - 1];
                index--;
            }
            arr[left] = value;

            // Update all the pointers
            left++;
            mid++;
            start2++;
        }
    }
}


template<typename T>
void algorithms<T>::print_info(T contact)
{
    cout << toUpperCase(contact.name) << " "
        << toUpperCase(contact.surname) << " "
        << toUpperCase(contact.number) << " "
        << toUpperCase(contact.city) << endl;
}

template <typename T>
void algorithms<T>::merge_sort(vector<T>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

template<typename T>
void algorithms<T>::percDown(vector<T>& a, int i, int n)//from cs300 slides
{
    int child;
    T tmp;

    for (tmp = a[i]; leftChild(i) < n; i = child)
    {
        child = leftChild(i);
        if (child != n - 1 && a[child] < a[child + 1])
            child++;
        if (a[child] > tmp)
            a[i] = a[child];
        else
            break;
    }
    a[i] = tmp;
}


template <class T>
void algorithms<T>::heapsort(vector<T>& a)//from cs300 slides
{
    // buildHeap
    for (int i = a.size() / 2; i >= 0; i--)
        percDown(a, i, a.size());

    // sort
    for (int j = a.size() - 1; j > 0; j--)
    {
        Swap(a[0], a[j]);    // swap max to the last pos.
        percDown(a, 0, j); // re-form the heap
    }
}

template <typename T>
void algorithms<T>::binary_search(vector<T>& contacts, const string& query, bool check) {
    int left = 0;
    int right = contacts.size() - 1;
    bool found = false;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (contacts[mid] == query) {
            // Print the match at mid
           
            
            found = true;

            // Search to the left of mid for any more matches
            int temp = mid - 1;
            while (temp >= left && contacts[temp] == query) {
                
                
                temp--;
            }

            // Search to the right of mid for any more matches
            temp++;
            while (temp <= right && contacts[temp] == query) {
                if(check)
                {
                  print_info(contacts[temp]);
                }
                
                temp++;
            }

            break; // Exit the loop after finding and printing all matches
        }
        else if (contacts[mid] < query) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    if (check && !found) {
        cout << toUpperCase(query) << " does NOT exist in the dataset" << endl;
    }
    
}

template<typename T>
void algorithms<T>::sequential_search(vector<T>& contacts, string query, bool check) {// check used not print in timing function
    bool enter = true;
    for (int i = 0; i < contacts.size(); i++) {
        if (contacts[i] == query && check) {
            print_info(contacts[i]);
            enter = false;
        }
    }
    if (check && enter)
    {
        cout << toUpperCase(query) << " does NOT exist in the dataset" << endl;
    }

}