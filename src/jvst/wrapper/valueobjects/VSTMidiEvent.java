package jvst.wrapper.valueobjects;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */

public class VSTMidiEvent extends VSTEvent {
  protected int noteLength;	// (in sample frames) of entire note, if available, else 0
  protected int noteOffset;	// offset into note from note start if available, else 0

  //private byte midiData[4];	// 1 thru 3 midi bytes; midiData[3] is reserved (zero)
  //ATTENTION! use data[] instead!
  protected byte detune;		// -64 to +63 cents; for scales other than 'well-tempered' ('microtuning')
  protected byte noteOffVelocity;
  protected byte reserved1;	// zero
  protected byte reserved2;	// zero



  public VSTMidiEvent() {
    //private byte midiData[4];	// 1 thru 3 midi bytes; midiData[3] is reserved (zero)
    //--> correct the data size...
    this.data = new byte[4];
  }

  public int getNoteLength() {
    return this.noteLength;
  }

  public void setNoteLength(int noteLength) {
    this.noteLength = noteLength;
  }

  public int getNoteOffset() {
    return this.noteOffset;
  }

  public void setNoteOffset(int noteOffset) {
    this.noteOffset = noteOffset;
  }

  public byte getDetune() {
    return this.detune;
  }

  public void setDetune(byte detune) {
    this.detune = detune;
  }

  public byte getNoteOffVelocity() {
    return this.noteOffVelocity;
  }

  public void setNoteOffVelocity(byte noteOffVelocity) {
    this.noteOffVelocity = noteOffVelocity;
  }

  public byte getReserved1() {
    return this.reserved1;
  }

  public void setReserved1(byte reserved1) {
    this.reserved1 = reserved1;
  }

  public byte getReserved2() {
    return this.reserved2;
  }

  public void setReserved2(byte reserved2) {
    this.reserved2 = reserved2;
  }


}