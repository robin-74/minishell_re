/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_reverse_alphabet.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryounes <ryounes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:51:24 by ryounes           #+#    #+#             */
/*   Updated: 2024/04/18 13:00:38 by ryounes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strchr(const char *s, int c)
{
	char	*ptr1;
	int		i;

	i = 0;
	ptr1 = (char *)s;
	while (ptr1[i] != '\0')
	{
		if (ptr1[i] == (char)c)
			return (&ptr1[i]);
		i++;
	}
	if ((char)c == '\0')
		return (&ptr1[i]);
	return (NULL);
}
