/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amishra <amishra@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 22:53:00 by amishra           #+#    #+#             */
/*   Updated: 2018/05/21 17:01:18 by amishra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** connect(t_tetris *tetris)
** Takes the whole valid tetris structure.
** Establishes a link between a tetramino and it's predecessor, defined as the
** closest previous tetramino which has the same value.
** The N tetramino is now linked to the N - 1 tetramino
*/

static void			connect(t_tetris *tetris)
{
	unsigned char	i;
	unsigned char	j;

	i = 0;
	while ((tetris[i]).letter != 0)
	{
		tetris[i].previous = NULL;
		j = i;
		while (j - 1 >= 0)
		{
			j--;
			if ((tetris[j].value ^ tetris[i].value) == 0LL)
			{
				tetris[i].previous = &(tetris[j]);
				j = 0;
			}
		}
		i++;
	}
}

/*
** start_point(t_tetris *tetra)
** Takes a pointer on a tetramino we are trying to place.
** Initialises the x-coordinate and the y-coordinate
** of the relative mark at (0,0) if
** there is no similar predecessor. Otherwise, doesn't try to place the new
** tetramino before its predecessor, by taking (previous->y and previous->x
** + width) as the starting point of the search.
*/

static void			start_point(t_tetris *tetra)
{
	if (tetra->previous == NULL)
	{
		tetra->y = 0;
		tetra->x = 0;
	}
	else
	{
		tetra->y = tetra->previous->y;
		tetra->x = tetra->previous->x + tetra->width - 1;
	}
}

/*
** solve_board(t_tetris *tetris, unsigned char const size,
** int16_t *board, unsigned char count)
** Takes a valid tetris array as read by read_input, the supposed minimal
** size of the solution, and the board of the solution
** The functions returns 1 if there is no more tetramino to be placed. Returns 0
** otherwise.
** Calls start_point in order to not start searching a valid place for a
** tetramino that has a similar predecessor.
** Recursive backtracking implementation.
** Compares the occupation sites of the board with the ones inside the value,
** by moving the local mark of the tetramino (represented by x & y) until the
** ones on the board (which represent the already placed tetraminos) are not
** supperposed on the ones from the tetramino.
** If there is, at least, one match, then the function shifts the position of
** the tetramino, i.e, shifts the value which
** represents the tetramino
** from 1 to the right. Does it as long as there is is no solution,
** and keeps the result in x (x-coordinate in the "board").
** If no horizontal shifts gives a
** solution, then the function shifts y-axis cooordinate (y),
** and stocks final ordinate in y.
** The final x & y are the position where the placed tetramino gives a solution
** for every other tetraminos. In order to explore every possibility for the
** others, the recursive call, if doesn't succeed, has to delete the placement
** of the tetramino, with the XOR operator (exclusive OR : ^=).
*/

static int			solve_board(t_tetris *tetris, unsigned char const size,
	int16_t *board)
{
	if (tetris->letter == 0)
		return (1);
	start_point(tetris);
	while (tetris->y + tetris->height <= size)
	{
		while (tetris->x + tetris->width <= size)
		{
			if (!(*(int64_t *)(board + tetris->y) & tetris->value << tetris->x))
			{
				*(int64_t *)(board + tetris->y) ^= tetris->value << tetris->x;
				if (solve_board(tetris + 1, size, board) == 1)
					return (1);
				*(int64_t *)(board + tetris->y) ^= tetris->value << tetris->x;
			}
			(tetris->x)++;
		}
		tetris->x = 0;
		(tetris->y)++;
	}
	return (0);
}

/*
** ft_save_and_retry(t_tetris *tetris, t_tetris *current, char letter)
** Copy the state of tetris to current, and put x and y of tetris to 0.
*/

static int			save_retry(t_tetris *tetris, t_tetris *current,
	char letter)
{
	while (letter-- >= 'A')
	{
		current->x = tetris->x;
		current->y = tetris->y;
		tetris->x = 0;
		tetris->y = 0;
		current->width = tetris->width;
		current->height = tetris->height;
		current->letter = tetris->letter;
		current++;
		tetris++;
	}
	return (0);
}

/*
** ft_solve(t_tetris *tetris, unsigned char count)
** Takes a valid tetris array as read by read_input and the number of\
** tetraminos.
** Initialises the solution board with 0.
** Increases size so the board can, at least, contain the area of the tetris
** structure.
** Connects the N tetramino of thwith the N - 1 tetramino of the same
** form.
** Calls solve_board with the given board and a specific size. Starting with
** size == 2 and increments size until size == 13 or until a solution is found.
** Indeed, a 13 x 13 square is the larger minimal square which can contain 26
** tetraminos.
** Returns the size of the solution, or 0 if the board could not be created.
*/

unsigned char		ft_solve(t_tetris *tetris, char const letter)
{
	unsigned char	size;
	int16_t			*board;
	t_tetris		*current;

	size = 13;
	if (!(current = (t_tetris *)malloc(sizeof(t_tetris) * (letter - 'A' + 1))))
		return (0);
	if (!(board = (int16_t *)malloc(sizeof(int16_t) * 16)))
		return (0);
	ft_bzero(board, sizeof(int16_t) * 16);
	connect(tetris);
	while (size * size >= ((letter + 1 - 'A') * 4) && solve_board(tetris,
		size, board) == 1)
	{
		size--;
		ft_bzero(board, sizeof(int16_t) * 16);
		save_retry(tetris, current, letter);
	}
	save_retry(current, tetris, letter);
	ft_memdel((void **)&board);
	ft_memdel((void **)&current);
	return (size + 1);
}
