#include "doomnukem.h"
#include <signal.h>

static int	combine_strings_count_length(char **str)
{
	int	x;
	int	y;
	int	len;

	y = 0;
	len = 0;
	while (str[y] != NULL)
	{
		x = 0;
		while (str[y][x] != '\0')
			x++;
		len += x;
		if (str[y + 1] != NULL)
			len += 1;
		y++;
	}
	return (len);
}

char	*combine_strings(char **str)
{
	int		x;
	int		y;
	int		len;
	char	*result;

	len = combine_strings_count_length(str);
	result = ft_strnew(len);
	if (result == NULL)
		return (result);
	y = 0;
	x = 0;
	while (str[y] != NULL)
	{
		ft_strcpy(&result[x], str[y]);
		x += ft_strlen(str[y]);
		result[x] = ' ';
		x++;
		y++;
	}
	result[len] = '\0';
	return (result);
}

static char	*signal_name(int wait_status)
{
	static char	signal[16];

	if (WTERMSIG(wait_status) == SIGSEGV)
		ft_strcpy(signal, "SIGSEGV");
	if (WTERMSIG(wait_status) == SIGBUS)
		ft_strcpy(signal, "SIGSEGV");
	if (WTERMSIG(wait_status) == SIGILL)
		ft_strcpy(signal, "SIGILL");
	if (WTERMSIG(wait_status) == SIGABRT)
		ft_strcpy(signal, "SIGABRT");
	if (WTERMSIG(wait_status) == SIGIOT)
		ft_strcpy(signal, "SIGIOT");
	if (WTERMSIG(wait_status) == SIGTRAP)
		ft_strcpy(signal, "SIGTRAP");
	/* if (WTERMSIG(wait_status) == SIGEMT) // doesnt work on linux, says it's undeclared
		ft_strcpy(signal, "SIGEMT"); */
	if (WTERMSIG(wait_status) == SIGSYS)
		ft_strcpy(signal, "SIGSYS");
	return (signal);
}

int	handle_exit(int	wait_status)
{
	if (WIFSIGNALED(wait_status))
	{
		error_window(combine_strings((char *[5]){
				"child process raised a signal that caused it to exit:",
				s_itoa(WTERMSIG(wait_status)), "-", signal_name(wait_status)}));
		return (1);
	}
	else if (WIFEXITED(wait_status))
	{
		if (WEXITSTATUS(wait_status) != 0)
		{
			// could differentiate behaviors for different exit codes here
			error_window(combine_strings((char *[3]){
					"child process exited with status:",
					s_itoa(WEXITSTATUS(wait_status))}));
			return (1);
		}
		return (0);
	}
	else
	{
		error_window("no exit signal or status from the child process");
		return (1);
	}
	return (0);
}
