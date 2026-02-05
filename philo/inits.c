/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:53:19 by cde-sous          #+#    #+#             */
/*   Updated: 2024/12/05 11:54:43 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		philo->fork_left = &philo->table->forks[philo->id
			% philo->table->nb_philo];
		philo->fork_right = &philo->table->forks[philo->id - 1];
	}
	else
	{
		philo->fork_left = &philo->table->forks[philo->id - 1];
		philo->fork_right = &philo->table->forks[philo->id
			% philo->table->nb_philo];
	}
}

int	init_philos(t_philo **philo_list, t_table *table)
{
	int	i;

	*philo_list = malloc(sizeof(t_philo) * table->nb_philo);
	if (!*philo_list)
		return (0);
	i = -1;
	while (++i < table->nb_philo)
	{
		(*philo_list)[i].id = i + 1;
		(*philo_list)[i].table = table;
		(*philo_list)[i].times_eaten = 0;
		(*philo_list)[i].last_meal_time = 0;
		assign_forks(&(*philo_list)[i]);
		if (pthread_create(&(*philo_list)[i].thread_id, NULL, run_routine,
			&(*philo_list)[i]) != 0)
			return (0);
	}
	if (pthread_create(&table->monitor_thread, NULL, monitor_routine,
			philo_list) != 0)
		return (0);
	return (1);
}

int	init_forks(t_table *table)
{
	int	i;

	table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	if (!table->forks)
		return (0);
	i = -1;
	while (++i < table->nb_philo)
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (destroy_forks(table));
	return (1);
}

int	init_mutexes(t_table *table)
{
	if (init_forks(table) == 0)
		return (0);
	if (pthread_mutex_init(&table->print_lock, NULL) != 0)
		return (destroy_forks(table));
	if (pthread_mutex_init(&table->philo_state_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&table->print_lock);
		return (destroy_forks(table));
	}
	if (pthread_mutex_init(&table->time_lock, NULL) != 0)
	{
		unlock_destroy_mutexes(&table->print_lock, &table->philo_state_lock, 2);
		return (destroy_forks(table));
	}
	if (pthread_mutex_init(&table->is_dead_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&table->time_lock);
		unlock_destroy_mutexes(&table->print_lock, &table->philo_state_lock, 2);
		return (destroy_forks(table));
	}
	table->are_mutex_init = 1;
	return (1);
}
