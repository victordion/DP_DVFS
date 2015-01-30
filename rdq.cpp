#include "rdq.h"
#include "task.h"
#include <vector>
#include <list>
#include <iostream>
using namespace std;

vector<task *> rdq::getTaskq(){
	vector<task *> ret;
	for(list<task *>::iterator it = taskq.begin(); it != taskq.end(); it++){
		ret.push_back(*it);
	}
	return ret;
}

void rdq::push(task * newtask){
	taskq.push_back(newtask);
}

void rdq::pop(){
	taskq.pop_front();
}

int rdq::getSize(){ return taskq.size();}
bool rdq::isEmpty() {return taskq.empty();}
task * rdq::getfront(){ return taskq.front(); }


void rdq::show(){
	list<task *> taskq_tmp = taskq;
	task * tmp;
	cout << endl << "Showing RDQ Info" << endl;
	while(!taskq_tmp.empty()){
		tmp = taskq_tmp.front();
		taskq_tmp.pop_front();
		cout << "ID:" << tmp->id << " T_arr:" << tmp->t_arrival << " T_ddl:" << tmp->t_deadline << " Cycles:" << tmp->cycles << " Missed ddl?:" << tmp->missed << " Energy:" << tmp->e_consumed << endl;
	}
}
