#include <vector>
#include <string>

using namespace std;

template<class T, class key>
class sort_vector
{
private:

	struct node
	{
		T data;
		key value;

		node(T data, key value)
			:data(data), value(value)
		{}

	};

	vector<node> database;

	int partition (vector<node> &arr, int low, int high);
	void quickSort_helper(vector<node> &arr, int low, int high); 
	void quickSort_starter(vector<node> arr, int low, int high) ;

	void insertionSort_helper(vector<node> arr, int n);

	void mergeSort_helper(vector<node> &arr, int l, int r);
	void merge(vector<node> &arr, int l, int m, int r) ;

	void heapSort_helper(vector<node> arr, int n);
	void heapify(vector<node> &arr, int n, int i);



	void swap(vector<node> &arr, int a, int b)  
	{  
		node temp = arr[a];  
		arr[a] = arr[b];  
		arr[b] = temp;  
	}  

public:

	sort_vector(){}
	~sort_vector()
	{
		database.clear();
	}

	void insert(T data, key value);
	void find(key searchword);
	void quickSort();
	void quickSort_real();
	void insertionSort();
	void mergeSort();
	void heapSort();

	void print()
	{
		for (int i = 0; i < database.size(); i++)
		{
			cout<< database[i].value << endl;
		}
		cout<<"=================="<<endl;
	}


};

template<class T, class key>
void sort_vector<T,key>::insert(T data, key value)
{
	node temp(data,value);
	database.push_back(temp);
}

template<class T, class key>
void sort_vector<T,key>::find(key searchword)
{
	for( int i=0; i<database.size(); i++)
	{
		if( database[i].value < searchword )
		{
			continue;
		}
		else if( database[i].value == searchword )
		{
			break;
			
		}
		else if( (database[i].value).substr(0,searchword.length()) == searchword)
		{
			continue;
		}
		else
		{
			break;
		}

	}
}

template<class T, class key>
void sort_vector<T,key>::quickSort()
{  
	quickSort_starter(database, 0, database.size()-1 );
}  

template<class T, class key>
void sort_vector<T,key>::quickSort_real()
{  
	quickSort_helper(database, 0, database.size()-1 );
}  

template<class T, class key>
void sort_vector<T,key>::quickSort_helper(vector<node> &arr, int low, int high)  
{  
	if (low < high)  
	{  
		int pi = partition(arr, low, high);  
  
		quickSort_helper(arr, low, pi - 1);  
		quickSort_helper(arr, pi + 1, high);  
	}  
} 

template<class T, class key>
void sort_vector<T,key>::quickSort_starter(vector<node> arr, int low, int high)  
{  
	if (low < high)  
	{  
		int pi = partition(arr, low, high);  
  
		quickSort_helper(arr, low, pi - 1);  
		quickSort_helper(arr, pi + 1, high);  
	}  
} 

template<class T, class key>
int sort_vector<T,key>::partition (vector<node> &arr, int low, int high)  
{  
	key pivot = arr[high].value;
	
	 
	int i = (low - 1);  
  
	for (int j = low; j <= high - 1; j++)  
	{    
		if (arr[j].value < pivot)  
		{  
			i++;
			swap(arr, i, j);  
		}  
	}  
	swap(arr, i + 1, high);  
	return (i + 1);  
} 

template<class T, class key>
void sort_vector<T,key>::insertionSort()
{
	insertionSort_helper(database,database.size() );
}

template<class T, class key>
void sort_vector<T,key>::insertionSort_helper(vector<node> arr, int n)  
{  
    int i, j;  
	key key;

    for (i = 1; i < n; i++) 
    {  
        key = arr[i].value;  
        j = i - 1;  
  
        while (j >= 0 && arr[j].value > key) 
        {  
            arr[j + 1] = arr[j];  
            j = j - 1;  
        }  
        arr[j + 1].value = key;  
    }  
}  

template<class T, class key>
void sort_vector<T,key>::mergeSort()
{
	vector<node> temp = database;
	mergeSort_helper(temp, 0, database.size()-1 );
}

template<class T, class key>
void sort_vector<T,key>:: merge(vector<node> &arr, int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 

    vector<node> L, R; 

    for (int i = 0; i < n1; i++) 
        L.push_back(arr[l + i]);
    for (int j = 0; j < n2; j++) 
        R.push_back(arr[m + 1+ j]); 
  

    i = 0; 
	j = 0; 
    k = l; 

    while (i < n1 && j < n2) 
    { 
        if (L[i].value <= R[j].value) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 

    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
 
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 


template<class T, class key>
void sort_vector<T,key>::mergeSort_helper(vector<node> &arr, int l, int r) 
{ 
    if (l < r) 
    { 

        int m = l+(r-l)/2; 
  
        mergeSort_helper(arr, l, m); 
        mergeSort_helper(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
} 

template<class T, class key>
void sort_vector<T,key>::heapSort()
{
	heapSort_helper(database, database.size() );
}


template<class T, class key>
void sort_vector<T,key>::heapify(vector<node> &arr, int n, int i) 
{ 
	int largest = i; 
	int l = 2*i + 1; 
	int r = 2*i + 2; 

	if (l < n && arr[l].value > arr[largest].value ) 
		largest = l; 
  
 
	if (r < n && arr[r].value > arr[largest].value ) 
		largest = r; 
  

	if (largest != i) 
	{ 
		swap(arr, i, largest); 

		heapify(arr, n, largest); 
	} 
} 

template<class T, class key>
void sort_vector<T,key>::heapSort_helper(vector<node> arr, int n) 
{ 
    for (int i = n / 2 - 1; i >= 0; i--) 
        heapify(arr, n, i); 
  
    for (int i=n-1; i>0; i--) 
    { 
        swap(arr, 0, i); 
 
        heapify(arr, i, 0); 
    } 
}