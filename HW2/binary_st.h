#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;


template<class T,class key>
class binary_st
{
private:

	template<class T,class key>
	struct bst_str
	{
		T data;
		key value;

		bst_str<T,key> *right, *left;

		int height, balance;


		bst_str(T data, key value)
			:data(data), value(value),
			right(nullptr),left(nullptr),
			height(1), balance(0)
		{}



	};

	bst_str<T,key>* root;

	void destroy(bst_str<T,key>* current);
	void addhelper(bst_str<T,key>* &current, bst_str<T,key>* &new_n,bool &addOk);
	void deletehelper(bst_str<T,key>* &current, key value, bool &delOk);
	void remove(bst_str<T,key>* &root,bst_str<T,key>* &current, bool &delOk);
	void searchhelper( bst_str<T,key>* &current, key item, bool &isFound);
	void in_order( bst_str<T,key>* current, ofstream &writer);
	void pre_order( bst_str<T,key>* current, ofstream &writer);
	void schematic( bst_str<T,key>* current, string message, ofstream &writer);

	int right_height(bst_str<T,key>* current);
	int left_height(bst_str<T,key>* current);
	int max(int x, int y);
	int height(bst_str<T,key>* current);
	int balancefactor(bst_str<T,key>* current);
	void ComputeBalance(bst_str<T,key>* &current);
	void balanceTraverse(bst_str<T,key>*& current, bool &balance);

public:

	binary_st();
	~binary_st();
	void add(T data, key value,bool &addOk);
	void search(key item);
	void del(key value, bool &delOk);
	void print_in_order();
	void print_pre_order();
	void draw_tree();
	void check_height(int &right, int &left);
	bool isBalanced();
	
};

template<class T,class key>
binary_st<T,key>::binary_st()
{
	root = nullptr;
}



template<class T,class key>
binary_st<T,key>::~binary_st()
{
	destroy(root);
}

template<class T,class key>
void binary_st<T,key>::destroy(bst_str<T,key>* current)
{
	if( current->right != nullptr)
	{
		destroy(current->right);
	}
	if( current->left != nullptr)
	{
		destroy(current->left);
	}

	delete current;

}



template<class T,class key>
void binary_st<T,key>::add(T data, key value, bool &addOk)
	{
		bst_str<T,key>* temp = new bst_str<T,key>(data, value);
		addOk = true;

		if(root == nullptr)
		{
			root = temp;
		}
		else
		{
			addhelper(root, temp, addOk);
		}
	}


template<class T,class key>
void binary_st<T,key>::addhelper(bst_str<T,key>* &current, bst_str<T,key>* &new_n, bool &addOk)
{
	if( new_n->value > current->value )
	{
		if(current->right)
		{
			addhelper(current->right, new_n, addOk);
		}
		else
		{
			current->right = new_n;
		}

	}
	else if( new_n->value < current->value )
	{
		if(current->left)
		{
			addhelper(current->left, new_n, addOk);
		}
		else
		{
			current->left = new_n;
		}
	}
	else
	{
		cout<< "The given contact full name already exists in database."  << endl;
		addOk = false;
	}

	ComputeBalance(current);
}


template<class T,class key>
void binary_st<T,key>::del(key value, bool &delOk)
{
	delOk = false;

	if(root == nullptr)
	{
		cout<< "Phonebook is empty." << endl;
	}
	else
	{
		deletehelper(root, value, delOk);
	}

}

template<class T,class key>
void binary_st<T,key>::deletehelper(bst_str<T,key>* &current, key value,  bool &delOk)
{
	if( current)
	{
		if( value == current->value)
		{
			bst_str<T,key>* temp = current;

			if (!current->right)
			{
				current = current->left;
				delOk = true;
				delete temp;
			}
			else if (!current->left)
			{
				current = current->right;
				delOk = true;
				delete temp;
			}
			else
			{
				remove(current, current->left, delOk);

				if (delOk)
				{
					ComputeBalance(current);
				}

				delOk = true;
			}

		}
		else if( value < current->value )
		{
			deletehelper( current->left, value, delOk);

			if(delOk)
			{
				ComputeBalance(current);
			}
		}
		else
		{
			deletehelper( current->right, value, delOk);

			if(delOk)
			{
				ComputeBalance(current);
			}
		}
	}
	else
	{
		cout << "Contact cannot be found to be deleted." << endl;
	}
}

template<class T,class key>
void binary_st<T,key>::remove(bst_str<T,key>* &root,bst_str<T,key>* &current, bool &delOk)
{
	if (!current->right)
	{
		root->value = current->value;
		root->data = current->data;
		bst_str<T,key>* temp = current;
		current = current ->left;
		delete temp;
		delOk = true;
	}
	else
	{
		remove(root, current->right, delOk);

		if (delOk)
		{
			ComputeBalance(root);
		}
	}
}


template<class T,class key>
void binary_st<T,key>:: ComputeBalance(bst_str<T,key>* &current)
{
	if (current)
	{
		short leftDepth = current->left ? current->left->height : 0;
		short rightDepth = current->right ? current->right->height : 0;

		current->height = 1 + ((leftDepth > rightDepth) ? leftDepth : rightDepth);
		current->balance = rightDepth - leftDepth;
	}
}


template<class T,class key>
void binary_st<T,key>:: search(key item)
{
	bool isFound = false;

	if(root)
	{
		
		searchhelper( root, item, isFound);
	}

}

template<class T,class key>
void binary_st<T,key>:: searchhelper(bst_str<T,key>* &current, key item, bool &isFound)
{
	if (current)
	{

		if (current->value == item)
		{

			current->data.write(cout);
			isFound = true;
			return;
		}

		else if ((current->value).substr(0,item.length()) == item)
		{
			current->data.write(cout);
			isFound = true;

			if (item < current->value)
			{
				searchhelper(current->right, item , isFound);
			}

		}

		if (item < current->value)
		{
			searchhelper(current->left, item , isFound);
		}

		else
		{
			searchhelper(current->right, item , isFound);
		}
	}

	else if (!isFound)
	{
		cout<< "There is no contact in phonebook with information: " << item << endl;
	}
}

template<class T,class key>
void binary_st<T,key>:: print_in_order()
{
	ofstream writer;
	writer.open( "phonebookInOrderBST.txt" );
	
	if(root)
	{
		in_order(root, writer);
	}
	else
	{
		writer<< "Tree is empty..." << endl;
	}


	writer.close();
}

template<class T,class key>
void binary_st<T,key>::in_order( bst_str<T,key>* current, ofstream &writer)
{
	if (current)
	{
		in_order(current->left,writer);
		current->data.write(writer);
		in_order(current->right,writer);
	}
}

template<class T,class key>
void binary_st<T,key>:: print_pre_order()
{
	ofstream writer;
	writer.open( "phonebookPreOrderBST.txt" );

	if(root)
	{
		pre_order(root, writer);
	}
	else
	{
		writer<< "Tree is empty..." << endl;
	}

	writer.close();
}

template<class T,class key>
void binary_st<T,key>::pre_order( bst_str<T,key>* current, ofstream &writer)
{
	if (current)
	{
		current->data.write(writer);
		pre_order(current->left,writer);
		pre_order(current->right,writer);
		
	}
}

template<class T,class key>
void binary_st<T,key>:: draw_tree()
{
	ofstream writer;
	writer.open( "phonebookTreeBST.txt" );

	if(root != nullptr)
	{
		string message = "    ";

		writer<< "|__ " << root->value << endl;

		if(root->left)
		{
			writer<<message<<"|-- ";
			schematic(root->left,message+ "|   ",writer);
		}
		if(root->right)
		{
			writer<<message<<"|__ ";
			schematic(root->right,message+"    ",writer);
		}
	}
	else
	{
		writer<< "Tree is empty..." << endl;
	}

	writer.close();
}

template<class T,class key>
void binary_st<T,key>::schematic( bst_str<T,key>* current, string message, ofstream &writer)
{
	writer<< current->value << endl;
	if(current->left)
	{
		writer<<message<<"|-- ";
		schematic(current->left,message+ "|   ",writer);
	}
	if(current->right)
	{
		writer<<message<<"|__ ";
		schematic(current->right,message+"    ",writer);
	}
}

template<class T,class key>
void binary_st<T,key>::check_height(int &right, int &left)
{
	if(root)
	{
		right = right_height(root);
		left = left_height(root);
	}
	else
	{
		right=0;
		left=0;
	}
}

template<class T,class key>
int binary_st<T,key>::right_height(bst_str<T,key>* current)
{
	return current->right ? current->right->height :0 ;
}

template<class T,class key>
int binary_st<T,key>::left_height(bst_str<T,key>* current)
{
	return current->left ?current->left->height :0;
}

template<class T,class key>
int binary_st<T,key>::max( int x, int y)
{
	return (x > y) ? x : y;
}

template<class T,class key>
int binary_st<T,key>::height(bst_str<T,key>* current)
{
	if( current == nullptr)
	{
		return 0;
	}
	else
	{
		return current->height;
	}
}

template<class T,class key>
int binary_st<T,key>:: balancefactor(bst_str<T,key>* current)
{
if(current)
		return current->balance;
	else
		return 0;
}

template<class T,class key>
bool binary_st<T,key>::isBalanced()
{
	bool balance = true;

	balanceTraverse(root, balance);

	return balance;
}

template<class T,class key>
void binary_st<T,key>::balanceTraverse(bst_str<T,key>*& current, bool &balance)
{
	if (current)
	{
		balanceTraverse(current->left,balance);
		
		if(current->balance <-1 || current->balance > 1)
		{
			balance = false;
		}

		balanceTraverse(current->right,balance);
	}
}