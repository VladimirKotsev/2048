#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

constexpr int WinningNumber = 2048;

bool isDimensionValid(int n)
{
	return n >= 4 && n <= 10;
}

int** initializeMatrix(size_t size)
{
	int** matrix = new int* [size];
	for (size_t row = 0; row < size; row++)
	{
		matrix[row] = new int[size + 1];
		for (size_t col = 0; col < size + 1; col++)
		{
			matrix[row][col] = 0;
		}
	}

	return matrix;
}

void printMatrix(int** matrix, size_t size)
{
	for (size_t row = 0; row < size; row++)
	{
		for (size_t col = 1; col < size + 1; col++)
		{
			cout << matrix[row][col] << setw(5);
		}

		cout << setw(0) << endl;
	}
}

void freeMatrix(int** matrix, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		delete[] matrix[i];
	}

	delete[] matrix;
}

void clearConsoleRows(size_t numRows)
{
	for (size_t i = 0; i < numRows + 1; i++)
	{
		std::cout << "\x1b[A";  //Move cursor up one row
		std::cout << "\x1b[2K";  //Clear entire line
	}
}

bool isRowAvaliable(int** matrix, size_t size, size_t row)
{
	for (size_t col = 0; col < size; col++)
		if (matrix[row][col] == 0)
			return true;

	return false;
}

void generateSpawnPoint(int** matrix, size_t size, size_t& row, size_t& col)
{
	while (true)
	{
		row = rand() % size;
		if (isRowAvaliable(matrix, size, row))
			break;
	}

	col = rand() % size;

	while (matrix[row][col] != 0)
		col = rand() % size;
}

bool sumPointsOfRows(int** matrix, size_t size)
{
	bool isWinner = false;
	for (size_t row = 0; row < size; row++)
	{
		int sum = 0;
		for (size_t col = 0; col < size + 1; col++)
		{
			sum += matrix[row][col];
		}

		matrix[row][0] = sum;

		if (sum == WinningNumber)
			isWinner = true; //game ends
	}

	if(isWinner)
		return true; //game ends
	return false; //game continues
}

void moveUp(int** matrix, size_t size)
{
	int currentRow, currentCol;
	for (size_t col = 1; col < size + 1; col++)
	{
		currentRow = 0;
		currentCol = col;
		for (size_t row = 1; row < size; row++)
		{
			if (matrix[row][col] != 0)
			{
				if (matrix[row - 1][col] == 0 || matrix[row - 1][col] == matrix[row][col])
				{
					if (matrix[currentRow][currentCol] == matrix[row][col])
					{
						matrix[currentRow][currentCol] *= 2;
						matrix[row][col] = 0;
					}
					else
					{
						if (matrix[currentRow][currentCol] == 0)
						{
							matrix[currentRow][currentCol] = matrix[row][col];
							matrix[row][col] = 0;
						}
						else
						{
							matrix[++currentRow][currentCol] = matrix[row][col];
							matrix[row][col] = 0;
						}
					}
				}
				else currentRow++;
			}
		}
	}
}

void moveDown(int** matrix, size_t size)
{
	int currentRow, currentCol;
	for (size_t col = 1; col < size + 1; col++)
	{
		currentRow = size - 1, currentCol = col;
		for (int row = size - 2; row >= 0; row--)
		{
			if (matrix[row][col] != 0)
			{
				if (matrix[row + 1][col] == 0 || matrix[row + 1][col] == matrix[row][col])
				{
					if (matrix[currentRow][currentCol] == matrix[row][col])
					{
						matrix[currentRow][currentCol] *= 2;
						matrix[row][col] = 0;
					}
					else
					{
						if (matrix[currentRow][currentCol] == 0)
						{
							matrix[currentRow][currentCol] = matrix[row][col];
							matrix[row][col] = 0;
						}
						else
						{
							matrix[--currentRow][currentCol] = matrix[row][col];
							matrix[row][col] = 0;
						}
					}
				}
				else currentRow--;
			}
		}
	}
}

void moveRight(int** matrix, size_t size)
{
	int currentRow, currentCol;
	for (size_t row = 0; row < size; row++)
	{
		currentRow = row, currentCol = size;
		for (int col = size; col >= 1; col--)
		{
			if (matrix[row][col] != 0)
			{
				if (matrix[row][col + 1] == 0 || matrix[row][col + 1] == matrix[row][col])
				{
					if (matrix[currentRow][currentCol] == matrix[row][col])
					{
						matrix[currentRow][currentCol] *= 2;
						matrix[row][col] = 0;
					}
					else
					{
						if (matrix[currentRow][currentCol] == 0)
						{
							matrix[currentRow][currentCol] = matrix[row][col];
							matrix[row][col] = 0;
						}
						else
						{
							matrix[currentRow][--currentCol] = matrix[row][col];
							matrix[row][col] = 0;
						}
					}
				}
				else currentCol--;
			}
		}
	}
}

void moveLeft(int** matrix, size_t size)
{
	int currentRow, currentCol;
	for (size_t row = 0; row < size; row++)
	{
		currentRow = row, currentCol = 1;
		for (size_t col = 2;col < size + 1; col++)
		{
			if (matrix[row][col] != 0)
			{
				if (matrix[row][col - 1] == 0 || matrix[row][col - 1] == matrix[row][col])
				{
					if (matrix[currentRow][currentCol] == matrix[row][col])
					{
						matrix[currentRow][currentCol] *= 2;
						matrix[row][col] = 0;
					}
					else
					{
						if (matrix[currentRow][currentCol] == 0)
						{
							matrix[currentRow][currentCol] = matrix[row][col];
							matrix[row][col] = 0;
						}
						else
						{
							matrix[currentRow][++currentCol] = matrix[row][col];
							matrix[row][col] = 0;
						}
					}
				}
				else currentCol++;
			}
		}
	}
}

void gameOn(int** matrix, size_t size)
{
	size_t rdmRow, rdmCol;
	generateSpawnPoint(matrix, size, rdmRow, rdmCol);

	int numberToAdd = rand() % 2 == 0 ? 2 : 4; //ternary operation

	matrix[rdmRow][rdmCol] = numberToAdd;
	printMatrix(matrix, size);

	while (!sumPointsOfRows(matrix, size))
	{

		char direction;
		cin >> direction;

		switch (direction)
		{
			case 'w':
				moveUp(matrix, size);
				break;
			case 'a':
				moveLeft(matrix, size);
				break;
			case 's':
				moveDown(matrix, size);
				break;
			case 'd':
				moveRight(matrix, size);
				break;
		}

		generateSpawnPoint(matrix, size, rdmRow, rdmCol);

		int numberToAdd = rand() % 2 == 0 ? 2 : 4; //ternary operation

		matrix[rdmRow][rdmCol] = numberToAdd;
		//clearConsoleRows(size);
		cout << "---------------------------------" << endl;
		printMatrix(matrix, size);

	}

	int score = 0;
	for (size_t row = 0; row < size; row++)
	{
		score += matrix[row][0];
	}
}

int main()
{
	char command[12] = "";
	char nickname[256] = "";
	size_t size = 0;

	cout << "Start game" << endl << "Leaderboard" << endl << "Quit" << endl;

	cin.getline(command, 12);

	if (strcmp(command, "Quit") == 0)
	{
		return 0;
	}
	else if (strcmp(command, "Leaderboard") == 0)
	{
		//print the current leaderboard
	}
	else if (strcmp(command, "Start game") == 0)
	{
		cout << "Enter a nickname: ";
		cin.getline(nickname, 256);

		cout << "Enter dimension: ";
		cin >> size;

		if (!isDimensionValid(size))
		{
			cout << "Invalid dimension";
			return 0;
		}

		cout << endl;
		int** matrix = initializeMatrix(size);
		gameOn(matrix, size);


		freeMatrix(matrix, size);
	}
	else
	{
		return 0;
	}

}