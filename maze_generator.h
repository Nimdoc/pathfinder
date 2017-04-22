// NOTE: Maze needs to receive an odd number as parameter as mazes
// with even number of cells don't generate as nicely

#ifndef Included_maze_generator_H
#define Included_maze_generator_H

#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stack>

// max size of maze x/y value
#define MAX_SIZE 55

// cell types
#define SPACE 0
#define WALL 1
#define START 2
#define END 3
#define CHEAT_PATH 4

// wall directions
enum class Direction{
	NORTH,
	SOUTH,
	WEST,
	EAST
};

using namespace std;

// cell struct for each element of the array
// cell can be an empty slot, wall, start, or end of maze
// each cell keeps track of the walls to each of its sides
// and whether or not its been visited
struct Cell {
	bool visited;
	bool north_wall;
	bool south_wall;
	bool west_wall;
	bool east_wall;

	// 0 = empty; 1 = wall; 2 = start; 3 = end; 4 = cheat_path
	int cell_type;
};

class generate_maze {
private:
	int start_X, start_Y;
	int end_X, end_Y;
	int map_size;

	Cell map[MAX_SIZE][MAX_SIZE];
	void generate_map(int map_size);
public:
	generate_maze();
	generate_maze(int size) {
		start_X = 0;
		start_Y = 0;
		end_X = 0;
		end_Y = 0;
		map_size = size;
	}

	void init_map();
	void generate_map();
	void print_map();
	void cheat_mode(stack<int> cheat_Y, stack<int>cheat_X);

	// getters for start coords
	int get_start_x() {return start_X;}
	int get_start_y() {return start_Y;}
	// getters for end coords
	int get_end_x() {return end_X;}
	int get_end_y() {return end_Y;}

	// setters for start and end coords
	void set_start_pos(int Y, int X) {start_Y = Y; start_X = X;}
	void set_end_pos(int Y, int X) {end_Y = Y; end_X = X;}

	int get_square(int y, int x) {return map[y][x].cell_type;}

	bool is_wall(int y, int x) {return map[y][x].cell_type == WALL;}
	bool is_end(int y, int x) {return map[y][x].cell_type == END;}
	bool is_start(int y, int x) {return map[y][x].cell_type == START;}
};

#endif
