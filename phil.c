/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:11:26 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/10 14:22:55 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

void	time_printf(t_all *all, const char *message, int phil_id)
{
	pthread_mutex_lock(&all->print_lock);
	if (all->die)
	{
		pthread_mutex_unlock(&all->print_lock);
		return ;
	}
	printf("%lld %d %s\n", get_time() - all->start_time, phil_id, message);
	pthread_mutex_unlock(&all->print_lock);
}

void	mutex_eat(t_phil *phil)
{
	time_printf(phil->all, "has taken a fork", phil->i);
	time_printf(phil->all, "has taken a fork", phil->i);
	time_printf(phil->all, "is eating", phil->i);
	ft_usleep(phil->all->time_to_eat, phil->all);
	pthread_mutex_lock(&phil->meal_lock);
	phil->last_meal_time = get_time();
	phil->num_meals++;
	pthread_mutex_unlock(&phil->meal_lock);
}

void	time_to_eat(t_phil *phil)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

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
		return ;
	}
	mutex_eat(phil);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}

void	*phil_loop(void *arg)
{
	t_phil	*phil;

	phil = (t_phil *)arg;
	if (phil->i % 2 == 0)
    	usleep(phil->all->time_to_eat * 1000);
	while (1)
	{
		if (phil->all->num_philos == 1)
		{
			time_printf(phil->all, "is thinking", phil->i);
			ft_usleep(phil->all->time_to_die + 10, phil->all);
			return (NULL);
		}
		time_to_eat(phil);
		if (phil->all->num_meals > 0 && phil->num_meals >= phil->all->num_meals)
			return (NULL);
		time_printf(phil->all, "is sleeping", phil->i);
		ft_usleep(phil->all->time_to_sleep, phil->all);
		time_printf(phil->all, "is thinking", phil->i);
		if (phil->all->die)
			return (NULL);
	}
	return (NULL);
}
