/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdios-el <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:02:57 by mdios-el          #+#    #+#             */
/*   Updated: 2024/11/25 19:03:00 by mdios-el         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*free_str(char **str)
{
	if (str != NULL && *str != NULL)
	{
		free(*str);
		*str = NULL;
	}
	return (NULL);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)&s[i]);
	return (NULL);
}

char	*manage_rest(char **rest)
{
	char	*line;
	char	*newline_pos;
	char	*temp;

	if ((*rest)[0] == '\0')
		return (free_str(rest));
	line = NULL;
	newline_pos = ft_strchr(*rest, '\n');
	if (!newline_pos)
	{
		line = *rest;
		*rest = NULL;
		return (line);
	}
	line = ft_substr(*rest, 0, newline_pos - *rest + 1);
	if (!line)
		return (free_str(rest));
	temp = ft_strdup(newline_pos + 1);
	if (!temp)
		return (free_str(rest), free_str(&line));
	free_str(rest);
	*rest = temp;
	return (line);
}

ssize_t	process_buffer(int fd, char **rest)
{
	char	*buffer;
	ssize_t	bytes_read;
	char	*temp;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free_str(rest), -1);
	buffer[BUFFER_SIZE] = '\0';
	bytes_read = 1;
	while (bytes_read > 0 && ft_strchr(*rest, '\n') == NULL)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			temp = ft_strjoin(*rest, buffer);
			free_str(rest);
			*rest = temp;
			if (!*rest)
				return (free_str(&buffer), -1);
		}
	}
	free_str(&buffer);
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*rest[1024];
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (rest[fd] == NULL)
	{
		rest[fd] = ft_strdup("");
		if (!rest[fd])
			return (NULL);
	}
	bytes_read = process_buffer(fd, &rest[fd]);
	if (bytes_read == -1)
		return (free_str(&rest[fd]));
	else if (bytes_read == 0 && !rest[fd])
		return (NULL);
	line = manage_rest(&rest[fd]);
	return (line);
}

/*
int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	if (argc != 2)
	{
		printf("Usage: %s <file>\n", argv[0]);
		return (1);
	}
	
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		return (1);
	}
	
	printf("Reading file: %s\n", argv[1]);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}

	if (close(fd) < 0)
	{
		perror("Error closing file");
		return (1);
	}

	return (0);
}*/
