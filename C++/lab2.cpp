#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Функция для заполнения массива от 1 до N*N.
 * 
 * @param array Двумерный массив элементов.
 * @param N Размерность матрицы.
 */
void fillArray(int** array, int N) {
    int value = 1;
    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < N; j++) {
            array[i][j] = value;
            value++;
        }
    }
}

/**
 * Функция для вывода элементов двумерного массива на консоль.
 * 
 * @param array Двумерный массив элементов.
 * @param N Размерность матрицы.
 */
void printArray(int** array, int N) {
    cout << "Matrix: " << N << "*" << N << endl;
    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < N; j++) {
            cout << array[i][j] << '\t';
        }
        cout << endl;
    }
}

/**
 * Функция для восстановления прямого порядка элементов матрицы.
 * 
 * @param array Двумерный массив элементов.
 * @param N Размерность матрицы.
 */
void restore_direct_order(int** array, int N) {
    cout << "Original matrix:" << endl;
    fillArray(array, N);
    printArray(array, N);
}

/**
 * Функция для реверсирования порядка элементов в матрице.
 * 
 * @param array Двумерный массив элементов.
 * @param N Размерность матрицы.
 */
void reverse_order(int** array, int N) {
    cout << "Reversing matrix:" << endl;
    for (unsigned i = 0; i < N; i++) {
        reverse(array[i], array[i] + N);;
    }
    reverse(array, array + N);
    printArray(array, N);
}

/**
 * Функция для размещения элементов вдоль главной диагонали.
 *
 * @param array Двумерный массив элементов.
 * @param N Размерность матрицы.
 */
void main_diagonal(int** array, int N) {
    cout << "A matrix with elements laid out along the main diagonal:" << endl;

    int index = 1;
    for (int d = 0; d < 2 * N - 1; d++) {
        for (int i = 0; i <= d; i++) {
            int j = N - 1 - (d - i);
            if (i < N && j >= 0) {
                array[i][j] = index++;
            }
        }
    }
    printArray(array, N);
}

/**
 * Функция для размещения элементов вдоль побочной диагонали.
 * 
 * @param array Двумерный массив элементов.
 * @param N Размерность матрицы.
 */
void secondary_diagonal(int** array, int N) {
    cout << "A matrix with elements laid out along the secondary diagonal:" << endl;

    int index = 1;
    for (int d = 0; d < 2 * N - 1; d++) {
        for (int i = 0; i <= d; i++) {
            int j = d - i;
            if (i < N && j < N) {
                array[i][j] = index++;
            }
        }
    }
    printArray(array, N);
}

/**
 * Функция для размещения элементов в матрице по спирали.
 * 
 * @param array Двумерный массив элементов.
 * @param N Размерность матрицы.
 */
void spiral(int** array, int N) {
    cout << "A matrix with elements whose values ​​are twisted clockwise in a spiral:" << endl;

    int top = 0, left = 0, right = N - 1, bottom = N - 1;
    int** spiralMatrix = new int* [N];
    for (unsigned i = 0; i < N; i++) {
        spiralMatrix[i] = new int[N];
    }

    int index = 1;
    while (top <= bottom && left <= right) {
        for (int i = left; i <= right; i++) {
            spiralMatrix[top][i] = index++;
        }
        top++;
        for (int i = top; i <= bottom; i++) {
            spiralMatrix[i][right] = index++;
        }
        right--;
        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                spiralMatrix[bottom][i] = index++;
            }
            bottom--;
        }
        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                spiralMatrix[i][left] = index++;
            }
            left++;
        }
    }

    printArray(spiralMatrix, N);

    for (unsigned i = 0; i < N; i++) {
        delete[] spiralMatrix[i];
    }
    delete[] spiralMatrix;
}

int main() {
    int N;
    cout << "Enter the size of the array (square matrix): ";
    cin >> N;

    int** array{ new int* [N] {} };
    for (unsigned i{}; i < N; i++) {
        array[i] = new int[N] {};
    }

    fillArray(array, N);
    printArray(array, N);

    int choice;
    do {
        cout << "Select an action by entering numbers from 1 to 5:" << endl;
        cout << "1 - Restore the direct order of the elements" << endl;
        cout << "2 - Reverse the order of the elements" << endl;
        cout << "3 - Make the order of the elements such that they are laid out along the main diagonal" << endl;
        cout << "4 - Make the order of the elements such that they are laid out along the secondary diagonal" << endl;
        cout << "5 - Make the order of the elements such that all values are twisted clockwise in a spiral" << endl;
        cout << "0 - Exit" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            restore_direct_order(array, N);
            break;
        case 2:
            reverse_order(array, N);
            break;
        case 3:
            main_diagonal(array, N);
            break;
        case 4:
            secondary_diagonal(array, N);
            break;
        case 5:
            spiral(array, N);
            break;
        case 0:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice, please try again." << endl;
        }

    } while (choice != 0);

    for (unsigned i{}; i < N; i++)
    {
        delete[] array[i];
    }
    delete[] array;

    return 0;
}
