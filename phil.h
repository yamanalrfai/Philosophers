/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:45:49 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/07 16:05:25 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHIL_H
# define PHIL_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

typedef struct s_all t_all;

typedef struct s_phil
{
    int				i;
    int				num_meals;
    long long		last_meal_time;
    pthread_mutex_t	fork_left;
    pthread_mutex_t	fork_right;
    pthread_mutex_t	meal_lock;
    t_all            *all;
    pthread_t		thread;
}	t_phil;

typedef struct s_all
{
    int		        num_philos;
    int		        time_to_die;
    int		        time_to_eat;
    int		        time_to_sleep;
    int		        num_meals;
    long long	    start_time;
    _Atomic int     die;
    pthread_mutex_t *forks;
    t_phil		    *philos;
}	t_all;

long long	atoll(const char *str);
int		check_INT_MAX(int j, int i, char *str);
int		valid_atoi(char *str);
int		input_check(int c, char **v);
int		init(t_all *all, int c, char **v);
void	free_forks(t_all *all);
void	free_philos(t_all *all);
int		free_all(t_all *all);
int		start(t_all *all);
int		init_forks(t_all *all);
int		init_philos(t_all *all);
int		get_time(void);
void	time_to_eat(t_phil *phil);
void	*phil_loop(void *arg);


#endif