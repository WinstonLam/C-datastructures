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
    if (m == NULL)
    {
        return ERROR;
    }
    // Defining int's for initialization
    int start_r, start_c, end_r, end_c, current_r, current_c, i, result;
    // Link start and end coordinates to the tuple structs.
    maze_start(m, &start_r, &start_c);
    maze_set(m, start_r, start_c, 'S');
    maze_destination(m, &end_r, &end_c);
    maze_set(m, end_r, end_c, 'D');
    // Intialize current position as start.
    current_r = start_r;
    current_c = start_c;

    // Check if the start position = the end position.
    if (start_r == end_r && start_c == end_c)
    {
        return 0;
    }
    // Create a stack for later use.
    struct stack *s = stack_init(2000);

    /* Check for each possible moveset if it is a valid move, by
    looping through the possible movesets. */
    int moveset_size = sizeof(m_offsets) / sizeof(m_offsets[0]);
    i = 0;
    while (i < moveset_size)
    {
        // Store coordinates of move to be taken in temp value.
        int temp_r = current_r + m_offsets[i][0];
        int temp_c = current_c + m_offsets[i][1];
        // If coordinates are valid and position is not occupied move to that spot.
        if (maze_valid_move(m, temp_r, temp_c) && (maze_get(m, temp_r, temp_c) == ' ' ||
                                                   maze_get(m, temp_r, temp_c) == 'D'))
        {
            printf("%c", maze_get(m, temp_r, temp_c));
            // Renew current with the values stored in temp.
            current_r = temp_r;
            current_c = temp_c;
            // Reset i for new iteration.
            i = -1;
            // Change coordinates into int value for the stack.
            int int_loc = (temp_r * maze_size(m)) + temp_c;
            // Push new current location onto stack.
            stack_push(s, int_loc);
            maze_set(m, temp_r, temp_c, 'x');
            // Check if the new move is the destination.
            if (end_r == temp_r && end_c == temp_c)
            {
                result = stack_size(s);
                return result;
            }
        }
        else if (i == moveset_size - 1)
        {
            stack_pop(s);
            if (stack_peek(s) == -1)
            {
                result = NOT_FOUND;
                return result;
            }
            maze_set(m, current_r, current_c, '.');
            current_r = maze_row(m, stack_peek(s));
            current_c = maze_col(m, stack_peek(s));
            // Reset i for new iteration.
            i = -1;
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
