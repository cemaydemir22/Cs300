#include "stack.h"
using namespace std;

template<typename T>
STACK<T>::STACK() : top_node(nullptr) {}

template<typename T>
void STACK<T>::push(T obj)
{
	Node<T>* node;
	node = new Node<T>;

	node->object = obj;

	// If stack is empty then node is the first node
	if (isEmpty())
	{
		top_node = node;
		node->next = nullptr;

	}
	else
	{
		node->next = top_node;
		top_node = node;
	}
}

template<typename T>
void STACK<T>::pop()
{
	Node<T>* temp;

	if (!isEmpty())
	{
		temp = top_node;
		top_node = top_node->next;
		delete temp;
		
	}
	
}

template<typename T>
bool STACK<T>::isEmpty()
{
	return (top_node == nullptr); // if true then empty
}

template<typename T>
T STACK<T>::top()
{
    if (top_node)
    {
        return (top_node->object);
    }
    else
    {
        // Return a default value indicating an empty stack
        // For example, if T is a class type, return a default-constructed object
        return T(); // This assumes T has a default constructor
    }
}





