package jvst.wrapper.valueobjects;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */

public class VSTPatchChunkInfo {

  private int version;	     // Format Version (should be 1)
  private int pluginUniqueID;// UniqueID of the plugin
  private int pluginVersion; // Plugin Version
  private int numElements;   // Number of Programs (Bank) or Parameters (Program)
  private String future;     //[48];

}