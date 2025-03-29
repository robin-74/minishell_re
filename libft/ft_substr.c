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

static size_t	llen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ans;
	size_t	ans_len;

	if (!s)
		return (0);
	ans_len = llen((char *)s);
	if (start > ans_len)
		return (ft_strdup(""));
	if (ans_len - start >= len)
		ans = (char *)malloc((len + 1) * sizeof(char));
	else
		ans = (char *)malloc((ans_len - start + 1) * sizeof(char));
	if (!ans)
		return (0);
	ft_strlcpy(ans, (s + start), (len + 1));
	return (ans);
}
