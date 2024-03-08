/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:43:18 by tkashi            #+#    #+#             */
/*   Updated: 2023/11/16 12:08:01 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"
#include "libft.h"

void	polish(t_list **list)
{
	int		i;
	int		j;
	char	*str;

	free_list(list, FALSE);
	i = 0;
	str = (*list)->tail->buf;
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	if (str[i] == '\0')
	{
		free_list(list, TRUE);
		return ;
	}
	i++;
	j = 0;
	while (str[i] != '\0')
		str[j++] = str[i++];
	str[j] = '\0';
	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	if (str[i] == '\n')
		i++;
	(*list)->len = i;
}

char	*copy_new_line(t_list *list)
{
	int			i;
	int			j;
	t_lst_node	*temp;
	char		*new_line;

	if (!list || list->len == 0)
		return (NULL);
	new_line = (char *)malloc((list->len + 1) * sizeof(char));
	if (!new_line)
		return (NULL);
	temp = list->head;
	j = 0;
	while (j < list->len)
	{
		i = 0;
		while (temp->buf[i] != '\0' && temp->buf[i] != '\n')
			new_line[j++] = temp->buf[i++];
		if (temp->buf[i] == '\n')
			new_line[j++] = '\n';
		temp = temp->next;
	}
	new_line[j] = '\0';
	return (new_line);
}

int	append_to_list(t_list *list, ssize_t bytes_read, char *buf)
{
	ssize_t			new_line_idx;
	t_lst_node		*new_node;

	new_node = create_node(&buf);
	if (!new_node)
		return (0);
	if (!list->head)
		list->head = new_node;
	else
		list->tail->next = new_node;
	list->tail = new_node;
	new_line_idx = find_new_line(list);
	if (new_line_idx == -1)
		list->len += bytes_read;
	else
		list->len += new_line_idx + 1;
	return (1);
}

int	prepare_next_line(int fd, t_list *list)
{
	ssize_t	bytes_read;
	char	*buf;

	while (find_new_line(list) == -1)
	{
		buf = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
		if (!buf)
			return (0);
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			free(buf);
			if (bytes_read < 0)
				return (0);
			return (1);
		}
		buf[bytes_read] = '\0';
		if (!append_to_list(list, bytes_read, buf))
			return (0);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	char			*next_line;
	static t_list	*list;

	if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, &next_line, 0) < 0)
	{
		free_list(&list, TRUE);
		return (NULL);
	}
	if (!init_list(&list))
		return (NULL);
	if (!prepare_next_line(fd, list))
	{
		free_list(&list, TRUE);
		return (NULL);
	}
	next_line = copy_new_line(list);
	if (!next_line)
	{
		free_list(&list, TRUE);
		return (NULL);
	}
	polish(&list);
	return (next_line);
}
