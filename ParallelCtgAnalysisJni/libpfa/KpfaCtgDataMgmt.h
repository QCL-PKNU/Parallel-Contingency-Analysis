/*
 * KpfaCtgDataMgmt.h
 *
 *  Created on: 2015. 5. 24.
 *      Author: Youngsun Han
 */

#ifndef _KPFA_CTG_DATA_MGMT_H_
#define _KPFA_CTG_DATA_MGMT_H_

#include "KpfaDebug.h"
#include "KpfaConfig.h"
#include "KpfaCtgData.h"

/**
 * Data type for a list of contingency data
 */
typedef std::vector<KpfaCtgData *> KpfaCtgDataList_t;

/**
 * The declaration of the class for Contingency Data Management
 */
class KpfaCtgDataMgmt {

private:

    // Contingency data list
    KpfaCtgDataList_t m_rCtgDataList;

public:

	KpfaCtgDataMgmt();

	virtual ~KpfaCtgDataMgmt();

    /**
     * This function will return the number of contingency data
     *
     * @return the number of contingency data
     */
    inline uint32_t GetCtgDataNumber() {
        return m_rCtgDataList.size();
    }

    /**
     * This function will return the list of the contingency data
     *
     * @return the list of contingency data
     */
    inline KpfaCtgDataList_t &GetCtgDataList() {
        return m_rCtgDataList;
    }

	KpfaError_t ReadCtgDataFile(const char *pFilePath);

	///////////////////////////////////////////////////////////////////
	// Debugging Functions
	///////////////////////////////////////////////////////////////////

	virtual void Write(ostream &rOut);

	friend ostream &operator << (ostream &rOut, KpfaCtgDataMgmt *pDataMgmt);
};

#endif /* _KPFA_CTG_DATA_MGMT_H_ */
