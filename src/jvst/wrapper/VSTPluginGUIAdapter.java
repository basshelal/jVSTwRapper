package jvst.wrapper;


import javax.swing.*;
import java.awt.*;

import jvst.wrapper.gui.*;


public abstract class VSTPluginGUIAdapter extends JFrame implements VSTPluginGUI {

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

    log("\nJAVA GUI Plugin intitialised properly!");
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
