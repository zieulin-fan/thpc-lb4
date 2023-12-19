#include <cstdlib>
#include <iostream>
#include <iostream>
#include <omp.h>

int** generateMatrix(int N);
void printMatrix(int** matrix, int N);
void deleteMatrix(int** matrix, int N);
int findMinColumnSum(int** matrix, int N);
int findOpenMPMinColumnSum(int** matrix, int N);
void printResults(int N);

int main() {
	printResults(500);
	printResults(1000);
	printResults(5000);
	printResults(10000);
	printResults(15000);
	printResults(20000);
	return 0;
}

int** generateMatrix(int N) {
	int** matrix = new int* [N];
	for (int i = 0; i < N; ++i) {
		matrix[i] = new int[N];
	}

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			matrix[i][j] = rand() % 100; 
		}
	}

	return matrix;
}

void printMatrix(int** matrix, int N) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			std::cout << matrix[i][j] << " \n"[j == N - 1];
}

void deleteMatrix(int** matrix, int N) {
	for (int i = 0; i < N; ++i) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

int findMinColumnSum(int** matrix, int N) {
	int minSum = INT_MAX;

	std::cout << "Number of threads: " << omp_get_num_threads() << std::endl;

	for (int i = 0; i < N; ++i) {
		int colSum = 0;
		for (int j = 0; j < N; ++j) {
			colSum += matrix[j][i];
		}
		minSum = std::min(minSum, colSum);
	}

	return minSum;
}

int findOpenMPMinColumnSum(int** matrix, int N) {
	int minSum = INT_MAX;
#pragma omp parallel
	{
		int localMinSum = INT_MAX;
#pragma omp master
		{
			std::cout << "Number of threads: " << omp_get_num_threads() << std::endl;
		}
#pragma omp for 
		for (int i = 0; i < N; ++i) {
			int colSum = 0;
			for (int j = 0; j < N; ++j) {
				colSum += matrix[j][i];
			}
			localMinSum = std::min(localMinSum, colSum);
		}
#pragma omp critical
		{
			minSum = std::min(minSum, localMinSum);
		}
	}
	return minSum;
}

void printResults(int N) {
	std::cout << "=== MATRIX SIZE: " << N << "x" << N << " ===" << std::endl;
	int** matrix = generateMatrix(N);
	//printMatrix(matrix, N);

	double start = omp_get_wtime();
	int result = findMinColumnSum(matrix, N);
	double end = omp_get_wtime();
	double executionTime = end - start;

	std::cout << "Result: " << result << std::endl;
	std::cout << "Execution time: " << executionTime << " s\n" << std::endl;

	double ompStart = omp_get_wtime();
	int ompResult = findOpenMPMinColumnSum(matrix, N);
	double ompEnd = omp_get_wtime();
	double ompExecutionTime = ompEnd - ompStart;

	std::cout << "Result: " << ompResult << std::endl;
	std::cout << "Execution time: " << ompExecutionTime << " s\n" << std::endl;

	deleteMatrix(matrix, N);
}
