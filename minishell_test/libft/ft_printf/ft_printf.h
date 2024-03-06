/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achappui <achappui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:54:27 by marvin            #+#    #+#             */
/*   Updated: 2023/11/23 10:39:18 by achappui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>

typedef struct s_options
{
	char	flags;
	char	spe;
	int		width;
	int		precision;
}	t_options;

typedef struct s_count
{
	int	saved_len;
	int	options_len;
	int	printed_chars;
}	t_count;

# define BUFFER_SIZE			10
# define MAX_OCTAL_LLLEN		22

# define BASE_TEN				"0123456789\000#"
# define LOW_HEX				"0123456789abcdef\0020x"
# define UPP_HEX				"0123456789ABCDEF\0020X"
# define SPECIFIERS				"cspdiuxX%"
# define FLAGS					"#-0 +"

# define F_HASHTAG				0b00000001
# define F_MINUS				0b00000010
# define F_ZERO					0b00000100
# define F_SPACE				0b00001000
# define F_PLUS					0b00010000
# define F_PRECISION			0b00100000

int	update_options(const char *str, t_options *o);
int	ft_putchars(int n, char c);
int	ft_print_str(const char *s, t_options *o);
int	ft_print_char(unsigned char c, t_options *o);
int	ft_print_sint(long long n, t_options *o);
int	ft_print_uint(unsigned long long n, const char *base, \
					char blen, t_options *o);

#endif