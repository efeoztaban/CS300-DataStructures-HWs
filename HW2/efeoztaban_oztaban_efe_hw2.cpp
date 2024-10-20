#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "binary_st.h"
#include "avl_tree.h"

using namespace std;
struct phonebook
{
	string name_surname, phone_num, city;

	phonebook(string name, string num, string city)
		:name_surname(name),phone_num(num),city(city)
	{}

	void write(ofstream &writer)
	{
		writer<< name_surname << " " << phone_num << " " << city << endl ;
	}
	void write(ostream &writer)
	{
		writer<< name_surname << " " << phone_num << " " << city << endl ;
	}
};

void file_read(binary_st<phonebook,string> &database_bst,avl_tree<phonebook, string> &database_avl, string file_name);
void seaching(binary_st<phonebook, string> &database_bst,avl_tree<phonebook, string> &database_avl);
void adding(binary_st<phonebook, string> &database_bst,avl_tree<phonebook, string> &database_avl);
void deleting(binary_st<phonebook, string> &database_bst,avl_tree<phonebook, string> &database_avl);
void printing(binary_st<phonebook, string> &database_bst,avl_tree<phonebook, string> &database_avl);
void drawing(binary_st<phonebook, string> &database_bst,avl_tree<phonebook, string> &database_avl);

int main()
{
	string file_name;
	bool check = true;
	

	cout<< "Please enter the contact file name: " << endl;
	cin>> file_name;

	binary_st<phonebook, string> database_bst;
	avl_tree<phonebook, string> database_avl;

	file_read(database_bst, database_avl, file_name);

	while (check)
	{
		
		cout<< "Choose which action to perform from 1 to 6: " << endl;

		cout<< "1 - Search a phonebook contact" << endl;
		cout<< "2 - Add a phonebook contact" << endl;
		cout<< "3 - Delete a phonebook contact" << endl;
		cout<< "4 - Print the phonebook to a file (inorder)"<< endl 
			<< "    Print the phonebook to a file (preorder)" << endl;
		cout<< "5 - Draw the phonebook as a tree to a file" << endl;
		cout<< "6 - Press 6 to exit" << endl;

		string choice; 

		cin.clear();
		cout<< "choice: ";
		cin >> choice;


		if (choice ==  "1")
			seaching(database_bst,database_avl);

		else if (choice ==  "2")
			adding(database_bst,database_avl);

		else if (choice ==  "3")
			deleting(database_bst,database_avl);

		else if (choice ==  "4")
			printing(database_bst,database_avl);

		else if (choice ==  "5")
			drawing(database_bst,database_avl);
		
		else if (choice ==  "6")
		{
			check = false;
			cout<< "Exiting..." << endl;
		}

		else
			cout << "Invalid choice" << endl << endl;
		

		
	}

	return 0;
}

void file_read(binary_st<phonebook,string> &database_bst, avl_tree<phonebook, string> &database_avl, string file_name)
{
	ifstream reader;
	string line, name, surname, pnum, city;
	bool bst=true, avl=true;

	reader.open(file_name);

	while(reader.fail())
	{
		cout<<"There is no file named " << file_name << " in the directory." << endl
			<<"Please enter the contact file name: " << endl;

		cin>>file_name;

		reader.open(file_name);
	}

	
	cout << endl ;
	cout <<	"Loading the phonebook into a BST..." << endl; 


	auto start_bst = chrono::steady_clock::now();

	if( !reader.fail() )
	{
		while( getline(reader, line) )
		{
			istringstream ss(line);

			ss >> name >> surname >> pnum >> city;

				phonebook new_add(name+" "+surname,pnum,city);

				database_bst.add(new_add, name+" "+surname, bst);
		}
	}

	reader.close();

	auto end_bst = chrono::steady_clock::now();

	cout<< endl << "Phonebook creation in BST took " 
		<<chrono::duration_cast<chrono::nanoseconds>(end_bst-start_bst).count()
		<< " nanoseconds..." << endl << endl;

	bool bst_check = database_bst.isBalanced();

	if(bst_check)
	{
		cout<< "Tree is balanced. " << endl << endl;
	}
	else
	{
		cout<< "Tree is not balanced. " << endl << endl;
	}


	int right_bst, left_bst;
	database_bst.check_height(right_bst, left_bst);

	cout<< "The heights of BST are for left: " <<  left_bst << " and right: " <<  right_bst << endl ;

	cout<< "===========================================" << endl <<endl;

	cout <<	"Loading the phonebook into a AVL..." << endl; 

	reader.open(file_name);

	auto start_avl = chrono::steady_clock::now();

	if( !reader.fail() )
	{
		while( getline(reader, line) )
		{
			istringstream ss(line);

			ss >> name >> surname >> pnum >> city;

				phonebook new_add(name+" "+surname,pnum,city);

				database_avl.add(new_add, name+" "+surname, avl);
		}
	}

	reader.close();

	auto end_avl = chrono::steady_clock::now();

	cout<< endl << "Phonebook creation in AVL took " 
		<<chrono::duration_cast<chrono::nanoseconds>(end_avl-start_avl).count()
		<< " nanoseconds..." << endl << endl;

	bool avl_check = database_avl.isBalanced();

	if(avl_check)
	{
		cout<< "Tree is balanced. " << endl << endl;
	}
	else
	{
		cout<< "Tree is not balanced. " << endl << endl;
	}

	int right_avl, left_avl;
	database_avl.check_height(right_avl, left_avl);

	cout<< "The heights of AVL are for left: " <<  left_avl << " and right: " <<  right_avl << endl;

	cout << endl << endl;

}

void seaching(binary_st<phonebook, string> &database_bst, avl_tree<phonebook, string> &database_avl)
{
	
	cout << endl << endl;

	string input;
	vector<phonebook> result;

	cout<< "Search for a contact: " << endl ;

	cin.clear();
	cin.ignore();

	getline(cin,input);

	cout<< "Searching an item in the phonebook (BST) ..." << endl;
	cout<< "=============================================" << endl;
	cout<< "Phonebook : Searching for : (" << input << ")" << endl;
	cout<< "============================" << endl<< endl;
	
	auto start_bst = chrono::steady_clock::now();
	database_bst.search(input);
	auto end_bst = chrono::steady_clock::now();

	cout<< endl;

	cout<< "Searching an item in the phonebook (AVL) ..." << endl;
	cout<< "============================================="  << endl;
	cout<< "Phonebook : Searching for : (" << input << ")" << endl;
	cout<< "============================"<< endl<< endl;

	auto start_avl = chrono::steady_clock::now();
	database_avl.search(input);
	auto end_avl = chrono::steady_clock::now();
	
	cout << endl;

	cout<<"The search BST took " 
		<< chrono::duration_cast<chrono::nanoseconds>(end_bst-start_bst).count()
		<< " nanoseconds..." << endl << endl;

	cout<<"The search AVL took " 
		<< chrono::duration_cast<chrono::nanoseconds>(end_avl-start_avl).count()
		<< " nanoseconds..." << endl << endl;

	cout << endl << endl;
}


void adding(binary_st<phonebook, string> &database_bst, avl_tree<phonebook, string> &database_avl)
{
	bool bst=true, avl=true;

	cout << endl << endl;

	string name, num, city;

	cout << "Enter the fullname of the contact to be added: " << endl;

	cout << "Name: ";
	cin.clear();
	cin.ignore();

	getline(cin,name);

	cout<< "Tel: ";
	cin>>num;

	cout<< "City: ";
	cin>>city;

	phonebook new_add(name,num,city);


	cout << "Adding an item from the phonebook (BST)..." << endl;
	cout << "==========================================" << endl;

	auto start_bst = chrono::steady_clock::now();
	database_bst.add(new_add, name, bst);
	auto end_bst = chrono::steady_clock::now();

	if(bst)
	{
		cout<< "Contact has been added succesfully to the BST" << endl ;
	}

	cout << endl;
	cout << "Adding an item from the phonebook (AVL)..." << endl;
	cout << "==========================================" << endl;

	auto start_avl = chrono::steady_clock::now();
	database_avl.add(new_add, name, avl);
	auto end_avl = chrono::steady_clock::now();

	if(avl)
	{
		cout<< "Contact has been added succesfully to the AVL" << endl;
	}
		
	cout<< endl;
	cout<<"Adding a contact to the Binary Tree took " 
		<< chrono::duration_cast<chrono::nanoseconds>(end_bst-start_bst).count()
		<< " nanoseconds..." << endl << endl;

	cout<<"Adding a contact to the AVL Tree took " 
		<< chrono::duration_cast<chrono::nanoseconds>(end_avl-start_avl).count()
		<< " nanoseconds..." << endl << endl;
	
	cout << endl << endl;
}

void deleting(binary_st<phonebook, string> &database_bst, avl_tree<phonebook, string> &database_avl)
{
	bool bst=true, avl=true;

	cout << endl << endl;

	string item; 

	cout << "Deleting an item from the phonebook ..." << endl;
	cout << "========================" << endl;
	cout << "Enter the fullname of the contact to be deleted: " << endl;

	cin.clear();
	cin.ignore();

	getline(cin,item);

	cout<< endl;

	auto start_bst = chrono::steady_clock::now();
	database_bst.del(item,bst);
	auto end_bst = chrono::steady_clock::now();

	auto start_avl = chrono::steady_clock::now();
	database_avl.del(item,avl);
	auto end_avl = chrono::steady_clock::now();

	if(avl && bst)
	{
		cout<< "Deleted succesfully." << endl ;
	}

	cout<< endl;

	cout<<"Deleting from Binary Tree took " 
		<< chrono::duration_cast<chrono::nanoseconds>(end_bst-start_bst).count()
		<< " nanoseconds..." << endl << endl;

	cout<<"Deleting from AVL Tree took " 
		<< chrono::duration_cast<chrono::nanoseconds>(end_avl-start_avl).count()
		<< " nanoseconds..." << endl << endl;

	cout << endl << endl;

}
void printing(binary_st<phonebook, string> &database_bst, avl_tree<phonebook, string> &database_avl)
{

	cout << endl << endl;

	cout << "Saving the phonebook to a file (In-Order)..." << endl;
	cout << "===========================================" << endl;

	auto start_in_bst = chrono::steady_clock::now();
	database_bst.print_in_order();
	auto end_in_bst = chrono::steady_clock::now();

	
	auto start_in_avl = chrono::steady_clock::now();
	database_avl.print_in_order();
	auto end_in_avl = chrono::steady_clock::now();

	cout<< "Saving the phonebook took (BST In-Order)" 
		<<	chrono::duration_cast<chrono::nanoseconds>(end_in_bst-start_in_bst).count()
		<<" nanoseconds..." << endl << endl;

	cout<< "Saving the phonebook took (AVL In-Order)" 
		<<	chrono::duration_cast<chrono::nanoseconds>(end_in_avl-start_in_avl).count()
		<<" nanoseconds..." << endl << endl << endl;

	cout << "Saving the phonebook to a file (Pre-Order)..." << endl;
	cout << "===========================================" << endl;

	auto start_bst = chrono::steady_clock::now();
	database_bst.print_pre_order();
	auto end_bst = chrono::steady_clock::now();

	auto start_avl = chrono::steady_clock::now();
	database_avl.print_pre_order();
	auto end_avl = chrono::steady_clock::now();

	cout<< "Saving the phonebook took (BST Pre-Order)" 
		<<	chrono::duration_cast<chrono::nanoseconds>(end_bst-start_bst).count()
		<<" nanoseconds..." << endl << endl;

	cout<< "Saving the phonebook took (AVL Pre-Order)" 
		<<	chrono::duration_cast<chrono::nanoseconds>(end_avl-start_avl).count()
		<<" nanoseconds..." << endl << endl;

	cout << endl << endl;

}
void drawing(binary_st<phonebook, string> &database_bst, avl_tree<phonebook, string> &database_avl)
{

	cout << endl << endl;

	cout << "Drawing the phonebook as a tree to a file..." << endl;
	cout << "===========================================" << endl;

	auto start_bst = chrono::steady_clock::now();
	database_bst.draw_tree();
	auto end_bst = chrono::steady_clock::now();

	auto start_avl = chrono::steady_clock::now();
	database_avl.draw_tree();
	auto end_avl = chrono::steady_clock::now();

	cout<< endl;

	cout<< "Drawing the phonebook as a tree (BST) took " 
		<<	chrono::duration_cast<chrono::nanoseconds>(end_bst-start_bst).count()
		<<" nanoseconds..." << endl << endl;

	cout<< "Drawing the phonebook as a tree (AVL) took " 
		<<	chrono::duration_cast<chrono::nanoseconds>(end_avl-start_avl).count()
		<<" nanoseconds..." << endl << endl;

	cout << endl << endl;
	
}
