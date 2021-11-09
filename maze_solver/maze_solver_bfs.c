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
static int neighbours(struct maze *m, int n){
    if (m == NULL) return -1;
    // Get the row and column of the spot to check it's neigbours.
    int main_r = maze_row(m, n);
    int main_c = maze_row(m, n);
    int num_moves = sizeof(m_offsets) / sizeof(m_offsets[0]);
    // Create list for all neighbours that are non empty.
    int neighbours[num_moves];
    /* For all neighbours check if is non empty, if so add it to the 
    non empty neighbours list. */
    for (int i = 0; i < m_offsets; i++){
        int temp_r = main_r + m_offsets[i][0];
        int temp_c = main_c + m_offsets[i][1];
        int neighbour[2] = {temp_r,temp_c};
        if (maze_get(m, temp_r, temp_c) != ' '){
            neighbours[i] = neighbour;
        }
    }
    // This int states how many neighbours are non empty.
    int non_empty = sizeof(neighbours) / sizeof(neighbours[0]);
    // If all neighbours are non empty return 0 else 1.
    if (non_empty == 4){
        return 0;
    }
    return 1;
}

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int bfs_solve(struct maze *m) {
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
    // Create a queue for later use.
    struct queue *q = queue_init(2000);   
    /* Check for each possible moveset if it is a valid move, by
    looping through the possible movesets. */
    int moveset_size = sizeof(m_offsets) / sizeof(m_offsets[0]);
    while (i < moveset_size)
    {
        // Store coordinates of move to be taken in temp value.
        int temp_r = current_r + m_offsets[i][0];
        int temp_c = current_c + m_offsets[i][1];
        // If coordinates are valid and position is not occupied move to that spot.
        if (maze_valid_move(m, temp_r, temp_c) && (maze_get(m, temp_r, temp_c) == ' ' ||
                                                   maze_get(m, temp_r, temp_c) == 'D'))
        {
         /*    printf("%c", maze_get(m, temp_r, temp_c));
            // Renew current with the values stored in temp.
            current_r = temp_r;
            current_c = temp_c; */
            // Reset i for new iteration.
            i = -1;
            // Change coordinates into int value for the queue.
            int int_loc = (temp_r * maze_size(m)) + temp_c;
            // Push new current location onto stack.
            queue_push(q, int_loc);
            maze_set(m, temp_r, temp_c, 'x');
            // Check if the new move is the destination.
            if (end_r == temp_r && end_c == temp_c)
            {
                result = queue_size(q);
                return result;
            }
        }
        if (i == moveset_size - 1)
        {
            if (queue_peek(q) == -1)
            {
                result = NOT_FOUND;
                return result;
            }
            current_r = maze_row(m, queue_peek(q));
            current_c = maze_col(m, queue_peek(q));
            // Current coordinates of the location translated into a single int.
            int cur_loc = (current_r * maze_size(m)) + current_c;
            // Check if all neighbours are non-empty.
            if (neighbours(m, cur_loc) == 0){
                maze_set(m, current_r, current_c, '.');
                queue_pop(q);
                if (queue_peek(q) == -1)
                {
                    result = NOT_FOUND;
                    return;
                }
                current_r = maze_row(m, queue_peek(q));
                current_c = maze_col(m, queue_peek(q));
                 // Reset i for new iteration.
                i = -1;
            }
        }

        i++;
    }
    return result;
}


int main(void) {
    /* read maze */
    struct maze *m = maze_read();
    if (!m) {
        printf("Error reading maze\n");
        return 1;
    }

    /* solve maze */
    int path_length = bfs_solve(m);
    if (path_length == ERROR) {
        printf("bfs failed\n");
        maze_cleanup(m);
        return 1;
    } else if (path_length == NOT_FOUND) {
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
