package jvst.wrapper.valueobjects;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */

public class MidiProgramName {
  public static final int MIDI_IS_OMNI = 1;
  // default is multi. for omni mode, channel 0
  // is used for inquiries and program changes


  private int thisProgramIndex;		// >= 0. fill struct for this program index.
  private String name = "";             //[64];
  private byte midiProgram = -1;	// -1:off, 0-127
  private byte midiBankMsb = -1;        // -1:off, 0-127
  private byte midiBankLsb = -1;        // -1:off, 0-127
  private byte reserved = 0;		// zero
  private int parentCategoryIndex = -1;	// -1:no parent category
  private int flags = 0;		// omni etc, see above



  public int getThisProgramIndex() {return this.thisProgramIndex;}
  public int getParentCategoryIndex() {return this.parentCategoryIndex;}
  public int getFlags() {return this.flags;}
  public String getName() {return this.name;}
  public byte getReserved() {return this.reserved;}
  public byte getMidiProgram() {return this.midiProgram;}
  public byte getMidiBankMsb() {return this.getMidiBankMsb();}
  public byte getMidiBankLsb() {return this.getMidiBankLsb();}


  public void setReserved(byte p) {this.reserved = p;}
  public void setThisProgramIndex(int p) {this.thisProgramIndex = p;}
  public void setParentCategoryIndex(int p) {this.parentCategoryIndex = p;}
  public void setFlags(int p) {this.flags = p;}
  public void setName(String p) {this.name = p;}
  public void setMidiProgram(byte p) {this.midiProgram = p;}
  public void setMidiBankMsb(byte p) {this.midiBankMsb = p;}
  public void setMidiBankLsb(byte p) {this.midiBankLsb = p;}

}