/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   area.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 13:51:29 by amazurie          #+#    #+#             */
/*   Updated: 2019/02/15 10:57:09 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

t_area	**main_areas(void)
{
	static t_area	*areas[3];

	return (areas);
}

t_area	*get_area(t_type type)
{
	if (type == TINY)
		return (main_areas()[0]);
	else if (type == SMALL)
		return (main_areas()[1]);
	return (main_areas()[2]);
}

void	add_area(t_area *narea, t_type type)
{
	t_area	*area;

	if (type == TINY)
		area = main_areas()[0];
	else if (type == SMALL)
		area = main_areas()[1];
	else
		area = main_areas()[2];
	if (!area)
	{
		if (type == TINY)
			main_areas()[0] = narea;
		else if (type == SMALL)
			main_areas()[1] = narea;
		else
			main_areas()[2] = narea;
		return ;
	}
	while (area->next)
		area = area->next;
	area->next = narea;
}

void	init_area(t_area *area, size_t area_size)
{
	area->bytes_used = 0;
	area->area_size = area_size - sizeof(t_area);
	area->blocks = NULL;
	area->next = NULL;
}

t_area	*new_area(t_type type, size_t size)
{
	t_area	*area;
	size_t	area_size;

	if (type == TINY)
		area_size = TINY_REGION_ROW * getpagesize();
	else if (type == SMALL)
		area_size = SMALL_REGION_ROW * getpagesize();
	else
		area_size = size + sizeof(t_area);
	if ((area = (t_area *)mmap(0, area_size, PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED || !area)
		return (NULL);
	init_area(area, area_size);
	add_area(area, type);
	return (area);
}
