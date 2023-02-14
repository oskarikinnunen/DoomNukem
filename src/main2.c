/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raho <raho@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 12:41:37 by raho              #+#    #+#             */
/*   Updated: 2023/02/02 15:11:54 by raho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>  // header for linux and mac

//#include <process.h> // header for windows
/* exec functions are standard on linux and mac but on windows it's sort of an emulation of that. */

//on windows the func is _exec()


int	main(void)
{
	int	errnum;
	int	stat;
	int	id;
	
	id = fork();
	if (id == -1)
	{
		errnum = errno;
		printf("fork failed with errno set to: %d\n", errnum);
	}
	else if (id == 0)
	{
		int		child_errnum;
		char	*argv[] = {"sh", "-c", "./DoomNukem", NULL};
		char	*env[] = {NULL};
		execve("/bin/sh", argv, env);
		child_errnum = errno;
		if (child_errnum)
			printf("execve failed with child_errno set to: %d\n", child_errnum);
	}
	else
	{
		wait(&stat);
		if (WIFEXITED(stat))
			printf("child exit status: %d\n", WEXITSTATUS(stat));
		else
			printf("no child exit status\n");
	}
	return (0);
}
