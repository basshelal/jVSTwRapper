package jvst.wrapper.valueobjects;

/**
 * <p>Title: </p>
 * <p>Description: a block of events for the current audio block</p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */

public class VSTEvents {

  private int numEvents;
  private int reserved;           // zero
  private VSTEvent events[];      // variable

  public VSTEvents() {
  }

  public int getNumEvents() {
    return this.numEvents;
  }

  public void setNumEvents(int numEvents) {
    this.numEvents = numEvents;
  }

  public int getReserved() {
    return this.reserved;
  }

  public void setReserved(int reserved) {
    this.reserved = reserved;
  }

  public VSTEvent[] getEvents() {
    return this.events;
  }

  public void setEvents(VSTEvent[] events) {
    this.events = events;
  }

}