/*
 * KpfaInterface.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: youngsun
 */

#include "KpfaInterface.h"

KpfaError_t
KpfaPerformPowerflowAnalysis(KpfaRawDataMgmt *pRawDataMgmt, KpfaRetDataMgmt *pRetDataMgmt) {

	KpfaError_t error;

	KpfaYMatrix *ymat = new KpfaYMatrix();
	error = ymat->BuildMatrix(pRawDataMgmt);

	if(error != KPFA_SUCCESS) {
		KPFA_ERROR("KpfaYMatrix->BuildMatrix: error - %d", error);
		return error;
	}

	// Perform the Newton-Raphson method
	KpfaNewtonRaphson *ntrap = new KpfaNewtonRaphson(pRawDataMgmt);
	error = ntrap->Calculate(ymat);

	if(error != KPFA_SUCCESS) {
		KPFA_ERROR("KpfaNewtonRaphson->Calculate: error - %d", error);
		return error;
	}

	return KpfaGatherResultData(pRawDataMgmt, pRetDataMgmt, ntrap);
}

KpfaError_t
KpfaGatherResultData(KpfaRawDataMgmt *pRawDataMgmt, KpfaRetDataMgmt *pRetDataMgmt, KpfaNewtonRaphson *pNtrap) {

	uint32_t i = 0;
	KpfaComplexVector_t::iterator iter;
	KpfaComplexVector_t &vmat = pNtrap->GetVMatrix();

	for(iter = vmat.begin(); iter != vmat.end(); iter++) {
		KpfaBusData *bus = pRawDataMgmt->GetBusDataAt(i++);
		KPFA_CHECK(bus != NULL, KPFA_ERROR_INVALID_BUS_INDEX);

		// Build a return bus data and insert to the return data list
		pRetDataMgmt->InsertRetBusData(new KpfaRetBusData(bus->m_nI, (*iter).real()));
	}

	return KPFA_SUCCESS;
}

