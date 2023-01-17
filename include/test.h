/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 01:12:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/17 01:30:01 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

typedef struct s_noc
{
	float	x;
	int		y;
}	t_noc;

typedef struct s_container
{
	int		z;
	t_noc	noc;
	float	w;
}	t_container;

#endif