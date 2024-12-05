#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Matrixlib.h"

int main(){
	float** matrix_1, ** matrix_2, ** matrix_3;
	int row_1, row_2, row_3, column_1, column_2, column_3;
	
	matrix_1 = Matrix_input(matrix_1, &row_1, &column_1);
	matrix_2 = Matrix_input(matrix_2, &row_2, &column_2);
	
	matrix_3 = Matrix_sum(matrix_1, row_1, column_1, matrix_2, row_2, column_2, matrix_3, &row_3, &column_3);
	
	printf("%d ", row_3);
	printf("%d\n", column_3);
	
	Matrix_x(matrix_1, row_1, column_1);
	
	matrix_3 = Matrix_mul(matrix_1, row_1, column_1, matrix_2, row_2, column_2, matrix_3, &row_3, &column_3);
	
	printf("%d ", row_3);
	printf("%d\n", column_3);
	
	
	printf("The determinant is: %.2f\n", Matrix_det(matrix_1, row_1, column_1));
	
	
	Matrix_output(matrix_1, row_1, column_1);
	Matrix_output(matrix_2, row_2, column_2);
	Matrix_output(matrix_3, row_3, column_3);
	
	Matrix_free(matrix_1, row_1);
	Matrix_free(matrix_2, row_2);
	Matrix_free(matrix_3, row_3);
	
	return 0;
}
