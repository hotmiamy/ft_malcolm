/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llopes-n < llopes-n@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/05 20:23:55 by llopes-n          #+#    #+#             */
/*   Updated: 2022/05/14 02:08:14 by llopes-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strjoin(const char *str1, const char *str2)
{
	char	*str_conca;
	int		str_concalen;
	int		index;
	int		index2;

	if (!str1 || !str2)
		return (NULL);
	str_concalen = ft_strlen(str1) + ft_strlen(str2) + 1;
	str_conca = (char *)malloc(str_concalen * sizeof(char));
	if (!str_conca)
		return (0);
	index = 0;
	while (str1[index])
	{
		str_conca[index] = str1[index];
		index++;
	}
	index2 = 0;
	while (str2[index2])
	{
		str_conca[index + index2] = str2[index2];
		index2++;
	}
	str_conca[index + index2] = '\0';
	return (str_conca);
}
