package jvst.wrapper.valueobjects;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */

public class MidiKeyName {

  private int thisProgramIndex; // >= 0. fill struct for this program index.
  private int thisKeyNumber;	// 0 - 127. fill struct for this key number.
  private String keyName = "";  //[64];
  private int reserved = 0;	// zero
  private int flags = 0;	// reserved, none defined yet, zero.



  public int getThisProgramIndex() {return this.thisProgramIndex;}
  public int getThisKeyNumber() {return this.thisKeyNumber;}
  public int getReserved() {return this.reserved;}
  public int getFlags() {return this.flags;}
  public String getKeyName() {return this.keyName;}


  public void setThisProgramIndex(int p) {this.thisProgramIndex = p;}
  public void setThisKeyNumber(int p) {this.thisKeyNumber = p;}
  public void setReserved(int p) {this.reserved = p;}
  public void setFlags(int p) {this.flags = p;}
  public void setKeyname(String n) {this.keyName = n;}

}