/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llopes-n < llopes-n@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/06 10:05:32 by llopes-n          #+#    #+#             */
/*   Updated: 2022/06/04 02:48:57 by llopes-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../libft.h"

char	*ft_strtrim(char const *str, char const *set)
{
	char	*strtrim;
	int		strtrimlen;

	if (!str)
		return (0);
	while (ft_strchr(set, *str) != 0 && *str != '\0')
		str++;
	strtrimlen = ft_strlen(str);
	while (ft_strchr(set, str[strtrimlen - 1]) != 0 && strtrimlen > 0)
		strtrimlen--;
	strtrim = ft_substr(str, 0, strtrimlen);
	return (strtrim);
}
