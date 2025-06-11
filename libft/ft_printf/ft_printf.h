/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llopes-n < llopes-n@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 14:28:27 by llopes-n          #+#    #+#             */
/*   Updated: 2022/05/14 02:10:33 by llopes-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include "../libft.h"

# define HEX "0123456789abcdef"
# define UPPER_HEX "0123456789ABCDEF"

# if __APPLE__
#  define IS_MACOS 1
# else
#  define IS_MACOS 0
# endif

int		ft_printf(const char *format, ...);
int		ft_type_c(int chr);
int		ft_type_s(char *str);
int		ft_type_p(unsigned long ptr);
int		ft_type_diu(int nbr, char flag, char type);
int		ft_type_x(unsigned int nbr, char flag, char type);
char	*flags(char *str, char flag, char type);

#endif
