/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jvst.examples.javafx;

import java.awt.BorderLayout;
import javafx.reflect.FXClassType;
import javafx.reflect.FXContext;
import javafx.reflect.FXLocal;
import javafx.reflect.FXObjectValue;
import javafx.reflect.FXValue;
import javax.swing.JComponent;


/**
 * @author Richard Bair and Jasper Potts
 * 
 * http://blogs.sun.com/javafx/entry/how_to_use_javafx_in
 *
 * slighly modified by DM
 *
 * NOTE: only works with JavaFX 1.0
 *
 * for the modifications necessary in order to make it run in JavaFX 1.1, see here:
 * http://karussell.wordpress.com/2009/02/27/calling-javafx-from-java/
 *
 *
 * First things first: What I'm about to show you is a hack. It is completely
 * unsupported, uses private APIs, and almost certainly will break in the next
 * version. However, if we see a lot of interest we may add a permanent supported
 * API in a future release of JavaFX, so let us know what you think of it. Second,
 * this is a hack. Most likely not all features of JavaFX will work in this
 * scenario and we haven't done any more than trivial testing. Don't go building
 * a production application using this technique. All that said, on with the
 * show:
 *
 * The JXScene class
 *
 * The core of this hack is the JXScene class, a Swing component which knows
 * how to draw JavaFX scene graph nodes. It uses reflection to instantiate
 * your JavaFX scene class and then pull out the internal canvas object.
 * 
 */


/**
 * Use SceneToJComponent instead
 */
@Deprecated
public class JXScene extends JComponent {

    private Object scriptObject;

    public JXScene() {
        setLayout(new BorderLayout());
    }

    // fully qualified name of script class that extends javafx.scene.Scene
    public void setScene(Object scene) throws Exception {
        if (this.scriptObject!=null)
            throw new IllegalStateException("FX Scene already set, cannot re-set on the same instance!");

        String helperName = "com.sun.javafx.scene.JSGPanelSceneImpl";
        FXClassType type = FXContext.getInstance().findClass(helperName);
        FXObjectValue panelSceneImpl = type.allocate();

        panelSceneImpl.initVar("scene", FXLocal.getContext().mirrorOf(scene));
        panelSceneImpl.initialize();

        FXValue jsgPanelV = type.getVariable("jsgPanel").getValue(panelSceneImpl);
        JComponent jsgPanel = (JComponent)((FXLocal.ObjectValue) jsgPanelV).asObject();
        add(jsgPanel, BorderLayout.CENTER);
        
        scriptObject = scene;
    }

    public Object getScene() {
        return scriptObject;
    }
}

