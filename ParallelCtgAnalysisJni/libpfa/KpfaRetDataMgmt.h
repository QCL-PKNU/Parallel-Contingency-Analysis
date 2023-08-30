/*
 * KpfaRetDataMgmt.h
 *
 *  Created on: Jun 23, 2015
 *      Author: youngsun
 */

#ifndef _KPFA_RET_DATA_MGMT_H_
#define _KPFA_RET_DATA_MGMT_H_

#include "KpfaDebug.h"
#include "KpfaConfig.h"
#include "KpfaRetData.h"
#include "KpfaRetBusData.h"

// Return Data List
typedef std::vector<KpfaRetData *> KpfaRetDataList_t;

class KpfaRetDataMgmt {

private:

	// return bus data list
	KpfaRetDataList_t m_rRetBusDataList;

public:

	KpfaRetDataMgmt();

	virtual ~KpfaRetDataMgmt();

	/**
	 * This function will be used to insert a new return bus data to the bus list.
	 *
	 * @param pRetBusData return bus data to be inserted
	 */
	inline void InsertRetBusData(KpfaRetBusData *pRetBusData) {
		m_rRetBusDataList.push_back(pRetBusData);
	}

	/**
	 * This function will return the return bus data list
	 *
	 * @return return bus data list
	 */
	inline KpfaRetDataList_t &GetRetBusDataList() {
		return m_rRetBusDataList;
	}

	///////////////////////////////////////////////////////////////////
	// Debugging Functions
	///////////////////////////////////////////////////////////////////

	virtual void Write(ostream &rOut);

	friend ostream &operator << (ostream &rOut, KpfaRetDataMgmt *pRetDataMgmt);
};

#endif /* _KPFA_RET_DATA_MGMT_H_ */
