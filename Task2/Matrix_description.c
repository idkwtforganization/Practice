#include <stdio.h>
#include <string.h>
#include <stdlib.h>

float** Matrix_input (float** matrix, int* row, int* column){
	printf("Type in your matrix size (number of rows and columns):\n");
	scanf("%d", row);
	scanf("%d", column);
	matrix = (float**) malloc(sizeof(float*) * *row);
	for (int i = 0; i < *row; i++){
		matrix[i] = (float*) malloc(*column * sizeof(float));
		for (int j = 0; j < *column; j++)
				scanf("%f", &matrix[i][j]);
	}
	printf("\n");
	return matrix;
}

void Matrix_free(float** matrix, int row){
	for (int i = 0; i < row; i++)
		free(matrix[i]);
	free(matrix);
}

void Matrix_output (float** matrix, int row, int column){
	for (int i = 0; i < row; i++){
		for (int j = 0; j < column; j++)
			printf("%.2f    ", matrix[i][j]);
		printf("\n");
	}
	printf("\n");
}

void Matrix_x (float** matrix, int row, int column){
	float number;
	printf("Put in a number to multiply\n");
	scanf("%f", &number);
	for (int i = 0; i < row; i++)
		for (int j = 0; j < column; j++)
			matrix[i][j] *= number;
	printf("\n");
}

float** Matrix_sum (float** matrix_1, int row_1, int column_1, float** matrix_2, int row_2, int column_2, float** matrix_3, int* row_3, int* column_3){
	if ((row_1 != row_2) || (column_1 != column_2)){
		printf("The sizes of the matrices are different!");
		printf("\n");
		return matrix_3;
	}
	Matrix_free(matrix_3, *row_3);
	*row_3 = row_1;
	*column_3 = column_1;
	matrix_3 = (float**) malloc(sizeof(float*) * *row_3);
	for (int i = 0; i < *row_3; i++){
		matrix_3[i] = (float*) malloc(*column_3 * sizeof(float));
		for (int j = 0; j < *column_3; j++)
			matrix_3[i][j] = matrix_1[i][j] + matrix_2[i][j];
	}
	return matrix_3;
}

float** Matrix_mul (float** matrix_1, int row_1, int column_1, float** matrix_2, int row_2, int column_2, float** matrix_3, int* row_3, int* column_3){
	if (column_1 != row_2){
		printf("The sizes of the matrices do not fit each other!");
		printf("\n");
		return matrix_3;
	}
	Matrix_free(matrix_3, *row_3);
	*row_3 = row_1;
	*column_3 = column_2;
	float sum;
	matrix_3 = (float**) malloc(sizeof(float*) * *row_3);
	for (int i = 0; i < *row_3; i++){
		matrix_3[i] = (float*) malloc(*column_3 * sizeof(float));
		for (int j = 0; j < *column_3; j++){
			sum = 0.0;
			for (int p = 0; p < row_2; p++)
				sum += matrix_1[i][p] * matrix_2[p][j];
			matrix_3[i][j] = sum;
		}
	}			
	return matrix_3;
}

float Matrix_det (float** matrix, int row, int column){
	float determinant = 1.0;
	if (row != column){
		printf("The matrix is not square!\n");
		return 0.0;
	}
	for (int j = 0; j < column; j++){
		int i = j, flag = 0, swap = 0;
		while (i < row){
			if (matrix[i][j] != 0.0){
				flag = 1;
				float deducted = matrix[i][j];
				if (i != j)
					swap = i;
				for (int p = i; p < row; p++)
					if (i != p){
						float deducted_2 = matrix[p][j] / deducted;
						for (int k = j; k < column; k++)
							matrix[p][k] -= deducted_2 * matrix[i][k];
					}
			}
			i++;
		}
		if (flag == 0){
			return 0.0;
		}
		if (swap){
			float* c = matrix[j];
			matrix[j] = matrix[swap];
			matrix[swap] = c;
			determinant *= -1;
			free(c);
		}
		determinant *= matrix[j][j];
		//Matrix_output(matrix, row, column);
	}
	return determinant;
}
