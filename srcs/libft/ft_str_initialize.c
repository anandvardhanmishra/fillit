/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_initialize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amishra <amishra@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 23:38:31 by amishra           #+#    #+#             */
/*   Updated: 2018/04/23 08:39:54 by amishra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_str_initialize(size_t size, char c)
{
	char		*str;
	size_t		i;

	if (!(str = (char *)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	else
	{
		i = 0;
		while (i < size)
			str[i++] = c;
		str[i] = 0;
		return (str);
	}
}
