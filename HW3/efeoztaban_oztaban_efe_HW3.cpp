#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "hashtable.h"

using namespace std;

struct info
{
	double support;
	vector<string> elements;

	info(double sup)
		:support(sup)
	{}

};

struct words
{
	string name;
	bool exist;

	words(string name)
		:name(name)
	{
		exist = false;
	}
};

void selector(HashTable<info, string> &prev,HashTable<info, string> &selection, double treshold, int num);
void first_read(HashTable<info, string> &prev, int &num,string filename);
void second_read(HashTable<info, string> &prev, vector<vector<words>> &source,string filename);
void third_read(HashTable<info, string> &triple_product, HashTable<info, string>&quadruple_product, vector<vector<words>> &trios, vector<vector<words>> &quadros, string filename);
void output(vector<vector<vector<string>>> &transactions,HashTable<info, string> &second, HashTable<info, string> &single_product, HashTable<info, string> &double_product, HashTable<info, string> &triple_product, HashTable<info, string> &quadruple_product, int line, double confidence_treshold);

int main()
{
	double support_treshold;
	double confidence_treshold;
	string filename;

	int linenumber = 0;

	vector<vector<words>> pairs;
	vector<vector<words>> trios;
	vector<vector<words>> quadros;

	vector<vector<vector<string>>> transactions;

	HashTable<info, string> first, single_product;
	HashTable<info, string> second, double_product;
	HashTable<info, string> triple_product, quadruple_product;

	cout<< "Please enter the transaction file name: ";
	cin>>filename;
	cout<<"Please enter support and confidence values between 0 and 1: ";
	cin>> support_treshold;
	cin>> confidence_treshold;


	first_read(first, linenumber,filename);

	selector(first, single_product, support_treshold, linenumber);

	
	vector<string> one = single_product.getElements();
	vector<words> temp;

	for(int i = 0; i<one.size(); i++)
	{
		for(int j=i+1; j<one.size(); j++)
		{
			words pair_1(one[i]);
			words pair_2(one[j]);

			temp.push_back(pair_1);
			temp.push_back(pair_2);

			pairs.push_back(temp);

			temp.clear();
		}
	}

	second_read(second, pairs,filename);

	selector(second, double_product, support_treshold, linenumber);

	vector<string> two_product = double_product.getElements();
	vector<vector<string>> two;

	for( auto x: two_product)
	{
		two.push_back(double_product.findData(x).elements);
	}

	vector<string> temp2;
	vector<string> temp3;
	vector<vector<string>> temp4;

	
	for(int i = 0; i<one.size(); i++)
	{
		for (int j = i+1; j < one.size(); j++)
		{
			temp2.push_back(one[i]);
			temp3.push_back(one[j]);

			temp4.push_back(temp2);
			temp4.push_back(temp3);

			transactions.push_back(temp4);

			temp4.clear();

			temp4.push_back(temp3);
			temp4.push_back(temp2);

			transactions.push_back(temp4);

			temp2.clear();
			temp3.clear();
			temp4.clear();
			
		}
	}

	for(int i = 0; i<one.size(); i++)
	{
		for (int j = 0; j < two.size(); j++)
		{
			if (one[i] != two[j][0] && one[i] != two[j][1])
			{
				temp2.push_back(one[i]);

				temp4.push_back(temp2);
				temp4.push_back(two[j]);

				bool ch=true;

				for(int a=0; a<transactions.size(); a++)
				{
					if(transactions[a][0].size() == 1 && transactions[a][1].size() == 2)
					{
						if( transactions[a][0][0] == one[i] && transactions[a][1][0] == two[j][0] && transactions[a][1][1] == two[j][1])
						{
							ch = false;
						}
					}
				}
				if(ch)
				{
					transactions.push_back(temp4);
				}

				temp4.clear();
				
				temp4.push_back(two[j]);
				temp4.push_back(temp2);

				ch=true;

				for(int a=0; a<transactions.size(); a++)
				{
					if(transactions[a][0].size() == 2 && transactions[a][1].size() == 1)
					{
						if( transactions[a][1][0] == one[i] && transactions[a][0][0] == two[j][0] && transactions[a][0][1] == two[j][1])
						{
							ch = false;
						}
					}
				}
				if(ch)
				{
					transactions.push_back(temp4);
				}
				

				words trio1(one[i]);
				words trio2(two[j][0]);
				words trio3(two[j][1]);

				temp.push_back(trio1);
				temp.push_back(trio2);
				temp.push_back(trio3);

				

				vector<words> check;
				bool valid = true;

				for(int a=0; a<trios.size(); a++)
				{
					check = trios[a];

					if(check[0].name == trio1.name || check[0].name == trio2.name || check[0].name == trio3.name)
					{
						check[0].exist=true;
					}
					if(check[1].name == trio1.name || check[1].name == trio2.name || check[1].name == trio3.name)
					{
						check[1].exist=true;
					}
					if(check[2].name == trio1.name || check[2].name == trio2.name || check[2].name == trio3.name)
					{
						check[2].exist=true;
					}

					if(check[0].exist && check[1].exist && check[2].exist )
					{
						valid = false;
					}
				}

				if(valid)
				{
					trios.push_back(temp);
				}
			
				temp.clear();
				temp2.clear();
				temp3.clear();
				temp4.clear();
			}
		}
	}

	for(int i = 0; i<two.size(); i++)
	{
		for (int j = i+1; j < two.size(); j++)
		{

			if( two[i][0] != two[j][0]  && two[i][0] != two[j][1] &&
				two[i][1] != two[j][0]  && two[i][1] != two[j][1] )
			{

				temp4.push_back(two[i]);
				temp4.push_back(two[j]);

				bool ch=true;

				for(int a=0; a<transactions.size(); a++)
				{
					if(transactions[a][0].size() == 2 && transactions[a][1].size() == 2)
					{
						if( transactions[a][0][0] == two[i][0] && transactions[a][0][1] == two[i][1] && transactions[a][1][0] == two[j][0] && transactions[a][1][1] == two[j][1])
						{
							ch = false;
						}
					}
				}
				if(ch)
				{
					transactions.push_back(temp4);
				}



				words quadro1(two[i][0]);
				words quadro2(two[i][1]);
				words quadro3(two[j][0]);
				words quadro4(two[j][1]);

				temp.push_back(quadro1);
				temp.push_back(quadro2);
				temp.push_back(quadro3);
				temp.push_back(quadro4);

				vector<words> check;
				bool valid = true;

				for(int a=0; a<quadros.size(); a++)
				{
					check = quadros[a];

					if(check[0].name == quadro1.name || check[0].name == quadro2.name || check[0].name == quadro3.name || check[0].name == quadro4.name)
					{
						check[0].exist=true;
					}
					if(check[1].name == quadro1.name || check[1].name == quadro2.name || check[1].name == quadro3.name || check[1].name == quadro4.name)
					{
						check[1].exist=true;
					}
					if(check[2].name == quadro1.name || check[2].name == quadro2.name || check[2].name == quadro3.name || check[2].name == quadro4.name)
					{
						check[2].exist=true;
					}
					if(check[3].name == quadro1.name || check[3].name == quadro2.name || check[3].name == quadro3.name || check[3].name == quadro4.name)
					{
						check[3].exist=true;
					}


					if(check[0].exist && check[1].exist && check[2].exist && check[3].exist)
					{
						valid = false;
					}
				}

				if(valid)
				{
					quadros.push_back(temp);
				}

				

				temp4.clear();

				temp4.push_back(two[j]);
				temp4.push_back(two[i]);

				ch=true;

				for(int a=0; a<transactions.size(); a++)
				{
					if(transactions[a][0].size() == 2 && transactions[a][1].size() == 2)
					{
						if( transactions[a][0][0] == two[j][0] && transactions[a][0][1] == two[j][1] && transactions[a][1][0] == two[i][0] && transactions[a][1][1] == two[i][1])
						{
							ch = false;
						}
					}
				}
				if(ch)
				{
					transactions.push_back(temp4);
				}


				temp.clear();
				temp2.clear();
				temp3.clear();
				temp4.clear();
			}
		}
	}


	third_read(triple_product, quadruple_product, trios, quadros,filename);

	output(transactions,second, single_product, double_product, triple_product, quadruple_product, linenumber, confidence_treshold);


	return 0;

	
	
}

void selector(HashTable<info, string> &prev, HashTable<info, string> &selection, double treshold, int num)
{
	vector<string> elements = prev.getElements();

	int count;

	for( auto x: elements)
	{
		count = prev.findNum(x);

		double result = (count/double(num));

		if( result >= treshold )
		{
			info a(result);

			a.elements = prev.findData(x).elements;

			selection.insert(a,x);
		}
	}
}

void first_read(HashTable<info, string> &prev, int &num, string filename)
{
	ifstream reader;
	string word,line;

	reader.open(filename);

	while( getline(reader,line) )
	{

		istringstream ss(line);
		num++;
		
		while( ss>>word)
		{	
			info temp(0);
			temp.elements.push_back(word);
			prev.insert(temp,word);
		}

	}

	reader.close();
}

void second_read(HashTable<info, string> &prev, vector<vector<words>> &source,string filename)
{
	ifstream reader;
	string word,line;
	vector<vector<words>> everyline;

	reader.open(filename);

	while( getline(reader,line) )
	{
		everyline = source;

		istringstream ss(line);
		
		while( ss>>word)
		{
			for(int i=0; i<everyline.size(); i++)
			{
				if(word == everyline[i][0].name)
				{
					everyline[i][0].exist = true;
				}
				else if(word == everyline[i][1].name)
				{
					everyline[i][1].exist = true;
				}
			}

		}
		for(int i=0; i<everyline.size(); i++)
		{
			if(everyline[i][0].exist && everyline[i][1].exist)
			{
				info temp(0);
				temp.elements.push_back(everyline[i][0].name);
				temp.elements.push_back(everyline[i][1].name);

				prev.insert(temp, everyline[i][0].name+" "+everyline[i][1].name);
				prev.insert(temp, everyline[i][1].name+" "+everyline[i][0].name);
			}
		}

		everyline.clear();
	}

	reader.close();
}

void third_read(HashTable<info, string> &triple_product, HashTable<info, string> &quadruple_product, 
				vector<vector<words>> &trios, vector<vector<words>> &quadros, string filename)
{

	ifstream reader;
	string word,line;
	vector<vector<words>> everyline1;
	vector<vector<words>> everyline2;

	reader.open(filename);

	while( getline(reader,line) )
	{
		everyline1 = trios;
		everyline2 = quadros;

		istringstream ss(line);
		
		while( ss>>word)
		{
			for(int i=0; i<everyline1.size(); i++)
			{
				if(word == everyline1[i][0].name)
				{
					everyline1[i][0].exist = true;
				}
				else if(word == everyline1[i][1].name)
				{
					everyline1[i][1].exist = true;
				}
				else if(word == everyline1[i][2].name)
				{
					everyline1[i][2].exist = true;
				}
			}

			for(int i=0; i<everyline2.size(); i++)
			{
				if(word == everyline2[i][0].name)
				{
					everyline2[i][0].exist = true;
				}
				else if(word == everyline2[i][1].name)
				{
					everyline2[i][1].exist = true;
				}
				else if(word == everyline2[i][2].name)
				{
					everyline2[i][2].exist = true;
				}
				else if(word == everyline2[i][3].name)
				{
					everyline2[i][3].exist = true;
				}
			}

			
		}
		for(int i=0; i<everyline1.size(); i++)
		{
			if(everyline1[i][0].exist && everyline1[i][1].exist && everyline1[i][2].exist)
			{
				info temp(0);
				temp.elements.push_back(everyline1[i][0].name);
				temp.elements.push_back(everyline1[i][1].name);
				temp.elements.push_back(everyline1[i][2].name);

				triple_product.insert(temp, everyline1[i][0].name+" "+everyline1[i][1].name+" "+everyline1[i][2].name);
				triple_product.insert(temp, everyline1[i][0].name+" "+everyline1[i][2].name+" "+everyline1[i][1].name);
				triple_product.insert(temp, everyline1[i][2].name+" "+everyline1[i][1].name+" "+everyline1[i][0].name);
				triple_product.insert(temp, everyline1[i][2].name+" "+everyline1[i][0].name+" "+everyline1[i][1].name);
				triple_product.insert(temp, everyline1[i][1].name+" "+everyline1[i][0].name+" "+everyline1[i][2].name);
				triple_product.insert(temp, everyline1[i][1].name+" "+everyline1[i][2].name+" "+everyline1[i][0].name);
				
			}
		}

		for(int i=0; i<everyline2.size(); i++)
		{
			if(everyline2[i][0].exist && everyline2[i][1].exist && everyline2[i][2].exist && everyline2[i][3].exist)
			{
				info temp(0);
				temp.elements.push_back(everyline2[i][0].name);
				temp.elements.push_back(everyline2[i][1].name);
				temp.elements.push_back(everyline2[i][2].name);
				temp.elements.push_back(everyline2[i][3].name);

				quadruple_product.insert(temp, everyline2[i][0].name+" "+everyline2[i][1].name+" "+everyline2[i][2].name+" "+everyline2[i][3].name);
				quadruple_product.insert(temp, everyline2[i][0].name+" "+everyline2[i][1].name+" "+everyline2[i][3].name+" "+everyline2[i][2].name);
				quadruple_product.insert(temp, everyline2[i][0].name+" "+everyline2[i][2].name+" "+everyline2[i][3].name+" "+everyline2[i][1].name);
				quadruple_product.insert(temp, everyline2[i][0].name+" "+everyline2[i][2].name+" "+everyline2[i][1].name+" "+everyline2[i][3].name);
				quadruple_product.insert(temp, everyline2[i][0].name+" "+everyline2[i][3].name+" "+everyline2[i][2].name+" "+everyline2[i][1].name);
				quadruple_product.insert(temp, everyline2[i][0].name+" "+everyline2[i][3].name+" "+everyline2[i][1].name+" "+everyline2[i][2].name);

				quadruple_product.insert(temp, everyline2[i][1].name+" "+everyline2[i][0].name+" "+everyline2[i][2].name+" "+everyline2[i][3].name);
				quadruple_product.insert(temp, everyline2[i][1].name+" "+everyline2[i][0].name+" "+everyline2[i][3].name+" "+everyline2[i][2].name);
				quadruple_product.insert(temp, everyline2[i][1].name+" "+everyline2[i][2].name+" "+everyline2[i][3].name+" "+everyline2[i][0].name);
				quadruple_product.insert(temp, everyline2[i][1].name+" "+everyline2[i][2].name+" "+everyline2[i][0].name+" "+everyline2[i][3].name);
				quadruple_product.insert(temp, everyline2[i][1].name+" "+everyline2[i][3].name+" "+everyline2[i][0].name+" "+everyline2[i][2].name);
				quadruple_product.insert(temp, everyline2[i][1].name+" "+everyline2[i][3].name+" "+everyline2[i][2].name+" "+everyline2[i][0].name);

				quadruple_product.insert(temp, everyline2[i][2].name+" "+everyline2[i][0].name+" "+everyline2[i][1].name+" "+everyline2[i][3].name);
				quadruple_product.insert(temp, everyline2[i][2].name+" "+everyline2[i][0].name+" "+everyline2[i][3].name+" "+everyline2[i][1].name);
				quadruple_product.insert(temp, everyline2[i][2].name+" "+everyline2[i][1].name+" "+everyline2[i][3].name+" "+everyline2[i][0].name);
				quadruple_product.insert(temp, everyline2[i][2].name+" "+everyline2[i][1].name+" "+everyline2[i][0].name+" "+everyline2[i][3].name);
				quadruple_product.insert(temp, everyline2[i][2].name+" "+everyline2[i][3].name+" "+everyline2[i][1].name+" "+everyline2[i][0].name);
				quadruple_product.insert(temp, everyline2[i][2].name+" "+everyline2[i][3].name+" "+everyline2[i][0].name+" "+everyline2[i][1].name);

				quadruple_product.insert(temp, everyline2[i][3].name+" "+everyline2[i][0].name+" "+everyline2[i][1].name+" "+everyline2[i][2].name);
				quadruple_product.insert(temp, everyline2[i][3].name+" "+everyline2[i][0].name+" "+everyline2[i][2].name+" "+everyline2[i][1].name);
				quadruple_product.insert(temp, everyline2[i][3].name+" "+everyline2[i][1].name+" "+everyline2[i][0].name+" "+everyline2[i][2].name);
				quadruple_product.insert(temp, everyline2[i][3].name+" "+everyline2[i][1].name+" "+everyline2[i][2].name+" "+everyline2[i][0].name);
				quadruple_product.insert(temp, everyline2[i][3].name+" "+everyline2[i][2].name+" "+everyline2[i][0].name+" "+everyline2[i][1].name);
				quadruple_product.insert(temp, everyline2[i][3].name+" "+everyline2[i][2].name+" "+everyline2[i][1].name+" "+everyline2[i][0].name);


			}
		}

		everyline1.clear();
		everyline2.clear();
	}

	reader.close();

}
void output(vector<vector<vector<string>>> &transactions,HashTable<info, string> &second,
	HashTable<info, string> &single_product, HashTable<info, string> &double_product,
	HashTable<info, string> &triple_product, HashTable<info, string> &quadruple_product,
	int line, double confidence_treshold)
{
	ofstream writer;

	int iteration = 0;

	writer.open("results.txt");

	double confidence;
	double a, b;

	for(int i=0; i< transactions.size(); i++)
	{
		if( transactions[i][0].size() == 1 && transactions[i][1].size() == 1 )
		{
			if( second.isExist(transactions[i][0][0]+" "+transactions[i][1][0]))
			{
				a = second.findNum(transactions[i][0][0]+" "+transactions[i][1][0])/double(line);
			}
			else if( second.isExist(transactions[i][1][0]+" "+transactions[i][0][0]))
			{
				a = second.findNum(transactions[i][1][0]+" "+transactions[i][0][0])/double(line);
			}
			else
			{
				a = 0;
			}
			b = single_product.findData(transactions[i][0][0]).support;
			confidence =  a/b ;

			confidence *=100;
			int x = confidence;
			confidence = x/double(100);

			if( confidence> confidence_treshold)
			{
				writer<<transactions[i][0][0]<<";"<<transactions[i][1][0]<<";";
				writer<< fixed << setprecision(2) <<confidence<<endl;
				iteration++;
			}
		}
		else if( transactions[i][0].size() == 2 && transactions[i][1].size() == 1 )
		{
			if(triple_product.isExist(transactions[i][0][0]+" "+transactions[i][0][1]+" "+transactions[i][1][0]))
			{
				a = ((triple_product.findNum(transactions[i][0][0]+" "+transactions[i][0][1]+" "+transactions[i][1][0]))/double(line));
			}
			else
			{
				a=0;
			}
			if(double_product.isExist(transactions[i][0][0]+" "+transactions[i][0][1]))
			{
				b =double_product.findData(transactions[i][0][0]+" "+transactions[i][0][1]).support;
			}
			else
			{
				b= INT_MIN;
			}
			confidence = a/b;

			confidence *=100;
			int x = confidence;
			confidence = x/double(100);

			if( confidence> confidence_treshold)
			{
				writer<<transactions[i][0][0] << "," << transactions[i][0][1] <<";"<<transactions[i][1][0]<<";";
				writer<< fixed << setprecision(2) <<confidence<<endl;
				iteration++;
			}
		}
		else if( transactions[i][0].size() == 1 && transactions[i][1].size() == 2 )
		{
			if(triple_product.isExist(transactions[i][0][0]+" "+transactions[i][1][0]+" "+transactions[i][1][1])) 
			{
				a= ((triple_product.findNum(transactions[i][0][0]+" "+transactions[i][1][0]+" "+transactions[i][1][1]))/double(line));
			}
			else
			{
				a=0;
			}

			b= single_product.findData(transactions[i][0][0]).support;
			confidence =a/b;

			confidence *=100;
			int x = confidence;
			confidence = x/double(100);

			if( confidence> confidence_treshold)
			{
				writer<<transactions[i][0][0] << ";" << transactions[i][1][0] <<","<<transactions[i][1][1]<<";";
				writer<< fixed << setprecision(2) <<confidence<<endl;
				iteration++;
			}
		}
		else if( transactions[i][0].size() == 2 && transactions[i][1].size() == 2 )
		{
			if( quadruple_product.isExist( transactions[i][0][0]+" "+transactions[i][0][1]+" "+transactions[i][1][0]+" "+transactions[i][1][1]) )
			{
				a= ((quadruple_product.findNum(transactions[i][0][0]+" "+transactions[i][0][1]+" "+transactions[i][1][0]+" "+transactions[i][1][1]))/double(line));
			}
			else
			{
				a=0;
			}
			if( double_product.isExist(transactions[i][0][0]+" "+transactions[i][0][1]) )
			{
				b= double_product.findData(transactions[i][0][0]+" "+transactions[i][0][1]).support;
			}
			else
			{
				b=INT_MIN;
			}
			confidence = a/b;

			confidence *=100;
			int x = confidence;
			confidence = x/double(100);

			if( confidence> confidence_treshold)
			{
				writer<<transactions[i][0][0] << "," <<transactions[i][0][1]<< ";" << transactions[i][1][0] <<","<<transactions[i][1][1]<<";";
				writer<< fixed << setprecision(2) << confidence<<endl;
				iteration++;
			}
		}
	}


	writer.close();

	if(iteration == 0)
	{
		cout<< "There is no rule for the given support and confidence values." << endl;
	}
	else
	{
		cout << iteration <<" rules are written to results.txt." << endl;
	}
}