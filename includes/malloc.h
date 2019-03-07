/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 12:26:07 by amazurie          #+#    #+#             */
/*   Updated: 2019/02/20 10:32:39 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <sys/mman.h>
# include <pthread.h>
# include "../libft/includes/libft.h"

# define TINY_REGION_ROW	512
# define TINY_MAX_AREA		1
# define TINY_RESOLUTION	16
# define TINY_MAX_SIZE		993

# define SMALL_REGION_ROW	4096
# define SMALL_MAX_AREA		1
# define SMALL_RESOLUTION	512
# define SMALL_MAX_SIZE		15345

extern pthread_mutex_t	g_mutex;

typedef enum		e_type
{
	TINY,
	SMALL,
	LARGE
}					t_type;

typedef struct		s_block
{
	struct s_block	*next;
	unsigned int	bytes_used;
	char			inuse;
}					t_block;

typedef struct		s_area
{
	unsigned		bytes_used;
	t_block			*blocks;
	unsigned		area_size;
	struct s_area	*next;
}					t_area;

void				*malloc(size_t size);
void				free(void *ptr);
void				*realloc(void *ptr, size_t size);
void				show_alloc_mem();

void				*ft_malloc(size_t size);
void				ft_free(void *ptr);

t_area				**main_areas();
t_area				*get_area(t_type type);
t_area				*new_area(t_type type, size_t size);

t_block				*create_block(t_area *area, size_t size, t_type type);
char				check_blocks(t_area *area, t_block *block, void *ptr);
t_block				*shrink_block(t_area *area, t_block *block,
					size_t size, t_type type);

size_t				get_offset(size_t alloc_size, int offset);

#endif
