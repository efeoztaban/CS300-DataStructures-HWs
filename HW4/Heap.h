#include <string>
#include <vector>

using namespace std;

template <class T>
class Heap{

public:

	Heap(){}
	~Heap(){}

	virtual void insert(T element) = 0;
	virtual void removeTop() = 0;

	T getTop()
	{
		return heap[1];
	}

	bool isEmpty()
	{
		return size==0;
	}
	int getSize()
	{
		return size;
	}


protected:

	vector<T> heap;
	int size;

	void makeEmpty()
	{
		heap.clear();
	}

	void swap_elements(int index1, int index2)
	{
		T temp = heap[index1];
		heap[index1] = heap[index2];
		heap[index2] = temp;
	}

	int max(int index1, int index2)
	{
		if(index1<= size && index2<= size)
		{
			if( heap[index1] < heap[index2] )
			{
				return index2;
			}
			else
			{
				return index1;
			}
		}
		else if(index1<= size)
		{
			return index1;
		}
		else if(index2<= size)
		{
			return index2;
		}
		else
		{
			return 0;
		}
	}

	int min(int index1, int index2)
	{
		if(index1<= size && index2<= size)
		{
			if( heap[index1] < heap[index2] )
			{
				return index1;
			}
			else
			{
				return index2;
			}
		}
		else if(index1<= size)
		{
			return index1;
		}
		else if(index2<= size)
		{
			return index2;
		}
		else
		{
			return 0;
		}
	}


	int ancestor_index(int index)
	{
		return int(index/2) ;
	}
	int leftChild_index(int index)
	{
		return index*2;
	}
	int rightChild_index(int index)
	{
		return (index*2)+1;
	}
	int sibling_index(int index)
	{
		if( index%2==0 )
		{
			return index+1;
		}
		else
		{
			return index-1;
		}
	}


	T ancestor(int index)
	{
		return heap[ancestor_index(index)];
	}
	T leftChild(int index)
	{
		return heap[leftChild_index(index)];
	}
	T rightChild(int index)
	{
		return heap[rightChild_index(index)];
	}
	T sibling(int index)
	{
		return heap[sibling_index(index)];
	}


};

template <class T>
class MaxHeap: public Heap<T>
{
public:

	MaxHeap(T random)
	{
		heap.push_back(random);
		size=0;
	}
	~MaxHeap()
	{
		makeEmpty();
	}

	void insert(T element)
	{
		size++;
		int index = size;

		heap.push_back(element);
		
		while(index !=1 && heap[index] > heap[ancestor_index(index)] )
		{
			swap_elements(index, ancestor_index(index));

			index = ancestor_index(index);
		}

		
	}
	void removeTop()
	{
		heap[1] = heap[size];
		heap.pop_back();
		size--;

		int index=1;

		while( max( rightChild_index(index), leftChild_index(index) ) && heap[index] < heap[max( rightChild_index(index), leftChild_index(index) )] )
		{
			int max_ind = max( rightChild_index(index), leftChild_index(index) );

			swap_elements( index, max_ind );

			index = max_ind;
		}
	}


};

template <class T>
class MinHeap: public Heap<T>
{
public: 

	MinHeap(T random)
	{
		heap.push_back(random);
		size=0;
	}
	~MinHeap()
	{
		makeEmpty();
	}


	void insert(T element)
	{
		size++;
		int index = size;

		heap.push_back(element);
		
		while(index !=1 && heap[index] < heap[ancestor_index(index)] )
		{
			swap_elements(index, ancestor_index(index));

			index = ancestor_index(index);
		}

		
	}

	void removeTop()
	{
		heap[1] = heap[size];
		heap.pop_back();
		size--;

		 
		int index=1;

		while(min( rightChild_index(index), leftChild_index(index) ) && heap[index] > heap[min( rightChild_index(index), leftChild_index(index) )] )
		{
			int min_ind = min( rightChild_index(index), leftChild_index(index) );

			swap_elements( index, min_ind );

			index = min_ind;
		}
	}
};