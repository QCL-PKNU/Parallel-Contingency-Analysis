/*
 * KpfaRetBusData.h
 *
 *  Created on: Jun 23, 2015
 *      Author: youngsun
 */

#ifndef _KPFA_RET_BUS_DATA_H_
#define _KPFA_RET_BUS_DATA_H_

#include "KpfaRetData.h"

class KpfaRetBusData : public KpfaRetData {

public:

	// Bus number (1 ~ 999997)
	uint32_t m_nI;

	// Bus voltage magnitude
	double m_nVm;

public:

	KpfaRetBusData(uint32_t nI, double nVm);

	virtual ~KpfaRetBusData();

	///////////////////////////////////////////////////////////////////
	// Debugging Functions
	///////////////////////////////////////////////////////////////////

	virtual void Write(ostream &rOut);

	friend ostream &operator << (ostream &rOut, KpfaRetData *pRetData);
};

#endif /* _KPFA_RET_BUS_DATA_H_ */
