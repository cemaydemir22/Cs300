#include <iostream>
#include "stack.h" // Template based class
#include <vector>
#include <string>
#include <algorithm> // Shuffle
#include "stack.cpp" 
using namespace std;


struct for_maze_cells 
{

	int upp_cell = 1; // Upper boundary
	int left_cell = 1; //  Left boundary
	int right_cell = 1; // Right boundary
	int down_cell = 1; //  Down boundary
	bool visit = false;
	int row; // for  coordinate of the cells
	int column; // for coordinate of the cells

	for_maze_cells(int x, int y) : column(x), row(y), upp_cell(1), down_cell(1), right_cell(1), left_cell(1), visit(false){}
    for_maze_cells(){  // default constructor
		int upp_cell = 1; // Upper boundary
		int left_cell = 1; //  Left boundary
		int right_cell = 1; // Right boundary
		int down_cell = 1; //  Down boundary
		bool visit = false;
		int row; // for  coordinate of the cells
		int column; // for coordinate of the cells

	}
};


int main() 
{
    STACK<for_maze_cells> stack_maze;
	vector<vector<vector<for_maze_cells>>> Mazes; // This 3-D vector is to hold multiplep mazes
	
	int N, K, M; // K is for the number of mazes

	cout << "Enter the number of mazes: ";
	cin >> K;
	cout << "Enter the number of rows and columns (M and N): ";
	cin >> M >> N;
	cout << "All mazes are generated."<<endl;

	vector<vector<for_maze_cells>> Maze(M,vector<for_maze_cells>(N));// This matrix will hold cells to form a maze (M=#rows N=#columns)
	stack_maze.push(Maze[0][0]);
    Maze[0][0].visit=true;


	for (int i = 1; i < K; i++) // This loop is to create K mazes
	{
		for (int j = 0; j < Maze.size(); j++)
		{
			for (int k = 0; k < Maze[0].size(); k++)
			{
				Maze[j][k].row=j;
				Maze[j][k].column=k; // Maze içerisindeki  hücrelere konum yazıldı
			}
			
		}
		
        
		
	

	








	}
	return 0;
}

