#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "queue.h"

#define NOT_FOUND -1
#define ERROR -2

/* Checks if all neighbours of a certain spot in the maze
are non empty, if so return 0 otherwise return 1. 
If function fails return -1. */
static int neighbours(struct maze *m, int n)
{
    if (m == NULL) return -1;
    // Get the row and column of the spot to check it's neigbours.
    int main_r = maze_row(m, n);
    int main_c = maze_row(m, n);
    // Create list for all neighbours that are non empty.
    int non_empty;
    /* For all neighbours check if is non empty, if so add it to the 
    non empty neighbours list. */
    for (int i = 0; i < N_MOVES; i++)
    {   
        int temp_r = main_r + m_offsets[i][0];
        int temp_c = main_c + m_offsets[i][1];
        non_empty += 0;
        if (maze_get(m, temp_r, temp_c) != ' ') non_empty += 1;
        
    }
    // If all neighbours are non empty return 0 else 1.
    if (non_empty == 4) return 0;
    return 1;
}

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int bfs_solve(struct maze *m)
{
    if (m == NULL) return ERROR;
    // Defining int's for initialization
    int start_r, start_c, end_r, end_c, current_r, current_c, result;
    // Link start and end coordinates to the tuple structs.
    maze_start(m, &start_r, &start_c);
    maze_set(m, start_r, start_c, 'S');
    maze_destination(m, &end_r, &end_c);
    maze_set(m, end_r, end_c, 'D');
    // Intialize current position as start.
    current_r = start_r;
    current_c = start_c;
    // Check if the start position = the end position.
    if (start_r == end_r && start_c == end_c) return 0;
    // Create a queue for later use.
    int m_size = maze_size(m) * maze_size(m);
    struct queue *q = queue_init((size_t)m_size);
    // Push start position onto the queue.
    queue_push(q, maze_index(m,start_r,start_c));
    /* Check for each possible moveset if it is a valid move, by
    looping through the possible movesets. */
    int i = 0;
    int predecessors[m_size];
    while (queue_empty(q) == 0)
    {
        // Store coordinates of move to be taken in temp value.
        int temp_r = current_r + m_offsets[i][0];
        int temp_c = current_c + m_offsets[i][1];
        // If coordinates are valid and position is not occupied move to that spot.
        if (maze_valid_move(m, temp_r, temp_c) && (maze_get(m, temp_r, temp_c) == ' ' ||
                                                   maze_get(m, temp_r, temp_c) == 'D'))
        {
            // Change coordinates into int value for the queue.
            int int_loc = maze_index(m,temp_r,temp_c);
            // Push new current location onto stack.
            queue_push(q, int_loc);
            predecessors[maze_index(m, temp_r, temp_c)] = maze_index(m, current_r, current_c);

            maze_set(m, temp_r, temp_c, '.');
            // Check if the new move is the destination.
            if (end_r == temp_r && end_c == temp_c)
            {
                // Intialize path counter with value of 2 to account for start and end index.
                int path = 2;
                // Store predecessor of index in prev.
                int prev = predecessors[maze_index(m, end_r, end_c)];
                // Loop back all the way to the start by checking each predecessor.
                while (predecessors[prev] != maze_index(m, start_r, start_c))
                {
                    // Set the predecessor to x since it is in our path.
                    maze_set(m, maze_row(m, prev), maze_col(m, prev), 'x');
                    // Increment path length.
                    path++;
                    // Set prev one step back.
                    prev = predecessors[prev]; 
                }
                // Account for last x to be placed outside the while loop.
                maze_set(m, maze_row(m, prev), maze_col(m, prev), 'x');
                // Set path as result, clean up the queue and break.
                result = path;
                queue_cleanup(q);
                return result;
                break;
            }
        }
        if (i == N_MOVES - 1)
        {
            if (queue_peek(q) == -1)
            {
                result = NOT_FOUND;
                queue_cleanup(q);
                break;
            }
            maze_set(m, current_r, current_c, '.');
            queue_pop(q);
            // Reset i for new iteration.
            i = -1;
            current_r = maze_row(m, queue_peek(q));
            current_c = maze_col(m, queue_peek(q));
            // Current coordinates of the location translated into a single int.
            int cur_loc = maze_index(m,current_r,current_c);

            // Check if all neighbours are non-empty.
            if (neighbours(m, cur_loc) == 0)
            {
                maze_set(m, current_r, current_c, '.');
                queue_pop(q);
                current_r = maze_row(m, queue_peek(q));
                current_c = maze_col(m, queue_peek(q));
            }
        }
        i++;
    }
    return result;
}

int main(void)
{
    /* read maze */
    struct maze *m = maze_read();
    if (!m)
    {
        printf("Error reading maze\n");
        return 1;
    }

    /* solve maze */
    int path_length = bfs_solve(m);
    if (path_length == ERROR)
    {
        printf("bfs failed\n");
        maze_cleanup(m);
        return 1;
    }
    else if (path_length == NOT_FOUND)
    {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("bfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
