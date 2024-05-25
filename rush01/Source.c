#include <stdio.h>
#include <stdlib.h>

int Max(int* arr, int len)
{
	int current = 0;
	int i = 0;
	while (i < len)
	{
		if (arr[i] > current) current = arr[i];
		i++;
	}
	return current;
}
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
		if (args[i] != ' ')
		{
			arr[j] = args[i] - 48;
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
	int j = 0;
	while (i < size)
	{
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

int main(int count, char** args)
{
	int params = 0;
	int* hints = SplitInput(args[1], &params);
	int max = Max(hints, params);
	int sideLen = params / 4;
	int** board = CreateBoard(sideLen);
	int* colUp = malloc(sideLen * sizeof(int));
	int* colDown = malloc(sideLen * sizeof(int));	
	int* rowLeft = malloc(sideLen * sizeof(int));
	int* rowRight = malloc(sideLen * sizeof(int));

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
	while (j <= params)
	{		
		if (j < sideLen)
		{
			colUp[j] = hints[j];
		}
		else if (j < sideLen * 2)
		{
			int index = j % sideLen;
			colDown[index] = hints[j];
		}
		else if (j < sideLen * 3)
		{
			int index = j % sideLen;
			rowLeft[index] = hints[j];
		}
		else
		{
			int index = j % sideLen;
			rowRight[index] = hints[j];
		}
		j++;
	}

	//clean up
	free(hints);
	free(colUp);
	free(colDown);
	free(rowLeft);
	free(rowRight);
	FreeBoard(board, sideLen);
	return 0;
}