package jvst.wrapper.valueobjects;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */


public class VSTVariableIO {

  private float[][] inputs = new float[1][1];
  private float[][] outputs = new float[1][1];
  private int numSamplesInput;
  private int numSamplesOutput;
  private int numSamplesInputProcessed;
  private int numSamplesOutputProcessed;

  public VSTVariableIO() {
  }

  public float[][] getInputs() {
    return this.inputs;
  }

  public void setInputs(float[][] inputs) {
    this.inputs = inputs;
  }

  public float[][] getOutputs() {
    return this.outputs;
  }

  public void setOutputs(float[][] outputs) {
    this.outputs = outputs;
  }

  public int getNumSamplesInput() {
    return this.numSamplesInput;
  }

  public void setNumSamplesInput(int numSamplesInput) {
    this.numSamplesInput = numSamplesInput;
  }

  public int getNumSamplesOutput() {
    return this.numSamplesOutput;
  }

  public void setNumSamplesOutput(int numSamplesOutput) {
    this.numSamplesOutput = numSamplesOutput;
  }

  public int getNumSamplesInputProcessed() {
    return this.numSamplesInputProcessed;
  }

  public void setNumSamplesInputProcessed(int numSamplesInputProcessed) {
    this.numSamplesInputProcessed = numSamplesInputProcessed;
  }

  public int getNumSamplesOutputProcessed() {
    return this.numSamplesOutputProcessed;
  }

  public void setNumSamplesOutputProcessed(int numSamplesOutputProcessed) {
    this.numSamplesOutputProcessed = numSamplesOutputProcessed;
  }

}