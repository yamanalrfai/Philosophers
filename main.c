/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:45:18 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/07 09:11:14 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

void *start_eat(void *arg)
{
    t_phil *phil;

    phil = (t_phil *)arg;
    while (1)
    {
        
    }
    return (NULL);
}

void start(t_all *all)
{
    int i;

    i = 0;
    while (i < all->num_philos)
    {
        if (pthread_create(&all->philos[i].thread, NULL, start_eat, &all->philos[i]) != 0)
            return (1);
        i++;
    }
}

int main(int c, char **v)
{
    t_all all;
    
    if(input_check(c, v))
        return (1);
    if(init(&all, c, v))
        return (1);
    start(&all);
    return (0);
}
