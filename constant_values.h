#ifndef CONSTANT_VALUES_H
#define CONSTANT_VALUES_H

#define SHOW_DETAILS

const double _INFINITY = 10000000.0;

const int    CPU_MAX_STATES = 5;
const double cpu_v[] = {1.8, 2.175, 2.55, 2.925, 3.3};
const double cpu_f[] = {4e6, 5e6, 6e6, 7e6, 8e6};
const double cpu_alpha = 0.1128;
const double cpu_beta = 0.1738;
const double cpu_gamma = -0.2832;
const double cpu_ar = 0.3;

const double Cb = 1e-8;
const double Imax = 2e-6;

const double DCm = 0.01;
const double DCa = 8.3030e-05; 
const double DCh = 0.95;

const double V0 = 0.0;
const double Vcap = 20;

const int NUM_TOTAL_TASK = 20;
const int NUM_TASK_AVERAGE_CYCLES_INDEX = 150;

const double TDPDVFS_T = 0.001;

//#define SHOW_DETAIL
#endif