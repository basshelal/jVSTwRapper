package jvst.wrapper.valueobjects;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */


public class VSTParameterProperties {
  //Parameter Properties Flags
  public static final int VST_PARAMETER_IS_SWITCH = 1 << 0;
  public static final int VST_PARAMETER_USES_INTEGER_MIN_MAX = 1 << 1;
  public static final int VST_PARAMETER_USES_FLOAT_STEP = 1 << 2;
  public static final int VST_PARAMETER_USES_INT_STEP = 1 << 3;
  public static final int VST_PARAMETER_SUPPORTS_DISPLAY_INDEX = 1 << 4;
  public static final int VST_PARAMETER_SUPPORTS_DISPLAY_CATEGORY= 1 << 5;
  public static final int VST_PARAMETER_CAN_RAMP = 1 << 6;


  private float stepFloat;
  private float smallStepFloat;
  private float largeStepFloat;
  private String label;
  private int flags;			// see constants
  private int minInteger;
  private int maxInteger;
  private int stepInteger;
  private int largeStepInteger;
  private String shortLabel;		// recommended: 6 + delimiter

  // the following are for remote controller display purposes.
  // note that the kVstParameterSupportsDisplayIndex flag must be set.
  // host can scan all parameters, and find out in what order
  // to display them:

  private int displayIndex;		// for remote controllers, the index where this parameter
                                        // should be displayed (starting with 0)

  // host can also possibly display the parameter group (category), such as
  // ---------------------------
  // Osc 1
  // Wave  Detune  Octave  Mod
  // ---------------------------
  // if the plug supports it (flag kVstParameterSupportsDisplayCategory)
  private int category;		// 0: no category, else group index + 1
  private int numParametersInCategory;
  private int reserved;
  private String categoryLabel;	// for instance, "Osc 1"

  private String future;




  public VSTParameterProperties() {
  }

  public VSTParameterProperties(
    float stepfl, float smallstepfl, float largestepfl, String lab, int fl, int minInt,
    int maxInt, int stepInt, int largeStepInt, String shortLab, int displayIdx, int cat,
    int numParamsInCat, int res, String catLabel, String fut) {

    this.stepFloat = stepfl;
    this.smallStepFloat = smallstepfl;
    this.largeStepFloat = largestepfl;
    this.label = lab;
    this.flags = fl;
    this.minInteger = minInt;
    this.maxInteger = maxInt;
    this.stepInteger = stepInt;
    this.largeStepInteger = largeStepInt;
    this.shortLabel = shortLab;
    this.displayIndex = displayIdx;
    this.category = cat;
    this.numParametersInCategory = numParamsInCat;
    this.reserved = res;
    this.categoryLabel = catLabel;
    this.future = fut;
  }

  public float getStepFloat() {
    return this.stepFloat;
  }

  public void setStepFloat(float stepFloat) {
    this.stepFloat = stepFloat;
  }

  public float getSmallStepFloat() {
    return this.smallStepFloat;
  }

  public void setSmallStepFloat(float smallStepFloat) {
    this.smallStepFloat = smallStepFloat;
  }

  public float getLargeStepFloat() {
    return this.largeStepFloat;
  }

  public void setLargeStepFloat(float largeStepFloat) {
    this.largeStepFloat = largeStepFloat;
  }

  public String getLabel() {
    return this.label;
  }

  public void setLabel(String label) {
    this.label = label;
  }

  public int getFlags() {
    return this.flags;
  }

  public void setFlags(int flags) {
    this.flags = flags;
  }

  public int getMinInteger() {
    return this.minInteger;
  }

  public void setMinInteger(int minInteger) {
    this.minInteger = minInteger;
  }

  public int getMaxInteger() {
    return this.maxInteger;
  }

  public void setMaxInteger(int maxInteger) {
    this.maxInteger = maxInteger;
  }

  public int getStepInteger() {
    return this.stepInteger;
  }

  public void setStepInteger(int stepInteger) {
    this.stepInteger = stepInteger;
  }

  public int getLargeStepInteger() {
    return this.largeStepInteger;
  }

  public void setLargeStepInteger(int largeStepInteger) {
    this.largeStepInteger = largeStepInteger;
  }

  public String getShortLabel() {
    return this.shortLabel;
  }

  public void setShortLabel(String shortLabel) {
    this.shortLabel = shortLabel;
  }

  public int getDisplayIndex() {
    return this.displayIndex;
  }

  public void setDisplayIndex(int displayIndex) {
    this.displayIndex = displayIndex;
  }

  public int getCategory() {
    return this.category;
  }

  public void setCategory(int category) {
    this.category = category;
  }

  public int getNumParametersInCategory() {
    return this.numParametersInCategory;
  }

  public void setNumParametersInCategory(int numParametersInCategory) {
    this.numParametersInCategory = numParametersInCategory;
  }

  public int getReserved() {
    return this.reserved;
  }

  public void setReserved(int reserved) {
    this.reserved = reserved;
  }

  public String getCategoryLabel() {
    return this.categoryLabel;
  }

  public void setCategoryLabel(String categoryLabel) {
    this.categoryLabel = categoryLabel;
  }

  public String getFuture() {
    return this.future;
  }

  public void setFuture(String future) {
    this.future = future;
  }



}