/*
 * VSTfxPluginGUI.fx
 *
 * Created on 12.12.2008, 20:27:23
 */

package jvst.examples.javafx.fxdelay;

import javafx.scene.CustomNode;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Rectangle;
import javafx.scene.*;
import javafx.stage.*;
import jvst.wrapper.VSTPluginAdapter;
import jvst.examples.jaydlay.*;
import java.lang.*;
import jvst.examples.javafx.components.*;


/**
 * @author nix
 */


public class FXDelayGUI extends Scene, FXDelayJavaInterop {

    var plug : JayDLay;
    var delegator : FXDelayGUIDelegator;
    
    
    public var delay : Number = 0.5 on replace oldval {
        //notify the plugin
        if (not FXDelayGUIDelegator.DEBUG) delegator.setParameterAutomated(JayDLay.PARAM_ID_DELAY, delay);
    };
    public var feedback : Number = 0.5 on replace oldval {
        //notify the plugin
        if (not FXDelayGUIDelegator.DEBUG) delegator.setParameterAutomated(JayDLay.PARAM_ID_FEEDBACK, feedback);
    };
    public var volume : Number = 1.0 on replace oldval {
        //notify the plugin
        // EXAMPLE: directly call the plugin without using the delegator
        // --> Note the creation of a new object instance --> BAD, since GC needs to cleanup this one
        if (not FXDelayGUIDelegator.DEBUG) plug.setParameterAutomated(JayDLay.PARAM_ID_OUT, new Float(volume));
    };


    public override function setVolume(v) : Void {
        volume = v; //trigger replace trigger
    }
    public override function setFeedback(f) : Void {
        feedback = f; //trigger replace trigger
    }
    public override function setDelay(d) : Void {
        delay = d; //trigger replace trigger
    }


    //scene is created here
    init {
        var tree = RecursiveTree {
            translateX: 140 translateY: 120
            angle: 45
            leftFactor: 1.3
            rightFactor: 1.3
        };
        tree.anim.play();

        fill = Color.LIGHTGREY;

        //cursor = ...

        content = [
            LabeledSlider {
                translateX: 10
                translateY: 10
                text: "Volume"
                value : bind volume with inverse
            }
            LabeledSlider {
                translateX: 10
                translateY: 30
                text: "Delay"
                value : bind delay with inverse
            }
            LabeledSlider {
                translateX: 10
                translateY: 50
                text: "Feedback"
                value : bind feedback with inverse
            }
            tree
        ]
    }


    public override function setPluginInstance(p : VSTPluginAdapter): Void {
        plug = p as JayDLay; // CAST!
    }

    public override function setDelegator(p : FXDelayGUIDelegator): Void {
        delegator = p;
    }
}

// debug main function for an easy way of running the GUI without all the VST mess
function run(args : String[]) {
    Stage {
        width: 280
        height: 300
        scene: new FXDelayGUI();
    }
}
