package jvst.examples.dreinulldrei;


import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;

import jvst.wrapper.*;


public class DreiNullDreiGUI extends VSTPluginGUIAdapter implements ChangeListener {
  private JSlider VolumeSlider;
  private JSlider Freg1Slider;
  private JSlider Freg2Slider;
  private JSlider Level1Slider;
  private JSlider Level2Slider;

  private JRadioButton SawRadio1;
  private JRadioButton SawRadio2;
  private JRadioButton PulseRadio1;
  private JRadioButton PulseRadio2;

  private JTextField VolumeText;
  private JTextField Freg1Text;
  private JTextField Freg2Text;
  private JTextField Level1Text;
  private JTextField Level2Text;


  private VSTPluginAdapter pPlugin;


  public DreiNullDreiGUI() throws Exception {
    log("DreiNullDreiGUI <init>");

    this.setTitle("DreiNullDrei v0.6");
    this.setSize(380, 290);
    this.setResizable(false);
    
    //use the napkin look-and-feel
	UIManager.setLookAndFeel("napkin.NapkinLookAndFeel");
    SwingUtilities.updateComponentTreeUI(this);
  }


  public void init(VSTPluginAdapter e) {
    this.pPlugin = e;//remember reference to plugin in order to react to slider changes, ...

//
//    this.VolumeSlider = new JSlider(JSlider.VERTICAL, 1, 100, (int)(this.pPlugin.getParameter(JayVSTxSynthProgram.PARAM_ID_VOLUME) * 100F));
//    this.Freg1Slider = new JSlider(JSlider.HORIZONTAL, 1, 100, (int)(this.pPlugin.getParameter(JayVSTxSynthProgram.PARAM_ID_FREQ1) * 100F));
//    this.Freg2Slider = new JSlider(JSlider.HORIZONTAL, 1, 100, (int)(this.pPlugin.getParameter(JayVSTxSynthProgram.PARAM_ID_FREQ2) * 100F));
//    this.Level1Slider = new JSlider(JSlider.HORIZONTAL, 1, 100, (int)(this.pPlugin.getParameter(JayVSTxSynthProgram.PARAM_ID_VOLUME1) * 100F));
//    this.Level2Slider = new JSlider(JSlider.HORIZONTAL, 1, 100, (int)(this.pPlugin.getParameter(JayVSTxSynthProgram.PARAM_ID_VOLUME2) * 100F));

    

    this.VolumeSlider = new JSlider(JSlider.VERTICAL, 1, 100, 1);
    this.Freg1Slider = new JSlider(JSlider.HORIZONTAL, 1, 100, 1);
    this.Freg2Slider = new JSlider(JSlider.HORIZONTAL, 1, 100, 1);
    this.Level1Slider = new JSlider(JSlider.HORIZONTAL, 1, 100, 1);
    this.Level2Slider = new JSlider(JSlider.HORIZONTAL, 1, 100, 1);

    this.VolumeSlider.addChangeListener(this);
    this.Freg1Slider.addChangeListener(this);
    this.Freg2Slider.addChangeListener(this);
    this.Level1Slider.addChangeListener(this);
    this.Level2Slider.addChangeListener(this);

    this.SawRadio1 = new JRadioButton("Saw", true);
    this.SawRadio2 = new JRadioButton("Saw");
    this.PulseRadio1 = new JRadioButton("Pulse");
    this.PulseRadio2 = new JRadioButton("Pulse", true);
    this.SawRadio1.addChangeListener(this);
    this.SawRadio2.addChangeListener(this);
    this.PulseRadio1.addChangeListener(this);
    this.PulseRadio2.addChangeListener(this);
    ButtonGroup Wave1Group = new ButtonGroup();
    ButtonGroup Wave2Group = new ButtonGroup();
    Wave1Group.add(this.SawRadio1);
    Wave1Group.add(this.PulseRadio1);
    Wave2Group.add(this.SawRadio2);
    Wave2Group.add(this.PulseRadio2);


//    this.VolumeText = new JTextField((int)(this.pPlugin.getParameter(JayVSTxSynthProgram.PARAM_ID_VOLUME) * 100F) + "%");
//    this.Freg1Text = new JTextField(this.pPlugin.getParameterDisplay(JayVSTxSynthProgram.PARAM_ID_FREQ1) + " " + this.pPlugin.getParameterLabel(JayVSTxSynthProgram.PARAM_ID_FREQ1));
//    this.Freg2Text = new JTextField(this.pPlugin.getParameterDisplay(JayVSTxSynthProgram.PARAM_ID_FREQ2) + " " + this.pPlugin.getParameterLabel(JayVSTxSynthProgram.PARAM_ID_FREQ2));
//    this.Level1Text = new JTextField(this.pPlugin.getParameterDisplay(JayVSTxSynthProgram.PARAM_ID_VOLUME1) + " " + this.pPlugin.getParameterLabel(JayVSTxSynthProgram.PARAM_ID_VOLUME1));
//    this.Level2Text = new JTextField(this.pPlugin.getParameterDisplay(JayVSTxSynthProgram.PARAM_ID_VOLUME2) + " " + this.pPlugin.getParameterLabel(JayVSTxSynthProgram.PARAM_ID_VOLUME2));

    this.VolumeText = new JTextField((1 * 100F) + "%");
    this.Freg1Text = new JTextField("234 Hz");
    this.Freg2Text = new JTextField("2345 Hz");
    this.Level1Text = new JTextField("324 db");
    this.Level2Text = new JTextField("345 db");


    JLabel VolumeLabel = new JLabel("Volume");
    JLabel Freg1Label = new JLabel("Freqency");
    JLabel Freg2Label = new JLabel("Freqency");
    JLabel Level1Label = new JLabel("Volume");
    JLabel Level2Label = new JLabel("Volume");

    GridLayout grids = new GridLayout(1, 3);
    this.getContentPane().setLayout(grids);

    Box VolumeBox = new Box(BoxLayout.Y_AXIS);
    Box Osc1box = new Box(BoxLayout.Y_AXIS);
    Box Osc2box = new Box(BoxLayout.Y_AXIS);


    VolumeBox.add(VolumeLabel);
    VolumeBox.add(this.VolumeSlider);
    VolumeBox.add(this.VolumeText);
    VolumeBox.setBorder(BorderFactory.createTitledBorder("Amp"));

    Osc1box.add(this.SawRadio1);
    Osc1box.add(this.PulseRadio1);
    Osc1box.add(Box.createVerticalStrut(15));
    Osc1box.add(Freg1Label);
    Osc1box.add(this.Freg1Slider);
    Osc1box.add(this.Freg1Text);
    Osc1box.add(Box.createVerticalStrut(15));
    Osc1box.add(Level1Label);
    Osc1box.add(this.Level1Slider);
    Osc1box.add(this.Level1Text);
    Osc1box.setBorder(BorderFactory.createTitledBorder("Osc1"));

    Osc2box.add(this.SawRadio2);
    Osc2box.add(this.PulseRadio2);
    Osc2box.add(Box.createVerticalStrut(15));
    Osc2box.add(Freg2Label);
    Osc2box.add(this.Freg2Slider);
    Osc2box.add(this.Freg2Text);
    Osc2box.add(Box.createVerticalStrut(15));
    Osc2box.add(Level2Label);
    Osc2box.add(this.Level2Slider);
    Osc2box.add(this.Level2Text);
    Osc2box.setBorder(BorderFactory.createTitledBorder("Osc2"));

    this.getContentPane().add(Osc1box);
    this.getContentPane().add(Osc2box);
    this.getContentPane().add(VolumeBox);
  }


  public void stateChanged(ChangeEvent e) {
    Object o = e.getSource();

//    if (o.equals(this.VolumeSlider)) {
//      JSlider s = (JSlider)o;
//      this.pPlugin.setParameter(JayVSTxSynthProgram.PARAM_ID_VOLUME, (float)((float)s.getValue() / 100F));
//      this.VolumeText.setText((int)(this.pPlugin.getParameter(JayVSTxSynthProgram.PARAM_ID_VOLUME) * 100F) + "%");
//    }
//    else if (o.equals(this.Freg1Slider)) {
//      JSlider s = (JSlider)o;
//      this.pPlugin.setParameter(JayVSTxSynthProgram.PARAM_ID_FREQ1, (float)((float)s.getValue() / 100F));
//      this.Freg1Text.setText(this.pPlugin.getParameterDisplay(JayVSTxSynthProgram.PARAM_ID_FREQ1) + " " + this.pPlugin.getParameterLabel(JayVSTxSynthProgram.PARAM_ID_FREQ1));
//    }
//    else if (o.equals(this.Freg2Slider)) {
//      JSlider s = (JSlider)o;
//      this.pPlugin.setParameter(JayVSTxSynthProgram.PARAM_ID_FREQ2, (float)((float)s.getValue() / 100F));
//      this.Freg2Text.setText(this.pPlugin.getParameterDisplay(JayVSTxSynthProgram.PARAM_ID_FREQ2) + " " + this.pPlugin.getParameterLabel(JayVSTxSynthProgram.PARAM_ID_FREQ2));
//    }
//    else if (o.equals(this.Level1Slider)) {
//      JSlider s = (JSlider)o;
//      this.pPlugin.setParameter(JayVSTxSynthProgram.PARAM_ID_VOLUME1, (float)((float)s.getValue() / 100F));
//      this.Level1Text.setText(this.pPlugin.getParameterDisplay(JayVSTxSynthProgram.PARAM_ID_VOLUME1) + " " + this.pPlugin.getParameterLabel(JayVSTxSynthProgram.PARAM_ID_VOLUME1));
//    }
//    else if (o.equals(this.Level2Slider)) {
//      JSlider s = (JSlider)o;
//      this.pPlugin.setParameter(JayVSTxSynthProgram.PARAM_ID_VOLUME2, (float)((float)s.getValue() / 100F));
//      this.Level2Text.setText(this.pPlugin.getParameterDisplay(JayVSTxSynthProgram.PARAM_ID_VOLUME2) + " " + this.pPlugin.getParameterLabel(JayVSTxSynthProgram.PARAM_ID_VOLUME2));
//    }
//    else if (o.equals(this.PulseRadio1)) {
//      JRadioButton r = (JRadioButton)o;
//      if (r.isEnabled()) this.pPlugin.setParameter(JayVSTxSynthProgram.PARAM_ID_WAVEFORM1, 0.9F);
//    }
//    else if (o.equals(this.PulseRadio2)) {
//      JRadioButton r = (JRadioButton)o;
//      if (r.isEnabled()) this.pPlugin.setParameter(JayVSTxSynthProgram.PARAM_ID_WAVEFORM2, 0.9F);
//    }
//    else if (o.equals(this.SawRadio1)) {
//      JRadioButton r = (JRadioButton)o;
//      if (r.isEnabled()) this.pPlugin.setParameter(JayVSTxSynthProgram.PARAM_ID_WAVEFORM1, 0.1F);
//    }
//    else if (o.equals(this.SawRadio2)) {
//      JRadioButton r = (JRadioButton)o;
//      if (r.isEnabled()) this.pPlugin.setParameter(JayVSTxSynthProgram.PARAM_ID_WAVEFORM2, 0.1F);
//    }

  }


  //for testing purpose only!
  public static void main(String[] args) throws Exception {
  	DreiNullDreiGUI gui = new DreiNullDreiGUI();
    gui.init(null);
    gui.open();
  }

}
