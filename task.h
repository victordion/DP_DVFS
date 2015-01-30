#ifndef TASK_H
#define TASK_H

struct task{
	int id;
	double t_arrival;
	double t_deadline;
	bool missed;
	double e_consumed;
	int cycles;

	task(int id, double t_arrival, double t_deadline,  int cycles){
		task::id = id;
		task::cycles = cycles;
		task::t_arrival = t_arrival;
		task::t_deadline = t_deadline;
		task::missed = 0;
		task::e_consumed = 0.0;
	}
};

#endif