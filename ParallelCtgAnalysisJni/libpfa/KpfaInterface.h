/*
 * KpfaInterface.h
 *
 *  Created on: Jun 23, 2015
 *      Author: youngsun
 */

#ifndef _KPFA_INTERFACE_H_
#define _KPFA_INTERFACE_H_

#include "KpfaDebug.h"
#include "KpfaConfig.h"
#include "KpfaYMatrix.h"
#include "KpfaRetDataMgmt.h"
#include "KpfaRawDataMgmt.h"
#include "KpfaRawDataReader.h"
#include "KpfaNewtonRaphson.h"

extern "C" {
// Powerflow analysis main routine
KpfaError_t KpfaPerformPowerflowAnalysis(KpfaRawDataMgmt *, KpfaRetDataMgmt *);

// Return the result of powerflow analysis
KpfaError_t KpfaGatherResultData(KpfaRawDataMgmt *, KpfaRetDataMgmt *, KpfaNewtonRaphson *);
};

#endif /* KPFAINTERFACE_H_ */
