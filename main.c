/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:45:18 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/07 12:46:45 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

void time_to_eat(t_phil *phil)
{
    if (&phil->fork_left < &phil->fork_right)
    {
        pthread_mutex_lock(&phil->fork_left);
        pthread_mutex_lock(&phil->fork_right);
    }
    else
    {
        pthread_mutex_lock(&phil->fork_right);
        pthread_mutex_lock(&phil->fork_left);
    }
    printf("%lld %d has taken a fork\n", get_time() - phil->all->start_time, phil->i);
    printf("%lld %d has taken a fork\n", get_time() - phil->all->start_time, phil->i);
    phil->last_meal_time = get_time();
    printf("%lld %d is eating\n", phil->last_meal_time - phil->all->start_time, phil->i);
    pthread_mutex_unlock(&phil->fork_left);
    pthread_mutex_unlock(&phil->fork_right);
    usleep(phil->all->time_to_eat * 1000);
    phil->num_meals++;
}

void *phil_loop(void *arg)
{
    t_phil *phil;

    phil = (t_phil *)arg;
    if (phil->i % 2)
		usleep(1500);
    while (1)
    {
        if (get_time() - phil->last_meal_time > phil->all->time_to_die && phil->all->die == 0)
        {
            phil->all->die = 1;
            printf("%lld %d has died\n", get_time() - phil->all->start_time, phil->i);
            return (NULL);
        }
        if (phil->all->die)
            return (NULL);
        if (phil->all->num_meals > 0 && phil->num_meals >= phil->all->num_meals)
            return (NULL);
        time_to_eat(phil);
        printf("%lld %d is sleeping\n", get_time() - phil->all->start_time, phil->i);
        usleep(phil->all->time_to_sleep * 1000);
        printf("%lld %d is thinking\n", get_time() - phil->all->start_time, phil->i);
        usleep(500);
    }
    return (NULL);
}

int start(t_all *all)
{
    int i;

    i = 0;
    while (i < all->num_philos)
    {
        if (pthread_create(&all->philos[i].thread, NULL, phil_loop, &all->philos[i]) != 0)
            return (1);
        i++;
    }
    return (0);
}

void end(t_all *all)
{
    int i;

    i = 0;
    while (i < all->num_philos)
    {
        pthread_join(all->philos[i].thread, NULL);
        i++;
    }
    free_all(all);
}

int main(int c, char **v)
{
    t_all all;
    
    if(input_check(c, v))
        return (1);
    if(init(&all, c, v))
        return (1);
    start(&all);
    end(&all);
    return (0);
}
