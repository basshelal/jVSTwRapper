/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jvst.examples.javafx.fxdelay;

import java.awt.BorderLayout;
import javax.swing.JComponent;
import javax.swing.JFrame;
import jvst.examples.javafx.JXScene;
import jvst.examples.javafx.SceneToJComponent;
import jvst.wrapper.VSTPluginAdapter;
import jvst.wrapper.gui.VSTPluginGUIRunner;

import jvst.wrapper.VSTPluginGUIAdapter;


/**
 *
 * @author nix
 *
 * TODO:
 *  - tabs / other fx samples (clock, stopwatch, game)
 * 
 */
public class FXDelayGUIDelegator extends VSTPluginGUIAdapter {

    protected static boolean DEBUG = false;
    protected static final String FX_GUI_CLASS = "jvst.examples.javafx.fxdelay.FXDelayGUI";

    
    public FXDelayGUIDelegator(VSTPluginGUIRunner runner, VSTPluginAdapter plug) {
        super(runner, plug);
        try {
            log("JayDLayFXGUIDelegator <init>");

            //windows properties
            this.setTitle("JayDLayFXGUI");
            this.setSize(290, 100);
            this.setResizable(false);

            // instantiate JavaFX GUI (which is a JavaFX Scene) here
            // use classloader
            FXDelayJavaInterop fxscene =
                    (FXDelayJavaInterop) this.getClass().getClassLoader().loadClass(FX_GUI_CLASS).newInstance();
            fxscene.setPluginInstance(plug);

            //give the plug a refence to the gui
            //--> so that the plug updates the GUI on param change (loading presets, etc.)
            if (!DEBUG) ((FXDelay)plug).setFxgui(fxscene);
            
            //embedd JavaFX GUI into Swing

            //use SceneToJComponent
            JComponent s = SceneToJComponent.loadScene(fxscene, FX_GUI_CLASS);

            //use JXscene
            //JXScene s = new JXScene();
            //s.setScene(fxscene);

            this.setLayout(new BorderLayout(10,10));
            this.add(s, BorderLayout.CENTER);

            
            //this is needed on the mac only,
            //java guis are handled there in a pretty different way than on win/linux
            //XXX
            if (RUNNING_MAC_X) {
                this.show();
            }
        }
        catch (Exception ex) {
            log("** ERROR: Fatal error when loading JavaFX GUI: " + ex.toString());
            ex.printStackTrace();
        } 
    }


    /*
     * main method for convenient GUI debugging
     */
    public static void main(String[] args) throws Throwable {
        DEBUG = true;

        FXDelayGUIDelegator gui = new FXDelayGUIDelegator(new VSTPluginGUIRunner(), null);
        gui.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        gui.show();
    }
    
}
