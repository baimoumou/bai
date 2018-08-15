#pragma once
#include<iostream>
#include<Windows.h>
#include"Heap.h"
using namespace std;
template <class T>
struct HuffmantreeNode
{
	T _w;
	HuffmantreeNode<T>* left;
	HuffmantreeNode<T>* right;
	HuffmantreeNode<T>* parent;
	HuffmantreeNode(const T&w)
		:_w(w)
		,left(NULL)
		,right(NULL)
		,parent(NULL)
	{}
};
template<class T>
class Huffmantree
{
	typedef HuffmantreeNode<T> Node;
public:
	Huffmantree()
		:_root(NULL)
	{}
	Huffmantree(T* w, size_t n,const T& invalid)
	{
		struct PNodeCompare
		{
			bool operator()(const Node*left,const Node*right)const
			{
				return left->_w < right->_w;
			}
		};
		Heap<Node*, PNodeCompare> minheap;
		for (size_t i = 0;i < n; ++i)
		{
			if (w[i] != invalid)
			{
				minheap.Push(new Node(w[i]));
			}
		}
		while (minheap.Size() > 1)//*
		{
			Node*left = minheap.Top();
			minheap.Pop();
			Node*right = minheap.Top();
			minheap.Pop();
			Node*parent = new Node(left->_w + right->_w);
			parent->left = left;
			parent->right = right;
			left->parent = parent;
			right->parent = parent;
			minheap.Push(parent);
		}
		_root = minheap.Top();
	}
	Node* GetRoot()
	{
		return _root;
	}
	~Huffmantree()
	{
		Destory(_root);
	}
	void Destory(Node* root)
	{
		if (root == NULL)
			return;
		Destory(root->left);
		Destory(root->right);
		delete root;
		root == NULL;
	}
protected:
	Node* _root;
};