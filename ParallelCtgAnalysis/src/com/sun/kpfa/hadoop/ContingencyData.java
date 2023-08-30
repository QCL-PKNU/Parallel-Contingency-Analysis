package com.sun.kpfa.hadoop;

import java.io.BufferedReader;
import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.Iterator;
import java.util.Vector;

import org.apache.hadoop.io.WritableComparable;
import org.slf4j.LoggerFactory;
import org.slf4j.Logger;

public class ContingencyData implements WritableComparable<ContingencyData> {
	
	private final String TAG_CTG_BEG = "(Contingency)";
	private final String TAG_CTG_END = "(End of contingency)"; 
	
	// Logger
	private static final Logger LOG = LoggerFactory.getLogger(ContingencyData.class);
	
	// Contingency name
	public String m_rName;
	
	// Contingency status
	private int m_nStatus;
	
	// Outage data list
	private Vector<OutageData> m_rOtgDataList;
	
	// Constructor
	public ContingencyData() {
		m_rOtgDataList = new Vector<OutageData>();
	}
	
	/**
	 * This function will return the name of the contingency.
	 * 
	 * @return contingency name
	 */
	public String getName() {
		return m_rName;
	}
	
	/**
	 * This function indicates whether the contingency is valid or not.
	 * 
	 * @return the status of the contingency
	 */
	public boolean isValid() {
		return (m_nStatus == 1);
	}
	
	/**
	 * This function will read a contingency from the given input stream. 
	 * @throws IOException 
	 * @param rIs input stream
	 * @return true if reading a contingency data is completed
	 */
	public boolean readCtgData(BufferedReader rReader) throws IOException {
		
		boolean ctgflag = false;
		String linebuf = null;
		
		// Find the starting tag of a contingency 
		while((linebuf = rReader.readLine()) != null) {
			if(linebuf.indexOf(TAG_CTG_BEG) >= 0) {
				ctgflag = true;
				break;
			}
		}

		// Check if there is a contingency 
		if(ctgflag == false) {
			return false;
		}
		
		// Parse the contingency information
		m_rOtgDataList.clear();

		while((linebuf = rReader.readLine()) != null) {
			if(linebuf.indexOf(TAG_CTG_END) >= 0) {
				break;
			}
			
			// Contingency name
			if(linebuf.indexOf("name") >= 0) {
				m_rName = linebuf.substring(linebuf.indexOf('=')+1).trim();
			}
			// Contingency status
			else if(linebuf.indexOf("status") >= 0) {				
				m_nStatus = Integer.parseInt(
							linebuf.substring(linebuf.indexOf('=')+1).trim());
			}
			// Outages
			else if(linebuf.indexOf("Outage") >= 0) {
				OutageData otgData = new OutageData();
				if(otgData.parseOtgData(linebuf) == false) {
					LOG.error("Parsing outage is failed: " + linebuf);
					return false;
				}
				m_rOtgDataList.addElement(otgData);
			}
			// Exception handling
			else {
				LOG.error("Invalid string in contingency data: " + linebuf);
				return false;
			}
		}			
		
		return true;
	}
	
	/**
	 * This function will return the list of outages.
	 * 
	 * @return the list of outages
	 */
	public Vector<OutageData> getOutageDataList() {
		return m_rOtgDataList;
	}
	
	/**
	 * This function will return the size of the outage list.
	 * 
	 * @return the size of outage list
	 */
	public int getOutageDataListSize() {
		return m_rOtgDataList.size();
	}
	
	/**
	 * This function will return the outage indicated by the given index.
	 * 
	 * @param nIndex outage index
	 * @return the outage with the given index
	 */
	public OutageData getOutageData(int nIndex) {
		if(nIndex < m_rOtgDataList.size()) {
			return m_rOtgDataList.get(nIndex);
		}
		return null;
	}

	@Override
	public void readFields(DataInput rIn) throws IOException {
		// TODO Auto-generated method stub
	}

	@Override
	public void write(DataOutput rOut) throws IOException {
		// TODO Auto-generated method stub
	}

	@Override
	public int compareTo(ContingencyData rCtgData) {
		return m_rName.compareTo(rCtgData.getName());
	}
	
	public String toString() {
		
		StringBuffer buf = new StringBuffer("Contingency: ");
		buf.append(m_rName);
		buf.append(",");
		buf.append(m_nStatus);
		
		Iterator<OutageData> iter = m_rOtgDataList.iterator();
		while(iter.hasNext()) { 	
			buf.append("\n");
			buf.append((OutageData)iter.next());
		}
		
		return buf.toString();
	}
}
