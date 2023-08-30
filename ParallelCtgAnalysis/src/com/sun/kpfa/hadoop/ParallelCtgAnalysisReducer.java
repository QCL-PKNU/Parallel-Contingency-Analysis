package com.sun.kpfa.hadoop;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Reducer;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ParallelCtgAnalysisReducer extends Reducer<IntWritable, DoubleWritable, IntWritable, DoubleWritable> {
	
	// Logger
	private static final Logger LOG = LoggerFactory.getLogger(ParallelCtgAnalysisReducer.class);
	
	@Override
	protected void reduce(IntWritable rBusId, Iterable<DoubleWritable> rVmList, Context rContext)
			throws IOException, InterruptedException {	
		
		// a list for caching the vm values of the iterable rVmList
		ArrayList<Double> vmListCache = new ArrayList<Double>();
		
		// Calculate the standard deviation of the bus with 'nBusId' 
		int cnt = 0;
		double avg = 0, stdev = 0;
		Iterator<DoubleWritable> vmiter;
		
		// calculate the average with the given list of Vm
		for(vmiter = rVmList.iterator(); vmiter.hasNext(); cnt++) {
			double tmp = vmiter.next().get();
			avg += tmp;
			vmListCache.add(tmp);
		}
		
		avg /= cnt;
				
		// calculate the standard deviation with the given list of Vm and 
		// the pre-calculated average.
		for(int i = 0; i < vmListCache.size(); i++) {
			stdev += Math.pow((vmListCache.get(i) - avg), 2.0);
		}

		stdev = Math.sqrt(stdev / cnt);
		
		/*
		LOG.info("ParallelCtgAnalysisReducer - # of reduce: " + cnt);		
		LOG.info("ParallelCtgAnalysisReducer - average: " + avg);		
		LOG.info("ParallelCtgAnalysisReducer - standard deviation: " + stdev);		
		*/
		
		rContext.write(rBusId, new DoubleWritable(stdev));
	}	
}
