package jvst.wrapper.communication;


/**
 *  VST 1.0 Host -> Plug Communication
 *
 *@author     daniel
 *@created    14. Oktober 2003
 */
public interface VSTV10ToPlug {

  //not supported
  //virtual long dispatcher (long opCode, long index, long value, void *ptr, float opt);


  //VST 1.0 REQUIRED
  //*****************
  public void setParameter(int index, float value);
  public float getParameter(int index);
  public void process(float[][] inputs, float[][] outputs, int sampleFrames);
  public void processReplacing(float[][] inputs, float[][] outputs, int sampleFrames);
  public int getProgram();
  public void setProgram(int index);
  public void setProgramName(String name);
  public String getProgramName();
  public String getParameterName(int index);
  public String getParameterDisplay(int index);
  public String getParameterLabel(int index);


  //VST 1.0 OPTIONAL
  //*****************
  public void open();
  public void close();
  public void suspend();
  public void resume();
  public float getVu();
  public int getChunk(float data[][], boolean isPreset);
  public int setChunk(float data[], int byteSize, boolean isPreset);
  public void setBlockSize(int blockSize);
  public void setSampleRate(float sampleRate);


  //custom enhancements by dm
  //**************************

  /**
   *  because setting this param in constructor is bad design!
   */
  public int getNumPrograms();


  /**
   *  because setting this param in constructor is bad design!
   */
  public int getNumParams();

}
