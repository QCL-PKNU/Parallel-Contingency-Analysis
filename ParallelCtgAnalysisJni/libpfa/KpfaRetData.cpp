/*
 * KpfaRetData.cpp
 *
 *  Created on: 2015. 6. 23.
 *      Author: Youngsun Han
 */

#include "KpfaRetData.h"

/**
 * Ret data type strings
 */
string g_KpfaRetDataTypeString[KPFA_NUM_OF_RET_DATA_TYPES] = {

	"BUS",
};

KpfaRetData::KpfaRetData(KpfaRetDataType_t nDataType) {
	m_nDataType = nDataType;
}

KpfaRetData::~KpfaRetData() {
	// Do nothing
}

ostream &operator << (ostream &rOut, KpfaRetData *pRetData) {
	pRetData->Write(rOut);
	return rOut;
}
