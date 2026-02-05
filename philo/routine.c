/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:30:39 by cde-sous          #+#    #+#             */
/*   Updated: 2024/11/23 16:48:10 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead_in_action(t_philo *philo, t_status action)
{
	time_t	action_time;
	time_t	start_time;
	time_t	elapsed_time;

	if (action == EAT)
		action_time = philo->table->time_eat;
	else if (action == SLEEP)
		action_time = philo->table->time_sleep;
	else if (action == WAIT_TO_DIE)
		action_time = philo->table->time_death + 1;
	start_time = get_time_relative(philo->table);
	while (1)
	{
		elapsed_time = get_time_relative(philo->table) - start_time;
		if (elapsed_time >= action_time)
			break ;
		if (check_death_status(philo->table) == 0)
			return (0);
		usleep(100);
	}
	return (1);
}

int	only_one_philo(t_philo *philo)
{
	if (philo->table->nb_philo == 1)
	{
		pthread_mutex_lock(philo->fork_left);
		if (print_status(philo, FORK) == 0)
			return (unlock_destroy_mutexes(philo->fork_left, NULL, 1));
		ft_usleep(philo->table->time_death);
		return (unlock_destroy_mutexes(philo->fork_left, NULL, 1));
	}
	return (1);
}

int	philo_eat(t_philo *philo)
{
	if (only_one_philo(philo) == 0)
		return (0);
	pthread_mutex_lock(philo->fork_left);
	if (print_status(philo, FORK) == 0)
		return (unlock_destroy_mutexes(philo->fork_left, NULL, 1));
	pthread_mutex_lock(philo->fork_right);
	if (print_status(philo, FORK) == 0)
		return (unlock_destroy_mutexes(philo->fork_right, philo->fork_left, 1));
	pthread_mutex_lock(&philo->table->time_lock);
	philo->last_meal_time = get_time_relative(philo->table);
	pthread_mutex_unlock(&philo->table->time_lock);
	if (print_status(philo, EAT) == 0)
		return (unlock_destroy_mutexes(philo->fork_right, philo->fork_left, 1));
	if (is_dead_in_action(philo, EAT) == 0)
		return (unlock_destroy_mutexes(philo->fork_right, philo->fork_left, 1));
	pthread_mutex_lock(&philo->table->philo_state_lock);
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->table->philo_state_lock);
	unlock_destroy_mutexes(philo->fork_right, philo->fork_left, 1);
	return (1);
}

int	actions(t_philo *philo)
{
	if (philo->times_eaten == philo->table->max_meals)
		return (0);
	if (check_death_status(philo->table) == 0)
		return (0);
	if (philo_eat(philo) == 0)
		return (0);
	if (philo->times_eaten == philo->table->max_meals)
		return (0);
	if (print_status(philo, SLEEP) == 0)
		return (0);
	if (is_dead_in_action(philo, SLEEP) == 0)
		return (0);
	if (print_status(philo, THINK) == 0)
		return (0);
	return (1);
}

void	*run_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	check_all_ready(philo);
	if (philo->id % 2 == 0)
		ft_usleep(philo->table->time_eat / 10);
	while (1)
		if (actions(philo) == 0)
			break ;
	return (NULL);
}
