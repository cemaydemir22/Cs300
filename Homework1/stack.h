#pragma once 
#ifndef STACK_H
#define STACK_H

template<typename T>
struct Node
{
	T object;
	Node<T>* next;
	Node(): next(nullptr){}
};

template<typename T>
class STACK
{

private:

	Node<T> * top_node;

public:

	STACK();

	void push(T obj);


	void pop();


	bool isEmpty();


	T top();

};

#endif