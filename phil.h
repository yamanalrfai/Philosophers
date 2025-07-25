/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:16:49 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/10 10:33:30 by yaman-alrif      ###   ########.fr       */
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

typedef struct s_all	t_all;

typedef struct s_phil
{
	int				i;
	int				num_meals;
	long long		last_meal_time;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	pthread_mutex_t	meal_lock;
	t_all			*all;
	pthread_t		thread;
}	t_phil;

typedef struct s_all
{
	int					num_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					num_meals;
	long long			start_time;
	_Atomic int			die;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_lock;
	t_phil				*philos;
}	t_all;

long long	atoll(const char *str);
int			check_intmax(int j, int i, char *str);
int			valid_atoi(char *str);
int			input_check(int c, char **v);
int			init(t_all *all, int c, char **v);
void		free_forks(t_all *all);
void		free_philos(t_all *all);
int			free_all(t_all *all);
int			start(t_all *all);
int			init_forks(t_all *all);
int			init_philos(t_all *all);
void		time_to_eat(t_phil *phil);
void		*phil_loop(void *arg);
long long	get_time(void);
void		ft_usleep(long time, t_all *all);
void		time_printf(t_all *all, const char *message, int phil_id);
void		mutex_eat(t_phil *phil);
int			monitor_loop(t_all *all, int i);
void		*monitor(void *arg);
void		end(t_all *all);

#endif
