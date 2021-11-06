#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "stack.h"

#define NOT_FOUND -1
#define ERROR -2

int row, col;
/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int dfs_solve(struct maze *m)
{
    if (maze_start(m, &row, &col) == maze_destination(m, &row, &col))
    {
    }

    struct stack *s = stack_init(1000);

    // maze[start point] hoe vind ik deze
    // hoe loop ik vervolgens in de maze?

    // hoe geef ik aan waar ik in de maze ben, hoe define ik de rows en columms
    // hoe print ik dingen verveolgens naar de file
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
