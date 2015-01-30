#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include "rdq.h"

double getCPUV(int index);
double getCPUF(int index);

double getCPUPower(int index);
double getDCEfficiency(double Vin, double Vout, double Iout);
double getDCEfficiency(double Vin, int index);

double getTimeTransOverhead(double Vold, double Vnew);

double getEnergyTransOverhead(double Vold, double Vnew);
int findMinColumn(double * body, int bound);

void showMatrixDouble(double ** body, int ROWSIZE, int COLUMNSIZE);
void showMatrixInt(int ** body, int ROWSIZE, int COLUMNSIZE);

double getTotalEnergy(rdq irdq, vector<int> voltages_id);
int getTotalDeadlineMiss(rdq irdq, vector<int> voltages_id);
double getTotalDeadlineMissLength(rdq irdq, vector<int> voltages_id);
double getTotalDeadlineMissRatio(rdq irdq, vector<int> voltages_id);

void showStateEfficiencies();
#endif