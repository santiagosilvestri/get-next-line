/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 12:04:21 by sasilves          #+#    #+#             */
/*   Updated: 2026/06/26 08:44:42 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_read_file(int fd, char *storage)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free(storage), NULL);
	bytes_read = 1;
	while (!ft_strchr(storage, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(buffer), free(storage), NULL);
		buffer[bytes_read] = '\0';
		storage = ft_join_free(storage, buffer);
		if (!storage)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (storage);
}

static char	*ft_get_line(char *storage)
{
	char	*line;
	char	*newline;
	size_t	len;
	size_t	i;

	if (!storage || !storage[0])
		return (NULL);
	newline = ft_strchr(storage, '\n');
	if (newline)
		len = (newline - storage) + 1;
	else
		len = ft_strlen(storage);
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line[i] = storage[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*ft_update_storage(char *storage)
{
	char	*next_storage;
	char	*start;
	size_t	i;

	start = ft_strchr(storage, '\n');
	if (!start)
		return (free(storage), NULL);
	start++;
	next_storage = malloc(sizeof(char) * (ft_strlen(start) + 1));
	if (!next_storage)
		return (free(storage), NULL);
	i = 0;
	while (start[i])
	{
		next_storage[i] = start[i];
		i++;
	}
	next_storage[i] = '\0';
	return (free(storage), next_storage);
}

char	*get_next_line(int fd)
{
	static char	*storage;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free(storage), storage = NULL, NULL);
	storage = ft_read_file(fd, storage);
	if (!storage)
		return (NULL);
	line = ft_get_line(storage);
	if (!line)
		return (free(storage), storage = NULL, NULL);
	storage = ft_update_storage(storage);
	return (line);
}
