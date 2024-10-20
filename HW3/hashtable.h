#include <string>
#include <vector>

using namespace std;

template <class type, class keytype>
class HashTable
{

public:

	HashTable();
	~HashTable();
	void insert(type, keytype);
	void remove(keytype);
	bool isExist(keytype);
	int findNum(keytype);
	vector<keytype> getElements();
	type findData(keytype);


private:

	template<class type,class keytype>
	struct node
	{
		keytype key;
		type data;
		node* next;
		int num;

		node(keytype key, type data)
			:key(key),data(data)
		{
			next = nullptr;
			num = 1;
		}

		node(keytype key, type data, int num)
			:key(key),data(data),num(num)
		{
			next = nullptr;
		}

	};


	int size;
	int element;
	vector<node<type,keytype>*> table;


	void rehash();
	double getLoadFactor();
	void makeEmpty(vector<node<type,keytype>*> &list);
	void deleteHelper(node<type,keytype>* &current);
	void insert_with_num(type data, keytype key, int num);

	friend int hashing(string key, int size);


};

int hashing(string key, int size)
{
	int sum = 0;
    for (int i = 0; i < key.length(); i++)
	{
		sum = sum + key[ i ];
	}
	return (sum % size);
}

template <class type, class keytype>
HashTable <type,keytype> :: HashTable()
{
	size = 128;
	element = 0;

	node<type,keytype>* temp = nullptr;

	for(int i=0; i<size; i++)
	{
		table.push_back(temp);
	}
	
}

template <class type, class keytype>
HashTable <type,keytype> :: ~HashTable()
{
	makeEmpty(table);
}



template <class type, class keytype>
void HashTable <type,keytype> :: insert(type data, keytype key)
{
	int index = hashing(key,size);

	if( table[index] == nullptr )
	{
		table[index] = new node<type,keytype>(key,data);
		element++;
	}
	else
	{
		node<type,keytype>* temp = table[index];

		if( temp->key == key)
		{
			temp->num +=1;
		}
		else
		{
			while( temp->next != nullptr && temp->key != key)
			{
				temp = temp->next;
			}

			if( temp->key == key)
			{
				temp->num +=1;
			}
			else
			{
				temp->next = new node<type,keytype>(key,data);
				element++;
			}
		}
	}

	double loadFactor = getLoadFactor();

	if( loadFactor > 0.5)
	{
		rehash();
	}
}

template <class type, class keytype>
void HashTable <type,keytype> :: insert_with_num(type data, keytype key, int num)
{
	int index = hashing(key,size);

	if( table[index] == nullptr )
	{
		table[index] = new node<type,keytype>(key,data,num);
		element++;
	}
	else
	{
		node<type,keytype>* temp = table[index];

		if( temp->key == key)
		{
			temp->num +=1;
		}
		else
		{
			while( temp->next != nullptr && temp->key != key)
			{
				temp = temp->next;
			}

			if( temp->key == key)
			{
				temp->num +=1;
			}
			else
			{
				temp->next = new node<type,keytype>(key,data,num);
				element++;
			}
		}
	}
}

template <class type, class keytype>
void HashTable <type,keytype> :: remove(keytype key)
{
	int index = hashing(key,size);

	if( table[index] == nullptr )
	{
		cout<< "Item could not be found." << endl;
	}
	else
	{
		node<type,keytype>* temp = table[index];

		if(temp->key == key)
		{
			if(temp->next != nullptr)
			{
				table[index] = temp->next;
				delete temp;
				element--;
			}
			else
			{
				table[index] = nullptr;
				delete temp;
				element--;
			}
		}
		else if (temp->next != nullptr)
		{
			node<type,keytype>* prev = temp;
			temp = temp->next;

			while(temp->next && temp->key != key)
			{
				temp = temp->next;
				prev = prev->next;
			}
			if(temp->key == key)
			{
				if(temp->next == nullptr)
				{
					prev->next = nullptr;
					delete temp;
					element--;
				}
				else
				{
					prev->next = temp->next;
					delete temp;
					element--;
				}
			}
			else
			{
				cout<< "Item could not be found." << endl;
			}
		}
		else
		{
			cout<< "Item could not be found." << endl;
		}
	}
}

template <class type, class keytype>
bool HashTable <type,keytype> :: isExist(keytype key)
{

	int index = hashing(key,size);

	bool exist = false;

	if( table[index] != nullptr )
	{
		node<type,keytype>* temp = table[index];

		if(temp->key == key)
		{
			exist = true;
		}
		else
		{
			while(temp->next)
			{
				temp = temp->next;

				if(temp->key == key)
				{
					exist = true;
				}
				
			}
		}
	}

	return exist;

}

template <class type, class keytype>
void HashTable <type,keytype> :: rehash()
{
	vector<node<type,keytype>*> temp_list;

	temp_list = table;


	table.clear();
	size = size*2;
	element = 0;

	node<type,keytype>* temp = nullptr;

	for(int i=0; i<size; i++)
	{
		table.push_back(temp);
	}

	for(int i = 0; i<temp_list.size(); i++)
	{
		if( temp_list[i] )
		{
			node<type,keytype>* temp = temp_list[i];

			insert_with_num(temp->data, temp->key, temp->num);


			while(temp->next)
			{
				temp = temp->next;
				insert_with_num(temp->data, temp->key, temp->num);
			}
		}
	}
	
	makeEmpty(temp_list);
}

template <class type, class keytype>
double HashTable <type,keytype> :: getLoadFactor()
{
	return element/size;
}

template <class type, class keytype>
void HashTable <type,keytype> :: makeEmpty(vector<node<type,keytype>*> &list)
{
	for(int i = 0; i<list.size(); i++)
	{
		if( list[i] )
		{
			deleteHelper(list[i]);
			list[i] = nullptr;
		}
	}

	list.clear();
}

template <class type, class keytype>
void HashTable <type,keytype> :: deleteHelper(node<type,keytype>* &current)
{
	if(current->next)
	{
		deleteHelper(current->next);
	}

	delete current;
}

template <class type, class keytype>
int HashTable <type,keytype> :: findNum(keytype key)
{
	int index = hashing(key,size);

	if( table[index] == nullptr)
	{
		cout<< "Item could not be found." << endl;
	}
	else
	{
		node<type,keytype>* temp = table[index];

		if(temp->key == key)
		{
			return temp->num;
		}
		else if (temp->next != nullptr)
		{
			while(temp->next && temp->key != key)
			{
				temp = temp->next;
			}

			if(temp->key == key)
			{
				return temp->num;
			}
			else
			{
				cout<< "Item could not be found." << endl;
			}
		}
		else
		{
			cout<< "Item could not be found." << endl;
		}
	}
}

template <class type, class keytype>
vector<keytype> HashTable <type,keytype> :: getElements()
{
	vector<keytype> result;

	for(int i = 0; i<size; i++)
	{
		if( table[i] )
		{
			node<type,keytype>* temp = table[i];

			result.push_back(temp->key);

			while(temp->next)
			{
				temp = temp->next;
				result.push_back(temp->key);
			}
		}
	}

	return result;

}

template <class type, class keytype>
type HashTable <type,keytype> :: findData(keytype key)
{
	int index = hashing(key,size);

	if( table[index] == nullptr)
	{
		cout<< "Item could not be found." << endl;
	}
	else
	{
		node<type,keytype>* temp = table[index];

		if(temp->key == key)
		{
			return temp->data;
		}
		else if (temp->next != nullptr)
		{
			while(temp->next && temp->key != key)
			{
				temp = temp->next;
			}

			if(temp->key == key)
			{
				return temp->data;
			}
			else
			{
				cout<< "Item could not be found." << endl;
			}
		}
		else
		{
			cout<< "Item could not be found." << endl;
		}
	}
}


