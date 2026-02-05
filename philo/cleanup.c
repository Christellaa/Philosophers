/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 10:59:13 by cde-sous          #+#    #+#             */
/*   Updated: 2024/12/02 10:55:55 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(long int time)
{
	long int	start;

	start = get_time();
	while (get_time() - start < time)
		usleep(time / 10);
}

int	unlock_destroy_mutexes(pthread_mutex_t *first, pthread_mutex_t *second,
		int flag)
{
	if (flag == 1)
	{
		if (first)
			pthread_mutex_unlock(first);
		if (second)
			pthread_mutex_unlock(second);
	}
	if (flag == 2)
	{
		if (first)
			pthread_mutex_destroy(first);
		if (second)
			pthread_mutex_destroy(second);
	}
	return (0);
}

int	destroy_forks(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->nb_philo)
		pthread_mutex_destroy(&table->forks[i]);
	free(table->forks);
	return (0);
}

void	join_threads(t_philo *philo, t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->nb_philo)
		pthread_join(philo[i].thread_id, NULL);
}

int	ft_clean(t_table *table, t_philo *philo_list, int exit_type, char *msg)
{
	if (msg)
		printf("%s\n", msg);
	if (table)
	{
		if (philo_list)
		{
			pthread_join(table->monitor_thread, NULL);
			join_threads(philo_list, table);
			free(philo_list);
		}
		if (table->are_mutex_init == 1)
		{
			destroy_forks(table);
			pthread_mutex_destroy(&table->print_lock);
			pthread_mutex_destroy(&table->philo_state_lock);
		}
		free(table);
	}
	if (exit_type == 0)
		return (0);
	return (1);
}
