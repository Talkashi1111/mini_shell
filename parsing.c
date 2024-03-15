

#include "libft.h"



# define OPERATOR	0
# define WORD		1

typedef struct s_token_list
{
	char				type;
	char				*str;
	struct s_token_list	*next;
}	t_token_list;

typedef struct s_elem
{
	char			*str;
	struct s_elem	*next;
}	t_elem;

typedef struct s_cmdset
{
	char	*cmd;
	char	**op_args;
	char	**redirections;
	int		fd_in;
	int		fd_out;
}	t_cmdset;

# define WHITESPACE	1
# define OPERATOR	2
# define QUOTE		'\''
# define DQUOTE		'"'


/*
Cette fonction faire un premier decoupage en fonction des delimiteurs retournant
une liste de morceau (des strings).
Ici je donne la priorite a >> sur > pour des question de simplicite. (si on a >>> par exemple ou >>>>)

Liste des delimiters: white space | > >> < << && || ( )
*/

t_elem	*split_by_delimiters(char *str)
{
	t_elem			*start;
	t_elem			*index;
	unsigned int	start;
	unsigned int	i;
	char			is_delim;

	i = 0;
	start = 0;
	while (str[i] != '\0')
	{
		is_delim = is_delimiter(str[i]);
		if(is_delim == OPERATOR)
		{

		}
		else if (is_delim)

		else if (str[i] == QUOTE || str[i] == DQUOTE)
			i = end_of_quote(str[i], i, str);
		i++;
	}
}

/*
Cette fonction va parcourir la liste d'element en enlevant les quotes et en remplacant les $ par les
variables correspondantes.
Je fais les 2 a la fois parce que sinon il faudrait stocker quelque part pour chaque mot si il est
single quoted ou pas dans quel cas il ne faudrait pas expand le $
*/

// char	**quotes_and_dollars()
// {

// }

unsigned int	count_elems(char *str)
{

}

char	**split_elems(char *str, unsigned int elem_nb)
{

}