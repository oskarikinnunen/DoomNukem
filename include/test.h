/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 01:12:23 by okinnune          #+#    #+#             */
/*   Updated: 2023/01/17 11:22:10 by okinnune         ###   ########.fr       */
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
	int		z2;
	int		z3;
	float	w;
}	t_container;

#endif