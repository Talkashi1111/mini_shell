/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:50:00 by tkashi            #+#    #+#             */
/*   Updated: 2024/01/29 11:02:51 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stddef.h>
# include <stdarg.h>

typedef struct s_list_libft
{
	void				*content;
	struct s_list_libft	*next;
}					t_libft;

int		ft_isalnum(int c);
int		ft_isalpha(int argument);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);
int		ft_tolower(int c);
int		ft_toupper(int c);
void	ft_bzero(void *s, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	*ft_memset(void *s, int c, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
int		ft_atoi(const char *str);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strdup(const char *s1);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
char	*ft_itoa(int n);
void	ft_putnbr_fd(int n, int fd);
t_libft	*ft_lstnew(void *content);
void	ft_lstadd_front(t_libft **lst, t_libft *node);
int		ft_lstsize(t_libft *lst);
t_libft	*ft_lstlast(t_libft *lst);
void	ft_lstadd_back(t_libft **lst, t_libft *new);
void	ft_lstdelone(t_libft *lst, void (*del)(void *));
void	ft_lstclear(t_libft **lst, void (*del)(void *));
t_libft	*ft_lstmap(t_libft *lst, void *(*f)(void *), void (*del)(void *));
void	ft_lstiter(t_libft *lst, void (*f)(void *));
int		ft_printf(const char *fmt, ...);
int		ft_fprintf(int fd, const char *fmt, ...);
char	*get_next_line(int fd);
#endif
