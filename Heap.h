#pragma once
#include<iostream>
#include<vector>
#include<assert.h>
#include<Windows.h>
#include<memory>
using namespace std;
template<class T>
struct Less
{
	bool operator()(const T& l,const T&r)
	{
		return l < r;
	}
};
template<class T>
struct Greator
{
	bool operator()(const T&l, const T&r)
	{
		return l > r;
	}
};
template<class T,class Compare>
class Heap
{
public:
	Heap()
	{}
	Heap(T* a,size_t n)
	{
		_a.reserve(n);
		for (size_t i = 0; i < n; i++)
		{
			_a.push_back(a[i]);
		}
		for (int i = (_a.size()-2) / 2 ; i >= 0; --i)
		{
			AdjustDown(i);
		}
	}
	void Push(const T& i)
	{
		_a.push_back(i);
		AdjustUp(_a.size()-1);
	}
	void Pop()
	{
		assert(!_a.empty());
		swap(_a[0], _a[Size() - 1]);
		_a.pop_back();
		AdjustDown(0);
	}
	const T& Top()const
	{
		assert(!_a.empty());
		return _a[0];
	}
	size_t Size()
	{
		return _a.size();
	}
protected:
	void AdjustDown(int parent)
	{
		Compare ptr;
		int child = parent * 2 + 1;
		while ( child < _a.size() && _a[child]  )
		{
			if ((child + 1)<_a.size() && ptr(_a[child+1],_a[child]))
			{
				child++;
			}
			if (ptr(_a[child],_a[parent]))
			{
				swap(_a[parent], _a[child]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}

	void AdjustUp(int child)
	{
		Compare fun;
		int parent = (child - 1)>>1;
		while (child > 0)
		{
			if (fun(_a[child],_a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1)>>1;
			}
			else
			{
				break;
			}
		}
	}
protected:
	vector<T> _a;
};
//void AdjustDowm(int* heap,int k,int parent)
//{
//	int child = parent * 2 + 1;
//	while (child < k)
//	{
//		if (heap[child + 1] && heap[child] > heap[child + 1])
//		{
//			child++;
//		}
//		if (heap[child] > heap[parent])
//		{
//			swap(heap[child], heap[parent]);
//			parent = child;
//			child = parent * 2 + 1;
//		}
//		else
//		{
//			break;
//		}
//	}
//}
//void Topk(int *a, int k, int n)
//{
//	int* heap = new int[k];
//	for (size_t i = 0; i < k; i++)
//	{
//		heap[i] = a[i];
//	}
//	for (int i = k / 2 - 2; i >= 0; --i)
//	{
//		AdjustDowm(heap, k, i);
//	}
//	for (int i = k; i < n; i++)
//	{
//		if (a[i] > heap[0])
//		{
//			swap(a[i], heap[0]);
//			AdjustDowm(heap,k,0);
//		}
//	}
//}