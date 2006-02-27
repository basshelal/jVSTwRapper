package jvst.wrapper.communication;


import jvst.wrapper.valueobjects.*;

/**
 *  VST 2.0 Host -> Plug Communication
 *
 *@author     daniel
 *@created    14. Oktober 2003
 */
public interface VSTV20ToPlug extends VSTV10ToPlug {

  //Host canDo constants
  public static final String CANDO_HOST_SEND_VST_EVENTS = "sendVstEvents";
  public static final String CANDO_HOST_SEND_VST_MIDI_EVENT ="sendVstMidiEvent";
  public static final String CANDO_HOST_SEND_VST_TIME_INFO = "sendVstTimeInfo";
  public static final String CANDO_HOST_RECEIVE_VST_Events = "receiveVstEvents";
  public static final String CANDO_HOST_RECEIVE_VST_MIDI_EVENT = "receiveVstMidiEvent";
  public static final String CANDO_HOST_RECEIVE_VST_TIME_INFO = "receiveVstTimeInfo";

  public static final String CANDO_HOST_REPORT_CONNECTION_CHANGES = "reportConnectionChanges";
  public static final String CANDO_HOST_ACCEPT_IO_CHANGES = "acceptIOChanges";
  public static final String CANDO_HOST_SIZE_WINDOW = "sizeWindow";

  public static final String CANDO_HOST_ASYNC_PROCESSING = "asyncProcessing";
  public static final String CANDO_HOST_OFFLINE = "offline";
  public static final String CANDO_HOST_SUPPLY_IDLE = "supplyIdle";
  public static final String CANDO_HOST_SUPPORT_SHELL = "supportShell";	// 'shell' handling via uniqueID as suggested by Waves
  public static final String CANDO_HOST_OPEN_FILE_SELECTOR = "openFileSelector";
  //VST 2.2
  public static final String CANDO_HOST_EDIT_FILE = "editFile";
  public static final String CANDO_HOST_CLOSE_FILE_SELECTOR = "closeFileSelector";
  //VST 2.3
  public static final String CANDO_HOST_START_STOP_PROCESS = "startStopProcess";



  public final static int PLUG_CATEG_UNKNOWN = 0;
  public final static int PLUG_CATEG_EFFECT = 1;
  public final static int PLUG_CATEG_SYNTH = 2;
  public final static int PLUG_CATEG_ANALYSIS = 3;
  public final static int PLUG_CATEG_MASTERING = 4;
  public final static int PLUG_CATEG_SPACIALIZER = 5;     // 'panners'
  public final static int PLUG_CATEG_ROOM_FX = 6;         // delays and reverbs
  public final static int PLUG_CATEG_SURROUND_FX = 7;     // dedicated surround processor
  public final static int PLUG_CATEG_RESTORATION = 8;
  public final static int PLUG_CATEG_OFFLINE_PROCESS = 9;
  public final static int PLUG_CATEG_SHELL = 10;          // plugin which is only a container of plugins.
  public final static int PLUG_CATEG_GENERATOR = 11;


  //********************
  //*** Host -> Plug ***
  //********************

  //public void vendorSpecific();
  //not supported! because of void* parameter

  //public void getIcon();
  //not supported because of our own ui handling!
  //nah! doesnt matter, its deprecated since 2.4



  //vst 2.0 REQUIRED
  //*****************
  public int canDo(String feature);
  public boolean setBypass(boolean value);
  boolean string2Parameter(int index, String value);
  public String getProgramNameIndexed(int category, int index);
  public String getProductString();
  public String getVendorString();
  public int getPlugCategory();


  //vst 2.0 OPTIONAL
  //*****************
  public String getEffectName();
  public int getVendorVersion();

  public boolean canParameterBeAutomated(int index);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public boolean copyProgram(int destination);
  
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public int fxIdle();
  public float getChannelParameter(int channel, int index);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public int getNumCategories();

  public VSTPinProperties getInputProperties(int index);
  public VSTPinProperties getOutputProperties(int index);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public String getErrorText();
  public int getGetTailSize();
  public VSTParameterProperties getParameterProperties(int index);


  public int getVstVersion ();
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public void inputConnected (int index, boolean state);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public void outputConnected (int index, boolean state);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public boolean keysRequired ();


  //The VST offline interface is currently NOT supported
  /*
  public void offlineNotify ();
  public void offlineGetNumPasses ();
  public void offlineGetNumMetaPasses();
  public void offlinePrepare ();
  public void offlineRun ();
  */


  public int processEvents (VSTEvents e);
  public boolean processVariableIo (VSTVariableIO vario);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public int reportCurrentPosition ();
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public float[] reportDestinationBuffer ();
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public void setBlockSizeAndSampleRate(int blockSize, float sampleRate);
  public boolean setSpeakerArrangement (VSTSpeakerArrangement pluginInput, VSTSpeakerArrangement pluginOutput);
}
