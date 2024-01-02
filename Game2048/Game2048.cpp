#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

const char* RESET = "\033[0m";
const char* LIGHT_ORANGE = "\033[38;5;224m"; // Light shade of orange
const char* ORANGE = "\033[38;5;202m";  // Orange
const char* DARK_ORANGE = "\033[38;5;166m";  // Dark shade of orange
const char* LIGHT_RED = "\033[91m"; // Light shade of red
const char* RED = "\033[31m"; // Red
const char* DARK_RED = "\033[38;5;88m";   // Dark shade of red
const char* LIGHT_YELLOW = "\033[38;5;228m";  // Light shade of yellow
const char* YELLOW = "\033[93m";   // Yellow
const char* DARK_YELLOW = "\033[38;5;136m";  // Dark shade of yellow
const char* colors[] = { LIGHT_ORANGE, ORANGE, DARK_ORANGE, LIGHT_RED
						 ,RED, DARK_RED, LIGHT_YELLOW, YELLOW, DARK_YELLOW };

constexpr int WinningNumber = 2048;

int logFunc(int num)
{
	int result = 0;
	while (num != 2)
	{
		num >>= 1;
		result++;
	}

	return result;
}

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
			cout << setw(5);
			if (matrix[row][col] == 0)
				cout << ' ';
			else
			{
				int number = matrix[row][col];
				int colorIndex = logFunc(number);

				cout << colors[colorIndex] << setw(5) << matrix[row][col] << RESET;
			}
			cout << '|';
		}

		cout << endl;
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
	for (size_t i = 0; i < numRows; i++)
	{
		cout << "\x1b[A";  //Move cursor up one row
		cout << "\x1b[2K";  //Clear entire line
	}
}

bool isRowAvaliable(int** matrix, size_t size, size_t row)
{
	for (size_t col = 0; col < size; col++)
		if (matrix[row][col] == 0)
			return true;

	return false;
}

bool isGameOver(int** matrix, size_t size)
{
	for (size_t row = 0; row < size; row++)
	{
		for (size_t col = 0; col < size; col++)
		{
			if (matrix[row][col] == 0)
				return false;  //not a game over
		}
	}
	return true;  //game over
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

void sumScore(int** matrix)

bool sumFinalScore(int** matrix, size_t size, int& score)
{
	bool isWinner = false;
	int sum = 0;
	for (size_t row = 0; row < size; row++)
	{
		sum += matrix[row][0];
		for (size_t col = 0; col < size; col++)
		{
			if (matrix[row][col] == 2048)
				isWinner = true;
		}
	}

	return isWinner;
}

bool moveUp(int** matrix, size_t size)
{
	bool noMatrixChange = true;
	int currentRow, currentCol;
	for (size_t col = 0; col < size; col++)
	{
		currentRow = 0;
		currentCol = col;
		for (size_t row = 1; row < size; row++)
		{
			if (matrix[row][col] != 0)
			{
				if (matrix[row - 1][col] == 0 || matrix[row - 1][col] == matrix[row][col])
				{
					noMatrixChange = false;
					if (matrix[currentRow][currentCol] == matrix[row][col])
					{
						matrix[currentRow][currentCol] *= 2;
						matrix[row][col] = 0;
						//notChanged = false;
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

	return noMatrixChange;
}

bool moveDown(int** matrix, size_t size)
{
	bool noMatrixChange = true;
	int currentRow, currentCol;
	for (size_t col = 0; col < size; col++)
	{
		currentRow = size - 1, currentCol = col;
		for (int row = size - 2; row >= 0; row--)
		{
			if (matrix[row][col] != 0)
			{
				if (matrix[row + 1][col] == 0 || matrix[row + 1][col] == matrix[row][col])
				{
					noMatrixChange = false;
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

	return noMatrixChange;
}

bool moveRight(int** matrix, size_t size)
{
	bool noMatrixChange = true;
	int currentRow, currentCol;
	for (size_t row = 0; row < size; row++)
	{
		currentRow = row, currentCol = size;
		for (int col = size - 1; col >= 0; col--)
		{
			if (matrix[row][col] != 0)
			{
				if (matrix[row][col + 1] == 0 || matrix[row][col + 1] == matrix[row][col])
				{
					noMatrixChange = false;
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

	return noMatrixChange;
}

bool moveLeft(int** matrix, size_t size)
{
	bool noMatrixChange = true;
	int currentRow, currentCol;
	for (size_t row = 0; row < size; row++)
	{
		currentRow = row, currentCol = 0;
		for (size_t col = 1;col < size; col++)
		{
			if (matrix[row][col] != 0)
			{
				if (matrix[row][col - 1] == 0 || matrix[row][col - 1] == matrix[row][col])
				{
					noMatrixChange = false;
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

	return noMatrixChange;
}

void mainMenu();

void gameOn(int** matrix, size_t size)
{
	size_t rdmRow, rdmCol;
	generateSpawnPoint(matrix, size, rdmRow, rdmCol);

	int numberToAdd = rand() % 2 == 0 ? 2 : 4; //ternary operation

	matrix[rdmRow][rdmCol] = numberToAdd;

	printMatrix(matrix, size);

	while (!isGameOver(matrix, size))
	{
		char direction;
		cin >> direction;

		bool whileContinue = false;
		bool illegalMove = false;
		switch (direction)
		{
		case 'w':
			illegalMove = moveUp(matrix, size);
			break;
		case 'a':
			illegalMove = moveLeft(matrix, size);
			break;
		case 's':
			illegalMove = moveDown(matrix, size);
			break;
		case 'd':
			illegalMove = moveRight(matrix, size);
			break;
		default: whileContinue = true;
			break;
		}
		if (whileContinue || illegalMove) // wrong input loops while again
		{
			clearConsoleRows(1); //clears last row
			continue;
		}

		generateSpawnPoint(matrix, size, rdmRow, rdmCol);

		int numberToAdd = rand() % 2 == 0 ? 2 : 4; //ternary operation

		matrix[rdmRow][rdmCol] = numberToAdd;
		clearConsoleRows(size + 1); //erases game board from last move
		printMatrix(matrix, size); //prints the game board
	}

	int score = 0;
	if (sumFinalScore(matrix, size, score)) //print winner message
	{
		cout << "=============== YOU WON! ===============";
		cout << "Score: " << score;
	}
	else //print game over message
	{
		cout << "=============== GAME OVER! ===============";
	}

	clearConsoleRows(size + 3);
	mainMenu();
}

void mainMenu()
{
	char command[12] = "";
	char nickname[256] = "";
	size_t size = 0;

	cout << "Start game" << endl << "Leaderboard" << endl << "Quit" << endl;

	cin.getline(command, 12);

	if (strcmp(command, "Leaderboard") == 0) //print the current leaderboard
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
		}

		cout << endl;
		int** matrix = initializeMatrix(size);
		gameOn(matrix, size);

		freeMatrix(matrix, size);
	}
}

int main()
{
	mainMenu();

	return 0;
}