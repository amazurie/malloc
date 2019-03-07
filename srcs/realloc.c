/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 15:44:51 by amazurie          #+#    #+#             */
/*   Updated: 2019/02/18 10:58:57 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void	*merge_block(t_area *area, t_block *block, size_t size, t_type type)
{
	size_t	rem;
	t_block	*nblock;

	if (block->bytes_used > get_offset(size, (type == TINY ?
					TINY_RESOLUTION : SMALL_RESOLUTION)))
	{
		block->bytes_used = get_offset(size, (type == TINY ?
					TINY_RESOLUTION : SMALL_RESOLUTION));
		return (block + 1);
	}
	rem = get_offset(size, (type == TINY ?
				TINY_RESOLUTION : SMALL_RESOLUTION))
		- block->bytes_used - sizeof(t_block);
	if (block->next && block->next->inuse == 0
			&& block->next->bytes_used >= rem)
	{
		nblock = shrink_block(area, block->next, rem, type);
		block->bytes_used += nblock->bytes_used + sizeof(t_block);
		area->bytes_used += nblock->bytes_used;
		block->next = nblock->next;
		return (block + 1);
	}
	return (NULL);
}

void	*check_area_blocks(t_area *area, void *ptr, size_t size, t_type type)
{
	t_block	*block;
	void	*nblock;

	block = area->blocks;
	while (block)
	{
		if ((void *)(block + 1) == ptr)
		{
			nblock = merge_block(area, block, size, type);
			if (nblock)
				return (nblock);
			nblock = ft_malloc(size);
			if (!nblock)
				return (NULL);
			ft_memcpy(nblock, (char *)(block + 1),
					size > block->bytes_used ? block->bytes_used : size);
			ft_free(ptr);
			return (nblock);
		}
		block = block->next;
	}
	return (NULL);
}

void	*tiny_small_realloc(t_area *area, char *ptr, size_t size, t_type type)
{
	while (area)
	{
		if (ptr > (char *)(area + 1)
				&& ptr < (char *)(area + 1) + area->area_size)
			return (check_area_blocks(area, ptr, size, type));
		area = area->next;
	}
	return (NULL);
}

void	*realloc_large(void *ptr, size_t size)
{
	t_area	*area;
	void	*nblock;

	area = get_area(LARGE);
	while (area && ptr != (void *)(area + 1))
		area = area->next;
	if (area == NULL)
		return (NULL);
	nblock = ft_malloc(size);
	ft_memcpy(nblock, area + 1,
			size > area->area_size ? area->area_size : size);
	ft_free(area + 1);
	return (nblock);
}

void	*realloc(void *ptr, size_t size)
{
	void	*block;

	if (!ptr && size > 0)
		return (ft_malloc(size));
	if (!ptr || size == 0)
	{
		if (ptr)
			ft_free(ptr);
		return (NULL);
	}
	pthread_mutex_lock(&g_mutex);
	block = tiny_small_realloc(get_area(TINY), ptr, size, TINY);
	if (!block)
		block = tiny_small_realloc(get_area(SMALL), ptr, size, TINY);
	if (!block)
		block = realloc_large(ptr, size);
	pthread_mutex_unlock(&g_mutex);
	return (block);
}
