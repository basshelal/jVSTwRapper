package jvst.wrapper.gui;


import jvst.wrapper.*;



/**
 *  Description of the Interface
 *
 *@author     daniel
 *@created    14. Oktober 2003
 */
public interface VSTPluginGUI {
  /**
   *  Description of the Method
   *
   *@param  effect  Description of the Parameter
   */
  public void init(VSTPluginAdapter plugin);


  /**
   *  Description of the Method
   */
  public void open();


  /**
   *  Description of the Method
   */
  public void close();


  /**
   *  Description of the Method
   */
  public void destroy();
}
