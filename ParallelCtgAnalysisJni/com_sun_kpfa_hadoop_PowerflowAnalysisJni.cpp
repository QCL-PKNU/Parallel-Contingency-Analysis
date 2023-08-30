#include "com_sun_kpfa_hadoop_PowerflowAnalysisJni.h"

#include "hdfs.h"

#include "KpfaDebug.h"
#include "KpfaConfig.h"
#include "KpfaCtgData.h"
#include "KpfaOutageData.h"
#include "KpfaRawDataMgmt.h"
#include "KpfaRawDataReader.h"
#include "KpfaInterface.h"

#include <fstream>

using namespace std;

#define JNI_STRING 						"Ljava/lang/String;"

/**
 * This function will return the size of outage list.
 *
 * @param pEnv JNI environment
 * @param rCtgObj Java contingency data object
 * @return the number of outages
 */
static jint
GetOutageDataListSize(JNIEnv *pEnv, jobject rCtgObj) {

	jclass cls = pEnv->FindClass("com/sun/kpfa/hadoop/ContingencyData");
	if(cls == NULL) return 0;

	jmethodID mid = pEnv->GetMethodID(cls, "getOutageDataListSize", "()I");
	KPFA_CHECK(mid != NULL, -1);
	return pEnv->CallIntMethod(rCtgObj, mid);
}

/**
 * This function will return the list of outage data.
 *
 * @param pEnv JNI environment
 * @param rCtgObj Java contingency data object
 * @return the list of outages
 */
static jobject
GetOutageDataList(JNIEnv *pEnv, jobject rCtgObj) {

	jclass cls = pEnv->FindClass("com/sun/kpfa/hadoop/ContingencyData");
	if(cls == NULL) return 0;

	jmethodID mid = pEnv->GetMethodID(cls, "getOutageDataList", "()Ljava/util/Vector;");
	KPFA_CHECK(mid != NULL, NULL);
	return pEnv->CallObjectMethod(rCtgObj, mid);
}

/**
 * This function will return the outage data at the given index.
 *
 * @param pEnv JNI environment
 * @param rOtgListObj outage data list object
 * @param nIndex the index of the outage data to be returned
 * @return outage data
 */
static jobject
GetOutageDataAt(JNIEnv *pEnv, jobject rOtgListObj, jint nIndex) {

	// Class ID for 'java.util.Vector'
	jclass cls = pEnv->FindClass("java/util/Vector");
	KPFA_CHECK(cls != NULL, 0);

	// Method ID for 'java.util.Vector.elementAt'
	jmethodID mid = pEnv->GetMethodID(cls, "elementAt", "(I)Ljava/lang/Object;");
	KPFA_CHECK(mid != NULL, 0);

	// return the outage object at nIndex
	return pEnv->CallObjectMethod(rOtgListObj, mid, nIndex);
}

/**
 * This function will transform a Java outage data into a native data.
 *
 * @param pEnv JNI environment
 * @param rOtgObj outage object
 * @return native outage data
 */
static KpfaOutageData *
GetNativeOutageData(JNIEnv *pEnv, jobject rOtgObj) {

	KpfaOutageData *otg = new KpfaOutageData();
	KPFA_CHECK(otg != NULL, NULL);

	// outage class ID
	jclass cls = pEnv->FindClass("com/sun/kpfa/hadoop/OutageData");
	if(cls == NULL) return NULL;

	// Method IDs
	jmethodID mid_getI = pEnv->GetMethodID(cls, "getI", "()I");
	jmethodID mid_getJ = pEnv->GetMethodID(cls, "getJ", "()I");
	jmethodID mid_getK = pEnv->GetMethodID(cls, "getK", "()I");
	jmethodID mid_getCkt = pEnv->GetMethodID(cls, "getCkt", "()I");
	jmethodID mid_getDataType = pEnv->GetMethodID(cls, "getDataType", "()I");

	otg->m_nI = pEnv->CallIntMethod(rOtgObj, mid_getI);
	otg->m_nJ = pEnv->CallIntMethod(rOtgObj, mid_getJ);
	otg->m_nK = pEnv->CallIntMethod(rOtgObj, mid_getK);
	otg->m_nCkt = pEnv->CallIntMethod(rOtgObj, mid_getCkt);
	otg->m_nDataType = (KpfaOutageDataType_t)pEnv->CallIntMethod(rOtgObj, mid_getDataType);

	return otg;
}

// To change a char array into an istream
struct membuf : std::streambuf
{
    membuf(char* begin, int size) {
        this->setg(begin, begin, begin + size);
    }
};

/**
 * This function will be used to build a raw data reader with the given bytes.
 *
 * @param pEnv JNI environment
 * @param rRawDataByteArr raw data bytes
 * @param rRawDataReader raw data reader to parse the given raw data bytes
 * @return error information
 */
KpfaError_t
ReadRawDataFile(JNIEnv *pEnv, KpfaRawDataReader *rRawDataReader, jbyteArray rRawDataByteArr) {

	unsigned char isCopy;
	KPFA_CHECK(rRawDataReader != NULL, KPFA_ERROR_INVALID_ARGUMENT);

	// transform Java bytes into native bytes
	int rawDataByteSize = pEnv->GetArrayLength(rRawDataByteArr);
	char *rawDataBytes = (char *)pEnv->GetByteArrayElements(rRawDataByteArr, &isCopy);
	if(rawDataBytes == NULL) {
		fprintf(stderr, "Raw data bytes read error\n");
		return KPFA_ERROR_JNI_GET_RAW_DATA;
	}

	// read raw data
	membuf rawDataBuf(rawDataBytes, rawDataByteSize);
	istream rawDataStream(&rawDataBuf);
	return rRawDataReader->ReadRawDataFile(rawDataStream);
}

/**
 * This function will be used to add a result data with the given native return data
 *
 * @param pEnv JNI environment
 * @param rResMgmtObj result management object
 * @param pRetData native return data
 * @return true if adding the result data completes
 */
static bool_t
AddResultData(JNIEnv *pEnv, jobject rResMgmtObj, KpfaRetData *pRetData) {

	// ResultDataMgmt class
	jclass cls = pEnv->FindClass("com/sun/kpfa/hadoop/ResultDataMgmt");
	KPFA_CHECK(cls != NULL, FALSE);

	// Method ID
	jmethodID mid = pEnv->GetMethodID(cls, "addResultData", "(ID)V");
	KPFA_CHECK(mid != NULL, FALSE);

	// call addResultData method
	switch(pRetData->m_nDataType) {
		case KPFA_RET_BUS: {
			KpfaRetBusData *busData = (KpfaRetBusData *)pRetData;
			pEnv->CallVoidMethod(rResMgmtObj, mid, busData->m_nI, busData->m_nVm);
			break;
		}
		default:
			fprintf(stderr, "Unknown return data type: %d\n", pRetData->m_nDataType);
			return FALSE;
	}

	return TRUE;
}

/*
 * Class:     com_sun_kpfa_hadoop_PowerflowAnalysisJni
 * Method:    PerformCtgAnalysis
 * Signature: (Lcom/sun/kpfa/hadoop/ContingencyData;Lcom/sun/kpfa/hadoop/ResultDataMgmt;)Z
 */
JNIEXPORT jint JNICALL Java_com_sun_kpfa_hadoop_PowerflowAnalysisJni_PerformCtgAnalysis
  (JNIEnv *pEnv, jobject rThisObj, jobject rCtgObj, jbyteArray rRawDataByteArr, jobject rResMgmtObj) {

	KpfaError_t error = KPFA_SUCCESS;

	// native contingency data
	KpfaCtgData *ctgData = new KpfaCtgData();

	// outage data
	jint j_notg = GetOutageDataListSize(pEnv, rCtgObj);
	jobject j_otgList = GetOutageDataList(pEnv, rCtgObj);

	if(j_otgList != NULL) {
		for(jint i = 0; i < j_notg; i++) {
			// get the i-th outgate data
			jobject j_otgData = GetOutageDataAt(pEnv, j_otgList, i);
			ctgData->InsertOutageData(GetNativeOutageData(pEnv, j_otgData));
		}
	}
	else if(j_notg != 0) {
		return KPFA_ERROR_JNI_GET_OUTAGE_LIST;
	}

	// create a raw data reader with the given path
	KpfaRawDataReader *dataReader = new KpfaRawDataReader();
	error = ReadRawDataFile(pEnv, dataReader, rRawDataByteArr);
	if(error != KPFA_SUCCESS) {
		delete dataReader;
		delete ctgData;
		return error;
	}

	// return data management
	KpfaRetDataMgmt retDataMgmt;

	// For each raw data management
	KpfaRawDataMgmtList_t::iterator iter1;
	KpfaRawDataMgmtList_t &dataMgmtList = dataReader->GetRawDataMgmts();
	for(iter1 = dataMgmtList.begin(); iter1 != dataMgmtList.end(); iter1++) {
		KpfaRawDataMgmt *rawDataMgmt = *iter1;

		rawDataMgmt->ApplyContingencyData(ctgData);

		// perform a contingency analysis
		error = KpfaPerformPowerflowAnalysis(rawDataMgmt, &retDataMgmt);
		if(error != KPFA_SUCCESS) {
			delete dataReader;
			delete ctgData;
			return error;
		}

		rawDataMgmt->RetrieveFromContingency();
	}

	// For each return data
	KpfaRetDataList_t::iterator iter2;
	KpfaRetDataList_t &retDataList = retDataMgmt.GetRetBusDataList();

	for(iter2 = retDataList.begin(); iter2 != retDataList.end(); iter2++) {
		// copy return data to result data
		if(AddResultData(pEnv, rResMgmtObj, *iter2) == FALSE) {
			error = KPFA_ERROR_JNI_ADD_RESULT_DATA;
			break;
		}
	}

	delete dataReader;
	delete ctgData;
	return error;
}
