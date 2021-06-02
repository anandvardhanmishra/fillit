/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amishra <amishra@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 12:15:35 by amishra           #+#    #+#             */
/*   Updated: 2018/05/15 16:44:57 by amishra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** check_links(char const *buff)
** Takes buff, as read by read_input
** Verifies every link between hashes.
** Assuming there is only 4 and 4 hashes
** Returns 1 for valid tetramino, 0 otherwise
*/

static int		check_links(char const *buff)
{
	unsigned char	i;
	unsigned char	link;

	i = 0;
	link = 0;
	while (i < 20)
	{
		if (buff[i] == '#')
		{
			if (i - 1 >= 0 && buff[i - 1] == '#')
				link++;
			if (i + 1 < 20 && buff[i + 1] == '#')
				link++;
			if (i - 5 >= 0 && buff[i - 5] == '#')
				link++;
			if (i + 5 < 20 && buff[i + 5] == '#')
				link++;
		}
		i++;
	}
	return (link == 6 || link == 8);
}

/*
** verify_hashes(char const *buff, int const count)
** Given str, as read by read_input, and given the number of bytes read.
** Verifies if the buffer only contains '.' and '#', and '\n' at the end of each
** line.
** Verifies, if count == 21, that the 21th char read is a '\n'.
** Returns 1 if there is only four well placed hashes in str, and 0 if there is
** any error in the output form.
*/

static int		verify_hashes(char const *buff, int const count)
{
	unsigned char	i;
	unsigned char	hash;

	hash = 0;
	i = 0;
	while (i < 20)
	{
		if (i % 5 != 4)
		{
			if (buff[i] == '#')
				hash++;
			if (buff[i] != '.' && buff[i] != '#')
				return (0);
		}
		else if (buff[i] != '\n')
			return (0);
		i++;
	}
	if (count == 21 && buff[20] != '\n')
		return (0);
	if (hash != 4 || check_links(buff) == 0)
		return (0);
	return ((count == 21) ? 1 : 2);
}

/*
** find_area(char const *buff, unsigned char *area)
** Takes str, as read by read_input, and area, as created in ft_get_tetramino
** Given a valid str as defined by verify_hashes. The area is defined as the
** smallest rectangle which can contain the tetramino.
** area[0] = top corner
** area[1] = bottom corner
** area[2] = left corner
** area[3] = right corner
*/

static void		find_area(char const *buff, unsigned char *area)
{
	unsigned char	i;

	i = 0;
	area[0] = 3;
	area[1] = 0;
	area[2] = 3;
	area[3] = 0;
	while (i < 20)
	{
		if (buff[i] == '#')
		{
			if (i % 5 < area[0])
				area[0] = i % 5;
			if (i % 5 > area[1])
				area[1] = i % 5;
			if (i / 5 < area[2])
				area[2] = i / 5;
			if (i / 5 > area[3])
				area[3] = i / 5;
		}
		i++;
	}
}

/*
** ft_get_tetramino(char *str, char letter)
** Takes str, as read by read_input, & the letter identifying the tetramino
** Given a valid str as defined by verify_hashes.
** Builds the tetramino's structure as following.
** tetra->letter : identifier of the tetramino in the input file
** tetra->width : width of the tetramino
** tetra->height :  height of the tetramino
** tetra->value : representation of the hashes as 1 in a int64_t full of 0. This
** is done by shifting a 1LL (1 long long) by the x-axis and the y axis value of
** the sharps in the local mark;
** Returns NULL if area cant be allocated, returns the tetramino pointer
** otherwise.
*/

static t_tetris	*ft_get_tetramino(t_tetris *tetramino, char const *buff,
	char const letter)
{
	unsigned char	x;
	unsigned char	y;
	unsigned char	*area;

	if (!(area = (unsigned char *)malloc(sizeof(unsigned char) * 4)))
		return (NULL);
	find_area(buff, area);
	tetramino->letter = letter;
	tetramino->width = area[1] - area[0] + 1;
	tetramino->height = area[3] - area[2] + 1;
	tetramino->value = 0LL;
	y = 0;
	while (y < tetramino->height)
	{
		x = 0;
		while (x < tetramino->width)
		{
			if (buff[5 * (area[2] + y) + (area[0] + x)] == '#')
				tetramino->value |= 1LL << (16 * y + x);
			x++;
		}
		y++;
	}
	ft_memdel((void **)&area);
	return (tetramino);
}

/*
** read_input(int const fd, t_tetris *tetris)
** Given a filedescriptor where av[1] is opened, and an tetramino's array
** Stocks valid tetraminos, as defined in verify_hashes, in the array
** Returns 1 if the input file is valid and contains valid tetraminos
** Returns 0 otherwise
*/

char			read_input(int const fd, t_tetris *tetris)
{
	char		letter;
	int			count;
	char		buff[21];
	char		end;

	letter = 'A';
	while ((count = (int)read(fd, buff, 21)) >= 20)
	{
		if (letter - 'A' >= 26
		|| (end = verify_hashes((char const *)buff, count)) == 0)
			return (0);
		if (!(ft_get_tetramino(tetris++, (char const *)buff,
		(char const)letter)))
			return (0);
		letter++;
	}
	if (count != 0)
		return (0);
	else
		return ((letter > 'A' && end == 2) ? letter - 1 : 0);
}
