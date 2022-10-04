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
	
*/
void Matrix_x (float** matrix, int row, int column);
float** Matrix_sum (float** matrix_1, int row_1, int column_1, float** matrix_2, int row_2, int column_2, float** matrix_3, int* row_3, int* column_3);
float** Matrix_mul (float** matrix_1, int row_1, int column_1, float** matrix_2, int row_2, int column_2, float** matrix_3, int* row_3, int* column_3);
float Matrix_det (float** matrix, int row, int column);
