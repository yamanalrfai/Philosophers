/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utile.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 08:19:29 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/07 08:19:56 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

void free_forks(t_all *all)
{
    int i;

    if (all->forks)
    {
        for (i = 0; i < all->num_philos; i++)
        {
            pthread_mutex_destroy(&all->forks[i]);
        }
        free(all->forks);
    }
}
