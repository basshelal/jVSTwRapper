package jvst.examples.jaydlay;


import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;

import jvst.wrapper.*;


public class JayDLayGUI extends VSTPluginGUIAdapter implements ChangeListener {
  private JSlider DelaySlider;
  private JSlider FeedbackSlider;
  private JSlider VolumeSlider;
  private JTextField DelayText;
  private JTextField FeedbackText;
  private JTextField VolumeText;

  private VSTPluginAdapter pPlugin;


  public JayDLayGUI() {
    log("JayDLayGUI <init>");

    this.setTitle("JayDLay v0.8");
    this.setSize(200, 200);
    this.setResizable(false);
  }


  public void init(VSTPluginAdapter e) {
    this.pPlugin = e;//remember reference to plugin in order to react to slider changes, ...

    this.VolumeSlider = new JSlider(JSlider.VERTICAL, 1, 100, (int)(this.pPlugin.getParameter(DelayProgram.PARAM_ID_OUT) * 100F));
    this.FeedbackSlider = new JSlider(JSlider.VERTICAL, 1, 100, (int)(this.pPlugin.getParameter(DelayProgram.PARAM_ID_FEEDBACK) * 100F));
    this.DelaySlider = new JSlider(JSlider.VERTICAL, 1, 100, (int)(this.pPlugin.getParameter(DelayProgram.PARAM_ID_DELAY) * 100F));
    this.VolumeSlider.addChangeListener(this);
    this.FeedbackSlider.addChangeListener(this);
    this.DelaySlider.addChangeListener(this);

    this.VolumeText = new JTextField((int)(this.pPlugin.getParameter(DelayProgram.PARAM_ID_OUT) * 100F) + "%");
    this.FeedbackText = new JTextField((int)(this.pPlugin.getParameter(DelayProgram.PARAM_ID_FEEDBACK) * 100F) + "%");
    this.DelayText = new JTextField(this.pPlugin.getParameterDisplay(DelayProgram.PARAM_ID_DELAY));

    JLabel DelayLabel = new JLabel("Delay");
    JLabel FeedbackLabel = new JLabel("Feedback");
    JLabel VolumeLabel = new JLabel("Volume");

    GridLayout grids = new GridLayout(1, 3);
    this.getContentPane().setLayout(grids);

    Box VolumeBox = new Box(BoxLayout.Y_AXIS);
    Box FeedbackBox = new Box(BoxLayout.Y_AXIS);
    Box DelayBox = new Box(BoxLayout.Y_AXIS);

    VolumeBox.add(VolumeLabel);
    VolumeBox.add(this.VolumeSlider);
    VolumeBox.add(this.VolumeText);

    FeedbackBox.add(FeedbackLabel);
    FeedbackBox.add(this.FeedbackSlider);
    FeedbackBox.add(this.FeedbackText);

    DelayBox.add(DelayLabel);
    DelayBox.add(this.DelaySlider);
    DelayBox.add(this.DelayText);

    this.getContentPane().add(DelayBox);
    this.getContentPane().add(FeedbackBox);
    this.getContentPane().add(VolumeBox);
  }


  /**
   *  Slider value has changed...
   */
  public void stateChanged(ChangeEvent e) {
    JSlider sl = (JSlider)e.getSource();

    if (sl == this.VolumeSlider) {
      this.pPlugin.setParameter(DelayProgram.PARAM_ID_OUT, (float)((float)sl.getValue() / 100F));
      this.VolumeText.setText((int)(this.pPlugin.getParameter(DelayProgram.PARAM_ID_OUT) * 100F) + "%");
    }
    else if (sl == this.FeedbackSlider) {
      this.pPlugin.setParameter(DelayProgram.PARAM_ID_FEEDBACK, (float)((float)sl.getValue() / 100F));
      this.FeedbackText.setText((int)(this.pPlugin.getParameter(DelayProgram.PARAM_ID_FEEDBACK) * 100F) + "%");
    }
    else if (sl == this.DelaySlider) {
      this.pPlugin.setParameter(DelayProgram.PARAM_ID_DELAY, (float)((float)sl.getValue() / 100F));
      this.DelayText.setText(this.pPlugin.getParameterDisplay(DelayProgram.PARAM_ID_DELAY));
    }
  }

}
