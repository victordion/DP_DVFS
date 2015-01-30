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

vector<int> greedydvfs_schedule(rdq irdq){

	vector<task *> q = irdq.getTaskq();
	vector<double> Vassigned;
	vector<int> Vassignedid;
	Vassigned.resize(q.size());
	Vassignedid.resize(q.size());
	
	double t_curr = 0.0;
	double t_oh;
	size_t i, j;
	
	for(i = 0; i < q.size(); i++){
		for(j = 0; j < CPU_MAX_STATES; j++){
			if(i == 0){
				t_oh = getTimeTransOverhead(getCPUV(j),V0);
			}
			else{
				t_oh = getTimeTransOverhead(getCPUV(j),getCPUV(Vassignedid[i-1]));
			}

			if(t_curr + q[i]->cycles/getCPUF(j) + t_oh <= q[i]->t_deadline || j == CPU_MAX_STATES - 1){
				Vassignedid[i] = j;
				Vassigned[i] = cpu_v[j];
				t_curr += q[i]->cycles/getCPUF(j) + t_oh;
				break;
			}
		}
	}
#ifdef SHOW_DETAILS
	for(i = 0; i < q.size(); i++){
		cout << "Voltage for Task: " << i << " is scheduled to be " << Vassigned[i] << endl;
	}
#endif

	return Vassignedid;
}