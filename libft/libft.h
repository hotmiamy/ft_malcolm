/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 17:12:28 by llopes-n          #+#    #+#             */
/*   Updated: 2025/06/11 14:21:00 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 256
# endif

# define INT_MIN -2147483648
# define INT_MAX 2147483647

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

void	ft_bzero(void *b, size_t lenght);
void	ft_striteri(char *str, void (*ft)(unsigned int, char*));
void	ft_putchar_fd(char chr, int fd);
void	ft_putendl_fd(char *str, int fd);
void	ft_putnbr_fd(int nbr, int fd);
void	ft_putstr_fd(char *str, int fd);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	*ft_memset(void *dest, int c, size_t len);
void	*ft_calloc(size_t number, size_t size);
void	*ft_memchr(const void *byte, int chr, size_t len);
void	*ft_memmove(void *dest, const void *src, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t len);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));
char	*ft_strdup(const char *str);
char	*ft_substr(char const *str, unsigned int start, size_t len);
char	*ft_strjoin(const char *str1, const char *str2);
char	*ft_strtrim(char const *str, char const *set);
char	**ft_split(char const *str, char chr);
char	*ft_itoa_base_converter(unsigned long nbr, char *base);
char	*ft_itoa(int nbr);
char	*ft_strmapi(char const *str, char (*ft)(unsigned int, char));
char	*ft_uitoa(unsigned int nbr);
char	*get_next_line(int fd);
char	*ft_strchr(const char *string, int c);
char	*ft_strrchr(const char *string, int c);
char	*ft_strnstr(const char *bigStr, const char *littleStr, size_t len);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isdigit(int c);
int		ft_isxdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_strncmp(const char *s1, const char *s2, size_t len);
int		ft_memcmp(const void *byte1, const void *byte2, size_t len);
int		ft_atoi(const char *numstr);
int		ft_lstsize(t_list *lst);
size_t	ft_strlen(const char *str);
size_t	ft_strlcpy(char *dest, const char *src, size_t destsize);
size_t	ft_strlcat(char *dest, const char *src, size_t n);
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

#endif
