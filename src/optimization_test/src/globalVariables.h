
//  globalVariables.h
//  Sep 3, 2012
//  Project: Optimization Test
//  YL


#ifndef __GLOBAl_VARIABLES__
#define __GLOBAL_VARIABLES__

//This is a dirty workaround
#undef Success

#include <dm.h>
#include <dmArticulation.hpp>

#include "SimulationThread.h"
#include "MainFrame.h"
#include "OTDataLogger.h"

//#define EIGEN_NO_DEBUG
//#define OPTIM_DEBUG
//#define CONTROL_DEBUG




extern dmArticulation *G_robot;

extern MainFrame *frame;
extern SimulationThread * simThread;
extern OTDataLogger * logger;

//extern wxMutex dataMutex;


// variables for display 
extern volatile Float _ComPos[3];
extern volatile Float _ComPosDes[3];


#endif
