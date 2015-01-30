#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <queue>
#include <cmath>
#include <list>
#include "constant_values.h"
#include "taskpool.h"
#include "rdq.h"
#include "dpdvfs.h"
#include "greedydvfs.h"
#include "nodvfs.h"
#include "utility_functions.h"
using namespace std;

int main(){

	double t = 0.0;
	double t_delta = 0.0001;

	srand(time(NULL));

	taskpool itaskpool;
	itaskpool.gen(NUM_TOTAL_TASK);
	//itaskpool.show();

	rdq irdq;

	showStateEfficiencies();
	system("PAUSE");

	while(irdq.isEmpty() == false || itaskpool.getCursor() < itaskpool.getPoolSize()){
		t += t_delta;
		while(itaskpool.getCursor() < itaskpool.getPoolSize() && t >= itaskpool.getCursorTaskArrivalTime()){
			irdq.push(itaskpool.getCursorTask());
			//cout << "At time: " << setw(8) << setprecision(7) <<t << ", pushed task id = " << itaskpool.getCursorTask()->id << " into RDQ." << endl;
			itaskpool.incCursor();
		}

		// all tasks in the pool is now in the RDQ
		if(irdq.getSize() == itaskpool.getPoolSize()){
			double total_energy;
			int total_miss;
			double total_miss_time, miss_ratio;
			vector<int> voltages_assigned;

			cout << "DPDVFS: " << endl;
			voltages_assigned = dpdvfs_schedule(irdq);
			total_energy = getTotalEnergy(irdq, voltages_assigned);
			cout << "Under this schedule the total energy is " << total_energy << endl;
			total_miss = getTotalDeadlineMiss(irdq, voltages_assigned);
			cout << "Under this schedule the total deadline miss is " << total_miss << endl;
			total_miss_time = getTotalDeadlineMissLength(irdq, voltages_assigned);
			cout << "Under this schedule the total deadline miss time is " << total_miss_time << endl;
			miss_ratio = getTotalDeadlineMissRatio(irdq, voltages_assigned);
			cout << "Under this schedule the total deadline miss time ratio is " << miss_ratio << endl;

			cout << "GREEDYDVFS: " << endl;
			voltages_assigned = greedydvfs_schedule(irdq);
			total_energy = getTotalEnergy(irdq, voltages_assigned);
			cout << "Under this schedule the total energy is " << total_energy << endl;
			total_miss = getTotalDeadlineMiss(irdq, voltages_assigned);
			cout << "Under this schedule the total deadline miss is " << total_miss << endl;
			total_miss_time = getTotalDeadlineMissLength(irdq, voltages_assigned);
			cout << "Under this schedule the total deadline miss time is " << total_miss_time << endl;
			miss_ratio = getTotalDeadlineMissRatio(irdq, voltages_assigned);
			cout << "Under this schedule the total deadline miss time ratio is " << miss_ratio << endl;

			cout << "NODVFS: " << endl;
			voltages_assigned = nodvfs_schedule(irdq);
			total_energy = getTotalEnergy(irdq, voltages_assigned);
			cout << "Under this schedule the total energy is " << total_energy << endl;
			total_miss = getTotalDeadlineMiss(irdq, voltages_assigned);
			cout << "Under this schedule the total deadline miss is " << total_miss << endl;
			total_miss_time = getTotalDeadlineMissLength(irdq, voltages_assigned);
			cout << "Under this schedule the total deadline miss time is " << total_miss_time << endl;
			miss_ratio = getTotalDeadlineMissRatio(irdq, voltages_assigned);
			cout << "Under this schedule the total deadline miss time ratio is " << miss_ratio << endl;

			break;
		}

	}

	//irdq.show();

	return 0;
}