#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "stack.h"

#define NOT_FOUND -1
#define ERROR -2

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int dfs_solve(struct maze *m)
{
    // Check if maze is not NULL
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
    // Create a stack of the size of the maze for later use.
    int m_size = maze_size(m) * maze_size(m);
    struct stack *s = stack_init((size_t)m_size);
    /* Check for each possible moveset if it is a valid move, by
    looping through the possible movesets. */
    for (int i = 0; i < N_MOVES; i++)
    {
        // Store coordinates of move to be taken in temp value.
        int temp_r = current_r + m_offsets[i][0];
        int temp_c = current_c + m_offsets[i][1];
        // If coordinates are valid and position is not occupied move to that spot.
        if (maze_valid_move(m, temp_r, temp_c) && (maze_get(m, temp_r, temp_c) == ' ' ||
                                                   maze_get(m, temp_r, temp_c) == 'D'))
        {
            // Renew current with the values stored in temp.
            current_r = temp_r;
            current_c = temp_c;
            // Reset i for new iteration.
            i = -1;
            // Change coordinates into int value for the stack.
            int int_loc = maze_index(m,temp_r,temp_c);
            // Push new current location onto stack.
            stack_push(s, int_loc);
            maze_set(m, temp_r, temp_c, 'x');
            // Check if the new move is the destination, if so return the stack size and break.
            if (end_r == temp_r && end_c == temp_c){
                result = (int)stack_size(s);
                stack_cleanup(s);
                break;
            }
        }
        // If max num of moves is reached pop most recent item in the stack and continue from there.
        else if (i == N_MOVES - 1)
        {
            stack_pop(s);
            // If stack is empty, no moves could have been taken so return and break.
            if (stack_peek(s) == -1){
                result = NOT_FOUND;
                stack_cleanup(s);
                return result;
            }
            // Set current spot as visited.
            maze_set(m, current_r, current_c, '.');
            // Update current with new value of the top of the stack.
            current_r = maze_row(m, stack_peek(s));
            current_c = maze_col(m, stack_peek(s));
            // Reset i for new iteration.
            i = -1;
        }
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
    int path_length = dfs_solve(m);
    if (path_length == ERROR)
    {
        printf("dfs failed\n");
        maze_cleanup(m);
        return 1;
    }
    else if (path_length == NOT_FOUND)
    {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("dfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
