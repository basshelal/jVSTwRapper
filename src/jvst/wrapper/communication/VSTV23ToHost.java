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


public abstract class VSTV23ToHost extends VSTV22ToHost {

  public native boolean setPanLaw (int type, float val);
  // Set the Panning Law used by the Host

  public native int beginLoadBank (VSTPatchChunkInfo ptr);
  // Called before a Bank is loaded.
  // returns -1 if the Bank cannot be loaded, returns 1 if it can be loaded else 0 (for compatibility)

  public native int beginLoadProgram (VSTPatchChunkInfo ptr);
  // Called before a Program is loaded. (called before beginSetProgram)
  // returns -1 if the Program cannot be loaded, returns 1 if it can be loaded else 0 (for compatibility)


  //VST 2.3 TOOLS
  //**********************


   //NOT IMPORTANT --> we have a gc!!!
   /*
   public native bool allocateArrangement (VstSpeakerArrangement** arrangement, long nbChannels);
    // Allocate memory for a VstSpeakerArrangement containing the given
    // number of channels

   public native bool deallocateArrangement(VstSpeakerArrangement * * arrangement);
   // Delete/free memory for a speaker arrangement
   */

   public native boolean copySpeaker(VSTSpeakerProperties to, VSTSpeakerProperties from);
   // Feed the "to" speaker properties with the same values than "from"'s ones.
   // It is assumed here that "to" exists yet, ie this function won't
   // allocate memory for the speaker (this will prevent from having
   // a difference between an Arrangement's number of channels and
   // its actual speakers...)

   public native boolean matchArrangement(VSTSpeakerArrangement to, VSTSpeakerArrangement from);
   // "to" is deleted, then created and initialized with the same values as "from" ones ("from" must exist).


}