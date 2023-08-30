/*
 * KpfaCtgData.h
 *
 *  Created on: 2015. 5. 24.
 *      Author: Youngsun Han
 */

#ifndef _KPFA_CTG_DATA_H_
#define _KPFA_CTG_DATA_H_

#include "KpfaDebug.h"
#include "KpfaConfig.h"
#include "KpfaOutageData.h"

/**
 * Data type for a list of outage data
 */
typedef std::vector<KpfaOutageData *> KpfaOutageDataList_t;

/**
 * The declaration of the class for Contingency Data Management
 */
class KpfaCtgData {

private:

    // Contingency name
    string m_rName;

    // Contingency status
    string m_rStatus;

    // Outage data list
    KpfaOutageDataList_t m_rOutageDataList;

public:

	KpfaCtgData();

	virtual ~KpfaCtgData();

    /**
     * This function will return the list of outage data of the current contingency.
     *
     * @return the list of outage data
     */
    inline KpfaOutageDataList_t &GetOutageDataList() {
        return m_rOutageDataList;
    }

    /**
     * This function will be used to add a new outage to the contingency data.
     *
     * @param pOtgData a new outage data to be inserted
     */
    inline void InsertOutageData(KpfaOutageData *pOtgData) {
    	m_rOutageDataList.push_back(pOtgData);
    }

	KpfaError_t ReadCtgData(ifstream &rCtgFile, bool_t &bFinalCtg);

	///////////////////////////////////////////////////////////////////
	// Debugging Functions
	///////////////////////////////////////////////////////////////////

	virtual void Write(ostream &rOut);

	friend ostream &operator << (ostream &rOut, KpfaCtgData *pDataMgmt);

private:

    KpfaError_t ReadCtgHeader(ifstream &rCtgFile);
};

#endif /* _KPFA_CTG_DATA_H_ */
