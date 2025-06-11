/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llopes-n < llopes-n@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:30:30 by llopes-n          #+#    #+#             */
/*   Updated: 2022/04/23 20:07:11 by llopes-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	type_write(va_list arg, char type, char flag);
static int	type_flag_find(va_list arg, const char *format);

int	ft_printf(const char *format, ...)
{
	va_list	arg;
	int		len;

	va_start(arg, format);
	len = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			len += type_flag_find(arg, format);
			while (ft_strchr("cspdiuxX%", *format) == 0)
				format++;
		}
		else
			len += write(1, format, 1);
		format++;
	}
	va_end(arg);
	return (len);
}

static int	type_flag_find(va_list arg, const char *format)
{
	char	type;
	char	flag;
	int		len;

	flag = 0;
	type = 0;
	len = 0;
	while (*format)
	{
		if (ft_strchr("# +", *format))
			flag = *format;
		else if (ft_strchr("cspdiuxX%", *format))
		{
			type = *format;
			break ;
		}
		format++;
	}
	if (type != 0)
		len += type_write(arg, type, flag);
	return (len);
}

static int	type_write(va_list arg, char type, char flag)
{
	if (type == 'c')
		return (ft_type_c(va_arg(arg, int)));
	else if (type == 's')
		return (ft_type_s(va_arg(arg, char *)));
	else if (type == 'p')
		return (ft_type_p(va_arg(arg, unsigned long)));
	else if (ft_strchr("diu", type))
		return (ft_type_diu(va_arg(arg, int), flag, type));
	else if (ft_strchr("xX", type))
		return (ft_type_x(va_arg(arg, unsigned int), flag, type));
	else if (type == '%')
		return (write(1, "%", 1));
	return (0);
}
