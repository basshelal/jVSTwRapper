package jvst.wrapper.communication;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */




public interface VSTV23ToPlug extends VSTV22ToPlug {

  public int setTotalSampleToProcess(int value);

  //name is export parameter!
  public int getNextShellPlugin(String name);

  public int startProcess();

  public int stopProcess ();

}