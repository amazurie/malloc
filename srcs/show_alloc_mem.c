/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 15:43:37 by amazurie          #+#    #+#             */
/*   Updated: 2019/02/18 10:55:30 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void	puthexa(size_t d)
{
	size_t	i;

	if (d)
	{
		i = d / 16;
		puthexa(i);
		if (d % 16 < 10)
			ft_putchar('0' + d % 16);
		else
			ft_putchar('a' + (d % 16) - 10);
	}
	else
		ft_putstr("0x");
	return ;
}

void	print_area(t_area *area, t_type type)
{
	t_block	*block;

	ft_putstr(type == TINY ? "TINY : " : "SMALL : ");
	puthexa((size_t)area);
	ft_putchar('\n');
	block = area->blocks;
	while (block)
	{
		puthexa((size_t)(block + 1));
		ft_putstr(" - ");
		puthexa((size_t)((char *)(block + 1) + block->bytes_used));
		ft_putstr(" : ");
		ft_putnbr((((char *)(block + 1) + block->bytes_used)
					- (char *)(block + 1) + block->bytes_used) / 8);
		ft_putstr(" octets");
		ft_putstr("\n");
		block = block->next;
	}
}

void	show_tiny_small_area(t_area *area, t_type type)
{
	while (area)
	{
		print_area(area, type);
		area = area->next;
	}
}

void	print_large_area(t_area *area)
{
	ft_putstr("LARGE : ");
	puthexa((size_t)area);
	ft_putstr("\n");
	puthexa((size_t)(area + 1));
	ft_putstr(" - ");
	puthexa((size_t)((char *)(area + 1) + area->bytes_used));
	ft_putstr(" : ");
	ft_putnbr((((char *)(area + 1) + area->area_size)
				- (char *)(area + 1)) / 8);
	ft_putstr(" octets\n");
}

void	show_alloc_mem(void)
{
	t_area	*area;

	pthread_mutex_lock(&g_mutex);
	show_tiny_small_area(get_area(TINY), SMALL);
	show_tiny_small_area(get_area(SMALL), SMALL);
	area = get_area(LARGE);
	while (area)
	{
		print_large_area(area);
		area = area->next;
	}
	pthread_mutex_unlock(&g_mutex);
}
