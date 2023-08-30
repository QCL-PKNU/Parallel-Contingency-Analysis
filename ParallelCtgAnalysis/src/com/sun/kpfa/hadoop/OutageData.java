package com.sun.kpfa.hadoop;

import java.util.StringTokenizer;
import org.slf4j.LoggerFactory;
import org.slf4j.Logger;

public class OutageData {
	
	public static final int OTG_BUS = 0;
	public static final int OTG_GENERATOR = 1;
	public static final int OTG_BRANCH = 2;
	public static final int OTG_TRANSFORMER = 3;

	private final String TAG_OTG_BUS = "bus";
	private final String TAG_OTG_GENERATOR = "generator";
	private final String TAG_OTG_BRANCH = "branch";
	private final String TAG_OTG_TRANSFORMER = "transformer";
	
	// Logger
	private static final Logger LOG = LoggerFactory.getLogger(OutageData.class);

	// Outage data type
	private int m_nDataType;
	
	// Bus number (I, J, K)
	private int m_nI;
	private int m_nJ;
	private int m_nK;
	
	// Circuit ID
	private int m_nCkt;
	
	public OutageData() {
		
		m_nDataType = 0;
		m_nI = m_nJ = m_nK = 0;
		m_nCkt = 0;
	}
	
	/**
	 * This function will return the 1st bus ID.
	 * 
	 * @return 1st bus ID
	 */
	public int getI() { return m_nI; }

	/**
	 * This function will return the 2nd bus ID.
	 * 
	 * @return 2nd bus ID
	 */
	public int getJ() { return m_nJ; }

	/**
	 * This function will return the 3rd bus ID.
	 * 
	 * @return 3rd bus ID
	 */
	public int getK() { return m_nK; }

	/**
	 * This function will return Ckt value.
	 * 
	 * @return Ckt value
	 */
	public int getCkt() { return m_nCkt; }
	
	/**
	 * This function will return the outage type.
	 * 
	 * @return outage type
	 */
	public int getDataType() { return m_nDataType; }
	
	/**
	 * This function will be used to parse a string to an outage.
	 * 
	 * @param rStr input string
	 * @return return true if parsing the string completes without any error.
	 */
	public boolean parseOtgData(String rStr) {
		
		StringTokenizer tok = null;
		
		if(rStr.indexOf(TAG_OTG_BUS) >= 0) {
			tok = new StringTokenizer(rStr.substring(
					rStr.indexOf('=')+1), ",");
			// ID
			m_nI = Integer.parseInt(tok.nextToken().trim());
			
			// Data Type
			m_nDataType = OTG_BUS;
		}
		else if(rStr.indexOf(TAG_OTG_GENERATOR) >= 0) {
			tok = new StringTokenizer(rStr.substring(
					  rStr.indexOf('=')+1), ",");
			
			// ID
			m_nI = Integer.parseInt(tok.nextToken().trim());
			
			// Ckt
			m_nCkt = Integer.parseInt(tok.nextToken().replace('\'', ' ').trim());
			
			// Data Type
			m_nDataType = OTG_GENERATOR;
		}
		else if(rStr.indexOf(TAG_OTG_BRANCH) >= 0) {
			tok = new StringTokenizer(rStr.substring(
					  rStr.indexOf('=')+1), ",");
			
			// ID
			m_nI = Integer.parseInt(tok.nextToken().trim());
			m_nJ = Integer.parseInt(tok.nextToken().trim());
			
			// Ckt
			m_nCkt = Integer.parseInt(tok.nextToken().replace('\'', ' ').trim());
			
			// Data Type
			m_nDataType = OTG_BRANCH;
		}
		else if(rStr.indexOf(TAG_OTG_TRANSFORMER) >= 0) {
			tok = new StringTokenizer(rStr.substring(
					  rStr.indexOf('=')+1), ",");
			
			// ID
			m_nI = Integer.parseInt(tok.nextToken().trim());
			m_nJ = Integer.parseInt(tok.nextToken().trim());
			m_nK = Integer.parseInt(tok.nextToken().trim());
			
			// Ckt
			m_nCkt = Integer.parseInt(tok.nextToken().replace('\'', ' ').trim());
			
			// Data Type
			m_nDataType = OTG_TRANSFORMER;
		}
		else {
			LOG.error("Not supported outage: " + rStr);
			return false;
		}
				
		return true;
	}
	
	public String toString() {
		
		StringBuffer buf = new StringBuffer("Outage: ");
		
		// Data type
		switch(m_nDataType) {
			case OTG_BUS: 			buf.append(TAG_OTG_BUS); 			break;
			case OTG_GENERATOR: 	buf.append(TAG_OTG_GENERATOR);		break;
			case OTG_BRANCH:		buf.append(TAG_OTG_BRANCH);			break;
			case OTG_TRANSFORMER:	buf.append(TAG_OTG_TRANSFORMER);	break;
			default:				buf.append("unknown");				break;
		}
		
		// I, J, K, CKT
		buf.append(',');
		buf.append(m_nI);
		buf.append(',');
		buf.append(m_nJ);
		buf.append(',');
		buf.append(m_nK);
		buf.append(',');
		buf.append(m_nCkt);
		
		return buf.toString();
	}
}
