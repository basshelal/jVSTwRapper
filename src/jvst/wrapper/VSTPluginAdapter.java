package jvst.wrapper;


import java.text.DecimalFormat;

import jvst.wrapper.communication.VSTV23ToHost;
import jvst.wrapper.communication.VSTV23ToPlug;
import jvst.wrapper.valueobjects.MidiKeyName;
import jvst.wrapper.valueobjects.MidiProgramCategory;
import jvst.wrapper.valueobjects.MidiProgramName;
import jvst.wrapper.valueobjects.VSTEvents;
import jvst.wrapper.valueobjects.VSTParameterProperties;
import jvst.wrapper.valueobjects.VSTPinProperties;
import jvst.wrapper.valueobjects.VSTSpeakerArrangement;
import jvst.wrapper.valueobjects.VSTVariableIO;

/**
 *  Adapter to ignore VST 1.0, VST 2.0, VST 2.1, VST 2.2, VST 2.3
 *  OPTIONAL methods --> return defaults
 *  and some utility methods (number conversions...)
 *
 *@author     daniel
 *@created    14. Oktober 2003
 */
public abstract class VSTPluginAdapter extends VSTV23ToHost implements VSTV23ToPlug {


  //Constants
  //************************************
  public static final int PAN_LAW_LINEAR = 0;	  // L = pan * M; R = (1 - pan) * M;
  public static final int PAN_LAW_EQUAL_POWER = 1;// L = pow (pan, 0.5) * M; R = pow ((1 - pan), 0.5) * M;



  //provide defaults for vst 1.0 OPTIONAL methods
  //*************************************
  public void open() { }
  public void close() { }
  public void suspend() { }
  public void resume() { }
  public float getVu() {return 0F;}
  public int getChunk(float[][] data, boolean isPreset) {return 0;}
  public int setChunk(float data[], int byteSize, boolean isPreset) {return 0;}
  public void setBlockSize(int blockSize) { }
  public void setSampleRate(float sampleRate) { }


  //provide defaults for vst 2.0 OPTIONAL methods
  //********************************
  public String getEffectName() {return "nix";}
  public int getVendorVersion() {return 1000;}
  public boolean canParameterBeAutomated(int index) {return true;}
  public boolean copyProgram(int destination) {return false;}
  public int fxIdle() {return 0;}
  public float getChannelParameter(int channel, int index) {return 0.F;}
  public int getNumCategories() {return 1;}

  public VSTPinProperties getInputProperties(int index) {return null;}
  public VSTPinProperties getOutputProperties(int index) {return null;}

  public String getErrorText() {return null;}
  public int getGetTailSize() {return 0;}
  public VSTParameterProperties getParameterProperties(int index) {return null;}

  public int getVstVersion () {return 2200;} //TODO: return 2300 when implemented!
  public void inputConnected (int index, boolean state) {}
  public void outputConnected (int index, boolean state) {}
  public boolean keysRequired () {return false;}

  public int processEvents (VSTEvents e) {return 0;}
  public boolean processVariableIo (VSTVariableIO vario) {return false;}
  public int reportCurrentPosition () {return 0;}
  public float[] reportDestinationBuffer () {return new float[]{0.F};}
  public void setBlockSizeAndSampleRate(int blockSize, float sampleRate) {}

  public boolean setSpeakerArrangement (VSTSpeakerArrangement pluginInput, VSTSpeakerArrangement pluginOutput) {
    return false;
  }


  //provide defaults for vst 2.1 OPTIONAL methods
  //********************************
  public int getMidiProgramName (int channel, MidiProgramName midiProgramName) { return 0; }
  public int getCurrentMidiProgram(int channel, MidiProgramName currentProgram) { return -1; }
  public int getMidiProgramCategory(int channel,MidiProgramCategory category) { return 0; }
  public boolean hasMidiProgramsChanged(int channel) { return false; }
  public boolean getMidiKeyName(int channel, MidiKeyName keyName) { return false; }
  public boolean beginSetProgram() { return false;}
  public boolean endSetProgram() { return false;}


  //provide defaults for vst 2.3 OPTIONAL methods
  //********************************
  public int setTotalSampleToProcess (int value) { return value; }
  public int getNextShellPlugin(String name) { return 0; }
  public int startProcess() { return 0;}
  public int stopProcess() { return 0;}





  //utility
  //***********************************************************************

  protected String dbToString(float value) {
    String ret;
    DecimalFormat form = new DecimalFormat("#0.0");

    if (value <= 0F) ret = "-oo";
    else ret = form.format(20.0F * (float) (Math.log(value) / Math.log(10)) ); //log10(value)!

    return ret;
  }

  protected String hzToString(float value, int sampleRate) {
    String ret;
    DecimalFormat form = new DecimalFormat("#0");

    if (value != 0F) ret = form.format((float)sampleRate / value);
    else ret = "0";

    return ret;
  }

  protected String msToString(float value, int sampleRate) {
    String ret;
    DecimalFormat form = new DecimalFormat("#0");

    if (sampleRate!=0) ret = form.format(value * 1000.0F / (float)sampleRate);
    else ret = "0";

    return ret;
  }

  protected String formatFloatParam(float value) {
    DecimalFormat form = new DecimalFormat("#0.0#");
    return form.format(value);
  }  
  
}
