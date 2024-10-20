#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;


template<class T,class key>
class avl_tree
{
private:

	template<class T,class key>
	struct avl_str
	{
		T data;
		key value;

		avl_str<T,key> *right, *left;

		int height, balance;


		avl_str(T data, key value)
			:data(data), value(value),
			right(nullptr),left(nullptr),
			height(1), balance(0)
		{}

	};

	avl_str<T,key>* root;

	void destroy(avl_str<T,key>* current);
	void addhelper(avl_str<T,key>* &current, avl_str<T,key>* &new_n, bool &addOk);
	void deletehelper(avl_str<T,key>* &current, key value, bool &delOk);
	void remove(avl_str<T,key>* &root,avl_str<T,key>* &current, bool &delOk);
	void searchhelper( avl_str<T,key>* &current, key item, bool &isFound);
	void in_order( avl_str<T,key>* current, ofstream &writer);
	void pre_order( avl_str<T,key>* current, ofstream &writer);
	void schematic( avl_str<T,key>* current, string message, ofstream &writer);


	int right_height(avl_str<T,key>* current);
	int left_height(avl_str<T,key>* current);
	int max(int x, int y);
	int height(avl_str<T,key>* current);
	int balancefactor(avl_str<T,key>* current);
	void ComputeBalance(avl_str<T,key>* &current);
	void balanceTraverse(avl_str<T,key>*& current, bool &balance);

	void Balance(avl_str<T,key>*& current);
	void BalanceRight(avl_str<T,key>*& current);
	void BalanceLeft(avl_str<T,key>*& current);
	void RotateLeft(avl_str<T,key>*& current);
	void RotateRight(avl_str<T,key>*& current);


public:

	avl_tree();
	~avl_tree();
	void add(T data, key value, bool &addOk);
	void search(key item);
	void del(key value, bool &delOk);
	void print_in_order();
	void print_pre_order();
	void draw_tree();
	void check_height(int &right, int &left);
	bool isBalanced();

};

template<class T,class key>
avl_tree<T,key>::avl_tree()
{
	root = nullptr;
}


template<class T,class key>
avl_tree<T,key>::~avl_tree()
{
	destroy(root);
}

template<class T,class key>
void avl_tree<T,key>::destroy(avl_str<T,key>* current)
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
void avl_tree<T,key>::add(T data, key value, bool &addOk)
	{

		avl_str<T,key>* temp = new avl_str<T,key>(data, value);
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
void avl_tree<T,key>::addhelper(avl_str<T,key>* &current, avl_str<T,key>* &new_n, bool &addOk)
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
	else if( new_n->value < current->value)
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
		cout<< "The given contact full name already exists in database." << endl;
		addOk = false;
	}

	ComputeBalance(current);
	Balance(current);
}

template<class T,class key>
void avl_tree<T,key>::del(key value, bool &delOk)
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
void avl_tree<T,key>::deletehelper(avl_str<T,key>* &current, key value,  bool &delOk)
{
	if( current)
	{
		if( value == current->value)
		{
			avl_str<T,key>* temp = current;

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
					Balance(current);
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
				Balance(current);
			}
		}
		else
		{
			deletehelper( current->right, value, delOk);

			if(delOk)
			{
				ComputeBalance(current);
				Balance(current);
			}
		}
	}
	else
	{
		cout << "Contact cannot be found to be deleted." << endl;
	}
}

template<class T,class key>
void avl_tree<T,key>::remove(avl_str<T,key>* &root,avl_str<T,key>* &current, bool &delOk)
{
	if (!current->right)
	{
		root->value = current->value;
		root->data = current->data;
		avl_str<T,key>* temp = current;
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
			Balance(root);
		}
	}
}


template<class T,class key>
void avl_tree<T,key>:: ComputeBalance(avl_str<T,key>* &current)
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
void avl_tree<T,key>:: search(key item)
{
	bool isFound = false;

	if(root)
	{
		searchhelper( root, item, isFound);
	}

}

template<class T,class key>
void avl_tree<T,key>:: searchhelper(avl_str<T,key>* &current, key item, bool &isFound)
{
	
	if (current)
	{

		if (current->value == item)
		{
			isFound = true;
			return;
		}

		else if ((current->value).substr(0,item.length()) == item)
		{
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
		//cout<< "There is no contact in phonebook with information: " << item << endl;
	}
}

template<class T,class key>
void avl_tree<T,key>:: print_in_order()
{
	ofstream writer;
	writer.open( "phonebookInOrderAVL.txt" );

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
void avl_tree<T,key>::in_order( avl_str<T,key>* current, ofstream &writer)
{
	if (current)
	{
		in_order(current->left,writer);
		current->data.write(writer);
		in_order(current->right,writer);
	}
}

template<class T,class key>
void avl_tree<T,key>:: print_pre_order()
{
	ofstream writer;
	writer.open( "phonebookPreOrderAVL.txt" );

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
void avl_tree<T,key>::pre_order( avl_str<T,key>* current, ofstream &writer)
{
	if (current)
	{
		current->data.write(writer);
		pre_order(current->left,writer);
		pre_order(current->right,writer);
		
	}
}

template<class T,class key>
void avl_tree<T,key>:: draw_tree()
{
	ofstream writer;
	writer.open( "phonebookTreeAVL.txt" );

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
void avl_tree<T,key>::schematic( avl_str<T,key>* current, string message, ofstream &writer)
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
void avl_tree<T,key>::check_height(int &right, int &left)
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
int avl_tree<T,key>::right_height(avl_str<T,key>* current)
{
	return current->right ? current->right->height :0 ;
}

template<class T,class key>
int avl_tree<T,key>::left_height(avl_str<T,key>* current)
{
	return current->left ?current->left->height :0;
}

template<class T,class key>
int avl_tree<T,key>::max( int x, int y)
{
	return (x > y) ? x : y;
}

template<class T,class key>
int avl_tree<T,key>::height(avl_str<T,key>* current)
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
int avl_tree<T,key>:: balancefactor(avl_str<T,key>* current)
{
	if(current)
		return current->balance;
	else
		return 0;
}


template<class T,class key>
void avl_tree<T,key>::Balance(avl_str<T,key>*& current)
{
	
	if (balancefactor(current) > 1)
		BalanceRight(current);

	if (balancefactor(current) < -1)
		BalanceLeft(current);
}

template<class T,class key>
void avl_tree<T,key>::BalanceRight(avl_str<T,key>*& current)
{
	if (current->right)
	{
		if (balancefactor(current->right) >= 0)
		{
			RotateLeft(current);
		}
		else if (balancefactor(current->right) < 0)
		{
			RotateRight(current->right);
			RotateLeft(current);
		}
	}
}

template<class T,class key>
void avl_tree<T,key>::BalanceLeft(avl_str<T,key>*& current)
{
	if (current->left)
	{
		if (balancefactor(current->left) <= 0)
		{
			RotateRight(current);
		}
		else if (balancefactor(current->left)  > 0)
		{
			RotateLeft(current->left);
			RotateRight(current);
		}
	}
}

template<class T,class key>
void avl_tree<T,key>::RotateLeft(avl_str<T,key>*& current)
{

	avl_str<T,key>* temp = current;
	current = current->right;
	temp->right = current->left;
	current->left = temp;


	ComputeBalance(current->left);
	ComputeBalance(current->right);
	ComputeBalance(current);
}

template<class T,class key>
void avl_tree<T,key>::RotateRight(avl_str<T,key>*& current)
{
	avl_str<T,key>* temp = current;
	current = current->left;
	temp->left = current->right;
	current->right = temp;

	ComputeBalance(current->left);
	ComputeBalance(current->right);
	ComputeBalance(current);
}

template<class T,class key>
bool avl_tree<T,key>::isBalanced()
{
	bool balance = true;

	balanceTraverse(root, balance);

	return balance;
}

template<class T,class key>
void avl_tree<T,key>::balanceTraverse(avl_str<T,key>*& current, bool &balance)
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