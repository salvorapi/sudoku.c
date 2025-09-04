#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define N_ROWS 9
#define N_COLS 9
#define GRID_SIZE (N_ROWS * N_COLS)

char grid[N_ROWS*N_COLS];

int has_empty_space(char *grid) {
	for (int i = 0; i < GRID_SIZE; i++)
		if (grid[i] == '.')
			return 1;
	return 0;

}
int get_grid_index(int row, int col) {
    return (row * N_COLS) + col;
}
int get_square_index(int row, int col) {
   return row/3*3 + (col / 3) + 1;	
}

/* Reset grid add one dot(.) in every unit */
void reset_grid(char *grid) {
    for (int i = 0;i < GRID_SIZE; i++)
        grid[i] = '.';
}
/* Print grid */
void print_grid(char *grid) {
   int idx;
   for(int i = 0; i < N_ROWS; i++) { 
      for(int j = 0; j < N_COLS; j++) {
	idx = (i * N_ROWS) + j; 
	printf("%c ",grid[idx]);
	if (j % 3 == 2 && j < N_COLS - 1)
	    printf("|");	
      }
      printf("\n");
      for (int k = 0 ;k < N_COLS && i % 3 == 2 && i < N_ROWS -1;k++) {
	 printf("--");
	 if (k % 3 == 2 && k < N_COLS-1)
	     printf("+");
      }
      if (i % 3 == 2)
         printf("\n");
   }  
   printf("\n");
}
/* Replace value with empty space inside units */
void exclude_value(char *units, char value) {
	if (value == '.') 
		return;
	for (int i = 0; i < N_COLS; i++) {
		if (units[i] == value) {
			units[i] = ' '; // set empty space
			break;
		}
	}
	return;
}

/* Exclude value from units for relative square inside grid  */
void exclude_square_value(char *units, char *grid, int square_index) {
	int start_r = (square_index-1)/3 * 3;
	int start_c = (square_index-1) % 3 * 3 ;
	
	int end_r = start_r + 3;
	int end_c = start_c + 3;

	for (int r = start_r; r < end_r; r++)
	   for (int c = start_c; c < end_c; c++)
		exclude_value(units, grid[get_grid_index(r,c)]);	   	
}

/* Try to solve grid */
int solve_grid(char *grid) {
    // Find the first empty cell
    int r = -1, c = -1;
    for (int i = 0; i < N_ROWS && r == -1; i++) {
        for (int j = 0; j < N_COLS; j++) {
            if (grid[get_grid_index(i,j)] == '.') { r = i; c = j; break; }
        }
    }
    if (r == -1) return 1; // no empties: solved

    // Build candidates for (r,c)
    char cand[] = "123456789";
    for (int k = 0; k < N_COLS; k++) exclude_value(cand, grid[get_grid_index(r,k)]);
    for (int y = 0; y < N_ROWS; y++) exclude_value(cand, grid[get_grid_index(y,c)]);
    exclude_square_value(cand, grid, get_square_index(r,c));

    // Try each candidate in place
    for (int z = 0; z < 9; z++) {
        if (cand[z] != ' ') {
            grid[get_grid_index(r,c)] = cand[z];
            if (solve_grid(grid)) return 1;
        }
    }

    // Backtrack
    grid[get_grid_index(r,c)] = '.';
    return 0;
}



int main(void) {
   reset_grid(grid);
   print_grid(grid);
   memcpy(grid,"..3.2.6..9..3.5..1..18.64....81.29..7.......8..67.82....26.95..8..2.3..9..5.1.3..",81);
   solve_grid(grid);
   print_grid(grid);
   return 0;
}
