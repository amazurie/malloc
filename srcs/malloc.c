/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 13:56:31 by amazurie          #+#    #+#             */
/*   Updated: 2019/02/13 15:31:51 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void	*tiny_small_malloc(t_type type, size_t size)
{
	t_area	*area;
	t_block	*block;

	area = get_area(type);
	while (area)
	{
		if (area->area_size - area->bytes_used > size)
		{
			block = create_block(area, size, type);
			if (block)
			{
				block->inuse = 1;
				area->bytes_used += block->bytes_used;
				return (block + 1);
			}
		}
		area = area->next;
	}
	area = new_area(type, size);
	block = create_block(area, size, type);
	block->inuse = 1;
	area->bytes_used += block->bytes_used;
	return (block + 1);
}

void	*ft_malloc(size_t size)
{
	void	*ptr;
	t_area	*area;

	if (size <= TINY_MAX_SIZE)
		ptr = tiny_small_malloc(TINY, size);
	else if (size <= SMALL_MAX_SIZE)
		ptr = tiny_small_malloc(SMALL, size);
	else
	{
		if ((size_t) - sizeof(t_area) - 1 < size)
			return (NULL);
		area = new_area(LARGE, size);
		ptr = (area) ? area + 1 : NULL;
	}
	return (ptr);
}

void	*malloc(size_t size)
{
	void	*ptr;

	pthread_mutex_lock(&g_mutex);
	ptr = ft_malloc(size);
	pthread_mutex_unlock(&g_mutex);
	return (ptr);
}
