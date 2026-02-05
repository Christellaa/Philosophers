# PHILOSOPHERS
This is a concurrency project based on the dining philosophers problem.
The goal is to handle synchronization using mutexes and avoid deadlocks while simulating philosophers eating, sleeping, and thinking.

## Technologies used
- Language
	- C
- Tools
	- Makefile
- Norm
	- [42 norminette](https://github.com/42school/norminette)

## What I learned
- Use POSIX threads
- Use mutexes and avoid race conditions
- Simulate time-sensitive behavior (usleep, gettimeofday)
- Manage shared state between threads
- Deadlock prevention techniques

## How to run
```make```

### Example usage
```
make
./philo 5 800 200 200 8
<./philo number_of_philos time_to_die time_to_eat time_to_sleep [max_eat]>
```
