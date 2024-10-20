#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include "avl_tree.h"
#include "binary_st.h"
#include "sort_vector.h"

using namespace std;

struct phonebook
{
	string name;
	string phonenum;
	string city;

	phonebook(string name, string num, string city)
		:name(name), phonenum(num), city(city)
	{}

};

void file_read(string filename, sort_vector<phonebook,string> &quick_vector, 
			   sort_vector<phonebook,string> &insertion_vector,
			   sort_vector<phonebook,string> &merge_vector, 
			   sort_vector<phonebook,string> &heap_vector,
			   avl_tree<phonebook, string> &avl_database,
			   binary_st<phonebook, string> &bst_database);


int main()
{
	string filename, searchname;

	cout << "Please enter the contact file name: " << endl;
	cin >> filename;

	cout << "Please enter the word to be queried: " << endl;
	cin.clear();
	cin.ignore();
	getline(cin,searchname);
	cin.clear();

	sort_vector<phonebook,string> quick_vector;
	sort_vector<phonebook,string> insertion_vector;
	sort_vector<phonebook,string> merge_vector;
	sort_vector<phonebook,string> heap_vector;

	avl_tree<phonebook, string> avl_database;
	binary_st<phonebook, string> bst_database;

	file_read(filename,quick_vector,insertion_vector,merge_vector,heap_vector,avl_database,bst_database);


	auto start_quick = chrono::steady_clock::now();
	for(int i=0; i<5; i++)
	{
		quick_vector.quickSort();
	}
	auto end_quick = chrono::steady_clock::now();


	auto start_insertion = chrono::steady_clock::now();
	for(int i=0; i<5; i++)
	{
		insertion_vector.insertionSort();
	}
	auto end_insertion = chrono::steady_clock::now();


	auto start_merge = chrono::steady_clock::now();
	for(int i=0; i<5; i++)
	{
		merge_vector.mergeSort();
	}
	auto end_merge = chrono::steady_clock::now();


	auto start_heap = chrono::steady_clock::now();
	for(int i=0; i<5; i++)
	{
		heap_vector.heapSort();
	}
	auto end_heap = chrono::steady_clock::now();

	float quick = chrono::duration_cast<chrono::nanoseconds>(end_quick-start_quick).count()/5;
	float insertion = chrono::duration_cast<chrono::nanoseconds>(end_insertion-start_insertion).count()/5;
	float merge = chrono::duration_cast<chrono::nanoseconds>(end_merge-start_merge).count()/5;
	float heap = chrono::duration_cast<chrono::nanoseconds>(end_heap-start_heap).count()/5;


	cout << "Sorting the vector copies" << endl;
	cout << "======================================" << endl;
	cout << "Quick Sort Time: "<< chrono::duration_cast<chrono::nanoseconds>(end_quick-start_quick).count()/5 <<" Nanoseconds" << endl;
	cout << "Insertion Sort Time: "<< chrono::duration_cast<chrono::nanoseconds>(end_insertion-start_insertion).count()/5 <<" Nanoseconds" << endl;
	cout << "Merge Sort Time: "<< chrono::duration_cast<chrono::nanoseconds>(end_merge-start_merge).count()/5 <<" Nanoseconds" << endl;
	cout << "Heap Sort Time: "<< chrono::duration_cast<chrono::nanoseconds>(end_heap-start_heap).count()/5 <<" Nanoseconds" << endl;

	quick_vector.quickSort_real();


	auto start_avlsearch = chrono::steady_clock::now();
	for(int i=0; i<1000; i++)
	{
		avl_database.search(searchname);
	}
	auto end_avlsearch = chrono::steady_clock::now();


	auto start_bstsearch = chrono::steady_clock::now();
	for(int i=0; i<1000; i++)
	{
		bst_database.search(searchname);
	}
	auto end_bstsearch = chrono::steady_clock::now();


	auto start_binarysearch = chrono::steady_clock::now();
	for(int i=0; i<1000; i++)
	{
		quick_vector.find(searchname);
	}
	auto end_binarysearch = chrono::steady_clock::now();

	float bst_search = chrono::duration_cast<chrono::nanoseconds>(end_bstsearch-start_bstsearch).count()/1000;
	float avl_search = chrono::duration_cast<chrono::nanoseconds>(end_avlsearch-start_avlsearch).count()/1000;
	float binary_search = chrono::duration_cast<chrono::nanoseconds>(end_binarysearch-start_binarysearch).count()/1000;

	cout << "Searching for Shane" << endl;
	cout << "======================================" << endl;
	cout << "The search in BST took "<< chrono::duration_cast<chrono::nanoseconds>(end_bstsearch-start_bstsearch).count()/1000 <<" Nanoseconds" << endl;
	cout << "The search in AVL took "<< chrono::duration_cast<chrono::nanoseconds>(end_avlsearch-start_avlsearch).count()/1000 <<" Nanoseconds" << endl;
	cout << "Binary Search Time: "<< chrono::duration_cast<chrono::nanoseconds>(end_binarysearch-start_binarysearch).count()/1000 <<" Nanoseconds" << endl;


	cout << "SpeedUps in search" << endl;
	cout << "======================================" << endl;
	cout << "(BST / AVL) SpeedUp = " 
		<< bst_search/avl_search
		<< endl;
	cout << "(Binary Search / AVL) SpeedUp = " 
		<< binary_search/avl_search
		<< endl;
	cout << "(Binary Search / BST) SpeedUp = " 
		<< binary_search/bst_search
		<< endl;


	cout << "SpeedUps between Sorting Algorithms" << endl;
	cout << "======================================" << endl;
	cout << "(Insertion Sort/ Quick Sort) SpeedUp = " 
		<< insertion/quick
		<< endl;

	cout << "(Merge Sort / Quick Sort) SpeedUp = " 
		<< merge/quick
		<< endl;

	cout << "(Heap Sort / Quick Sort) SpeedUp = " 
		<< heap/quick
		<< endl;



	return 0;
}

void file_read(string filename, sort_vector<phonebook,string> &quick_vector, 
			   sort_vector<phonebook,string> &insertion_vector,
			   sort_vector<phonebook,string> &merge_vector, 
			   sort_vector<phonebook,string> &heap_vector,
			   avl_tree<phonebook, string> &avl_database,
			   binary_st<phonebook, string> &bst_database)
{

	ifstream reader;
	reader.open( filename );

	string w1, w2, name, num, city;
	bool check=false;

	while( reader>>w1 )
	{
		reader>> w2 >> num >> city;

		name = w1 + " " + w2;

		phonebook temp(name, num, city);

		quick_vector.insert(temp,name);
		insertion_vector.insert(temp,name);
		merge_vector.insert(temp,name);
		heap_vector.insert(temp,name);

		avl_database.add(temp,name,check);
		bst_database.add(temp,name,check);
	}

	reader.close();

}