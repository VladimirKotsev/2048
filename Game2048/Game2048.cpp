#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>

using namespace std;

//ANSI escape codes for coloring console
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

constexpr int colorsArraySize = 9;
constexpr int WinningNumber = 2048;
constexpr int CommandSize = 12;
constexpr int NicknameSize = 256;
constexpr int FileNameSize = 31;
constexpr int ScoreLenght = 10;
constexpr int TopFive = 5;
constexpr int LeaderboardSize = 5;
constexpr int PlacementSize = 2;
constexpr int LineSize = 512;

char nickname[NicknameSize] = "";
size_t timesPlayed = 0;

void printGameName()
{
	cout << " .----------------.  .----------------.  .----------------.  .----------------. " << endl;
	cout << "| .--------------. || .--------------. || .--------------. || .-------------.  |" << endl;
	cout << "| |    _____     | || |     ____     | || |   _    _     | || |     ____     | |" << endl;
	cout << "| |   / ___ `.   | || |   .'    '.   | || |  | |  | |    | || |   .' __ '.   | |" << endl;
	cout << "| |  |_/___) |   | || |  |  .--.  |  | || |  | |__| |_   | || |   | (__) |   | |" << endl;
	cout << "| |   .'____.'   | || |  | |    | |  | || |  |____   _|  | || |   .`____'.   | |" << endl;
	cout << "| |  / /____     | || |  |  `--'  |  | || |      _| |_   | || |  | (____) |  | |" << endl;
	cout << "| |  |_______|   | || |   '.____.'   | || |     |_____|  | || |  `.______.'  | |" << endl;
	cout << "| |              | || |              | || |              | || |              | |" << endl;
	cout << "| '--------------' || '--------------' || '--------------' || '--------------' |" << endl;
	cout << " '----------------'  '----------------'  '----------------'  '----------------' " << endl;
}

void inputBufferReset()
{
	cin.clear(); // clears errors flags from the cin
	cin.sync();	// discard unread characters from the input buffer
	cin.ignore(); // discard characters from the input buffer
}

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

unsigned colorSelector(int n)
{
	return n % 9;
}

int myStrcmp(const char* first, const char* second)
{
	while ((*first) && (*second) && ((*first) == (*second)))
	{
		first++;
		second++;
	}

	return (*first - *second);
}

//size_t topScoresCount(char*** leaderboard)
//{
//	if (leaderboard == nullptr)
//		return 0;
//
//	size_t length = 0;
//	while (*leaderboard[0] != " ")
//	{
//		length++;
//		leaderboard++;
//	}
//
//
//	return length;
//}

void myStrcpy(const char* source, char* dest)
{
	if (!source || !dest)
		return;

	while (*source)
	{
		*dest = *source;
		dest++;
		source++;
	}

	*dest = '\0';
}

char getCharFromDigit(int digit)
{
	if (digit < 0 || digit > 9)
		return '\0';
	return digit + '0';
}

unsigned getNumberLength(unsigned int n)
{

	if (n == 0)
		return 1;
	unsigned int res = 0;

	while (n != 0)
	{
		res++;
		n /= 10;
	}
	return res;
}

void toString(unsigned int n, char* str)
{
	unsigned int len = getNumberLength(n);

	for (int i = len - 1; i >= 0; i--)
	{
		str[i] = getCharFromDigit(n % 10);
		n /= 10;
	}

	str[len] = '\0';
}

unsigned getEndOfTokenIndex(const char* str, unsigned ch)
{
	for (int i = 0;; i++)
	{
		if (str[i] == '\0' || str[i] == ch)
			return i;
	}

	return -1;
}

void copyNChars(const char* source, char* dest, unsigned N)
{
	for (int i = 0; i < N; i++)
		dest[i] = source[i];
}

int convertCharToDigit(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	return -1;
}

unsigned convertStrToUnsigned(const char* str)
{
	if (!str)
		return 0;

	unsigned result = 0;
	while (*str)
	{
		int digit = convertCharToDigit(*str);
		if (digit == -1)
			return 0;
		(result *= 10) += digit;
		str++;
	}
	return result;
}

int convertStrToSigned(const char* str)
{
	if (!str)
		return 0;

	if (*str == '-')
		return -1 * convertStrToUnsigned(str + 1);
	else
		return convertStrToUnsigned(str);
}

char** split(const char* str, char separator)
{
	unsigned resultSize = 2;
	char** result = new char* [resultSize];

	unsigned resultIndex = 0;

	while (*str)
	{
		int sepIndex = getEndOfTokenIndex(str, separator);

		char* currentToken = new char[sepIndex + 1];
		currentToken[sepIndex] = '\0'; //the string sentinel

		copyNChars(str, currentToken, sepIndex);
		str += sepIndex; //skip the token

		result[resultIndex++] = currentToken;

		if (*str == separator)
			str++;
	}

	return result;
}

void freeLeaderboard(char*** leaderboard)
{
	for (int i = 0; i < LeaderboardSize; ++i)
	{
		for (int j = 0; j < PlacementSize; ++j)
			delete[] leaderboard[i][j];
		delete[] leaderboard[i];
	}

	delete[] leaderboard;
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
		matrix[row] = new int[size + 1];
		for (size_t col = 0; col < size + 1; col++)
		{
			matrix[row][col] = 0;
		}
	}

	return matrix;
}

char*** initializeLeaderboard()
{
	char*** leaderboard = new char** [LeaderboardSize];

	for (int i = 0; i < LeaderboardSize; ++i)
	{
		leaderboard[i] = new char* [PlacementSize];

		for (int j = 0; j < PlacementSize; ++j)
		{
			leaderboard[i][j] = new char[LineSize];
			myStrcpy("", leaderboard[i][j]);  // Initialize with a sample string
		}
	}

	return leaderboard;
}

void printMatrix(int** matrix, size_t size)
{
	for (size_t row = 0; row < size; row++)
	{
		for (size_t col = 1; col < size + 1; col++)
		{
			cout << setw(5);
			if (matrix[row][col] == 0)
				cout << ' ';
			else
			{
				int number = matrix[row][col];
				int colorIndex = logFunc(number);

				cout << colors[colorSelector(colorIndex)] << setw(5) << matrix[row][col] << RESET;
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
	for (size_t col = 1; col < size + 1; col++)
		if (matrix[row][col] == 0)
			return true;

	return false;
}

bool isGameOver(int** matrix, size_t size)
{
	for (size_t row = 0; row < size; row++)
	{
		for (size_t col = 1; col < size + 1; col++)
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

	col = rand() % size + 1;

	while (matrix[row][col] != 0)
		col = rand() % size + 1;
}

bool isWinner(int** matrix, size_t size)
{
	for (size_t row = 0; row < size; row++)
	{
		for (size_t col = 1; col < size + 1; col++)
		{
			if (matrix[row][col] == WinningNumber)
				return true;
		}
	}

	return false;
}

int sumFinalScore(int** matrix, size_t size)
{
	int score = 0;
	for (size_t row = 0; row < size; row++)
		score += matrix[row][0];

	return score;
}

void sumInMatrixByRow(int** matrix, size_t size, size_t row)
{
	matrix[row][0] = 0;
	for (size_t col = 1; col < size + 1; col++)
		matrix[row][0] += matrix[row][col];
}

bool moveUp(int** matrix, size_t size)
{
	bool noMatrixChange = true;
	int currentRow, currentCol;
	sumInMatrixByRow(matrix, size, 0);
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
					noMatrixChange = false;
					if (matrix[currentRow][currentCol] == matrix[row][col])
					{
						matrix[currentRow][currentCol] *= 2;

						matrix[currentRow][0] += matrix[row][col]; //changes the score
						matrix[row][0] -= matrix[row][col];

						matrix[row][col] = 0;
					}
					else
					{
						if (matrix[currentRow][currentCol] == 0)
						{
							matrix[currentRow][currentCol] = matrix[row][col];

							matrix[currentRow][0] += matrix[row][col]; //changes the score
							matrix[row][0] -= matrix[row][col];

							matrix[row][col] = 0;
						}
						else
						{
							matrix[++currentRow][currentCol] = matrix[row][col];

							matrix[currentRow][0] += matrix[row][col]; //changes the score
							matrix[row][0] -= matrix[row][col];

							matrix[row][col] = 0;
						}
					}
				}
				else currentRow++;
			}

			if (matrix[row][0] < 0)
				matrix[row][0] = 0;
		}
	}

	return noMatrixChange;
}

bool moveDown(int** matrix, size_t size)
{
	bool noMatrixChange = true;
	int currentRow, currentCol;
	sumInMatrixByRow(matrix, size, size - 1);
	for (size_t col = 1; col < size + 1; col++)
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

						matrix[currentRow][0] += matrix[row][col]; //changes the score
						matrix[row][0] -= matrix[row][col];

						matrix[row][col] = 0;
					}
					else
					{
						if (matrix[currentRow][currentCol] == 0)
						{
							matrix[currentRow][currentCol] = matrix[row][col];

							matrix[currentRow][0] += matrix[row][col]; //changes the score
							matrix[row][0] -= matrix[row][col];

							matrix[row][col] = 0;
						}
						else
						{
							matrix[--currentRow][currentCol] = matrix[row][col];

							matrix[currentRow][0] += matrix[row][col]; //changes the score
							matrix[row][0] -= matrix[row][col];

							matrix[row][col] = 0;
						}
					}
				}
				else currentRow--;
			}

			if (matrix[row][0] < 0)
				matrix[row][0] = 0;
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
		currentRow = row, currentCol = size + 1;
		for (int col = size; col >= 1; col--)
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

			if (matrix[row][0] < 0)
				matrix[row][0] = 0;
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
		currentRow = row, currentCol = 1;
		for (size_t col = 2; col < size + 1; col++)
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

			if (matrix[row][0] < 0)
				matrix[row][0] = 0;
		}
	}

	return noMatrixChange;
}

void printInstructions()
{
	cout << "  w --> up" << endl;
	cout << "  a --> left" << endl;
	cout << "  s --> down" << endl;
	cout << "  d --> right" << endl << endl;
}

char* getLeaderboardFileName(size_t size)
{
	char leaderboard[LeaderboardSize];
	char fileName[FileNameSize];

	switch (size)
	{
	case 4:
		myStrcpy("Leaderboards/Leaderboard_4.txt", fileName);
		break;
	case 5:
		myStrcpy("Leaderboards/Leaderboard_5.txt", fileName);
		break;
	case 6:
		myStrcpy("Leaderboards/Leaderboard_6.txt", fileName);
		break;
	case 7:
		myStrcpy("Leaderboards/Leaderboard_7.txt", fileName);
		break;
	case 8:
		myStrcpy("Leaderboards/Leaderboard_8.txt", fileName);
		break;
	case 9:
		myStrcpy("Leaderboards/Leaderboard_9.txt", fileName);
		break;
	case 10:
		myStrcpy("Leaderboards/Leaderboard_10.txt", fileName);
		break;
	}

	return fileName;
}

void readFromLeaderboard(size_t size)
{
	char leaderboard[LeaderboardSize];
	char* fileName = getLeaderboardFileName(size);

	ifstream ifs(fileName);

	if (ifs.is_open())
	{
		cout << "Leaderboard for size " << size << "x" << size << ":" << endl;
		for (size_t i = 1; i < TopFive; i++)
		{
			ifs.getline(leaderboard, LeaderboardSize, '\n');
			cout << leaderboard << endl;
		}
	}

	ifs.close();
}

void swapPlayers(char**& a, char**& b)
{
	char** temp = a;
	a = b;
	b = temp;
}

void sortLeaderboard(char*** leaderboard)
{
	size_t count = LeaderboardSize;
	unsigned lastSwapedIndex = count - 1;

	for (int i = 0; i < count - 1; i++)
	{
		int currentIterLastSwapped = 0;
		for (int j = 0; j < lastSwapedIndex; j++)
		{
			if (leaderboard[j][1] > leaderboard[j + 1][1])
			{
				swapPlayers(leaderboard[j], leaderboard[j + 1]);
				currentIterLastSwapped = j;
			}
		}

		if (currentIterLastSwapped == 0)
			return;

		lastSwapedIndex = currentIterLastSwapped;
	}
}

char*** getNewLeaderboard(char* fileName, int score)
{
	char*** leaderboard = initializeLeaderboard();
	ifstream ifs(fileName);

	bool newBest = false;
	if (ifs.is_open())
	{
		char strScore[ScoreLenght];
		toString(score, strScore);
		for (size_t i = 1; i <= TopFive; i++)
		{
			char currLine[LineSize];
			ifs.getline(currLine, LineSize, '\n');
			if (myStrcmp(currLine, "") == 0) // reached empty space ==> always a new best score!
			{
				leaderboard[i - 1][0] = nickname;
				leaderboard[i - 1][1] = strScore;
				newBest = true;
				break;
			}
			char** currPLayer = split(currLine, '-');
			leaderboard[i - 1][0] = currPLayer[0];
			leaderboard[i - 1][1] = currPLayer[1];
			if (myStrcmp(nickname, leaderboard[i - 1][0]) == 0 && score > convertStrToSigned(leaderboard[i - 1][1])) // if player has played before
			{
				leaderboard[i - 1][1] = strScore;
				newBest = true;
				break;
			}
			else if (score > convertStrToSigned(leaderboard[i - 1][1])) //removes last position(5) and adds the new one
			{
				leaderboard[4][0] = nickname;
				leaderboard[4][1] = strScore;
				newBest = true;
				break;
			}

		}
	}

	ifs.close();
	if (newBest)
	{
		sortLeaderboard(leaderboard);
		return leaderboard;
	}
	else
		return nullptr;

}

bool writeToLeaderboard(size_t size, int score)
{
	char* fileName = getLeaderboardFileName(size);
	char*** leaderboard = getNewLeaderboard(fileName, score);
	if (!leaderboard)
		return false;

	ofstream ofs(fileName);
	if (ofs.is_open())
	{
		for (size_t i = 0; i < TopFive; i++)
		{
			if (leaderboard[i][0] == "")
			{
				break;
			}
			
			ofs.put(i + 1);
			ofs << ". " << leaderboard[i][0] << "-" << leaderboard[i][1];
			ofs.put('\n');
		}
	}

	freeLeaderboard(leaderboard);

	ofs.clear(); // изчистваме грешките от потока
	ofs.close(); // затваряме потока
	return true;
}

int mainMenu();

int gameOn(int** matrix, size_t size)
{
	printInstructions();
	size_t turnCount = 0;
	size_t rdmRow, rdmCol;
	generateSpawnPoint(matrix, size, rdmRow, rdmCol);

	int numberToAdd = rand() % 2 == 0 ? 2 : 4; //ternary operation

	matrix[rdmRow][rdmCol] = numberToAdd; //adds the number in the random tile
	matrix[rdmRow][0] += numberToAdd; //adds to the sum of the row

	printMatrix(matrix, size);
	int score = numberToAdd;
	cout << "Score: " << score << endl << endl;

	while (!isGameOver(matrix, size))
	{
		turnCount++;
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
		default:
			whileContinue = true;
			break;
		}

		if (whileContinue || illegalMove) // wrong input loops while again
		{
			clearConsoleRows(1); //clears last row
			continue;
		}

		generateSpawnPoint(matrix, size, rdmRow, rdmCol);

		int numberToAdd = rand() % 2 == 0 ? 2 : 4; //ternary operation
		matrix[rdmRow][0] += numberToAdd;
		matrix[rdmRow][rdmCol] = numberToAdd;

		clearConsoleRows(size + 3); //erases game board from last move
		printMatrix(matrix, size); //prints the game board

		score = sumFinalScore(matrix, size);
		cout << "Score: " << score << endl << endl;
	}

	bool newBest = false;
	clearConsoleRows(size + 3);
	if (isWinner(matrix, size)) //print winner message
	{
		clearConsoleRows(6);
		cout << endl << "================== YOU WON! ==================";
	}
	else //print game over message
	{
		clearConsoleRows(6);
		cout << endl << "================== GAME OVER! ==================";
	}

	newBest = writeToLeaderboard(size, score);
	if (newBest)
		cout << endl << "New best score: " << score << endl << endl;
	else
		cout << endl << "Your score: " << score << endl << endl;

	freeMatrix(matrix, size);
	return mainMenu();
}

int mainMenu()
{
	int exitCode = -1;
	timesPlayed++;

	char command[CommandSize] = "";
	size_t size = 0;
	cout << "Enter: " << endl;
	cout << "Start game" << endl << "Leaderboard" << endl << "Quit" << endl;

	if (timesPlayed > 1)
		inputBufferReset();
	cin.getline(command, CommandSize);

	clearConsoleRows(5);

	if (myStrcmp(command, "Leaderboard") == 0) //print the current leaderboard
	{
		cout << "Ender size for leaderboard: ";
		cin >> size;
		clearConsoleRows(1);

		if (!isDimensionValid(size))
		{
			cout << "Invalid dimension";
			return -1;
		}

		readFromLeaderboard(size);

		mainMenu();
	}
	else if (myStrcmp(command, "Quit") == 0)
	{
		return 0;
	}
	else if (myStrcmp(command, "Start game") == 0)
	{
		if (timesPlayed == 1)
		{
			cout << "Enter a nickname: ";
			cin.getline(nickname, 256);
		}
		else if (timesPlayed > 1)
			clearConsoleRows(1);

		cout << "Enter dimension: ";
		cin >> size;

		if (!isDimensionValid(size))
		{
			cout << "Invalid dimension";
			return -1;
		}

		cout << endl;
		int** matrix = initializeMatrix(size);
		exitCode = gameOn(matrix, size);
	}

	return exitCode;
}

int main()
{
	srand(time(NULL));
	printGameName();

	return mainMenu();
}