#include <iostream>
#include <string>
#include <vector>
#include "avl.cpp"

using namespace std;


struct DocumentItem {
    string documentName;
    int count;
};

struct WordItem {
    string word;
    
};



int main() {


    int num_files;
    cout<<"Enter number of input files: ";
    cin>>num_files;
    for(int i=0;i<num_files;i++)
    {
        cout<<"Enter "<<(i+1)<< " file name: ";
    }
   
    return 0;
}