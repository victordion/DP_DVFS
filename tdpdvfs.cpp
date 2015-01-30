#include <chrono>
#include "tdpdvfs.h"
#include <vector>
#include "task.h"
#include "constant_values.h"
#include <iostream>
#include "rdq.h"
#include "utility_functions.h"
using namespace std;

vector<int> tdpdvfs_schedule(rdq irdq){
	int i, j, k, n_slice;
	double longest_deadline = 0.0;
	double V0;
	vector<task *> q = irdq.getTaskq();
	vector<int> ret;

	for(i = 0; i < irdq.getSize(); i++){
		if (q[i]->t_deadline > longest_deadline){
			longest_deadline = q[i]->t_deadline;
		}
	}
	n_slice = longest_deadline / TDPDVFS_T;

	double ** Em;
	Em = new double * [irdq.getSize()];
	for(i = 0; i < irdq.getSize(); i++){
		Em[i] = new double [n_slice];
	}
	for(i = 0; i < irdq.getSize(); i++){
		for(j = 0; j < n_slice; j++){
			V0 = (i == 0) ? 
			for(k = 0; k < CPU_MAX_STATES; k++){
				if(j * TDPDVFS_T <= q[i]->cycles / getCPUF(k) && (j + 1) * TDPDVFS_T >= q[i]->cycles / getCPUF(k)){
					Em[i][j] = getCPUPower(k)/getDCEfficiency(Vcap, k) * q[i]->cycles / getCPUF(k) + getEnergyTransOverhead(getCPUV(k),V0);	
				}
				else{
					Em[i][j] = -1.0;
				}
			}
		
		}
	}
	return ret;
	/*
	double ** Em;
	double ** Tm;
	int ** Sm;
	vector<task *> q = irdq.getTaskq();
	vector<double> Vassigned;
	vector<int> Vassignedid;
	Vassigned.resize(q.size());
	Vassignedid.resize(q.size());

	Em = new double * [irdq.getSize()];
	for(int i = 0; i < irdq.getSize(); i++){
		Em[i] = new double [CPU_MAX_STATES];
	}

	Tm = new double * [irdq.getSize()];
	for(int i = 0; i < irdq.getSize(); i++){
		Tm[i] = new double [CPU_MAX_STATES];
	}

	Sm = new int * [irdq.getSize()];
	for(int i = 0; i < irdq.getSize(); i++){
		Sm[i] = new int [CPU_MAX_STATES];
	}
	int i, j, k, s, sEmin;
	double T;
	double E, Emin, tEmin;
	bool allNA;

	for(j = 0; j < CPU_MAX_STATES; j++){
		T = q[0]->cycles/getCPUF(j) + getTimeTransOverhead(getCPUV(j), V0);
		if(T < q[0]->t_deadline){
			E = getCPUPower(j)/getDCEfficiency(Vcap, j) * q[0]->cycles / getCPUF(j) + getEnergyTransOverhead(getCPUV(j),V0);
			Em[0][j] = E;
			Tm[0][j] = T;
			Sm[0][j] = 0;
		}
		else{
			Em[0][j] = -1.0;
			Tm[0][j] = -1.0;
			Sm[0][j] = -1;
		}
	}

	allNA = true;
	for(s = 0; s < CPU_MAX_STATES; s++){
		if(Em[0][s] > 0){
			allNA = false;
			break;
		}
	}

	if(allNA == true){
		Em[0][CPU_MAX_STATES-1] = getCPUPower(CPU_MAX_STATES-1)/getDCEfficiency(Vcap, CPU_MAX_STATES-1) * q[0]->cycles / getCPUF(CPU_MAX_STATES-1) + getEnergyTransOverhead(V0,getCPUV(CPU_MAX_STATES-1));					
		Tm[0][CPU_MAX_STATES-1] = q[0]->cycles/getCPUF(CPU_MAX_STATES-1) + getTimeTransOverhead(V0,getCPUV(CPU_MAX_STATES-1));
		Sm[0][CPU_MAX_STATES-1] = 0;
	}

	for(i = 1; i < q.size(); i++){
		for(j = 0; j < CPU_MAX_STATES; j++){
			Emin = INFINITY;
			for(k = 0; k < CPU_MAX_STATES; k++){
				if(Em[i-1][k] > 0){
					T = q[i]->cycles/getCPUF(j) + getTimeTransOverhead(getCPUV(j),getCPUV(k));
					if(T + Tm[i-1][k] <= q[i]->t_deadline){
						E = Em[i-1][k] + getCPUPower(j)/getDCEfficiency(Vcap, j) * q[i]->cycles / getCPUF(j) + getEnergyTransOverhead(getCPUV(j),getCPUV(k));
						if(E < Emin){
							Emin = E;
							tEmin = T + Tm[i-1][k];
							sEmin = k;
						}
					}						
				}
			}
			if(abs(Emin - INFINITY) > 0.01){
				Em[i][j] = Emin;
				Tm[i][j] = tEmin;
				Sm[i][j] = sEmin;
			}
			else{
				Em[i][j] = -1.0;
				Tm[i][j] = -1.0;
				Sm[i][j] = -1;
			}
		}
				
		bool allNA = true;
		for(s = 0; s < CPU_MAX_STATES; s++){
			if(Em[i][s] > 0){
				allNA = false;
				break;
			}
		}
		if(allNA == true){
			s = findMinColumn(Em[i-1],CPU_MAX_STATES);
			Em[i][CPU_MAX_STATES-1] = Em[i-1][s] + getCPUPower(CPU_MAX_STATES-1)/getDCEfficiency(Vcap, CPU_MAX_STATES-1) * q[i]->cycles / getCPUF(CPU_MAX_STATES-1) + getEnergyTransOverhead(getCPUV(s),getCPUV(CPU_MAX_STATES-1));					
			Tm[i][CPU_MAX_STATES-1] = Tm[i-1][s] + q[i]->cycles/getCPUF(CPU_MAX_STATES-1) + getTimeTransOverhead(getCPUV(s),getCPUV(CPU_MAX_STATES-1));
			Sm[i][CPU_MAX_STATES-1] = s;
		}
	}
	s = findMinColumn(Em[q.size()-1],CPU_MAX_STATES);
	Vassigned[q.size()-1] = getCPUV(s);
	Vassignedid[q.size()-1] = s;
	s = Sm[q.size()-1][s];
	for(i = q.size() - 2; i >= 0; i--){
		Vassigned[i] = getCPUV(s);
		Vassignedid[i] = s;
		s = Sm[i][s];
	}
#ifdef SHOW_DETAILS
	cout << "Energy Matrix: " << endl;
	showMatrixDouble(Em, q.size(), CPU_MAX_STATES);
	cout << "Time Matrix: " << endl;
	showMatrixDouble(Tm, q.size(), CPU_MAX_STATES);
	cout << "Source Matrix: " << endl;
	showMatrixInt(Sm, q.size(), CPU_MAX_STATES);
	
	for(i = 0; i < q.size(); i++){
		cout << "Voltage for Task: " << i << " is scheduled to be " << Vassigned[i] << endl;
	}
#endif
	return Vassignedid;
	*/
}