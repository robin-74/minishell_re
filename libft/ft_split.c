/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryounes <ryounes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:51:24 by ryounes           #+#    #+#             */
/*   Updated: 2024/05/07 13:15:04 by ryounes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"

static size_t	ft_gen(const char *s, char c)
{
	size_t	ans;

	ans = 0;
	while (*s)
	{
		if (*s != c)
		{
			++ans;
			while (*s && *s != c)
				++s;
		}
		else
			++s;
	}
	return (ans);
}

char	**ft_split(const char *s, char c)
{
	char	**ans;
	size_t	i;
	size_t	len;

	if (!s)
		return (0);
	i = 0;
	ans = malloc(sizeof(char *) * (ft_gen(s, c) + 1));
	if (!ans)
		return (0);
	while (*s)
	{
		if (*s != c)
		{
			len = 0;
			while (*s && *s != c && ++len)
				++s;
			ans[i++] = ft_substr(s - len, 0, len);
		}
		else
			++s;
	}
	ans[i] = 0;
	return (ans);
}
