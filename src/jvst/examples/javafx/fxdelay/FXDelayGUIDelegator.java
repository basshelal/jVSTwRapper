/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jvst.examples.javafx.fxdelay;

import java.awt.BorderLayout;
import jvst.examples.javafx.*;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import jvst.examples.javafx.fxdemos.JavaInterop;
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
            this.setSize(280, 300);
            this.setResizable(false);

            //create tabbed pane
            JTabbedPane tabbedPane = new JTabbedPane();

            //first tab is the JayDLay GUI
            FXDelayJavaInterop fxscene =
                (FXDelayJavaInterop) this.getClass().getClassLoader().loadClass(FX_GUI_CLASS).newInstance();
            fxscene.setPluginInstance(plug);
            fxscene.setDelegator(this);

            //give the plug a refence to the gui
            //--> so that the plug updates the GUI on param change (loading presets, etc.)
            if (!DEBUG) ((FXDelay)plug).setFxgui(fxscene);

            //embedd into this (which is a JFrame)
            JXScene s = new JXScene();
            s.setScene(fxscene);
            tabbedPane.addTab("JayDLay", s);


            //add some more JavaFX stuff --> this is stolen from the sample fx files at javafx.com
            JPanel p;
            p = makeFXPanel("jvst.examples.javafx.fxdemos.AnalogClock");
            tabbedPane.addTab("AnalogClock", p);
            p = makeFXPanel("jvst.examples.javafx.fxdemos.BouncingBall");
            tabbedPane.addTab("BouncingBall", p);


            this.getContentPane().add(tabbedPane);

            
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

    private JPanel makeFXPanel(String fxGUIClass) throws Exception {
        JPanel panel = new JPanel(false);

        // instantiate JavaFX GUI (which is a JavaFX Scene) here
        // use classloader
        /*
        ClassLoader cl = this.getClass().getClassLoader();
        while(cl!=null) {
            System.out.println("CL = " + cl);
            System.out.println("sys CL = " + cl.getSystemClassLoader());

            cl=cl.getParent();
        }
        */

        JavaInterop fxobj =
                (JavaInterop) this.getClass().getClassLoader().loadClass(fxGUIClass).newInstance();
        //embedd into this (which is a JFrame)
        JXScene js = new JXScene();
        js.setScene(fxobj.getScene());

        panel.setLayout(new BorderLayout());
        panel.add(js, BorderLayout.CENTER);
        
        return panel;
    }


    /*
     * Convenience methods to convert stupid JavFX types to Java types and back
     * --> avoids the casts, that would involve creating a new object (new Float();) otherwise
     * --> save GC time!
     */

    public void setParameterAutomated(int idx, double val) {
        this.plugin.setParameterAutomated(idx,(float) val);
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
