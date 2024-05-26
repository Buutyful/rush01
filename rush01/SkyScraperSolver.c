#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int* ColUp;
	int* ColDown;
	int* RowLeft;
	int* RowRight;
}Hints;

int* SplitInput(char* args, int* argc)
{
	int params = 0;
	int index = 0;
	while (args[index])
	{
		if (args[index] != ' ') params++;
		index++;
	}
	*argc = params;

	int* arr = malloc(sizeof(int) * params);
	if (arr == NULL) return NULL;

	int i = 0;
	int j = 0;
	while (args[i])
	{
		while (args[i] == ' ') i++;

		if (args[i] >= '0' && args[i] <= '9' && j < params)
		{
			char c = args[i];
			arr[j] = c - '0';
			j++;
		}
		i++;
	}
	return arr;
}
int** CreateBoard(int size)
{
	int** board = malloc(sizeof(int*) * size);
	if (!board) return NULL;
	int i = 0;
	while (i < size)
	{
		board[i] = malloc(size * sizeof(int));
		i++;
	}
	i = 0;
	while (i < size)
	{
		int j = 0;
		while (j < size)
		{
			board[i][j] = 0;
			j++;
		}		
		i++;
	}
	return board;
}
void FreeBoard(int** board, int size)
{
	int i = 0;
	while (i < size)
	{
		free(board[i]);
		i++;
	}
	free(board);
}
int IsInside(int size, int row, int col)
{
	return row >= 0 && row < size && col >= 0 && col < size;
}
int IsEmpty(int** board, int row, int col, int size)
{
	return IsInside(size, row, col) && board[row][col] == 0;
}
int SetUpInitialBoardState(int** board, int size, Hints* hints)
{
	int max = size;
	int i = 0;
	while (i < size)
	{
		//set up colup hints
		if (hints->ColUp[i] == max)
		{
			int row = 0;
			int num = 1;
			while (row < size)
			{
				if (IsEmpty(board, row, i, size) || board[row][i] == num)
				{
					board[row][i] = num;
					num++;
					row++;
				}
				else
				{
					write(1, "Error\n", 6);
					return 0;
				}
			}
		}
		else if (hints->ColUp[i] == 1)
		{
			if (IsEmpty(board, 0, i, size) || board[0][i] == max)
			{
				board[0][i] = max;
			}
			else
			{
				write(1, "Error\n", 6);
				return 0;
			}
		}

		//set up coldown hints
		if (hints->ColDown[i] == max)
		{
			int row = size - 1;
			int num = 1;
			while (row >= 0)
			{
				if (IsEmpty(board, row, i, size) || board[row][i] == num)
				{
					board[row][i] = num;
					num++;
					row--;
				}
				else
				{
					write(1, "Error\n", 6);
					return 0;
				}
			}
		}
		else if (hints->ColDown[i] == 1)
		{
			if (IsEmpty(board, size - 1, i, size) || board[size - 1][i] == max)
			{
				board[size - 1][i] = max;
			}
			else
			{
				write(1, "Error\n", 6);
				return 0;
			}

		}

		// Set up RowLeft hints
		if (hints->RowLeft[i] == max)
		{
			int col = 0;
			int num = 1;
			while (col < size)
			{
				if (IsEmpty(board, i, col, size) || board[i][col] == num)
				{
					board[i][col] = num;
					num++;
					col++;
				}
				else
				{
					write(1, "Error\n", 6);
					return 0;
				}
			}
		}
		else if (hints->RowLeft[i] == 1)
		{
			if (IsEmpty(board, i, 0, size) || board[i][0] == max) 
			{
				board[i][0] = max;
			}
			else 
			{
				write(1, "Error\n", 6);
				return 0;
			}
		}

		// Set up RowRight hints
		if (hints->RowRight[i] == max) 
		{
			int col = size - 1;
			int num = 1;
			while (col >= 0) 
			{
				if (IsEmpty(board, i, col, size) || board[i][col] == num) 
				{
					board[i][col] = num;
					num++;
					col--;
				}
				else 
				{
					write(1, "Error\n", 6);
					return 0;
				}
			}
		}
		else if (hints->RowRight[i] == 1) 
		{
			if (IsEmpty(board, i, size - 1, size) || board[i][size - 1] == max) 
			{
				board[i][size - 1] = max;
			}
			else 
			{
				write(1, "Error\n", 6);
				return 0;
			}
		}

		i++;
	}

	return 1;

}


int CountVisible(int* arr, int size)
{
	int count = 0;
	int maxHeight = 0;
	for (int i = 0; i < size; i++)
	{
		if (arr[i] > maxHeight)
		{
			maxHeight = arr[i];
			count++;
		}
	}
	return count;
}
int IsValid(int** board, int size, int row, int col, int num, Hints* hints)
{
	// Check row uniqueness
	for (int x = 0; x < size; x++)
		if (board[row][x] == num)
			return 0;

	// Check column uniqueness
	for (int x = 0; x < size; x++)
		if (board[x][col] == num)
			return 0;

	// Place number temporarily to check visibility hints
	board[row][col] = num;

	// Check row left visibility
	if (hints->RowLeft[row] != 0 && CountVisible(board[row], size) > hints->RowLeft[row])
	{
		board[row][col] = 0; // Undo placement
		return 0;
	}

	// Check row right visibility
	int* reversedRow = (int*)malloc(size * sizeof(int));
	if (!reversedRow)
	{
		board[row][col] = 0;
		return 0;
	}
	for (int i = 0; i < size; i++)
		reversedRow[i] = board[row][size - 1 - i];
	if (hints->RowRight[row] != 0 && CountVisible(reversedRow, size) > hints->RowRight[row])
	{
		board[row][col] = 0; // Undo placement
		free(reversedRow);
		return 0;
	}
	free(reversedRow);

	// Check column up visibility
	int* colArr = (int*)malloc(size * sizeof(int));
	if (!colArr)
	{
		board[row][col] = 0;
		return 0;
	}
	for (int i = 0; i < size; i++)
		colArr[i] = board[i][col];
	if (hints->ColUp[col] != 0 && CountVisible(colArr, size) > hints->ColUp[col])
	{
		board[row][col] = 0; // Undo placement
		free(colArr);
		return 0;
	}
	free(colArr);

	// Check column down visibility
	int* reversedCol = (int*)malloc(size * sizeof(int));
	if (!reversedCol)
	{
		board[row][col] = 0;
		return 0;
	}
	for (int i = 0; i < size; i++)
		reversedCol[i] = board[size - 1 - i][col];
	if (hints->ColDown[col] != 0 && CountVisible(reversedCol, size) > hints->ColDown[col])
	{
		board[row][col] = 0; // Undo placement
		free(reversedCol);
		return 0;
	}
	free(reversedCol);

	board[row][col] = 0; // Undo placement, will be set by caller if valid
	return 1;
}
int Solve(int** board, int size, Hints* hints)
{
	int row = -1;
	int col = -1;
	int isEmpty = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (board[i][j] == 0)
			{
				row = i;
				col = j;
				isEmpty = 1;
				break;
			}
		}
		if (isEmpty)
			break;
	}

	if (!isEmpty)
		return 1;

	for (int num = 1; num <= size; num++)
	{
		if (IsValid(board, size, row, col, num, hints))
		{
			board[row][col] = num;
			if (Solve(board, size, hints))
				return 1;
			board[row][col] = 0;
		}
	}
	return 0;
}

int main(int count, char** args)
{
	int params = 0;
	int* hints = SplitInput(args[1], &params);
	for (int i = 0; i < params; i++) printf("%d, ", hints[i]);
	write(1, "\n", 1);
	int size = params / 4;	
	int** board = CreateBoard(size);

	printf("Board:\n");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d ", board[i][j]);
		}
		printf("\n");
	}
	int* colUp = malloc(size * sizeof(int));
	int* colDown = malloc(size * sizeof(int));
	int* rowLeft = malloc(size * sizeof(int));
	int* rowRight = malloc(size * sizeof(int));
	Hints instructions = { colUp, colDown, rowLeft, rowRight };

	if (colUp == NULL || colDown == NULL || rowLeft == NULL || rowRight == NULL)
	{
		write(1, "Error\n", 6);
		free(hints);
		if (colUp) free(colUp);
		if (colDown) free(colDown);
		if (rowLeft) free(rowLeft);
		if (rowRight) free(rowRight);
		return 1;
	}


	int j = 0;
	while (j < params)
	{
		if (j < size)
		{
			colUp[j] = hints[j];
		}
		else if (j < size * 2)
		{
			int index = j % size;
			colDown[index] = hints[j];
		}
		else if (j < size * 3)
		{
			int index = j % size;
			rowLeft[index] = hints[j];
		}
		else
		{
			int index = j % size;
			rowRight[index] = hints[j];
		}
		j++;
	}
	SetUpInitialBoardState(board, size, &instructions);

	if (Solve(board, size, &instructions))
	{
		printf("Solution found:\n");
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				printf("%d ", board[i][j]);
			}
			printf("\n");
		}
	}
	else
	{
		printf("No solution exists\n");
	}
	//clean up
	free(hints);
	free(colUp);
	free(colDown);
	free(rowLeft);
	free(rowRight);
	FreeBoard(board, size);
	return 0;
}