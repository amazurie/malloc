/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_block.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 15:49:55 by amazurie          #+#    #+#             */
/*   Updated: 2018/11/19 15:56:11 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void	free_block(t_area *area, t_block *bprec, t_block *block)
{
	if (!block->inuse)
		return ;
	area->bytes_used -= block->bytes_used;
	if (bprec && !bprec->inuse)
	{
		bprec->bytes_used += block->bytes_used + sizeof(t_block);
		bprec->next = block->next;
		block = bprec;
		area->bytes_used -= sizeof(t_block);
	}
	else if (block->next && !block->next->inuse)
	{
		block->bytes_used += block->next->bytes_used + sizeof(t_block);
		block->next = block->next->next;
		area->bytes_used -= sizeof(t_block);
	}
	block->inuse = 0;
}

char	check_blocks(t_area *area, t_block *block, void *ptr)
{
	t_block	*bprec;

	bprec = NULL;
	while (block)
	{
		if ((void *)(block + 1) == ptr)
		{
			free_block(area, bprec, block);
			return (1);
		}
		bprec = block;
		block = block->next;
	}
	return (0);
}
