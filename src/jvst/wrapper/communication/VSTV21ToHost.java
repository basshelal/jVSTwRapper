package jvst.wrapper.communication;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */

public abstract class VSTV21ToHost extends VSTV20ToHost {

  //********************
  //*** Plug -> Host ***
  //********************

  //vst 2.1 REQUIRED
  //****************
  //Nothing


  //vst 2.1 OPTIONAL
  //****************

  public native boolean beginEdit(int index);
  // To be called before a setParameterAutomated with mouse move (one per Mouse Down)

  public native boolean endEdit(int index);
  // To be called after a setParameterAutomated (on Mouse Up)


  //public boolean openFileSelector(VstFileSelect * ptr);
  // Open a Host File selector (see aeffectx.h for VstFileSelect definition)
  //NOT SUPPORTED!
}