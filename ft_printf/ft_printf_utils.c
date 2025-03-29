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

int	ft_putnbr(long nb)
{
	long		num;
	int			ans;
	int			aux;

	ans = 0;
	if (nb < 0)
	{
		if (write(1, "-", 1) == -1)
			return (-1);
		ans += 1;
		nb *= -1;
	}
	num = nb % 10 + '0';
	if (nb > 9)
	{
		aux = ft_putnbr(nb / 10);
		if (aux == -1)
			return (-1);
		ans += aux;
	}
	if (write(1, &num, 1) == -1)
		return (-1);
	ans += 1;
	return (ans);
}

int	ft_putnbr_hexa(unsigned long nbr, char option)
{
	int		ans;
	char	*base;
	int		aux;

	ans = 0;
	if (option == 'X')
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	if (nbr > 15)
	{
		aux = ft_putnbr_hexa((nbr / 16), option);
		if (aux == -1)
			return (-1);
		ans += aux;
	}
	if (write(1, &base[nbr % 16], 1) == -1)
		return (-1);
	ans += 1;
	return (ans);
}

int	ft_putptr(void *ptr)
{
	int	ans;
	int	aux;

	if (ptr == NULL)
	{
		if (ft_putstr("(nil)") == -1)
			return (-1);
		return (5);
	}
	else
	{
		ans = 0;
		aux = 0;
		if (ft_putstr("0x") == -1)
			return (-1);
		ans += 2;
		aux = ft_putnbr_hexa((unsigned long)ptr, 'x');
		if (aux == -1)
			return (-1);
		ans += aux;
		return (ans);
	}
}

int	ft_putstr(char *str1)
{
	int	ans;

	if (!str1)
	{
		write(1, "(null)", 6);
		return (6);
	}
	ans = 0;
	while (*str1 != '\0')
	{
		if (write(1, str1, 1) == -1)
			return (-1);
		ans++;
		str1++;
	}
	return (ans);
}
