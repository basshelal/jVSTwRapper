/*
 * VSTfxPluginGUI.fx
 *
 * Created on 12.12.2008, 20:27:23
 */

package jvst.examples.javafx.fxdelay;

import javafx.scene.paint.Color;
import javafx.scene.*;
import javafx.stage.*;
import jvst.wrapper.VSTPluginAdapter;
import jvst.examples.jaydlay.*;
import java.lang.*;
import jvst.examples.javafx.ui.*;


/**
 * @author nix
 */


public class FXDelayGUI extends Scene, FXDelayJavaInterop {

    var plug : JayDLay;

    
    public var delay : Number = 0.5 on replace oldval {
        //notify the plugin
        if (not FXDelayGUIDelegator.DEBUG) plug.setParameterAutomated(JayDLay.PARAM_ID_DELAY, delay);
    };
    public var feedback : Number = 0.5 on replace oldval {
        //notify the plugin
        if (not FXDelayGUIDelegator.DEBUG) plug.setParameterAutomated(JayDLay.PARAM_ID_FEEDBACK, feedback);
    };
    public var volume : Number = 1.0 on replace oldval {
        //notify the plugin
        if (not FXDelayGUIDelegator.DEBUG) plug.setParameterAutomated(JayDLay.PARAM_ID_OUT, volume);
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
        fill = Color.DARKGREY;

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
        ]
    }


    public override function setPluginInstance(p : VSTPluginAdapter): Void {
        plug = p as JayDLay; // CAST!
    }

}

// debug main function for an easy way of running the GUI without all the VST mess
function run(args : String[]) {
    Stage {
        width: 290
        height: 100
        scene: new FXDelayGUI();
    }
}
