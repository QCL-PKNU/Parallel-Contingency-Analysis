package com.sun.kpfa.hadoop;

public class PowerflowAnalysisJni {
	
	// Native interface for performing the powerflow analysis
	public native int PerformCtgAnalysis(ContingencyData rCtgData, byte[] rRawDataBytes, ResultDataMgmt rDataMgmt);
}
