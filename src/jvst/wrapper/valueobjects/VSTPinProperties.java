package jvst.wrapper.valueobjects;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */

public class VSTPinProperties {
  //pin properties flags
  public static final int VST_PIN_IS_ACTIVE = 1;
  public static final int VST_PIN_IS_STEREO = 2;
  public static final int VST_PIN_USE_SPEAKER = 4;


  private String Label;
  private int Flags;         // see pin properties flags
  private int ArrangementType;
  private String ShortLabel; // recommended: 6 + delimiter


  public VSTPinProperties() {
  }

  public VSTPinProperties(String lab, int fla, int arr, String sho) {
    this.Label = lab;
    this.Flags = fla;
    this.ArrangementType = arr;
    this.ShortLabel = sho;
  }


  public String getShortLabel() {
    return this.ShortLabel;
  }

  public void setShortLabel(String shortLabel) {
    if (shortLabel.length()>6) this.ShortLabel = shortLabel.substring(0,6);
    else this.ShortLabel = shortLabel;
  }

  public int getArrangementType() {
    return this.ArrangementType;
  }

  public void setArrangementType(int ArrangementType) {
    this.ArrangementType = ArrangementType;
  }

  public int getFlags() {
    return this.Flags;
  }

  public void setFlags(int Flags) {
    this.Flags = Flags;
  }

  public String getLabel() {
    return this.Label;
  }

  public void setLabel(String label) {
    if (label.length()>63) this.Label = label.substring(0,63);
    else this.Label = label;
  }
}