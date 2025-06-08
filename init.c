/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:59:08 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/07 21:21:01 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

int init_forks(t_all *all)
{
    int i;

    all->forks = malloc(sizeof(pthread_mutex_t) * all->num_philos);
    if (!all->forks)
        return (1);
    i = 0;
    while(i < all->num_philos)
    {
        if (pthread_mutex_init(&all->forks[i], NULL) != 0)
            return (1);
        i++;
    }
    return (0);
}

int init_philos(t_all *all)
{
    int i;

    all->philos = malloc(sizeof(t_phil) * all->num_philos);
    if (!all->philos)
        return (1);
    i = 0;
    while (i < all->num_philos)
    {
        // printf("Philosopher %d initialized\n", i + 1);
        all->philos[i].i = i + 1;
        all->philos[i].num_meals = 0;
        all->philos[i].last_meal_time = all->start_time;
        all->philos[i].thread = 0;
        all->philos[i].fork_left = &all->forks[i];
        all->philos[i].fork_right = &all->forks[(i + 1) % all->num_philos];
        all->philos[i].all = all;
        all->philos[i].last_meal_time = all->start_time;
        i++;
    }
    return (0);
}

int init(t_all *all, int c, char **v)
{
    all->num_philos = atoi(v[1]);
    all->time_to_die = atoi(v[2]);
    all->time_to_eat = atoi(v[3]);
    all->time_to_sleep = atoi(v[4]);
    if (c == 6)
        all->num_meals = atoi(v[5]);
    else
        all->num_meals = -1;
    all->start_time = get_time();
    all->die = 0;
    if(init_forks(all))
        return (1);
    if (init_philos(all))
    {
        free_forks(all);
        return (1);
    }
    return (0);
}
