package jvst.wrapper.valueobjects;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */

public class MidiProgramCategory {
  private int thisCategoryIndex;        // >= 0. fill struct for this category index.
  private String name = "";             //[64];
  private int parentCategoryIndex = -1; // -1:no parent category
  private int flags = 0;                // reserved, none defined yet, zero.


  public int getThisCategoryIndex() {return this.thisCategoryIndex;}
  public int getParentCategoryIndex() {return this.parentCategoryIndex;}
  public int getFlags() {return this.flags;}
  public String getName() {return this.name;}


  public void setThisCategoryIndex(int p) {this.thisCategoryIndex = p;}
  public void setParentCategoryIndex(int p) {this.parentCategoryIndex = p;}
  public void setFlags(int p) {this.flags = p;}
  public void setName(String p) {this.name = p;}

}