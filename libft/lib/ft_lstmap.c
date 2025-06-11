/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llopes-n < llopes-n@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 21:43:04 by llopes-n          #+#    #+#             */
/*   Updated: 2022/05/17 19:39:10 by llopes-n         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*lst_aux;

	if (!lst || !f)
		return (NULL);
	new = (t_list *)malloc(sizeof(t_list));
	if (new == NULL)
	{
		lst_aux = new->next;
		while (!new)
		{
			if (del)
			{
				del(new->content);
				free(new);
				new = lst_aux;
			}
		}
		return (NULL);
	}
	new->content = f(lst->content);
	new->next = ft_lstmap(lst->next, f, del);
	return (new);
}
