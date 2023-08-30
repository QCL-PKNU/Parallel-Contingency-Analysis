/*
 * KpfaRetData.h
 *
 *  Created on: 2015. 6. 23.
 *      Author: Youngsun Han
 */

#ifndef _KPFA_RET_DATA_H_
#define _KPFA_RET_DATA_H_

#include "KpfaDebug.h"
#include "KpfaConfig.h"

/**
 * Return data types
 */
typedef enum {

	KPFA_RET_BUS,

	// The number of return data types
	KPFA_NUM_OF_RET_DATA_TYPES,

} KpfaRetDataType_t;

/**
 * Superclass of all the return data classes
 */
class KpfaRetData {

public:

	// Return Data Type
	KpfaRetDataType_t m_nDataType;

public:

	KpfaRetData(KpfaRetDataType_t nDataType);

	virtual ~KpfaRetData();

	///////////////////////////////////////////////////////////////////
	// Debugging Functions
	///////////////////////////////////////////////////////////////////

	virtual void Write(ostream &rOut) = 0;

	friend ostream &operator << (ostream &rOut, KpfaRetData *pRetData);
};

#endif /* _KPFA_RET_DATA_H_ */
