#include <stdio.h>
#include <unistd.h>

int	ft_show_number(char c)
{
	write(1, &c, 1);
	return (c);
}

int	ft_strlen(char *ag)
{
	int	i;

	while(ag[i])
	{
		i++;
	}
	return (i);
}

int	main(int ac, char **ag)
{
	(void)ac;
	write(1,ag[0],ft_strlen(ag[0]));
	return (0);
}
