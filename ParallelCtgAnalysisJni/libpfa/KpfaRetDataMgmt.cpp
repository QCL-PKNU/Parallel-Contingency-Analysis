/*
 * KpfaRetDataMgmt.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: youngsun
 */

#include "KpfaRetDataMgmt.h"

KpfaRetDataMgmt::KpfaRetDataMgmt() {
	m_rRetBusDataList.clear();
}

KpfaRetDataMgmt::~KpfaRetDataMgmt() {
	m_rRetBusDataList.clear();
}

///////////////////////////////////////////////////////////////////
// Debugging Functions
///////////////////////////////////////////////////////////////////

void
KpfaRetDataMgmt::Write(ostream &rOut) {

	uint32_t i;

	// Bus
	for(i = 0; i < m_rRetBusDataList.size(); i++) {
		rOut << ">> #" << i << " - " << m_rRetBusDataList[i] << endl;
	}
}

ostream &operator << (ostream &rOut, KpfaRetDataMgmt *pRetDataMgmt) {
	pRetDataMgmt->Write(rOut);
	return rOut;
}
