#ifndef RDQ_H
#define RDQ_H
#include "task.h"
#include <list>
#include <vector>
using namespace std;

class rdq{
private:
	list<task *> taskq;
public:
	vector<task *> getTaskq();

	void push(task * newtask);

	void pop();
	int getSize();

	bool isEmpty();
	task * getfront();


	void show();
};

#endif