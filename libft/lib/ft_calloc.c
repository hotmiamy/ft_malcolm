/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llopes-n < llopes-n@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 14:55:16 by llopes-n          #+#    #+#             */
/*   Updated: 2022/07/13 02:08:54 by llopes-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_calloc(size_t number, size_t size)
{
	void	*ptr;

	if (number >= __SIZE_MAX__ / size)
		return (NULL);
	ptr = malloc(number * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, number * size);
	return (ptr);
}
