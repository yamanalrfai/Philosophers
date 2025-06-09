/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:45:18 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/09 22:41:02 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

void time_printf(t_all *all, const char *message, int phil_id)
{
    pthread_mutex_lock(&all->print_lock);
    if (all->die)
    {
        pthread_mutex_unlock(&all->print_lock);
        return;
    }
    printf("%lld %d %s\n", get_time() - all->start_time, phil_id, message);
    pthread_mutex_unlock(&all->print_lock);
}

void ft_usleep(long time_in_ms, t_all *table)
{
    long start = get_time();
    
    while (get_time() - start < time_in_ms && !table->die)
        usleep(100);
}

void time_to_eat(t_phil *phil)
{
    pthread_mutex_t *first_fork = phil->fork_left;
    pthread_mutex_t *second_fork = phil->fork_right;

    if (phil->i % 2 == 0)
    {
        first_fork = phil->fork_left;
        second_fork = phil->fork_right;
    }
    else
    {
        first_fork = phil->fork_right;
        second_fork = phil->fork_left;
    }
    pthread_mutex_lock(first_fork);
    pthread_mutex_lock(second_fork);
    if (phil->all->die)
    {
        pthread_mutex_unlock(first_fork);
        pthread_mutex_unlock(second_fork);
        return;
    }
    if (phil->all->die)
        return  ;
    time_printf(phil->all, "has taken a fork", phil->i);
    time_printf(phil->all, "has taken a fork", phil->i);
    time_printf(phil->all, "is eating", phil->i);
    ft_usleep(phil->all->time_to_eat, phil->all);
    pthread_mutex_lock(&phil->meal_lock);
    phil->last_meal_time = get_time();
    phil->num_meals++;
    pthread_mutex_unlock(&phil->meal_lock);
    pthread_mutex_unlock(first_fork);
    pthread_mutex_unlock(second_fork);
}

void *phil_loop(void *arg)
{
    t_phil *phil;

    phil = (t_phil *)arg;
    if (phil->i % 2 == 0)
        usleep(1000);
    while (1)
    {
        if (phil->all->num_philos == 1)
        {
            time_printf(phil->all, "is thinking", phil->i);
            ft_usleep(phil->all->time_to_die + 10, phil->all); // Wait until death
            return (NULL);
        }
        if (phil->all->die)
            return (NULL);
        time_to_eat(phil);
        if (phil->all->num_meals > 0 && phil->num_meals >= phil->all->num_meals)
            return (NULL);
        if (phil->all->die)
            return (NULL);
        time_printf(phil->all, "is sleeping", phil->i);
        ft_usleep(phil->all->time_to_sleep, phil->all);
        if (phil->all->die)
            return (NULL);
        time_printf(phil->all, "is thinking", phil->i);
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

int monitor_loop(t_all *all, int i)
{
    pthread_mutex_lock(&all->philos[i].meal_lock);
    if (get_time() - all->philos[i].last_meal_time > all->time_to_die &&
        !((all->num_meals > 0 && all->philos[i].num_meals >= all->num_meals)))
    {
        printf("%lld %d is dead\n", get_time() - all->start_time, all->philos[i].i);
        all->die = 1;
        pthread_mutex_unlock(&all->philos[i].meal_lock);
        return 1;
    }
    pthread_mutex_unlock(&all->philos[i].meal_lock);
    return 0;
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
            if (monitor_loop(all, i))
                return NULL;
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
