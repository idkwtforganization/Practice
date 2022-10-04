float** Matrix_input (float** matrix, int* row, int* column);
/*
	Allocates memory for matrix 'matrix' and read it
	Params:
	row - the number of rows
	column - the number of columns
*/
void Matrix_free(float** matrix, int row);
/*
	Frees memory from 'matrix' matrix with dimensions row x column
	Params:
	row - the number of rows
*/
void Matrix_output (float** matrix, int row, int column);
/*
	Outputs the matrix with dimensions row x column
	Params:
	row - the number of rows
	column - the number of columns
*/
void Matrix_x (float** matrix, int row, int column);
/*
	Multiplies the matrix by a number you type in inside the prodedure
	Params:
	row - the number of rows
	column - the number of columns
*/
float** Matrix_sum (float** matrix_1, int row_1, int column_1, float** matrix_2, int row_2, int column_2, float** matrix_3, int* row_3, int* column_3);
/*
	Sums up matrices matrix_1 and matrix_2 with dimensions row_1 x column_1 and row_2 x column_2 in matrix_3
	Params:
	row_1, row_2, row_3 - the number of rows
	column_1, column_2, column_3 - the number of columns
*/
float** Matrix_mul (float** matrix_1, int row_1, int column_1, float** matrix_2, int row_2, int column_2, float** matrix_3, int* row_3, int* column_3);
/*
	Multiplies matrices matrix_1 and matrix_2 with dimensions row_1 x column_1 and row_2 x column_2 in matrix_3
	Params:
	row_1, row_2, row_3 - the number of rows
	column_1, column_2, column_3 - the number of columns
*/
float Matrix_det (float** matrix, int row, int column);
/*
	Counts the determinant of the matrix
*/
