package jvst.examples.jaydlay;


import jvst.wrapper.*;


public class JayDLay extends VSTPluginAdapter {
  private DelayProgram[] programs;
  private float[] buffer;
  private float fDelay, fFeedBack, fOut;
  private int delay;
  private int size;
  private int cursor;
  private int currentProgram;


  public JayDLay() {
    log("Construktor JayDLay() START!");

    this.size = 44100;
    this.cursor = 0;
    this.delay = 0;
    this.buffer = new float[this.size];

    this.programs = new DelayProgram[16];
    for (int i = 0; i < this.programs.length; i++) {
      this.programs[i] = new DelayProgram();
    }
    this.fDelay = this.fFeedBack = 0F;
    this.fOut = 1.0F;

    this.setProgram(0);

    //communicate with the host
    this.setNumInputs(1);// mono input
    this.setNumOutputs(2);// stereo output
    this.hasVu(false);
    this.canProcessReplacing(true);
    this.setUniqueID(9876543);//random unique number registered at steinberg (4 byte)


    log("Construktor JayDLay() INVOKED!");
  }


  /**
   * to avoid that strange sounds appear if playback is restarted after stop.
   */
  public void resume() {
    log("resuming...");
    for (int i = 0; i < this.buffer.length; i++) this.buffer[i] = 0.0F;
  }


  public int canDo(String feature) {
    int ret = JayDLay.CANDO_DONT_KNOW;

    if (feature.equals(JayDLay.PLUG_1_IN_2_OUT)) ret = JayDLay.CANDO_YES;

    log("canDo: " + feature + " = " + ret);
    return ret;
  }

  public boolean setBypass(boolean value) {
    log("setBypass");
    //do not support soft bypass!
    return false;
  }

  public boolean string2Parameter(int index, String value) {
    log("string2parameter");
    boolean ret = false;


    try {
      if (value != null) this.setParameter(index, Float.parseFloat(value));
      ret=true;
    }
    catch(Exception e) {   //ignore
    }

    return ret;
  }

  public String getVendorString() { return "Daniel Martin / http://jvstwrapper.sourceforge.net/"; }

  public int getPlugCategory() {
    log("getPlugCategory");
    return VSTPluginAdapter.PLUG_CATEG_EFFECT;
  }

  public int getProgram() { return this.currentProgram; }

  public int getVendorVersion() {
    log("getVendor Version");
    return 1000;
  }

  public int getNumPrograms() {
    log("getNumPrograms");
    return this.programs.length;
  }

  public int getNumParams() {
    log("getNumParams");
    return DelayProgram.NUM_PARAMS;
  }

  public float getParameter(int index) {
    float v = 0;

    switch (index) {
        case DelayProgram.PARAM_ID_DELAY:
          v = this.fDelay;
          break;
        case DelayProgram.PARAM_ID_FEEDBACK:
          v = this.fFeedBack;
          break;
        case DelayProgram.PARAM_ID_OUT:
          v = this.fOut;
          break;
    }

    return v;
  }

  public String getParameterName(int index) {
    String label = "";

    switch (index) {
        case DelayProgram.PARAM_ID_DELAY:
          label = "Delay";
          break;
        case DelayProgram.PARAM_ID_FEEDBACK:
          label = "FeedBack";
          break;
        case DelayProgram.PARAM_ID_OUT:
          label = "Volume";
          break;
    }

    return label;
  }

  public String getProductString() {
    return "JayDLay 1.0";
  }

  public void setProgram(int index) {
    DelayProgram dp = this.programs[index];

    this.currentProgram = index;

    this.setParameter(DelayProgram.PARAM_ID_DELAY, dp.getDelay());
    this.setParameter(DelayProgram.PARAM_ID_FEEDBACK, dp.getFeedback());
    this.setParameter(DelayProgram.PARAM_ID_OUT, dp.getOut());
  }

  public void setParameter(int index, float value) {
    DelayProgram dp = this.programs[this.currentProgram];

    switch (index) {
        case DelayProgram.PARAM_ID_DELAY:
          this.setDelay(value);
          break;
        case DelayProgram.PARAM_ID_FEEDBACK:
          this.fFeedBack = value;
          dp.setFeedback(value);
          break;
        case DelayProgram.PARAM_ID_OUT:
          this.fOut = value;
          dp.setOut(value);
          break;
    }
  }

  public String getEffectName() {
    return "JayDLay";
  }

  public String getParameterLabel(int index) {
    String label = "";

    switch (index) {
        case DelayProgram.PARAM_ID_DELAY:
          label = "samples";
          break;
        case DelayProgram.PARAM_ID_FEEDBACK:
          label = "amount";
          break;
        case DelayProgram.PARAM_ID_OUT:
          label = "dB";
          break;
    }

    return label;
  }

  public void setProgramName(String name) {
    this.programs[this.currentProgram].setName(name);
  }

  public String getParameterDisplay(int index) {
    String text = "";

    switch (index) {
        case DelayProgram.PARAM_ID_DELAY:
          text = Integer.toString(this.delay);
          break;
        case DelayProgram.PARAM_ID_FEEDBACK:
          text = this.formatFloatParam(this.fFeedBack);
          break;
        case DelayProgram.PARAM_ID_OUT:
          text = this.dbToString(this.fOut);
          break;
    }

    return text;
  }

  public String getProgramName() {
    String name;

    if (this.programs[this.currentProgram].getName().equals("Init")) {
      name = this.programs[this.currentProgram].getName() + " " + (this.currentProgram + 1);
    }
    else {
      name = this.programs[this.currentProgram].getName();
    }

    return name;
  }

  public String getProgramNameIndexed(int category, int index) {
    log("getprogrammbnameindexed");
    String text = "";
    if (index < this.programs.length) text = this.programs[index].getName();
    if ("Init".equals(text)) text = text + " " + index;

    return text;
  }

  //processReplacing is REPLACING the calculated floats to the output
  public void processReplacing(float[][] inputs, float[][] outputs, int sampleFrames) {
    float[] in = inputs[0];
    float[] out1 = outputs[0];
    float[] out2 = outputs[1];

    for (int i = 0; i < sampleFrames; i++) {
      float x = in[i];
      float y = this.buffer[this.cursor] * this.fOut;

      this.buffer[this.cursor++] = x + y * this.fFeedBack;
      if (this.cursor >= this.delay) this.cursor = 0;

      out1[i] = y; //IMPORTANT!!! Here is the difference between process() [accumulation]
      out2[i] = y; //and processReplacing() [replacement]
    }
  }

  //process is ACCUMULATING the calculated floats to the output
  public void process(float[][] inputs, float[][] outputs, int sampleFrames) {
    float[] in = inputs[0];
    float[] out1 = outputs[0];
    float[] out2 = outputs[1];

    for (int i = 0; i < sampleFrames; i++) {
      float x = in[i];
      float y = this.buffer[this.cursor] * this.fOut;

      this.buffer[this.cursor++] = x + y * this.fFeedBack;
      if (this.cursor >= this.delay) this.cursor = 0;

      out1[i] += y; //IMPORTANT!!! Here is the difference between process() [accumulation]
      out2[i] += y; //and processReplacing() [replacement]
    }
  }




  private void setDelay(float fdelay) {
    this.fDelay = fdelay;
    this.programs[this.currentProgram].setDelay(fdelay);
    this.cursor = 0;
    this.delay = (int)(fdelay * (float)(size - 1));
  }
}


/**
 * Helper Class (VO) for the support of different presets for the plug
 * if you are using no gui (comment PluginUIClass out in jVSTwRapper.ini)
 * @author dm
 * @version 1.0
 */
class DelayProgram {
  public final static int PARAM_ID_DELAY = 0;
  public final static int PARAM_ID_FEEDBACK = 1;
  public final static int PARAM_ID_OUT = 2;

  public final static int NUM_PARAMS = PARAM_ID_OUT + 1;

  private String name = "Init";
  private float delay = 0.5F;
  private float feedback = 0.5F;
  private float out = 1F;



  public String getName() { return this.name; }
  public void setName(String name) { this.name = name; }

  public float getDelay() { return this.delay; }
  public void setDelay(float delay) { this.delay = delay; }

  public float getFeedback() { return this.feedback; }
  public void setFeedback(float feedback) { this.feedback = feedback; }

  public float getOut() { return this.out; }
  public void setOut(float out) { this.out = out; }
}