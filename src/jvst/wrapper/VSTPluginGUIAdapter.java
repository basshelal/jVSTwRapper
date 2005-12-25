package jvst.wrapper;


import javax.swing.*;
import java.awt.*;

import jvst.wrapper.gui.*;


public abstract class VSTPluginGUIAdapter extends JFrame implements VSTPluginGUI {

  static public boolean libraryOk=false;
  public boolean WindowAttached;
     
  public VSTPluginGUIAdapter() {
    //center window
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    Dimension frameSize = this.getSize();

    if (frameSize.height > frameSize.height) {
      frameSize.height = screenSize.height;
    }
    if (frameSize.width > screenSize.width) {
      frameSize.width = screenSize.width;
    }
    this.setLocation(
        (screenSize.width - frameSize.width) / 2,
        (screenSize.height - frameSize.height) / 2);

    //close behavior
    this.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);

    //IMPORTANT: hide window
    this.hide();
        
    //Prepare attaching window
    WindowAttached=false;
    if(!libraryOk) {
      //Load library for native-awt methods 
      try{
        System.loadLibrary("jawt");
        libraryOk=true;          
      } catch(Exception e){libraryOk=false;}
      catch(Error e){
          //Maybe library is already loaded
          if(e.getMessage().indexOf("already loaded")!=-1) libraryOk=true;
          else libraryOk=false;
      }
    }

    log("\nJAVA GUI Plugin intitialised properly!");
  }
  
  public void undecorate()
  {               
        if((this.WindowAttached)&&(!this.isUndecorated())) {
            javax.swing.JFrame f = new javax.swing.JFrame();
            f.pack();            
            this.dispose();
            this.setUndecorated(true);
            this.pack();
            f.dispose();
            java.awt.Rectangle bounds=this.getBounds();
            bounds.x=0;
            bounds.y=0;
            this.setBounds(bounds);            
        } 
  }

  //*********************************************************************
  public void open() {
    log("GUI open");
    this.show();
    this.toFront();
  }

  public void close() {
    this.log("GUI close");
    this.hide();
  }

  public void destroy() {
    this.log("GUI destroy");
    this.dispose();
  }

  //***********************************************************************
  protected void log(String s) {
    VSTPluginAdapter.log(s);
  }

}
