package jvst.wrapper.communication;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */

public abstract class VSTV22ToHost extends VSTV21ToHost {


  //bool closeFileSelector (VstFileSelect *ptr);
  // Close the Host File selector which was opened by openFileSelector
  //NOT SUPPORTED!

  //bool getChunkFile (void* nativePath);
  // Returns in platform format the path of the current chunk (could be called in setChunk ()) (FSSpec on MAC else char*)
  //NOT SUPPORTED! //deprecated since 2.4 anyways!

}