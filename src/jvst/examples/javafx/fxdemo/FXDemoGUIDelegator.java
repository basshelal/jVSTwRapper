/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jvst.examples.javafx.fxdemo;

import jvst.examples.javafx.fxdelay.*;
import java.awt.BorderLayout;
import jvst.examples.javafx.*;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import jvst.examples.javafx.fxdemo.JavaInterop;
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
public class FXDemoGUIDelegator extends VSTPluginGUIAdapter {

    protected static boolean DEBUG = false;
    protected static final String FX_GUI_CLASS = "jvst.examples.javafx.fxdelay.FXDelayGUI";

    
    public FXDemoGUIDelegator(VSTPluginGUIRunner runner, VSTPluginAdapter plug) {
        super(runner, plug);
        try {
            log("jVST FX Demo GUI <init>");

            //windows properties
            this.setTitle("JavaFX Demo VST GUI");
            this.setSize(600, 400);

            //create tabbed pane
            JTabbedPane tabbedPane = new JTabbedPane();

            //add some more JavaFX stuff --> this is stolen from the sample fx files at javafx.com
            JPanel p;
            p = makeFXPanel("jvst.examples.javafx.fxdemo.AnalogClock");
            tabbedPane.addTab("Analog Clock", p);
            p = makeFXPanel("jvst.examples.javafx.fxdemo.BouncingBall");
            tabbedPane.addTab("Bouncing Ball", p);
            p = makeFXPanel("jvst.examples.javafx.fxdemo.RecursiveTree");
            tabbedPane.addTab("Recursive Tree", p);
            p = makeFXPanel("jvst.examples.javafx.fxdemo.JigsawPuzzle");
            tabbedPane.addTab("Jigsaw Puzzle", p);
            p = makeFXPanel("jvst.examples.javafx.fxdemo.displayshelf.Main");
            tabbedPane.addTab("Display Shelf", p);
            p = makeFXPanel("jvst.examples.javafx.fxdemo.weather.Main");
            tabbedPane.addTab("Weather", p);

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
        //embedd using JXScene
        JXScene js = new JXScene();
        js.setScene(fxobj.getScene());

        panel.setLayout(new BorderLayout());
        panel.add(js, BorderLayout.CENTER);
        
        return panel;
    }




    /*
     * main method for convenient GUI debugging
     */
    public static void main(String[] args) throws Throwable {
        DEBUG = true;

        FXDemoGUIDelegator gui = new FXDemoGUIDelegator(new VSTPluginGUIRunner(), null);
        gui.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        gui.show();
    }
}
