package jvst.wrapper.communication;


public interface VSTV24ToPlug extends VSTV23ToPlug {

  public boolean setProcessPrecision (int precision);	///< Set floating-point precision used for processing (32 or 64 bit)

  public int getNumMidiInputChannels();		///< Return number of MIDI input channels to host
  public int getNumMidiOutputChannels();	///< Return number of MIDI output channels to host

  public void processDoubleReplacing (double[][] inputs, double[][] outputs, int sampleFrames); ///< Process 64 bit (double precision) floats (always in a resume state) \sa processReplacing
}