#include <iostream>
#include <fstream>
#include <string>
#include "Heap.h"

using namespace std;

void add(int num, MinHeap<int> &highers, MaxHeap<int> &lowers);
void rebalance(MinHeap<int> &highers, MaxHeap<int> &lowers);
void printMedian(MinHeap<int> &highers, MaxHeap<int> &lowers);

int main()
{

	MinHeap<int> highers(0);
	MaxHeap<int> lowers(0);

	string filename;

	cout<< "Please enter the next filename that contains integer stream: " << endl;

	while( getline(cin, filename) )
	{
		ifstream reader;
		reader.open(filename);

		if( !reader.fail() )
		{
			int num;

			while( reader>>num )
			{
				add( num, highers, lowers);
				rebalance( highers, lowers);
			}

			printMedian( highers, lowers);
			
		}

		cout<< "Please enter the next filename that contains integer stream: " << endl;

	}

	return 0;
}

void add(int num, MinHeap<int> &highers, MaxHeap<int> &lowers)
{
	if( lowers.getSize() == 0 || lowers.getTop() > num)
	{
		lowers.insert(num);
	}
	else
	{
		highers.insert(num);
	}
}
void rebalance(MinHeap<int> &highers, MaxHeap<int> &lowers)
{
	if( highers.getSize() - lowers.getSize() > 1 )
	{
		lowers.insert(highers.getTop());
		highers.removeTop();
	}
	else if( highers.getSize() - lowers.getSize() < -1 )
	{
		highers.insert(lowers.getTop());
		lowers.removeTop();
	}
}

void printMedian(MinHeap<int> &highers, MaxHeap<int> &lowers)
{
	if( highers.getSize() > lowers.getSize() )
	{
		cout<< "current median: " <<  highers.getTop() << endl;
	}
	else if( highers.getSize() < lowers.getSize() )
	{
		cout<< "current median: " << lowers.getTop() << endl;
	}
	else
	{
		cout<< "current median: " <<  ( highers.getTop() + lowers.getTop() )/2.0 << endl;
	}

}
