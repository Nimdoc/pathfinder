#include "maze_generator.h"

void generate_maze::generate_map()
{
	// direction of the wall to be removed in relation to current cell
	int direction;
	// total number of walls to be removed to make the maze
	int total_num_cells  = ((map_size-1)/2)*((map_size-1)/2);

	// pick a random starting point with odd coordinates
	srand(unsigned(time(0)));
	pos_X = ( ( (rand()/2) *2) +1) % (map_size -1);
	pos_Y = ( ( (rand()/2) *2) +1) % (map_size -1);

	// store starting pos in class members
	set_start_pos(pos_Y, pos_X);

	// mark the starting cell as a start cell, and mark it as visited
	map[start_Y][start_X].cell_type = START;
	map[start_Y][start_X].visited = true;

	// loop through until all the walls are knocked out that need to be
	while((num_visited_cells < total_num_cells))
	{
		// test if we can move to a target cell in any direction
		bool can_move_north = test_north(pos_Y, pos_X);
		bool can_move_south = test_south(pos_Y, pos_X);
		bool can_move_west = test_west(pos_Y, pos_X);
		bool can_move_east = test_east(pos_Y, pos_X);

		// if we can move to a target cell in any direction and clear the wall
		// between them
		if (can_move_north || can_move_south || can_move_west || can_move_east)
		{
			bool found_next_move = false;

			// cycle through until next move is found
			while (!found_next_move) {
				// set as true, target cell not found set back to false
				found_next_move = true;

				// pick a random wall bordering the current cell
				direction = (rand() % 4);

				// if we can move to a target cell, move there
				if (direction == NORTH && can_move_north)
					move_north();
				else if (direction == SOUTH && can_move_south)
					move_south();
				else if (direction == WEST && can_move_west)
					move_west();
				else if (direction == EAST && can_move_east)
					move_east();
				// else found_next_move is false
				else
					found_next_move = false;
			}
		}
		else
		{
			// pop off x and y pos, reiterate and check for walls on new x and y pos
			// until a new path can be formed
			if (traversed_X.size()) {
				pos_Y = traversed_Y.top();
				pos_X = traversed_X.top();
				traversed_Y.pop();
				traversed_X.pop();
				// pop off 2 of each for cheat since we're storing every
				// coordinate in cheat stack, traversed stores only odds
				cheat_X.pop();
				cheat_X.pop();
				cheat_Y.pop();
				cheat_Y.pop();
			}
		}
	}

	// mark last cell as End
	map[pos_Y][pos_X].cell_type = END;

	// store ending pos in class members
	set_end_pos(pos_Y, pos_X);

	// if cheats then label the cells which make up the correct path
	if (cheats)
		cheat_mode();

	// clears remaining 3 walls around start position so the path to the end
	// is less linear
	clear_start_walls();
}
bool generate_maze::test_north(int pos_Y, int pos_X)
{
	// if current cell is 2 or more cells from north edge of map
	// and the target cell hasn't been visited yet
	if ( (pos_Y > 1) && (map[pos_Y-2][pos_X].visited == false ) &&
			// and the north wall exists for current cell
			(map[pos_Y][pos_X].north_wall == true &&
			// and the south wall exists for target cell
			map[pos_Y-2][pos_X].south_wall == true) )
	{
		return true;
	}
	return false;
}

bool generate_maze::test_south(int pos_Y, int pos_X)
{
	// if current cell is 2 or more cells from south edge of map
	// and the target cell hasn't been visited yet
	if ( (pos_Y < map_size-2) && (map[pos_Y+2][pos_X].visited == false ) &&
			// and the south wall exists for current cell
			(map[pos_Y][pos_X].south_wall == true &&
			// and the north wall exists for target cell
			 map[pos_Y+2][pos_X].north_wall == true) )
	{
		return true;
	}
	return false;
}

bool generate_maze::test_west(int pos_Y, int pos_X)
{
	// if current cell is 2 or more cells from the west edge of map
	// and the target cell hasn't been visited yet
	if ( (pos_X > 1) && (map[pos_Y][pos_X-2].visited == false) &&
			// and the west wall exists for current cell
			(map[pos_Y][pos_X].west_wall == true &&
			// and the east wall exists for target cell
			 map[pos_Y][pos_X-2].east_wall == true) )
	{
		return true;
	}
	return false;
}

bool generate_maze::test_east(int pos_Y, int pos_X)
{
	// if  is 2 or more cells fromt he east edge of the map
	// and the target cell hasn't been visited yet
	if ( (pos_X < map_size-2) &&
			(map[pos_Y][pos_X+2].visited == false) &&
			// and the east wall exists for current cell
			(map[pos_Y][pos_X].east_wall == true &&
			// and the west wall exists for target pos
			 map[pos_Y][pos_X+2].west_wall == true) )
	{
		return true;
	}
	return false;
}

// checks the cell to each direction of the start direction
// if that cell is a WALL and the cell on the other side of the wall
// and the start_X != end_X for north/south; start_Y != end_Y for east/west
// then we clear the walls, this makes the maze feel more like a maze
// rather than a linear start path for half the maze
// this does mean the cheat path isn't the shortest path to the END
// but that shouldn't matter since we aren't using it anyway
// also, checking if start_Y != end_Y and start_X != end_X is a quick and dirty
// way of insuring you won't start a maze being able to see the END since it
// won't clear a wall in the X or Y direction if the END is in the same row
void generate_maze::clear_start_walls()
{
	// clear north wall
	if ((start_Y > 2) && (start_X != end_X) &&
		(map[start_Y-1][start_X].cell_type == WALL) &&
		(map[start_Y-2][start_X].cell_type != END))
	{
		map[start_Y-1][start_X].cell_type = EMPTY;
	}
	// clear south wall
	if ((start_Y < map_size-2) && (start_X != end_X) &&
		(map[start_Y+1][start_X].cell_type == WALL) &&
		(map[start_Y+2][start_X].cell_type != END))
	{
		map[start_Y+1][start_X].cell_type = EMPTY;
	}
	// clear west wall
	if ((start_X > 2) && (start_Y != end_Y) &&
		(map[start_Y][start_X-1].cell_type == WALL) &&
		(map[start_Y-1][start_X-2].cell_type != END))
	{
		map[start_Y][start_X-1].cell_type = EMPTY;
	}
	// clear east wall
	if ((start_X < map_size-2) && (start_Y != end_Y) &&
		(map[start_Y][start_X+1].cell_type == WALL) &&
		(map[start_Y][start_X+2].cell_type != END))
	{
		map[start_Y][start_X+1].cell_type = EMPTY;
	}
}

// mark the cheat path
void generate_maze::cheat_mode()
{
	while (cheat_Y.size() > 1)
	{
		map[cheat_Y.top()][cheat_X.top()].cell_type = 4;
		cheat_Y.pop();
		cheat_X.pop();
	}
}

// initialize the map
void generate_maze::init_map()
{
	for(int i = 0; i < map_size; i++)
	{
		for(int j = 0; j < map_size; j++)
		{
			map[i][j].cell_type = WALL;		// mark each cell as a wall
			map[i][j].visited = false;		// mark each cell as unvisited

			// mark each cell as surrounded by walls
			map[i][j].north_wall = true;
			map[i][j].south_wall = true;
			map[i][j].west_wall = true;
			map[i][j].east_wall = true;
		}
	}

	// for each cell bordering the edge
	// mark the walls bordering the edge as false
	// given the maze:
	// 0 0 0 0 0
	// 0 1 1 1 0
	// 0 1 0 1 0
	// 0 1 1 1 0
	// 0 0 0 0 0
	// the 1s denote the cells to be changed in this nested loop
	for(int i = 1; i < map_size-1; i++)
	{
		for(int j = 1; j < map_size-1; j++)
		{
			map[1][j].north_wall = false;
			map[map_size-2][j].south_wall = false;
			map[i][1].west_wall = false;
			map[i][map_size-2].east_wall = false;
		}
	}
}

// prints the map in console
void generate_maze::print_map()
{
	for (int i = 0; i < map_size; i++)
	{
		cout << endl;
		for (int j = 0; j < map_size; j++)
		{
			if (map[i][j].cell_type == EMPTY)
				cout << " ";
			else if (map[i][j].cell_type == WALL)
				cout << BOLDBLUE << "@" << RESET;
			else if (map[i][j].cell_type == START)
				cout << BOLDGREEN << "S" << RESET;
			else if (map[i][j].cell_type == END)
				cout << BOLDRED << "E" << RESET;
			else if (map[i][j].cell_type == CHEAT_PATH)
				cout << BOLDYELLOW << "." << RESET;
		}
	}
	cout << endl << endl;
}


// prints the player position in the map in console
void generate_maze::print_position(int x, int y)
{
	for (int i = 0; i < map_size; i++)
	{
		cout << endl;
		for (int j = 0; j < map_size; j++)
		{
			if (x == j && y == i)
				cout << BOLDYELLOW << "P" << RESET;
			else if (map[i][j].cell_type == EMPTY)
				cout << " ";
			else if (map[i][j].cell_type == WALL)
				cout << BOLDBLUE << "@" << RESET;
			else if (map[i][j].cell_type == START)
				cout << BOLDGREEN << "S" << RESET;
			else if (map[i][j].cell_type == END)
				cout << BOLDRED << "E" << RESET;
			else if (map[i][j].cell_type == CHEAT_PATH)
				cout << BOLDYELLOW << "." << RESET;
		}
	}
	cout << endl << endl;
}

void generate_maze::move_north()
{
	map[pos_Y-1][pos_X].cell_type = EMPTY;	// clear the cell
	map[pos_Y-1][pos_X].visited = true;	// mark cell as visited
	map[pos_Y][pos_X].north_wall = false;	// remove north wall

	// store x and y pos on stack
	traversed_X.push(pos_X);
	traversed_Y.push(pos_Y);

	// used store the path from start to end
	if (cheats) {
		cheat_X.push(pos_X);
		cheat_Y.push(pos_Y);
		cheat_X.push(pos_X);
		cheat_Y.push(pos_Y-1);
	}

	pos_Y -= 2;								// move to new cell
	map[pos_Y][pos_X].visited = true;		// mark new cell as visited
	map[pos_Y][pos_X].cell_type = EMPTY;		// clear the new cell
	map[pos_Y][pos_X].south_wall = false;	// remove south wall
	num_visited_cells++;
}

void generate_maze::move_south()
{
	map[pos_Y+1][pos_X].cell_type = EMPTY;	// clear the cell
	map[pos_Y+1][pos_X].visited = true;	// mark cell as visited
	map[pos_Y][pos_X].south_wall = false;	// remove south wall

	// store x and y pos on stack
	traversed_X.push(pos_X);
	traversed_Y.push(pos_Y);

	// used store the path from start to end
	if (cheats) {
		cheat_X.push(pos_X);
		cheat_Y.push(pos_Y);
		cheat_X.push(pos_X);
		cheat_Y.push(pos_Y+1);
	}

	pos_Y += 2;								// move to new cell
	map[pos_Y][pos_X].visited = true;		// mark new cell as visited
	map[pos_Y][pos_X].cell_type = EMPTY;		// clear the new cell
	map[pos_Y][pos_X].north_wall = false;	// remove north wall
	num_visited_cells++;
}

void generate_maze::move_west()
{
	map[pos_Y][pos_X-1].cell_type = EMPTY;	// clear the cell
	map[pos_Y][pos_X-1].visited = true;	// mark cell as visited
	map[pos_Y][pos_X].west_wall = false;	// remove west wall

	// store x and y pos on stack
	traversed_X.push(pos_X);
	traversed_Y.push(pos_Y);

	// used store the path from start to end
	if (cheats) {
		cheat_X.push(pos_X);
		cheat_Y.push(pos_Y);
		cheat_X.push(pos_X-1);
		cheat_Y.push(pos_Y);
	}



	pos_X -= 2;								// move to new cell
	map[pos_Y][pos_X].visited = true;		// mark new cell as visited
	map[pos_Y][pos_X].cell_type = EMPTY;		// clear the new cell
	map[pos_Y][pos_X].east_wall = false;	// remove east wall
	num_visited_cells++;
}

void generate_maze::move_east()
{
	map[pos_Y][pos_X+1].cell_type = EMPTY;	// clear the cell
	map[pos_Y][pos_X+1].visited = true;	// mark cell as visited
	map[pos_Y][pos_X].east_wall = false;	// remove east wall

	// store x and y pos on stack
	traversed_X.push(pos_X);
	traversed_Y.push(pos_Y);

	// used store the path from start to end
	if (cheats) {
		cheat_X.push(pos_X);
		cheat_Y.push(pos_Y);
		cheat_X.push(pos_X+1);
		cheat_Y.push(pos_Y);
	}

	pos_X += 2;								// move to new cell
	map[pos_Y][pos_X].visited = true;		// mark cell as visited
	map[pos_Y][pos_X].cell_type = EMPTY;		// clear the new cell
	map[pos_Y][pos_X].west_wall = false;	// remove west wall
	num_visited_cells++;
}


