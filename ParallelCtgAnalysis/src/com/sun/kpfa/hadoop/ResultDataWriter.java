package com.sun.kpfa.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.RecordWriter;
import org.apache.hadoop.mapreduce.TaskAttemptContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ResultDataWriter extends RecordWriter<IntWritable, DoubleWritable> {
	
	// Logger
	private static final Logger LOG = LoggerFactory.getLogger(ResultDataWriter.class);

	@Override
	public void close(TaskAttemptContext rContext) throws IOException,
			InterruptedException {
		// TODO Auto-generated method stub
	}

	@Override
	public void write(IntWritable rBusId, DoubleWritable rStdev)
			throws IOException, InterruptedException {
		
		if(rStdev.get() != 0) {
			LOG.info("Result Data: Bus ID - " + rBusId.get() + ", Standard Deviation of Vm - " + rStdev.get());
		}
	}
}
