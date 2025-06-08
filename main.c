/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:45:18 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/08 08:41:45 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

void ft_usleep(long time_in_ms, t_all *table)
{
    struct timeval start;
    struct timeval now;
    long timeout;

    gettimeofday(&start, NULL);
    timeout = 0;
    while (timeout <= time_in_ms && table->die == 0)
    {
        usleep(100);
        gettimeofday(&now, NULL);
        timeout = (now.tv_sec - start.tv_sec) * 1000
                + (now.tv_usec - start.tv_usec) / 1000;
    }
}

void time_to_eat(t_phil *phil)
{
    pthread_mutex_t *first_fork = phil->fork_left;
    pthread_mutex_t *second_fork = phil->fork_right;
    

    if (phil->i % 2 == 0)
    {
        first_fork = phil->fork_right;
        second_fork = phil->fork_left;
    }
    pthread_mutex_lock(first_fork);
    pthread_mutex_lock(second_fork);
    if (phil->all->die)
    {
        return;
    }
    printf("%lld %d has taken a fork\n", get_time() - phil->all->start_time, phil->i);
    printf("%lld %d has taken a fork\n", get_time() - phil->all->start_time, phil->i);
    printf("%lld %d is eating\n", get_time() - phil->all->start_time, phil->i);
    ft_usleep(phil->all->time_to_eat, phil->all);
    pthread_mutex_unlock(first_fork);
    pthread_mutex_unlock(second_fork);
    pthread_mutex_lock(&phil->meal_lock);
    phil->last_meal_time = get_time();
    pthread_mutex_unlock(&phil->meal_lock);
    phil->num_meals++;
}

void *phil_loop(void *arg)
{
    t_phil *phil;

    phil = (t_phil *)arg;
    if (phil->i % 2 == 0)
        ft_usleep(phil->all->time_to_eat / 2, phil->all);
    while (1)
    {
        if (phil->all->die)
            return (NULL);
        time_to_eat(phil);
        if (phil->all->num_meals > 0 && phil->num_meals >= phil->all->num_meals)
            return (NULL);
        if (phil->all->die)
            return (NULL);
        printf("%lld %d is sleeping\n", get_time() - phil->all->start_time, phil->i);
        ft_usleep(phil->all->time_to_sleep, phil->all);
        printf("%lld %d is thinking\n", get_time() - phil->all->start_time, phil->i);
        usleep(10);
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

void *monitor(void *arg)
{
    t_all *all;
    int i;
    int done;

    all = arg;
    while (!all->die)
    {
        i = 0;
        done = 1;
        while (i < all->num_philos)
        {
            pthread_mutex_lock(&all->philos[i].meal_lock);
            if (get_time() - all->philos[i].last_meal_time > all->time_to_die &&
                !((all->num_meals > 0 && all->philos[i].num_meals >= all->num_meals)))
            {
                pthread_mutex_unlock(&all->philos[i].meal_lock);
                all->die = 1;
                printf("%lld %d has died\n", get_time() - all->start_time, all->philos[i].i);
                return NULL;
            }
            pthread_mutex_unlock(&all->philos[i].meal_lock);
            if (!(all->num_meals > 0 && all->philos[i].num_meals >= all->num_meals))
                done = 0;
            i++;
        }
        if (all->num_meals > 0 && done)
        {
            all->die = 1;
            return NULL;
        }
        usleep(1000);
    }
    return NULL;
}

int main(int c, char **v)
{
    t_all all;
    pthread_t monitor_thread;
    
    if(input_check(c, v))
        return (1);
    if(init(&all, c, v))
        return (1);
    pthread_create(&monitor_thread, NULL, monitor, &all);
    start(&all);
    pthread_join(monitor_thread, NULL);
    end(&all);
    return (0);
}
