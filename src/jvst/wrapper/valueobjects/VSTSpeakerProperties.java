package jvst.wrapper.valueobjects;

/**
 * <p>Title: </p>
 * <p><b>Description:</b><br>
 * note: the origin for azimuth is right (as by math conventions dealing with radians);
 * the elevation origin is also right, visualizing a rotation of a circle across the
 * -pi/pi axis of the horizontal circle. thus, an elevation of -pi/2 corresponds
 * to bottom, and a speaker standing on the left, and 'beaming' upwards would have
 * an azimuth of -pi, and an elevation of pi/2.
 * for user interface representation, grads are more likely to be used, and the
 * origins will obviously 'shift' accordingly.
 * </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author dm
 * @version 1.0
 */


public class VSTSpeakerProperties {
  //Speaker Types
  public static final int SPEAKER_UNDEFINED = 0x7fffffff;	// Undefinded
  public static final int SPEAKER_M = 0;			// Mono (M)
  public static final int SPEAKER_L = 1;			// Left (L)
  public static final int SPEAKER_R = 2;			// Right (R)
  public static final int SPEAKER_C = 3;			// Center (C)
  public static final int SPEAKER_LFE = 4;			// Subbass (Lfe)
  public static final int SPEAKER_LS = 5;			// Left Surround (Ls)
  public static final int SPEAKER_RS = 6;			// Right Surround (Rs)
  public static final int SPEAKER_LC = 7;			// Left of Center (Lc)
  public static final int SPEAKER_RC = 8;			// Right of Center (Rc)
  public static final int SPEAKER_S = 9;			// Surround (S)
  public static final int SPEAKER_CS = SPEAKER_S;		// Center of Surround (Cs) = Surround (S)
  public static final int SPEAKER_SL = 10;			// Side Left (Sl)
  public static final int SPEAKER_SR = 11;	        	// Side Right (Sr)
  public static final int SPEAKER_TM = 12;			// Top Middle (Tm)
  public static final int SPEAKER_TFL = 13;			// Top Front Left (Tfl)
  public static final int SPEAKER_TFC = 14;			// Top Front Center (Tfc)
  public static final int SPEAKER_TFR = 15;			// Top Front Right (Tfr)
  public static final int SPEAKER_TRL = 16;			// Top Rear Left (Trl)
  public static final int SPEAKER_TRC = 17;			// Top Rear Center (Trc)
  public static final int SPEAKER_TRR = 18;			// Top Rear Right (Trr)
  public static final int SPEAKER_LFE2 = 19;			// Subbass 2 (Lfe2)

  // user-defined speaker types (to be extended in the negative range)
  // (will be handled as their corresponding speaker types with abs values:
  // e.g abs(kSpeakerU1) == kSpeakerL, abs(kSpeakerU2) == kSpeakerR)
  public static final int SPEAKER_U32 = -32;
  public static final int SPEAKER_U31 = -31;
  public static final int SPEAKER_U30 = -30;
  public static final int SPEAKER_U29 = -29;
  public static final int SPEAKER_U28 = -28;
  public static final int SPEAKER_U27 = -27;
  public static final int SPEAKER_U26 = -26;
  public static final int SPEAKER_U25 = -25;
  public static final int SPEAKER_U24 = -24;
  public static final int SPEAKER_U23 = -23;
  public static final int SPEAKER_U22 = -22;
  public static final int SPEAKER_U21 = -21;
  public static final int SPEAKER_U20 = -20;		// == SPEAKER_Lfe2
  public static final int SPEAKER_U19 = -19;		// == SPEAKER_Trr
  public static final int SPEAKER_U18 = -18;		// == SPEAKER_Trc
  public static final int SPEAKER_U17 = -17;		// == SPEAKER_Trl
  public static final int SPEAKER_U16 = -16;		// == SPEAKER_Tfr
  public static final int SPEAKER_U15 = -15;		// == SPEAKER_Tfc
  public static final int SPEAKER_U14 = -14;		// == SPEAKER_Tfl
  public static final int SPEAKER_U13 = -13;		// == SPEAKER_Tm
  public static final int SPEAKER_U12 = -12;		// == SPEAKER_Sr
  public static final int SPEAKER_U11 = -11;		// == SPEAKER_Sl
  public static final int SPEAKER_U10 = -10;		// == SPEAKER_Cs
  public static final int SPEAKER_U9 = -9;		// == SPEAKER_S
  public static final int SPEAKER_U8 = -8;		// == SPEAKER_Rc
  public static final int SPEAKER_U7 = -7;		// == SPEAKER_Lc
  public static final int SPEAKER_U6 = -6;		// == SPEAKER_Rs
  public static final int SPEAKER_U5 = -5;		// == SPEAKER_Ls
  public static final int SPEAKER_U4 = -4;		// == SPEAKER_Lfe
  public static final int SPEAKER_U3 = -3;		// == SPEAKER_C
  public static final int SPEAKER_U2 = -2;		// == SPEAKER_R
  public static final int SPEAKER_U1 = -1;		// == SPEAKER_L

  //                               units:	range:		except:
  private float azimuth;	// rad		-PI...PI	10.f for LFE channel
  private float elevation;	// rad		-PI/2...PI/2	10.f for LFE channel
  private float radius;		// meter			0.f for LFE channel
  private float reserved;	// 0.
  private String name;	        // for new setups, new names should be given (L/R/C... won't do)
  private int  type;		// speaker type
  private byte future[] = new byte[28];


  public VSTSpeakerProperties() {
  }

  public float getAzimuth() {
    return this.azimuth;
  }

  public void setAzimuth(float azimuth) {
    this.azimuth = azimuth;
  }

  public float getElevation() {
    return this.elevation;
  }

  public void setElevation(float elevation) {
    this.elevation = elevation;
  }

  public float getRadius() {
    return this.radius;
  }

  public void setRadius(float radius) {
    this.radius = radius;
  }

  public float getReserved() {
    return this.reserved;
  }

  public void setReserved(float reserved) {
    this.reserved = reserved;
  }

  public String getName() {
    return this.name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public int getType() {
    return this.type;
  }

  public void setType(int type) {
    this.type = type;
  }

  public byte[] getFuture() {
    return this.future;
  }

  public void setFuture(byte[] future) {
    this.future = future;
  }
}