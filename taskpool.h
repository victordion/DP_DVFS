#ifndef TASKPOOL_H
#define TASKPOOL_H
#include "task.h"
#include <vector>
using namespace std;

class taskpool{
private:
	vector<task *> taskbody;
	int cursor;
public:
	void gen(int num);
	double getCursorTaskArrivalTime();
	int getCursor();
	int getPoolSize();
	void incCursor();
	task * getCursorTask();
	void show();
};

#endif