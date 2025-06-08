/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utile.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 08:19:29 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/08 17:18:28 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

void free_forks(t_all *all)
{
    int i;

    if (all->forks)
    {
        i = 0;
        while (i < all->num_philos)
        {
            pthread_mutex_destroy(&all->forks[i]);
            i++;
        }
        free(all->forks);
    }
}

void free_philos(t_all *all)
{
    int i;
    
    if (all->philos)
    {
        i = 0;
        while (i < all->num_philos)
        {
            pthread_mutex_destroy(&all->philos[i].meal_lock);
            i++;
        }
    }
    free(all->philos);
}

int free_all(t_all *all)
{
    free_forks(all);
    free_philos(all);
    return (0);
}

long long get_time(void)
{
    struct timeval tv;
    long long time;

    gettimeofday(&tv, NULL);
    time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (time);
}
