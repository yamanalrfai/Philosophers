/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:14:10 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/10 14:44:23 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

int	start(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->num_philos)
	{
		if (pthread_create(&all->philos[i].thread,
				NULL, phil_loop, &all->philos[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	end(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->num_philos)
	{
		pthread_join(all->philos[i].thread, NULL);
		i++;
	}
	free_all(all);
}

int	monitor_loop(t_all *all, int i)
{
	pthread_mutex_lock(&all->philos[i].meal_lock);
	if (get_time() - all->philos[i].last_meal_time > all->time_to_die
		&& !((all->num_meals > 0
				&& all->philos[i].num_meals >= all->num_meals)))
	{
		time_printf(all, "died", all->philos[i].i);
		all->die = 1;
		pthread_mutex_unlock(&all->philos[i].meal_lock);
		return (1);
	}
	pthread_mutex_unlock(&all->philos[i].meal_lock);
	return (0);
}

void	*monitor(void *arg)
{
	t_all	*all;
	int		i;
	int		done;

	all = arg;
	while (!all->die)
	{
		i = -1;
		done = 1;
		while (++i < all->num_philos)
		{
			if (monitor_loop(all, i))
				return (NULL);
			if (!(all->num_meals > 0
					&& all->philos[i].num_meals >= all->num_meals))
				done = 0;
		}
		if (all->num_meals > 0 && done)
		{
			all->die = 1;
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

int	main(int c, char **v)
{
	t_all		all;
	pthread_t	monitor_thread;

	if (input_check(c, v))
		return (1);
	if (init(&all, c, v))
		return (1);
	pthread_create(&monitor_thread, NULL, monitor, &all);
	start(&all);
	pthread_join(monitor_thread, NULL);
	end(&all);
	return (0);
}
