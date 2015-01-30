#include "constant_values.h"
#include <cmath>
#include <iostream>
#include "rdq.h"
#include "task.h"
#include <vector>
using namespace std;

double getCPUV(int index){
	return cpu_v[index];
}

double getCPUF(int index){
	return cpu_f[index];
}

double getCPUPower(int index){
	double v, f;
	v = getCPUV(index);
	f = getCPUF(index);
	return cpu_ar * cpu_alpha * v * v * f/1.0e6 + cpu_beta * v + cpu_gamma;
}

double getDCEfficiency(double Vin, double Vout, double Iout){
	return (1 - DCm * (Vin - Vout)) * (DCh - DCa/Iout);

}

double getDCEfficiency(double Vin, int index){

	//return 1.0;	
	double Vout, Iout;
	Vout = getCPUV(index);
	Iout = getCPUPower(index)/Vout;
	return (1 - DCm * (Vin - Vout)) * (DCh - DCa/Iout);
	

}

double getTimeTransOverhead(double Vold, double Vnew){
	return  2 * Cb / Imax * abs(Vold - Vnew);
}

double getEnergyTransOverhead(double Vold, double Vnew){
	double Iout;
	int index;

	for(index = 0; index < CPU_MAX_STATES; index++){
		if(abs(cpu_v[index] - Vnew) < 0.01)
			break;
	}

	Iout = getCPUPower(index)/Vnew;
	return  (1 - getDCEfficiency(Vcap, Vnew, Iout)) * Cb * abs(Vold * Vold - Vnew * Vnew);
}


int findMinColumn(double * body, int bound){
	double min = 1000000;
	int ret;
	for(int i = 0; i < bound; i++ ){
		if(body[i] > 0 && body[i] < min){
			ret = i;
			min = body[i];
		}
	}
	return ret;
}


void showMatrixDouble(double ** body, int ROWSIZE, int COLUMNSIZE){
	//cout << endl;
	for(int i = 0; i < ROWSIZE; i++){
		for(int j = 0; j < COLUMNSIZE; j++){
			printf("%+5.5f ", body[i][j]);
		}
		cout << endl;
	}
}
void showMatrixInt(int ** body, int ROWSIZE, int COLUMNSIZE){
	//cout << endl;
	for(int i = 0; i < ROWSIZE; i++){
		for(int j = 0; j < COLUMNSIZE; j++){
			printf("%+2d ", body[i][j]);
		}
		cout << endl;
	}
}

double getTotalEnergy(rdq irdq, vector<int> voltages_id){
	vector<task *> q = irdq.getTaskq();
	size_t i;
	double ret = 0.0;

	for(i = 0; i < q.size(); i++){
		if(i == 0){
			ret += getEnergyTransOverhead(getCPUV(voltages_id[i]), V0) + \
				getCPUPower(voltages_id[i]) / getDCEfficiency(Vcap,voltages_id[i]) * q[i]->cycles / getCPUF(voltages_id[i]) ;
		}
		else{
			ret += getEnergyTransOverhead(getCPUV(voltages_id[i]),getCPUV(voltages_id[i-1])) + \
				getCPUPower(voltages_id[i]) / getDCEfficiency(Vcap,voltages_id[i]) * q[i]->cycles / getCPUF(voltages_id[i]);
		}
	}
	return ret;
}

int getTotalDeadlineMiss(rdq irdq, vector<int> voltages_id){
	vector<task *> q = irdq.getTaskq();
	size_t i;
	int ret = 0;
	double t_curr = 0.0;

	for(i = 0; i < q.size(); i++){
		if(i == 0){
			t_curr += getTimeTransOverhead(getCPUV(voltages_id[i]), 0.0) + q[i]->cycles / getCPUF(voltages_id[i]);
		}
		else{
			t_curr += getTimeTransOverhead(getCPUV(voltages_id[i]), getCPUV(voltages_id[i-1])) + q[i]->cycles / getCPUF(voltages_id[i]);
		}
		if(t_curr > q[i]->t_deadline){
#ifdef SHOW_DETAILS
			cout << "Deadline Miss for task " << i << endl;
#endif
			ret++;
		}
	}
	return ret;
}

double getTotalDeadlineMissLength(rdq irdq, vector<int> voltages_id){
	vector<task *> q = irdq.getTaskq();
	size_t i;
	double t_curr = 0.0;

	double deadline_miss_len = 0;

	for(i = 0; i < q.size(); i++){
		if(i == 0){
			t_curr += getTimeTransOverhead(getCPUV(voltages_id[i]), 0.0) + q[i]->cycles / getCPUF(voltages_id[i]);
		}
		else{
			t_curr += getTimeTransOverhead(getCPUV(voltages_id[i]), getCPUV(voltages_id[i-1])) + q[i]->cycles / getCPUF(voltages_id[i]);
		}
		if(t_curr > q[i]->t_deadline){
			deadline_miss_len += t_curr - q[i]->t_deadline;
		}
	}
	return deadline_miss_len;
}

double getTotalDeadlineMissRatio(rdq irdq, vector<int> voltages_id){
	vector<task *> q = irdq.getTaskq();
	size_t i;
	double t_curr = 0.0;

	double deadline_miss_len = 0.0;
	double deadline_miss_job_allowed_len = 0.0;

	for(i = 0; i < q.size(); i++){
		if(i == 0){
			t_curr += getTimeTransOverhead(getCPUV(voltages_id[i]), 0.0) + q[i]->cycles / getCPUF(voltages_id[i]);
		}
		else{
			t_curr += getTimeTransOverhead(getCPUV(voltages_id[i]), getCPUV(voltages_id[i-1])) + q[i]->cycles / getCPUF(voltages_id[i]);
		}
		if(t_curr > q[i]->t_deadline){
			deadline_miss_len += t_curr - q[i]->t_deadline;
			deadline_miss_job_allowed_len += q[i]->t_deadline - q[i]->t_arrival;
		}
	}
	if(deadline_miss_job_allowed_len > 0.01)
		return deadline_miss_len/deadline_miss_job_allowed_len;
	else
		return 0.0;

}

void showStateEfficiencies(){
	int state;
	cout << endl << "Showing DC-DC converter efficiency when the capacitor voltage is " << Vcap << "V" << endl;
	for(state = 0; state < CPU_MAX_STATES; state++){
		cout << "CPU V: " << cpu_v[state] << ", CPU F:" << cpu_f[state] << ", CPU Power: " << getCPUPower(state) << ", Efficiency: " << getDCEfficiency(Vcap, state) << endl;
	}
}