/*
 * KpfaCtgData.cpp
 *
 *  Created on: 2015. 5. 24.
 *      Author: Youngsun Han
 */

#include "KpfaCtgData.h"
#include "KpfaUtility.h"

// contingency tag
#define KPFA_CTG_TAG_START  "(Contingency)"
#define KPFA_CTG_TAG_END    "(End of contingency)"
#define KPFA_CTG_TAG_NAME   "Contingency name"
#define KPFA_CTG_TAG_STATUS "Contingency status"
#define KPFA_CTG_TAG_OUTAGE "Outage"

/**
 * This function will be used to initialize all the data structures for contingency data.
 */
KpfaCtgData::KpfaCtgData() {
    m_rOutageDataList.clear();
}

/**
 * This function will be used to finalize all the data structures for contingency data.
 */
KpfaCtgData::~KpfaCtgData() {
    m_rOutageDataList.clear();
}

/**
* This function will read contingency data from the given stream.
*
* @param rCtgFile contingency file stream
* @return error information
*/

#define _NOT_FOUND(STR)  (linebuf.find(STR) == string::npos)

KpfaError_t
KpfaCtgData::ReadCtgData(ifstream &rCtgFile, bool_t &bFinalCtg) {

	string linebuf;
	KpfaError_t error;

    // initialize the final ctg flag
    bFinalCtg = false;

	while(!getline(rCtgFile, linebuf).fail()) {

		// check the start of contingency data
		if(_NOT_FOUND(KPFA_CTG_TAG_START)) continue;

        // read contingency header
        error = ReadCtgHeader(rCtgFile);
        KPFA_CHECK(error == KPFA_SUCCESS, error);
        
        // parse all the string lines with outage data
    	while(!getline(rCtgFile, linebuf).fail()) {
            if(!_NOT_FOUND(KPFA_CTG_TAG_END)) {
                return KPFA_SUCCESS;
            }

            if(_NOT_FOUND(KPFA_CTG_TAG_OUTAGE)) {
                return KPFA_ERROR_CTG_OUTAGE_READ;
            }

            // parse the outage data
            KpfaOutageData *outage = new KpfaOutageData();
            error = outage->ParseOutageData(linebuf);
            KPFA_CHECK(error == KPFA_SUCCESS, error);

            // push back to the list
            m_rOutageDataList.push_back(outage);
    	}
    }

    // check the final ctg flag to notify the end of the ctg file
    bFinalCtg = true;

	return KPFA_SUCCESS;
}

/**
 * This function will read the contingency header information including name and status.
 *
 * @param rCtgFile input contingency file stream
 * @return error information
 */ 
KpfaError_t 
KpfaCtgData::ReadCtgHeader(ifstream &rCtgFile) {

	string linebuf;
    KpfaStringList_t tokens;
    
    // Read contingency name
    if(getline(rCtgFile, linebuf).fail() || _NOT_FOUND(KPFA_CTG_TAG_NAME) ||
       KpfaTokenize(linebuf, tokens, "=") != 2) {
        return KPFA_ERROR_CTG_NAME_READ;
    }

    m_rName = tokens[1];
    
    // Read contingency status
    if(getline(rCtgFile, linebuf).fail() || _NOT_FOUND(KPFA_CTG_TAG_STATUS) ||
       KpfaTokenize(linebuf, tokens, "=") != 2) {
        return KPFA_ERROR_CTG_STATUS_READ;
    }

    m_rStatus = tokens[1];

    return KPFA_SUCCESS;
}

#undef _NOT_FOUND

///////////////////////////////////////////////////////////////////
// Debugging Functions
///////////////////////////////////////////////////////////////////

void
KpfaCtgData::Write(ostream &rOut) {

    rOut << "Contingency" << endl;

    rOut << "name: " << m_rName << endl;
    
    rOut << "status: " << m_rStatus << endl;

    uint32_t i;
    KpfaOutageDataList_t::iterator iter;
    KpfaOutageDataList_t &dataList = m_rOutageDataList;

    for(iter = dataList.begin(), i = 0; iter != dataList.end(); iter++, i++) {
        rOut << i << ") " << *iter << endl;
    }
}

ostream &operator << (ostream &rOut, KpfaCtgData *pCtgData) {
	pCtgData->Write(rOut);
	return rOut;
}
