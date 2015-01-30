#include "greedydvfs.h"
#include <vector>
#include "task.h"
#include "constant_values.h"
#include <iostream>
#include "rdq.h"
#include "utility_functions.h"
#include <vector>
#include "task.h"
using namespace std;

vector<int> nodvfs_schedule(rdq irdq){

	vector<task *> q = irdq.getTaskq();
	vector<double> Vassigned(q.size(), getCPUV(CPU_MAX_STATES-1));
	vector<int> Vassignedid(q.size(), CPU_MAX_STATES-1);
	int i;
	
#ifdef SHOW_DETAILS
	for(i = 0; i < q.size(); i++){
		cout << "Voltage for Task: " << i << " is scheduled to be " << Vassigned[i] << endl;
	}
#endif


	return Vassignedid;
}