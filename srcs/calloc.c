/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 13:50:56 by amazurie          #+#    #+#             */
/*   Updated: 2019/02/18 10:52:19 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void	*calloc(size_t nmemb, size_t size)
{
	char		*p;
	uint64_t	ui;

	ui = nmemb * size;
	if (ui > ((size_t)-1))
		return (NULL);
	pthread_mutex_lock(&g_mutex);
	p = ft_malloc(nmemb * size);
	ft_bzero(p, ui);
	pthread_mutex_unlock(&g_mutex);
	return (p);
}
