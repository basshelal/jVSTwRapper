package jvst.examples.jayvstxsynth;

import javax.swing.SwingUtilities;
import javax.swing.UIManager;




public class JayVSTxSynthNapkinGUI extends JayVSTxSynthGUI {
 
  public JayVSTxSynthNapkinGUI() throws Exception {
  	//throws exception that the user gets a message box with the 
  	//stack trace if something fails in the constructor here...
  	
  	this.setSize(380, 290);
  	
  	//use the napkin look-and-feel
	UIManager.setLookAndFeel("napkin.NapkinLookAndFeel");
    SwingUtilities.updateComponentTreeUI(this);
  }

}
