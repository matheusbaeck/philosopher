/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math42 <math42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:12:41 by math42            #+#    #+#             */
/*   Updated: 2023/09/13 13:47:54 by math42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

// void	set_time_loop(t_philo *philo)
// {
// 	philo->time_loop = philo->time % (philo->time_to_die % 3);
// }

int	try_lock(t_philo *philo)
{
	int	e;
	int	time_loop;
	int	turn_size;
	int margin = 10;

	turn_size = philo->time_to_die / philo->div;
	while (!get_time(philo) && (philo->time - philo->last_meal) < philo->time_to_die)
	{
		time_loop = philo->time % philo->time_to_die;
		if (time_loop / turn_size == philo->name % philo->div
			&& time_loop % turn_size < margin)
		{
			get_time(philo);
			time_loop = philo->time % philo->time_to_die;
			e = pthread_mutex_lock(philo->fork[0]);
			if (e == 0)
			{
				printf("%ld %d took fork %d\n", philo->time - philo->born_time, philo->name + 1, philo->name);
				e = pthread_mutex_lock(philo->fork[1]);
				if (e == 0)
				{
					get_time(philo);
					printf("%ld %d took fork %d\n", philo->time - philo->born_time, philo->name + 1, philo->name + 2);
					return (0);
				}
				else
				{
					pthread_mutex_unlock(philo->fork[0]);
					printf(" fail locking %p    %d\n", &philo->fork[1], e);
					if (pthread_mutex_unlock(philo->fork[0]) == 0)
						printf(" unlocking\n");
					else
						printf(" fail unlocking");
				}
			}
		}
	}
	return (-1);
}

//current_time() - philo->start_time - philo->last_meal > philo->time_to_die

int	try_unlock(t_philo *philo)
{
	if (!pthread_mutex_unlock(philo->fork[0]))
		printf(" unlocked %p\n", philo->fork[0]);
	else
		printf(" fail unlocking %p\n", philo->fork[0]);
	if (!pthread_mutex_unlock(philo->fork[1]))
		printf(" unlocked %p\n", philo->fork[1]);
	else
		printf(" fail unlocking %p\n", philo->fork[1]);
	return (0);
}

int	pwait(int mseconds)
{
	struct timeval	tv;
	long int		start_time;
	long int		delta_time;

	mseconds *= 1000;
	gettimeofday(&tv, NULL);
	start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	delta_time = ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - start_time);
	while (delta_time < mseconds)
	{
		if ((mseconds - delta_time) / 999999 > 0)
			usleep(999999);
		else
			return (usleep((mseconds - delta_time) % 999999));
		gettimeofday(&tv, NULL);
		delta_time = ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - start_time);
	}
	return (0);
}
