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
#include <stdlib.h>
#include "libft.h"

static int	mylen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static int	inset(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	char	*ans;

	if (!s1)
		return (NULL);
	i = 0;
	while (s1[i] && inset(s1[i], set))
		i++;
	j = mylen(s1);
	while (j > i && inset(s1[j - 1], set))
		j--;
	ans = (char *)malloc(sizeof(char) * (j - i + 1));
	if (!ans)
		return (NULL);
	ft_memcpy(ans, s1 + i, j - i);
	ans[j - i] = '\0';
	return (ans);
}
