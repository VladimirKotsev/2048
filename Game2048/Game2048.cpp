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
		matrix[row] = new int[size];
		for (size_t col = 0; col < size; col++)
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
		for (size_t col = 0; col < size; col++)
		{
			cout << matrix[row][col] << setw(5);
		}
		cout << setw(0) << endl << endl;
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
	for (size_t i = 0; i < numRows; ++i)
	{
		std::cout << "\x1b[A";  //Move cursor up one row
		std::cout << "\x1b[2K";  //Clear entire line
	}
}

bool isWinner(int** matrix, size_t size)
{
	for (size_t row = 0; row < size; row++)
		for (size_t col = 0; col < size; col++)
			if (matrix[row][col] == WinningNumber)
				return true;

	return false;
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

void

void gameOn(int** matrix, size_t size)
{
	while (!isWinner(matrix, size))
	{
		size_t rdmRow, rdmCol;
		generateSpawnPoint(matrix, size, rdmRow, rdmCol);

		int numberToAdd = rand() % 2 == 0 ? 2 : 4; //ternary operation

		matrix[rdmRow][rdmCol] = numberToAdd;
		printMatrix(matrix, size);

		char direction;
		cin >> direction;
	}
}

int main()
{
	char command[12] = "Start game";
	char nickname[256] = "gfdgd";
	size_t size = 0;

	cout << "Start game" << endl << "Leaderboard" << endl << "Quit" << endl;

	//cin.getline(command, 12);

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
		//cout << "Enter a nickname: ";
		//cin.getline(nickname, 256);

		cout << "Enter dimension: ";
		cin >> size;

		if (!isDimensionValid(size))
		{
			cout << "Invalid dimension";
			return 0;
		}

		int** matrix = initializeMatrix(size);
		gameOn(matrix, size);


		freeMatrix(matrix, size);
	}
	else
	{
		return 0;
	}

}

