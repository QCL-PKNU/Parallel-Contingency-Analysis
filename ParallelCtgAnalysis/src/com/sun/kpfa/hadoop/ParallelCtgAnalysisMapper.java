package com.sun.kpfa.hadoop;

import java.io.IOException;
import java.util.Vector;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Mapper;
import org.slf4j.LoggerFactory;
import org.slf4j.Logger;

public class ParallelCtgAnalysisMapper extends
		Mapper<ContingencyData, NullWritable, IntWritable, DoubleWritable> {

	// JNI library initialization
	static {
		System.loadLibrary("ParallelCtgAnalysisJni");
	}
	
	// file read buffer size
	private static int MAX_BUF_SIZE = 1024;
	
	// Bus ID
	private IntWritable m_rBusId = new IntWritable();
	
	// Voltage magnitude
	private DoubleWritable m_nBusVm = new DoubleWritable();
	
	// Logger
	private static final Logger LOG = LoggerFactory.getLogger(ParallelCtgAnalysisMapper.class);
	
	/**
	 * This function will return the bytes of the file with the given path.
	 * 
	 * @param rRawFilePath raw file path
	 * @return byte array
	 * @throws IOException 
	 */
	private byte[] readRawDataBytes(String rRawFilePath) throws IOException {
		
		Path path = new Path(rRawFilePath);
		FileSystem fsys = FileSystem.get(new Configuration());
		FileStatus status = fsys.getFileStatus(path);
		
		// file length check
		long nbytes = status.getLen();
		if(nbytes > (long)Integer.MAX_VALUE) {
			LOG.error("Too large raw file size: " + nbytes);
			return null;
		}
		
		// read all of the raw data bytes
		FSDataInputStream fis = fsys.open(path);
		byte[] rawDataBytes = new byte[(int)nbytes];	
		
		for(int offset = 0; offset < nbytes; ) {
			
			int nread = fis.read(rawDataBytes, offset, MAX_BUF_SIZE);

			if(nread < 0) {
				break;
			}
			
			offset += nread;
		}
		
		fis.close();
		
		return rawDataBytes;
	}
	
	@Override
	protected void map(ContingencyData rCtgData, NullWritable rNullValue, Context rContext)
						throws IOException, InterruptedException {

		// Result data management 
		ResultDataMgmt resDataMgmt = new ResultDataMgmt();

		// JNI interface for powerflow analysis 
		PowerflowAnalysisJni pfa = new PowerflowAnalysisJni();
		
		// raw data bytes
		byte[] rawDataBytes = readRawDataBytes("./input2.raw");
		//byte[] rawDataBytes = readRawDataBytes("./input1.raw");
		if(rawDataBytes == null) {
			LOG.error("Raw data bytes read error");
			return;
		}
		
		int error = pfa.PerformCtgAnalysis(rCtgData, rawDataBytes, resDataMgmt);
		if(error != 0) {
			LOG.error("Contingency analysis error: " + error);
			return;
		}
		
		// Return pairs of new key and value
		Vector<ResultData> resultDataList = resDataMgmt.getResultDataList();
		
		for(int i = 0; i < resultDataList.size(); i++) {
			
			// i-th result data
			ResultData resultData = resultDataList.get(i);

			// just for debugging
			/*
			if(i == 0) {
				LOG.info("I: " + i + ", ID: " + resultData.m_nBusId + ", VM: " + resultData.m_nBusVm);
			}
			*/
			
			// <Bus ID, Voltage magnitude>
			m_rBusId.set(resultData.m_nBusId);
			m_nBusVm.set(resultData.m_nBusVm);
			
			// Write the key, value pairs to the context
			rContext.write(m_rBusId, m_nBusVm);
		}
	}
}
