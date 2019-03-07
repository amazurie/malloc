/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 13:53:02 by amazurie          #+#    #+#             */
/*   Updated: 2019/02/18 14:17:50 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void	init_block(t_block *block, size_t size)
{
	block->bytes_used = size;
	block->next = NULL;
	block->inuse = 0;
}

t_block	*shrink_block(t_area *area, t_block *block, size_t size, t_type type)
{
	t_block *nblock;

	if (block->bytes_used - size < (type == TINY
				? TINY_RESOLUTION : SMALL_RESOLUTION) + sizeof(t_block))
		return (block);
	nblock = (t_block *)((char *)(block + 1) + size);
	init_block(nblock, (block->next ? (char *)(block->next) :
				((char *)(area + 1) + area->area_size)) - (char *)(nblock + 1));
	block->bytes_used = size;
	nblock->next = block->next;
	block->next = nblock;
	area->bytes_used += sizeof(t_block);
	return (block);
}

t_block	*create_block(t_area *area, size_t size, t_type type)
{
	t_block	*block;
	size_t	padded;

	padded = get_offset(size,
			(type == TINY ? TINY_RESOLUTION : SMALL_RESOLUTION));
	block = area->blocks;
	if (!block)
	{
		block = (t_block *)(area + 1);
		init_block(block, ((size_t)((char *)(area + 1)
					+ area->area_size)) - (size_t)(area + 1) - sizeof(t_block));
		area->blocks = block;
		area->bytes_used += sizeof(t_block);
	}
	while (block && (block->inuse || block->bytes_used < padded))
		block = block->next;
	if (block)
		return (shrink_block(area, block, padded, type));
	return (NULL);
}
