#include "stack.h"
using namespace std;

template<typename T>
STACK<T>::STACK() : top(nullptr) {}

template<typename T>
void STACK<T>::push(T obj)
{
	Node<T>* node;
	node = new Node<T>;

	node->object = obj;

	// If stack is empty then node is the first node
	if (Is_empty())
	{
		top = node;
		node->next = nullptr;

	}
	else
	{
		node->next = top;
		top = node;
	}
}

template<typename T>
int STACK<T>::pop()
{
	Node<T>* temp;

	if (!Is_empty())
	{
		temp = top;
		top = top->next;
		delete temp;
		return 1;
	}
	else{
		return 5;
	}
}

template<typename T>
bool STACK<T>::Is_empty()
{
	return (top == nullptr); // if true then empty
}

template<typename T>
T STACK<T>::current_node()
{
	return (top->object);
	
}





