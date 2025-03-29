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

#include "libft.h"
#include <stdlib.h>
#include "libft.h"

int	ft_len(long num)
{
	int	ans;

	ans = 0;
	if (num == 0)
		return (1);
	else if (num < 0)
	{
		ans++;
		num *= -1;
	}
	while (num > 0)
	{
		num /= 10;
		ans++;
	}
	return (ans);
}

char	*ft_itoa(int n)
{
	int		lenstr;
	long	num;
	char	*str;

	num = n;
	lenstr = ft_len(num);
	str = (char *)malloc(sizeof(char) * (lenstr + 1));
	if (!str)
		return (NULL);
	*(str + lenstr--) = '\0';
	if (num == 0)
	{
		*(str + 0) = '0';
	}
	else if (num < 0)
	{
		num *= -1;
		*(str + 0) = '-';
	}
	while (num > 0)
	{
		*(str + lenstr--) = (48 + (num % 10));
		num /= 10;
	}
	return (str);
}
