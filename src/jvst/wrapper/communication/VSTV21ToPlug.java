package jvst.wrapper.communication;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */

import jvst.wrapper.valueobjects.*;


public interface VSTV21ToPlug extends VSTV20ToPlug {
  //VST 2.1 Required
  //nothing

  //VST2.1 Optional
  public int getMidiProgramName(int channel, MidiProgramName midiProgramName);
  // Struct will be filled with information for 'thisProgramIndex'.
  // returns number of used programIndexes.
  // If 0 is returned, no MidiProgramNames supported.

  public int getCurrentMidiProgram(int channel, MidiProgramName currentProgram);
  // Struct will be filled with information for the current program.
  // Returns the programIndex of the current program. -1 means not supported.

  public int getMidiProgramCategory(int channel, MidiProgramCategory category);
  // Struct will be filled with information for 'thisCategoryIndex'.
  // returns number of used categoryIndexes.
  // if 0 is returned, no MidiProgramCategories supported/used.

  public boolean hasMidiProgramsChanged(int channel);
  // Returns true if the MidiProgramNames, MidiKeyNames or
  // MidiControllerNames had changed on this channel.

  public boolean getMidiKeyName(int channel, MidiKeyName keyName);
  // Struct will be filled with information for 'thisProgramIndex' and 'thisKeyNumber'
  // if keyName is "" the standard name of the key will be displayed.
  // if false is returned, no MidiKeyNames defined for 'thisProgramIndex'.

  public boolean beginSetProgram();
  // Called before a program is loaded

  public boolean endSetProgram();
  // Called after...

}