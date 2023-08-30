package com.sun.kpfa.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.RecordWriter;
import org.apache.hadoop.mapreduce.TaskAttemptContext;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ResultOutputFormat extends FileOutputFormat<IntWritable, DoubleWritable>  {

	private static Logger LOG = LoggerFactory.getLogger(ResultOutputFormat.class);

	@Override
	public RecordWriter<IntWritable, DoubleWritable> getRecordWriter(
			TaskAttemptContext rContext) throws IOException, InterruptedException {
		return new ResultDataWriter();
	}
}
