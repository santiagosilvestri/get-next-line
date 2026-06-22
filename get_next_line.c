/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 12:04:21 by sasilves          #+#    #+#             */
/*   Updated: 2026/06/22 13:13:51 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_read_file(int fd, char *rest)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(rest, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			free(rest);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		rest = ft_join_free(rest, buffer);
		if (!rest)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (rest);
}

static char	*ft_get_line(char *rest)
{
	char	*line;
	char	*new_line;
	size_t	len;
	size_t	i;

	if (!rest || !rest[0])
		return (NULL);
	new_line = ft_strchr(rest, '\n');
	if (new_line)
		len = (new_line - rest) + 1;
	else
		len = ft_strlen(rest);
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line[i] = rest[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*ft_clean_rest(char *rest)
{
	char	*new_rest;
	char	*start;
	size_t	i;

	start = ft_strchr(rest, '\n');
	if (!start || !start[1])
		return (free(rest), NULL);
	start++;
	new_rest = malloc(sizeof(char) * (ft_strlen(start) + 1));
	if (!new_rest)
		return (free(rest), NULL);
	i = 0;
	while (start[i])
	{
		new_rest[i] = start[i];
		i++;
	}
	new_rest[i] = '\0';
	free(rest);
	return (new_rest);
}

char	*get_next_line(int fd)
{
	static char	*rest;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	rest = ft_read_file(fd, rest);
	if (!rest)
		return (NULL);
	line = ft_get_line(rest);
	rest = ft_clean_rest(rest);
	return (line);
}
