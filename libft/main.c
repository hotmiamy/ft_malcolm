/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 01:20:42 by llopes-n          #+#    #+#             */
/*   Updated: 2025/06/11 14:10:10 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf/ft_printf.h"
#include "libft.h"

int test_isxdigit(void){
	char	hex[21] = "ABCDEFabcdef123445789";;
	char	not_hex[40] = "qwrtyuiopsghjklzxvnmQWRTYUIOPSGHJKLZXVNM";

	for (int i = 0; i < ft_strlen(hex); i++){
		if (ft_isxdigit(hex[i]) != 0)
			continue;
		else{
			ft_printf("Error: ft_isxdigit cant propoly detect hex digits\n");
			return (-1);
		}
	}
	for (int i = 0; i < ft_strlen(not_hex); i++){
		if (ft_isxdigit(not_hex[i]) == 0)
			continue;
		else{
			ft_printf("Error: ft_isxdigit is detecting non hex digits\n");
			return (-1);
		} 
	}
	ft_printf("Success: ft_isxdigit pass all tests\n");
	return (0);
}

int	main(void)
{
	int test_return;
	ft_printf("Testing: ft_isxdigit\n");
	test_return = test_isxdigit();
	if (test_return != 0)
		return (test_return);
	return(0);
}
