/*
 * KpfaRawDataMgmt.cpp
 *
 *  Created on: 2014. 5. 19.
 *      Author: Youngsun Han
 */

#include "KpfaRawDataMgmt.h"

/**
 * This function will be used to initialize all the data structures for raw data.
 */ 
KpfaRawDataMgmt::KpfaRawDataMgmt(KpfaCaseData *pCaseData = NULL) {

    m_pCaseData = pCaseData;

	// Bus Data
	m_rBusDataList.clear();
	m_rBusDataTable.clear();

	// Generator Data
	m_rGenDataList.clear();
	m_rGenDataTable.clear();

	// Load Data
	m_rLoadDataList.clear();
	m_rLoadDataTable.clear();

#if KPFA_RAW_DATA_VERSION == 33
	// Fixed Shunt Data
	m_rFixedShuntDataList.clear();
#endif

	// Branch Data
	m_rBranchDataList.clear();

	// Transformer Data
	m_rTransformerDataList.clear();

	// Area Data
	m_rAreaDataList.clear();

	// Two Terminal DC Data
	m_rTwoTermDataList.clear();

	// VSC DC Data
	m_rVscDataList.clear();

	// Switched Shunt
	m_rSwitchedShuntDataList.clear();

	// Swing bus ID
	m_nSwingBusId = 0;    

    // Clear the applied contingency data
    m_pCtgData = NULL;
}

/**
 * This function will be used to finalize all the data structures for raw data.
 */
KpfaRawDataMgmt::~KpfaRawDataMgmt() {

	// Bus Data
	m_rBusDataList.clear();
	m_rBusDataTable.clear();

	// Generator Data
	m_rGenDataList.clear();
	m_rGenDataTable.clear();

	// Load Data
	m_rLoadDataList.clear();
	m_rLoadDataTable.clear();

#if KPFA_RAW_DATA_VERSION == 33
	// Fixed Shunt Data
	m_rFixedShuntDataList.clear();
#endif

	// Branch Data
	m_rBranchDataList.clear();

	// Transformer Data
	m_rTransformerDataList.clear();

	// Area Data
	m_rAreaDataList.clear();

	// Two Terminal DC Data
	m_rTwoTermDataList.clear();

	// VSC DC Data
	m_rVscDataList.clear();

	// Switched Shunt
	m_rSwitchedShuntDataList.clear();
}

/**
 * This function will be used to apply the given contingency data to 
 * this raw data management object. 
 *
 * @param pCtgData contingency data
 * @return error information
 */
KpfaError_t 
KpfaRawDataMgmt::ApplyContingencyData(KpfaCtgData *pCtgData) {

    KPFA_CHECK(pCtgData != NULL, KPFA_ERROR_INVALID_ARGUMENT);

    // Keep the applied contingency data for restoring
    m_pCtgData = pCtgData;

    // Get the iterator of outage list
    KpfaOutageDataList_t::iterator oiter;
    KpfaOutageDataList_t &otgDataList = pCtgData->GetOutageDataList();

    // Apply all the outage of the given contingency
    KpfaRawDataList_t::iterator riter;
        
    for(oiter = otgDataList.begin(); oiter != otgDataList.end(); oiter++) {

        KpfaOutageData *otg = *oiter;

        switch(otg->GetDataType()) {
            
            // Apply bus outage
            case KPFA_OUTAGE_BUS: {
                KpfaBusData *bus = GetBusData(otg->m_nI);
                KPFA_CHECK(bus != NULL, KPFA_ERROR_INVALID_BUS_ID);
                otg->m_nOrigState = (uint32_t)bus->m_nIde;
                bus->m_nIde = KPFA_ISOLATED_BUS;
                break;
            }

            // Apply branch outage
            case KPFA_OUTAGE_BRANCH: {
                KpfaRawDataList_t &branchDataList = m_rBranchDataList;
                for(riter = branchDataList.begin(); riter != branchDataList.end(); riter++) {
                    KpfaBranchData *branch = (KpfaBranchData *)*riter;
                    if(branch->m_nI == otg->m_nI && branch->m_nJ == otg->m_nJ && branch->m_nCkt == otg->m_nCkt) {
                        otg->m_nOrigState = (uint32_t)branch->m_bSt;
                        branch->m_bSt = FALSE;
                        break;
                    }
                }
                break;
            }

            // Apply transformer outage
            case KPFA_OUTAGE_TRANSFORMER: {
                KpfaRawDataList_t &transDataList = m_rTransformerDataList;
                for(riter = transDataList.begin(); riter != transDataList.end(); riter++) {
                    KpfaTransformerData *trans = (KpfaTransformerData *)*riter;
                    if(trans->m_nI == otg->m_nI && trans->m_nJ == otg->m_nJ && 
                       trans->m_nK == otg->m_nK && trans->m_nCkt == otg->m_nCkt) {
                       otg->m_nOrigState = (uint32_t)trans->m_nStat;
                       trans->m_nStat = 0;
                       break;
                    }
                }
                break;
            }

            // Apply generator outage
            case KPFA_OUTAGE_GEN: {
                KpfaGenData *gen = GetGenData(otg->m_nI);
                KPFA_CHECK(gen != NULL, KPFA_ERROR_INVALID_BUS_ID);
                otg->m_nOrigState = (uint32_t)gen->m_bStat;
                gen->m_bStat = FALSE;
                break;
            }

            // YOUNGSUN - CHKME (HVDC, WIND outage)
          
            // Apply HVDC outage 
            case KPFA_OUTAGE_HVDC: {
                break;
            }

            // Apply wind outage 
            case KPFA_OUTAGE_WIND: {
                break;
            }

            default: 
                KPFA_ERROR("Unknown Outage Data Type: %d\n", otg->GetDataType());
                break;
        }
    }
    
    return KPFA_SUCCESS;
}

/**
 * This function will retrieve the original raw data from the modified one by contingency data.  
 * 
 * @return error information
 */
KpfaError_t
KpfaRawDataMgmt::RetrieveFromContingency() {

    // If there is no registered contingency data, just return.
    // Otherwise, retrieve original raw data and clear the applied contigency data information.
    if(m_pCtgData == NULL)  return KPFA_SUCCESS;

    // Get the iterator of outage list
    KpfaOutageDataList_t::iterator oiter;
    KpfaOutageDataList_t &otgDataList = m_pCtgData->GetOutageDataList();

    // Retrieve from all the outages
    KpfaRawDataList_t::iterator riter;
        
    for(oiter = otgDataList.begin(); oiter != otgDataList.end(); oiter++) {

        KpfaOutageData *otg = *oiter;

        switch(otg->GetDataType()) {
            
            case KPFA_OUTAGE_BUS: {
                KpfaBusData *bus = GetBusData(otg->m_nI);
                KPFA_CHECK(bus != NULL, KPFA_ERROR_INVALID_BUS_ID);
                bus->m_nIde = (KpfaBusType_t)otg->m_nOrigState;
                break;
            }

            case KPFA_OUTAGE_BRANCH: {
                KpfaRawDataList_t &branchDataList = m_rBranchDataList;
                for(riter = branchDataList.begin(); riter != branchDataList.end(); riter++) {
                    KpfaBranchData *branch = (KpfaBranchData *)*riter;
                    if(branch->m_nI == otg->m_nI && branch->m_nJ == otg->m_nJ && 
                       branch->m_nCkt == otg->m_nCkt) {
                        branch->m_bSt = (bool_t)otg->m_nOrigState;
                        break;
                    }
                }
                break;
            }

            case KPFA_OUTAGE_TRANSFORMER: {
                KpfaRawDataList_t &transDataList = m_rTransformerDataList;
                for(riter = transDataList.begin(); riter != transDataList.end(); riter++) {
                    KpfaTransformerData *trans = (KpfaTransformerData *)*riter;
                    if(trans->m_nI == otg->m_nI && trans->m_nJ == otg->m_nJ && 
                       trans->m_nK == otg->m_nK && trans->m_nCkt == otg->m_nCkt) {
                       trans->m_nStat = (uint8_t)otg->m_nOrigState;
                       break;
                    }
                }
                break;
            }

            case KPFA_OUTAGE_GEN: {
                KpfaGenData *gen = GetGenData(otg->m_nI);
                KPFA_CHECK(gen != NULL, KPFA_ERROR_INVALID_BUS_ID);
                gen->m_bStat = (bool_t)otg->m_nOrigState;
                break;
            }

            // Apply HVDC outage 
            case KPFA_OUTAGE_HVDC: {
                break;
            }

            // Apply wind outage 
            case KPFA_OUTAGE_WIND: {
                break;
            }

            default: 
                KPFA_ERROR("Unknown Outage Data Type: %d\n", otg->GetDataType());
                break;
        }
    }    

    m_pCtgData = NULL;
    
    return KPFA_SUCCESS;
}

///////////////////////////////////////////////////////////////////
// Debugging Functions
///////////////////////////////////////////////////////////////////

void
KpfaRawDataMgmt::Write(ostream &rOut) {

	uint32_t i;

	// Case ID
	rOut << m_pCaseData << endl;

	// Bus
	for(i = 0; i < m_rBusDataList.size(); i++) {
		rOut << ">> #" << i << " - " << m_rBusDataList[i] << endl;
	}

	// Load
	for(i = 0; i < m_rLoadDataList.size(); i++) {
		rOut << ">> #" << i << " - " << m_rLoadDataList[i] << endl;
	}

#if KPFA_RAW_DATA_VERSION == 33
	// FixedShunt
	for(i = 0; i < m_rFixedShuntDataList.size(); i++) {
		rOut << ">> #" << i << " - " << m_rFixedShuntDataList[i] << endl;
	}
#endif

	// Generator
	for(i = 0; i < m_rGenDataList.size(); i++) {
		rOut << ">> #" << i << " - " << m_rGenDataList[i] << endl;
	}

	// Branch
	for(i = 0; i < m_rBranchDataList.size(); i++) {
		rOut << ">> #" << i << " - " << m_rBranchDataList[i] << endl;
	}

	// Transformer
	for(i = 0; i < m_rTransformerDataList.size(); i++) {
		rOut << ">> #" << i << " - " << m_rTransformerDataList[i] << endl;
	}

	// Area
	for(i = 0; i < m_rAreaDataList.size(); i++) {
		rOut << ">> #" << i << " - " << m_rAreaDataList[i] << endl;
	}

	// Two terminal DC
	for(i = 0; i < m_rTwoTermDataList.size(); i++) {
		rOut << ">> #" << i << " - " << m_rTwoTermDataList[i] << endl;
	}

	// VSC
	for(i = 0; i < m_rVscDataList.size(); i++) {
		rOut << ">> #" << i << " - " << m_rVscDataList[i] << endl;
	}

	// Switched Shunt
	for(i = 0; i < m_rSwitchedShuntDataList.size(); i++) {
		rOut << ">> #" << i << " - " << m_rSwitchedShuntDataList[i] << endl;
	}
}

ostream &operator << (ostream &rOut, KpfaRawDataMgmt *pDataMgmt) {
	pDataMgmt->Write(rOut);
	return rOut;
}
