package com.sun.kpfa.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.InputSplit;
import org.apache.hadoop.mapreduce.RecordReader;
import org.apache.hadoop.mapreduce.TaskAttemptContext;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.slf4j.LoggerFactory;
import org.slf4j.Logger;

public class ContingencyInputFormat extends FileInputFormat<ContingencyData, NullWritable> {
	
	private static Logger LOG = LoggerFactory.getLogger(ContingencyInputFormat.class);

	@Override
	public RecordReader<ContingencyData, NullWritable> createRecordReader(
			InputSplit rSplit, TaskAttemptContext rContext) throws IOException,
			InterruptedException {
		return new ContingencyDataReader();
	}
}
