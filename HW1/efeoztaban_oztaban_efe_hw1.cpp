#include "Stack.h"
#include "randgen.h"
#include "strutils.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct cell
{
	bool visited_create, visited_solve;
	bool r,l,u,d;
	int x,y;

	cell(int x_pos, int y_pos)
		:x(x_pos),y(y_pos),visited_create(false),visited_solve(false)
		,r(true),l(true),u(true),d(true)
	{}

};

void maze_generator(vector<vector<cell>> &temp_maze, int row, int column);
void maze_solver(vector<vector<cell>> &maze, int maze_num, int startx, int starty, int endx, int endy);

void maze_printer(vector<vector<vector<cell>>> &all_mazes);
void solution_writer(Stack<cell> &path_finder, int maze_num, int startx, int starty, int endx, int endy );
vector<string> path_checker(vector<vector<cell>> &temp_maze, int x, int y);
vector<string> road_finder(vector<vector<cell>> &temp_maze, int x, int y);


int main()
{
	int maze_count;
	int row, column;
	vector<vector<vector<cell>>> all_mazes;
	vector<vector<cell>> temp_maze;

	cout<< "Enter the number of mazes: " << endl;
	cin>> maze_count;

	cout<< "Enter the number of rows and columns (M and N): " << endl;
	cin>>row>>column;


	for(int i=1; i<= maze_count; i++)
	{
		maze_generator(temp_maze,row,column);
		all_mazes.push_back(temp_maze);
		temp_maze.clear();
	}

	maze_printer(all_mazes);

	cout<< "All mazes are generated." << endl;

	cout<< endl << endl;

	int maze_num;
	int startx,starty;
	int endx,endy;

	cout<< "Enter a maze ID between 1 to "<< maze_count <<" inclusive to find path: " << endl;
	cin>> maze_num;

	cout<< "Enter x and y coordinates of the entry points (x,y) or (column,row): " << endl;
	cin>> startx >> starty;

	cout<< "Enter x and y coordinates of the exit points (x,y) or (column,row): " << endl;
	cin>> endx >> endy;


	maze_solver(all_mazes[maze_num-1], maze_num, startx, starty, endx, endy);


	return 0;
}

void maze_generator(vector<vector<cell>> &temp_maze, int row, int column)
{
	vector<cell> temp_row;


	for(int i=0; i<row; i++)
	{
		temp_row.clear();

		for(int j=0; j<column; j++)
		{
			cell newcell(j,i);
			temp_row.push_back(newcell);
		}

		temp_maze.push_back(temp_row);
	}

	Stack<cell> wall_breaker;
	vector<string> possible_paths;
	RandGen generator;

	wall_breaker.push( temp_maze[0][0] );
	temp_maze[0][0].visited_create=true;

	while( !wall_breaker.isEmpty() )
	{
		possible_paths = path_checker(temp_maze, (wall_breaker.getTop()).x, (wall_breaker.getTop()).y);

		if( possible_paths.size() != 0 )
		{
			int rand_num = generator.RandInt(possible_paths.size());
			
			if( possible_paths[rand_num]  == "right" )
			{

				temp_maze[wall_breaker.getTop().y][wall_breaker.getTop().x].r=false;
				temp_maze[wall_breaker.getTop().y][wall_breaker.getTop().x+1].l=false;
				temp_maze[wall_breaker.getTop().y][wall_breaker.getTop().x+1].visited_create=true;
				
				wall_breaker.push( temp_maze[wall_breaker.getTop().y][wall_breaker.getTop().x+1] );
			}
			else if (possible_paths[rand_num]  == "left")
			{
				temp_maze[wall_breaker.getTop().y][wall_breaker.getTop().x].l=false;
				temp_maze[wall_breaker.getTop().y][wall_breaker.getTop().x-1].r=false;
				temp_maze[wall_breaker.getTop().y][wall_breaker.getTop().x-1].visited_create=true;
				
				wall_breaker.push( temp_maze[wall_breaker.getTop().y][wall_breaker.getTop().x-1] );
			}
			else if(possible_paths[rand_num]  == "up" )
			{
				temp_maze[wall_breaker.getTop().y][wall_breaker.getTop().x].u=false;
				temp_maze[wall_breaker.getTop().y+1][wall_breaker.getTop().x].d=false;
				temp_maze[wall_breaker.getTop().y+1][wall_breaker.getTop().x].visited_create=true;
				
				wall_breaker.push( temp_maze[wall_breaker.getTop().y+1][wall_breaker.getTop().x] );
			}
			else if (possible_paths[rand_num]  == "down")
			{
				temp_maze[wall_breaker.getTop().y][wall_breaker.getTop().x].d=false;
				temp_maze[wall_breaker.getTop().y-1][wall_breaker.getTop().x].u=false;
				temp_maze[wall_breaker.getTop().y-1][wall_breaker.getTop().x].visited_create=true;
				
				wall_breaker.push( temp_maze[wall_breaker.getTop().y-1][wall_breaker.getTop().x] );
			}

		}
		else
		{
			wall_breaker.pop();
		}

		possible_paths.clear();
	}	 


}

vector<string> path_checker(vector<vector<cell>> &temp_maze, int x, int y)
{
	vector<string> possible;

	int x_bound = temp_maze[0].size();
	int y_bound = temp_maze.size();

	if (x+1 > -1 && x+1 < x_bound && temp_maze[y][x+1].visited_create == false)
	{
		possible.push_back("right");
	}
	if (x-1 > -1 && x-1 < x_bound && temp_maze[y][x-1].visited_create == false)
	{
		possible.push_back("left");
	}
	if ( y+1 > -1 && y+1 < y_bound && temp_maze[y+1][x].visited_create == false)
	{
		possible.push_back("up");
	}
	if ( y-1 > -1 && y-1 < y_bound && temp_maze[y-1][x].visited_create == false)
	{
		possible.push_back("down");
	}

	return possible;
}

void maze_solver(vector<vector<cell>> &maze, int maze_num, int startx, int starty, int endx, int endy)
{
	Stack<cell> path_finder;
	vector<string> possible_paths;
	RandGen generator;

	path_finder.push( maze[starty][startx] );
	maze[starty][startx].visited_solve = true;

	while( ! ((path_finder.getTop().x == endx) && (path_finder.getTop().y == endy)) )
	{
		possible_paths = road_finder(maze, (path_finder.getTop()).x, (path_finder.getTop()).y);

		if( possible_paths.size() != 0 )
		{
			int rand_num = generator.RandInt(possible_paths.size());
			
			if( possible_paths[rand_num]  == "right" )
			{
				maze[path_finder.getTop().y][path_finder.getTop().x+1].visited_solve=true;
				
				path_finder.push( maze[path_finder.getTop().y][path_finder.getTop().x+1] );
			}
			else if (possible_paths[rand_num]  == "left")
			{
				maze[path_finder.getTop().y][path_finder.getTop().x-1].visited_solve=true;
				
				path_finder.push( maze[path_finder.getTop().y][path_finder.getTop().x-1] );
			}
			else if(possible_paths[rand_num]  == "up" )
			{
				maze[path_finder.getTop().y+1][path_finder.getTop().x].visited_solve=true;
				
				path_finder.push( maze[path_finder.getTop().y+1][path_finder.getTop().x] );
			}
			else if (possible_paths[rand_num]  == "down")
			{
				maze[path_finder.getTop().y-1][path_finder.getTop().x].visited_solve=true;
				
				path_finder.push( maze[path_finder.getTop().y-1][path_finder.getTop().x] );
			}

		}
		else
		{
			path_finder.pop();
		}

		possible_paths.clear();
	}

	solution_writer( path_finder, maze_num, startx, starty, endx, endy );

}

vector<string> road_finder(vector<vector<cell>> &temp_maze, int x, int y)
{
	vector<string> possible;

	int x_bound = temp_maze[0].size();
	int y_bound = temp_maze.size();

	if (x+1 > -1 && x+1 < x_bound && temp_maze[y][x].r == false && temp_maze[y][x+1].visited_solve == false)
	{
		possible.push_back("right");
	}
	if (x-1 > -1 && x-1 < x_bound && temp_maze[y][x].l == false && temp_maze[y][x-1].visited_solve == false)
	{
		possible.push_back("left");
	}
	if ( y+1 > -1 && y+1 < y_bound && temp_maze[y][x].u == false && temp_maze[y+1][x].visited_solve == false)
	{
		possible.push_back("up");
	}
	if ( y-1 > -1 && y-1 < y_bound && temp_maze[y][x].d == false && temp_maze[y-1][x].visited_solve == false)
	{
		possible.push_back("down");
	}

	return possible;
}

void maze_printer(vector<vector<vector<cell>>> &all_mazes )
{
	
	for( int i=0; i < all_mazes.size(); i++)
	{
		ofstream writer;
		string file_name = "maze_"+itoa(i+1)+".txt";

		writer.open(file_name);

		writer<< all_mazes[i].size() << " " << all_mazes[i][0].size() << endl;

		for(int j=0; j < all_mazes[i].size(); j++)
		{
			for(int k=0; k < all_mazes[i][j].size(); k++)
			{
				writer<< "x=" << all_mazes[i][j][k].x << " y=" << all_mazes[i][j][k].y ;
				if(all_mazes[i][j][k].l == true)
					writer << " l=1"; 
				else
					writer << " l=0";  

				if(all_mazes[i][j][k].r == true)
					writer<< " r=1";
				else
					writer<< " r=0";

				if(all_mazes[i][j][k].u == true)
					writer<< " u=1";
				else
					writer<< " u=0";

				if(all_mazes[i][j][k].d == true)
					writer<< " d=1";
				else
					writer<< " d=0";

				writer<<endl;
			}
		}

		writer.close();
	}
}

void solution_writer(Stack<cell> &path_finder, int maze_num, int startx, int starty, int endx, int endy )
{
	ofstream writer;
	string filename = "maze_"+itoa(maze_num+1)+"_path_"+itoa(startx); 
	filename += "_"+itoa(starty)+"_"+itoa(endx)+"_"+itoa(endy)+".txt";

	writer.open(filename);

	Stack<cell> ordered;

	while( !path_finder.isEmpty() )
	{
		cell temp = path_finder.getpop();
		ordered.push(temp);
	}

	while( !ordered.isEmpty() )
	{
		cell last = ordered.getpop();
		writer << last.x << " " << last.y << endl;
	}

	writer.close();

}