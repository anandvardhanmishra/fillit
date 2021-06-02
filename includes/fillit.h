/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amishra <amishra@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 22:57:01 by amishra           #+#    #+#             */
/*   Updated: 2018/05/21 17:07:53 by amishra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLIT_H
# define FILLIT_H
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include "libft.h"
# include <stdio.h>

typedef struct		s_tetris
{
	char			letter;
	unsigned char	width;
	unsigned char	height;
	unsigned char	x;
	unsigned char	y;
	int64_t			value;
	struct s_tetris	*previous;
}					t_tetris;
unsigned char		ft_solve(t_tetris *tetris, char const letter);
char				read_input(int const fd, t_tetris *tetris);
#endif
