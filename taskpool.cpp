#include "task.h"
#include "taskpool.h"
#include "constant_values.h"
#include <vector>
#include <ctime>
#include <iostream>
using namespace std;

void taskpool::gen(int num){
	double t_a, t_d;
	int cycles;
	t_a = t_d = 0.0;
	for(int i = 0; i < num; i++){
		t_a = t_a + rand() * 1.0 / RAND_MAX;
		t_d = t_a + rand() * 1.0 / RAND_MAX;
		cycles = rand()  * NUM_TASK_AVERAGE_CYCLES_INDEX;
		taskbody.push_back(new task(i,t_a,t_d,cycles));
	}
	cursor = 0;
}
	
double taskpool::getCursorTaskArrivalTime(){
	return taskbody[cursor]->t_arrival;
}

int taskpool::getCursor() {return cursor;}
int taskpool::getPoolSize() {return taskbody.size();}
void taskpool::incCursor() {cursor++;}

task * taskpool::getCursorTask(){
	if(cursor > taskbody.size())
		return NULL;
	else
		return taskbody[cursor];	
}

void taskpool::show(){
	cout << endl << "Showing Task Pool Info" << endl;
	for(int i = 0; i < taskbody.size(); i++){
		cout << "ID:" << taskbody[i]->id << " T_arr:" << taskbody[i]->t_arrival \
			<< " T_ddl:" << taskbody[i]->t_deadline << " Cycles:" << taskbody[i]->cycles \
			<< " Missed ddl?:" << taskbody[i]->missed << " Energy:" << taskbody[i]->e_consumed << endl;
	}
}