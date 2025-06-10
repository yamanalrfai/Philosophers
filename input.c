/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:05:28 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/10 10:34:12 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

long long	atoll(const char *str)
{
	long long		result;
	int				i;
	int				sign;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	check_intmax(int j, int i, char *str)
{
	long long	num;

	if (i - j >= 12 || (i - j == 11 && !(str[j] == '-' || str[j] == '+')))
		return (0);
	num = atoll(str + j);
	if (num < 0 || num > INT_MAX)
		return (0);
	return (1);
}

int	valid_atoi(char *str)
{
	int	i;
	int	j;

	if (!str || !str[0])
		return (0);
	i = 0;
	j = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	j = i;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (check_intmax(j, i, str));
}

int	check_args(int c, char **v)
{
	int	i;

	i = 1;
	while (i < c)
	{
		if (valid_atoi(v[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	input_check(int c, char **v)
{
	if (c < 5 || c > 6)
	{
		printf("Error: Wrong number of arguments.\n");
		return (1);
	}
	if (check_args(c, v) == 1)
	{
		printf("Error: Invalid arguments.\n");
		return (1);
	}
	return (0);
}
