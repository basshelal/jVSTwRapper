package jvst.wrapper.valueobjects;

/**
 * <p>Title: a generic timestamped event</p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */

public class VSTEvent {
  //VstEvent Types
  public static final int VST_EVENT_MIDI_TYPE = 1;	  // midi event, can be cast as VstMidiEvent (see below)
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public static final int VST_EVENT_AUDIO_TYPE = 2;	  // audio
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public static final int VST_EVENT_VIDEO_TYPE = 3;	  // video
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public static final int VST_EVENT_PARAMETER_TYPE = 4; // parameter
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public static final int VST_EVENT_TRIGGER_TYPE = 5;	  // trigger
  public static final int VST_EVENT_SYSEX_TYPE = 6;	  // midi system exclusive
  // ...etc


  protected int type;		// see enum above
  protected int byteSize;		// of this event, excl. type and byteSize
  protected int deltaFrames;	// sample frames related to the current block start sample position
  protected int flags;		// generic flags, used for midieventflags since 2.4
  protected byte[] data = new byte[16];		// size may vary but is usually 16

  public VSTEvent() {
  }

  public int getType() {
    return this.type;
  }

  public void setType(int type) {
    this.type = type;
  }

  public int getByteSize() {
    return this.byteSize;
  }

  public void setByteSize(int byteSize) {
    this.byteSize = byteSize;
  }

  public int getDeltaFrames() {
    return this.deltaFrames;
  }

  public void setDeltaFrames(int deltaFrames) {
    this.deltaFrames = deltaFrames;
  }

  public int getFlags() {
    return this.flags;
  }

  public void setFlags(int flags) {
    this.flags = flags;
  }

  public byte[] getData() {
    return this.data;
  }

  public void setData(byte[] data) {
    this.data = data;
  }

}