#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "stack.h"

#define NOT_FOUND -1
#define ERROR -2

// Tuple for start coordinates.
static struct start{
    int r;
    int c;
}*start;
// Tuple for end coordinates.
static struct end{
    int r;
    int c;
}*end;
// Tuple for current coordinates.
static struct current{
    int r;
    int c;
}*current;

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int dfs_solve(struct maze *m)
{
    if (m == NULL){
        return NOT_FOUND;
    }
    // Link start and end coordinates to the tuple structs.
    maze_start(m, &start->r, &start->c);
    maze_destination(m, &end->r, &end->c);
    // Intialize current position as start.
    current->r = start->r;
    current->c = current->c;
    
    // Check if the start position = the end position.
    if (start->r == end->r && start->c == end->c){
        return 0;
    }
    // Create a stack for later use.
    struct stack *s = stack_init(1000); // hoe groot moet de stack zijn

    /* Check for each possible moveset if it is a valid move, by
    looping through the possible movesets. */
    int moveset_size = sizeof(m_offsets)/sizeof(m_offsets[0]);

    for (int i = 0; i < moveset_size; i++){
        // Store coordinates of move to be taken in temp value. 
        int temp_r = current->r + m_offsets[i][0];
        int temp_c = current->c + m_offsets[i][1];
        // If coordinates are valid and position is not occupied move to that spot.
        if (maze_valid_move(m, temp_r, temp_c)
            && maze_get(m, temp_r, temp_c) == ' '){
                // Push new current location onto stack.
                stack_push(s, current); // wat moet ik precies op de stack pushen?
                maze_set(m, temp_r, temp_c, 'x');
                // Renew current with the values stored in temp.
                current->r = temp_r;
                current->c = temp_c;
                 // Check if the new move is the destination.
                if (end->r == current->r && end->c == current->c){
                    return stack_size(s);
                };
                // Reset i for new iteration.
                i = 0;
            };
        if (!maze_valid_move(m, temp_r, temp_c) || maze_get(m, temp_r, temp_c) != ' '){
            if (stack_pop(s) == -1) return NOT_FOUND;
            maze_set(m, current->r, current->c, ',');
            stack_pop(s);
            };
        };
};

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
