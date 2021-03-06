#ifndef ACMSIM_H
#define ACMSIM_H

#define INDUCTION_MACHINE 1
#define SYNCHRONOUS_MACHINE 2
#define MACHINE_TYPE INDUCTION_MACHINE

/* standard lib */
#include <stdio.h> // printf #include <stdbool.h> // bool for _Bool and true for 1
// #include <stdbool.h> // bool for _Bool and true for 1
#include <process.h>//reqd. for system function prototype
#include <conio.h> // for clrscr, and getch()
#include "stdlib.h" // for rand()
#include "math.h"
#include "time.h"

#if MACHINE_TYPE == INDUCTION_MACHINE
    #define VVVF_CONTROL 0
    #define IFOC 1
    #define DFOC 2
    #define CONTROL_STRATEGY IFOC

    #define TAJIMA96 0 
    #define TOTALLY_ADAPTIVE_OBSERVER 1 // Flux-MRAS
    #define OBSERVER_APPLIED 0

    #define SENSORLESS_CONTROL false
    #define VOLTAGE_CURRENT_DECOUPLING_CIRCUIT false

    #define SATURATED_MAGNETIC_CIRCUIT true
    #define INVERTER_NONLINEARITY true

#elif MACHINE_TYPE == SYNCHRONOUS_MACHINE
    #define NULL_D_AXIS_CURRENT_CONTROL -1
    #define MTPA -2 // not supported
    #define CONTROL_STRATEGY NULL_D_AXIS_CURRENT_CONTROL

    #define SENSORLESS_CONTROL false
#endif

/* How long should I go? */
#define NUMBER_OF_LINES (5*150000)

#define MACHINE_TS 1.25e-4
#define MACHINE_TS_INVERSE 8000
#define DOWN_FREQ_EXE 2
#define DOWN_FREQ_EXE_INVERSE 0.5
#define TS (MACHINE_TS*DOWN_FREQ_EXE) //2.5e-4 
#define TS_INVERSE (MACHINE_TS_INVERSE*DOWN_FREQ_EXE_INVERSE) // 4000
#define DOWN_SAMPLE 1 // 5 // 10

/* Macro for Part transformation*/
#define AB2M(A, B, COS, SIN)  ( (A)*COS  + (B)*SIN )
#define AB2T(A, B, COS, SIN)  ( (A)*-SIN + (B)*COS )
#define MT2A(M, T, COS, SIN)  ( (M)*COS - (T)*SIN )
#define MT2B(M, T, COS, SIN)  ( (M)*SIN + (T)*COS )

// Macro for Power-invariant inverse Clarke transformation
#define AB2U(A, B) ( 0.816496580927726 * ( A ) )
#define AB2V(A, B) ( 0.816496580927726 * ( A*-0.5 + B*0.8660254037844387 ) )
#define AB2W(A, B) ( 0.816496580927726 * ( A*-0.5 + B*-0.8660254037844385 ) )

/* General Constants */
#define TRUE True
#define FALSE False
#define True (1)
#define False (0)
#define true 1
#define false 0
#define lTpi                  0.15915494309189535 // 1/(2*pi)
#define TWO_PI_OVER_3         2.0943951023931953
#define SIN_2PI_SLASH_3       0.86602540378443871 // sin(2*pi/3)
#define SIN_DASH_2PI_SLASH_3 -0.86602540378443871 // sin(-2*pi/3)
#define SQRT_2_SLASH_3        0.81649658092772603 // sqrt(2.0/3.0)
// #define RPM_2_RAD_PER_SEC     0.20943951023931953 // (2/60*Tpi)
// #define RAD_PER_SEC_2_RPM     4.7746482927568605 // 1/(im.npp/60*Tpi)
#define abs                   use_fabs_instead_or_you_will_regret
#define RAD_PER_SEC_2_RPM (60.0/(2*M_PI*ACM.npp))
#define RPM_2_RAD_PER_SEC ((2*M_PI*ACM.npp)/60.0)
// #define PI_D 3.1415926535897932384626433832795 /* double */
#define M_PI_OVER_180   0.017453292519943295

// 补充的宏，为了实现实验/仿真代码大一统
#define Uint32 unsigned long int
#define Uint16 unsigned int
#define PHASE_NUMBER 3



#if MACHINE_TYPE == INDUCTION_MACHINE
    struct InductionMachineSimulated{
        double ial;
        double ibe;
        double psi_al;
        double psi_be;

        double ual;
        double ube;

        double x[13]; ////////////////////////////////
        double rpm;
        double rpm_cmd;
        double rpm_deriv_cmd;
        double Tload;
        double Tem;

        double Js;
        double npp;
        double mu_m;
        double Ts;

        double Lsigma;
        double rs;
        double rreq;
        double Lmu;
        double Lmu_inv;
        double alpha;

        double Lm;
        double rr;
        double Lls;
        double Llr;
        double Lm_slash_Lr;
        double Lr_slash_Lm;

        double LSigmal;

        double izq;
        double izd;
        double iz;
        
        double psimq;
        double psimd;
        double psim;
        double im;

        double iqs;
        double ids;
        double iqr;
        double idr;

        double ual_c_dist;
        double ube_c_dist;
        double dist_al;
        double dist_be;
    };
    extern struct InductionMachineSimulated ACM;

    #define UAL_C_DIST ACM.ual_c_dist // alpha-component of the distorted phase voltage = sine voltage + distored component
    #define UBE_C_DIST ACM.ube_c_dist
    #define DIST_AL ACM.dist_al // alpha-component of the distorted component of the voltage
    #define DIST_BE ACM.dist_be

#elif MACHINE_TYPE == SYNCHRONOUS_MACHINE
    struct SynchronousMachineSimulated{
        double x[5];
        double rpm;
        double rpm_cmd;
        double rpm_deriv_cmd;
        double Tload;
        double Tem;

        double R;
        double Ld;
        double Lq;
        double KE;
        double L0;
        double L1;

        double Js;
        double npp;
        double mu_m;
        double Ts;

        double id;
        double iq;

        double ial;
        double ibe;

        double ud;
        double uq;

        double ual;
        double ube;

        double theta_d;
    };
    extern struct SynchronousMachineSimulated ACM;
#endif


#include "controller.h"
#include "observer.h"
#include "Add_TAAO.h"
// #include "utility.h"
#include "inverter.h"


// saturation
#include "satlut.h"
#define ACMSIMC_DEBUG false

/* Declaration of Utility Function */
void write_header_to_file(FILE *fw);
void write_data_to_file(FILE *fw);
int isNumber(double x);
double sign(double x);
double fabs(double x);
#endif