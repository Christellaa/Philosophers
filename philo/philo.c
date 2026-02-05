/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 09:43:05 by cde-sous          #+#    #+#             */
/*   Updated: 2024/12/02 10:56:18 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ready_all_philos(t_table *table)
{
	while (1)
	{
		pthread_mutex_lock(&table->philo_state_lock);
		if (table->ready_philos_counter == (table->nb_philo + 1))
		{
			pthread_mutex_lock(&table->time_lock);
			table->start_time = get_time();
			unlock_destroy_mutexes(&table->time_lock, &table->philo_state_lock,
				1);
			break ;
		}
		pthread_mutex_unlock(&table->philo_state_lock);
		usleep(100);
	}
}

int	parse_args(int ac, char **av, t_table *args)
{
	int	i;

	i = 0;
	while (++i < ac)
		if ((i != 5 && ft_atoi(av[i]) == 0) || (i == 5 && ft_atoi(av[i]) == 0
				&& ft_strncmp(av[i], "0", ft_strlen(av[i])) != 0))
			return (0);
	args->nb_philo = ft_atoi(av[1]);
	args->time_death = ft_atoi(av[2]);
	args->time_eat = ft_atoi(av[3]);
	args->time_sleep = ft_atoi(av[4]);
	args->max_meals = -1;
	if (ac == 6)
		args->max_meals = ft_atoi(av[5]);
	args->ready_philos_counter = 0;
	args->start_time = -1;
	args->is_dead = 0;
	return (1);
}

int	main(int ac, char **av)
{
	t_table	*table;
	t_philo	*philo_list;

	table = malloc(sizeof(t_table));
	philo_list = NULL;
	if (!table)
		return (ft_clean(table, philo_list, 1,
				"Unable to allocate memory to table."));
	table->are_mutex_init = 0;
	if (ac != 5 && ac != 6)
		return (ft_clean(table, philo_list, 1, "Wrong number of arguments.\
				\n./philo nb_philo time_die time_eat time_sleep [max_eat]"));
	if (parse_args(ac, av, table) == 0)
		return (ft_clean(table, philo_list, 1, "Invalid arguments.\
			\nAll must be numeric and more than 0 (beside the 6th one)."));
	if (init_mutexes(table) == 0)
		return (ft_clean(table, philo_list, 1,
				"Unable to initialize mutexes."));
	if (init_philos(&philo_list, table) == 0)
		return (ft_clean(table, philo_list, 1,
				"Unable to initialize philosophers."));
	ready_all_philos(table);
	return (ft_clean(table, philo_list, 0, NULL));
}
