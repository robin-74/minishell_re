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

#include "ft_printf.h"
#include <stdarg.h>

static int	converter(const char option, va_list va)
{
	if (option == 'c')
		return (ft_putchar(va_arg(va, int)));
	else if (option == 'u')
		return (ft_putnbr(va_arg(va, unsigned int)));
	else if ((option == 'i') || (option == 'd'))
		return (ft_putnbr(va_arg(va, int)));
	else if (option == 's')
		return (ft_putstr(va_arg(va, char *)));
	else if (option == 'x' || option == 'X')
		return (ft_putnbr_hexa(va_arg(va, unsigned int), option));
	else if (option == 'p')
		return (ft_putptr(va_arg(va, void *)));
	else if (option == '%')
		return (ft_putchar('%'));
	return (-1);
}

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_printf(char const *str, ...)
{
	va_list	varg;
	int		len;
	int		temp;

	va_start(varg, str);
	len = 0;
	while (*str)
	{
		if (*str == '%')
		{
			temp = converter(*(++str), varg);
			if (temp == -1)
				return (-1);
			len += temp;
		}
		else
		{
			if (write(1, str, 1) == -1)
				return (-1);
			len++;
		}
		str++;
	}
	va_end(varg);
	return (len);
}
