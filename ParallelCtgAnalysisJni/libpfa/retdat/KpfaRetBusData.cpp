/*
 * KpfaRetBusData.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: youngsun
 */

#include "KpfaRetBusData.h"

KpfaRetBusData::KpfaRetBusData(uint32_t nI, double nVm)
:KpfaRetData(KPFA_RET_BUS){
	m_nI = nI;
	m_nVm = nVm;
}

KpfaRetBusData::~KpfaRetBusData() {
	// Do nothing
}

///////////////////////////////////////////////////////////////////
// Debugging Functions
///////////////////////////////////////////////////////////////////

void
KpfaRetBusData::Write(ostream &rOut) {

	rOut << "KPFA RETURN BUS DATA: " << endl;

	// Bus ID
	rOut << "Bus ID: " << m_nI << endl;

	// Voltage magnitude
	rOut << "Voltage magnitude: " << m_nVm << endl;
}

ostream &operator << (ostream &rOut, KpfaRetBusData *pRetData) {
	pRetData->Write(rOut);
	return rOut;
}
