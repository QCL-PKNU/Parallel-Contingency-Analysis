package com.sun.kpfa.hadoop;

import java.net.URI;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

public class ParallelCtgAnalysis {
	
	public static void main(String[] args) throws Exception{
		
		Configuration conf = new Configuration();
		Job job = Job.getInstance(conf, "Parallel Contingency Analysis");
		job.addCacheFile(new Path("./libParallelCtgAnalysisJni.so").toUri());
			
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
	
		job.setOutputKeyClass(IntWritable.class);
		job.setOutputValueClass(DoubleWritable.class);

		job.setMapperClass(ParallelCtgAnalysisMapper.class);
		job.setReducerClass(ParallelCtgAnalysisReducer.class);

		job.setInputFormatClass(ContingencyInputFormat.class);
		job.setOutputFormatClass(ResultOutputFormat.class);
	
		job.setJarByClass(ParallelCtgAnalysis.class);
		job.waitForCompletion(true);
		
		System.out.println("Job.isSuccessful " + job.isSuccessful());
	}
}
