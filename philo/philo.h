/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cde-sous <cde-sous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:05:11 by cde-sous          #+#    #+#             */
/*   Updated: 2024/12/05 11:53:42 by cde-sous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	pthread_t		thread_id;
	int				times_eaten;
	time_t			last_meal_time;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	int				nb_philo;
	int				time_death;
	int				time_eat;
	int				time_sleep;
	int				max_meals;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	philo_state_lock;
	int				ready_philos_counter;
	int				are_mutex_init;
	time_t			start_time;
	pthread_mutex_t	time_lock;
	int				is_dead;
	pthread_mutex_t	is_dead_lock;
	pthread_t		monitor_thread;
}					t_table;

typedef enum e_status
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DEAD,
	WAIT_TO_DIE
}					t_status;

// cleanup.c
void				ft_usleep(long int time);
int					unlock_destroy_mutexes(pthread_mutex_t *first,
						pthread_mutex_t *second, int flag);
int					destroy_forks(t_table *table);
void				join_threads(t_philo *philo, t_table *table);
int					ft_clean(t_table *table, t_philo *philo_list, int exit_type,
						char *msg);
// utils.c
int					ft_atoi(const char *nptr);
int					ft_strncmp(char *s1, const char *s2, size_t n);
size_t				ft_strlen(const char *s);
time_t				get_time(void);
time_t				get_time_relative(t_table *table);
// status.c
void				check_all_ready(t_philo *philo);
int					check_death_status(t_table *table);
int					print_status(t_philo *philo, t_status status);
int					check_if_dead(time_t current_time, t_table *table,
						t_philo **philo_list);
void				*monitor_routine(void *arg);
// routine.c
int					is_dead_in_action(t_philo *philo, t_status action);
int					only_one_philo(t_philo *philo);
int					philo_eat(t_philo *philo);
int					actions(t_philo *philo);
void				*run_routine(void *arg);
// inits.c
void				assign_forks(t_philo *philo);
int					init_philos(t_philo **philo_list, t_table *table);
int					init_forks(t_table *table);
int					init_mutexes(t_table *table);

#endif