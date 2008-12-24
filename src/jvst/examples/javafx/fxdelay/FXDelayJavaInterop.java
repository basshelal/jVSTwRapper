/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jvst.examples.javafx.fxdelay;

import jvst.wrapper.VSTPluginAdapter;

/**
 *
 * @author nix
 */
public interface FXDelayJavaInterop {
    public void setPluginInstance(VSTPluginAdapter plug);
    public void setDelegator(FXDelayGUIDelegator delegator);

    public void setVolume(float v);
    public void setFeedback(float f);
    public void setDelay(float d);
}
