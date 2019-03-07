/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 13:54:54 by amazurie          #+#    #+#             */
/*   Updated: 2019/02/18 10:53:42 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void	check_max_areas(t_area *aprec, t_area *area)
{
	t_block	*block;

	block = area->blocks;
	while (block && !block->inuse)
		block = block->next;
	if (block)
		return ;
	aprec->next = area->next;
	munmap(area, area->area_size + sizeof(t_area));
}

char	check_area(t_area *area, char *ptr, t_type type)
{
	t_area	*aprec;
	size_t	i;
	char	a;

	i = 0;
	while (area)
	{
		if (ptr > (char *)(area + 1)
				&& ptr < (char *)(area + 1) + area->area_size)
		{
			a = check_blocks(area, area->blocks, ptr);
			if (a && i > (type == TINY ? TINY_MAX_AREA : SMALL_MAX_AREA))
				check_max_areas(aprec, area);
			return (a);
		}
		aprec = area;
		area = area->next;
		i++;
	}
	return (0);
}

void	large_free(t_area *area, void *ptr)
{
	t_area	*prec;

	prec = NULL;
	while (area)
	{
		if ((void *)(area + 1) == ptr)
		{
			if (prec)
				prec->next = area->next;
			else
				main_areas()[2] = NULL;
			munmap(area, area->area_size + sizeof(t_area));
			return ;
		}
		prec = area;
		area = area->next;
	}
}

void	ft_free(void *ptr)
{
	char	a;

	if (!ptr)
		return ;
	a = 0;
	a = check_area(get_area(TINY), ptr, TINY);
	if (!a)
		a = check_area(get_area(SMALL), ptr, SMALL);
	if (!a)
		large_free(get_area(LARGE), ptr);
}

void	free(void *ptr)
{
	pthread_mutex_lock(&g_mutex);
	ft_free(ptr);
	pthread_mutex_unlock(&g_mutex);
}
