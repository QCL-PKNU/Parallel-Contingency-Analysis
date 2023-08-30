package com.sun.kpfa.hadoop;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.InputSplit;
import org.apache.hadoop.mapreduce.RecordReader;
import org.apache.hadoop.mapreduce.TaskAttemptContext;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;

public class ContingencyDataReader extends RecordReader<ContingencyData, NullWritable> {
	
	// current contingency data
	private ContingencyData m_rCtgData;
	
	// buffered reader for reading the contingencies
	private BufferedReader m_rCtgDataReader;

	public ContingencyDataReader() {
		m_rCtgData = null;
		m_rCtgDataReader = null;
	}

	@Override
	public void close() throws IOException {
		m_rCtgDataReader.close();
	}

	@Override
	public ContingencyData getCurrentKey() throws IOException,
			InterruptedException {
		return m_rCtgData;
	}

	@Override
	public NullWritable getCurrentValue() throws IOException,
			InterruptedException {
		return NullWritable.get();
	}

	@Override
	public float getProgress() throws IOException, InterruptedException {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void initialize(InputSplit rGenericSplit, TaskAttemptContext rContext)
			throws IOException, InterruptedException {

		FileSplit split = (FileSplit)rGenericSplit;
		
		// Open the file and seek the start of the split
		FileSystem fs = FileSystem.get(rContext.getConfiguration());
		FSDataInputStream fin = fs.open(split.getPath());	
		fin.seek(0);

		// Open a buffered reader with the input stream of the split 
		m_rCtgDataReader = new BufferedReader(new InputStreamReader(fin));
	}

	@Override
	public boolean nextKeyValue() throws IOException, InterruptedException {

		// Read a contingency and return the result
		m_rCtgData = new ContingencyData();	
		return m_rCtgData.readCtgData(m_rCtgDataReader);
	}
}
