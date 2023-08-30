package com.sun.kpfa.hadoop;

import java.util.Vector;

// Result data class
class ResultData {	
	// bus ID
	public int m_nBusId;
	// voltage magnitude
	public double m_nBusVm;
	
	public ResultData(int nBusId, double nBusVm) {
		m_nBusId = nBusId;
		m_nBusVm = nBusVm;
	}
	
	public String toString() {
		return "ID: " + m_nBusId + ", VM: " + m_nBusVm;
	}
}

// Result data class
public class ResultDataMgmt {

	// result data list
	private Vector<ResultData> m_rDataList;
	
	public ResultDataMgmt() {
		m_rDataList = new Vector<ResultData>();
		m_rDataList.clear();
	}
	
	/**
	 * This function will be used to add a new result data to the list.
	 * 
	 * @param rData a new result data to be appended
	 */
	public void addResultData(ResultData rData) {
		m_rDataList.addElement(rData);
	}
	
	/**
	 * This function will be used to add a new pair of bus ID and voltage to the list.
	 * 
	 * @param nBusId Bus ID
	 * @param nBusVm Voltage magnitude
	 */
	public void addResultData(int nBusId, double nBusVm) {
		m_rDataList.addElement(new ResultData(nBusId, nBusVm));
	}	
	
	/**
	 * This function will return the list of result data
	 * 
	 * @return the list of result data
	 */
	public Vector<ResultData> getResultDataList() {
		return m_rDataList;
	}
}
