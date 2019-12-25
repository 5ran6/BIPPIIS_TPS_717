/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package GBFRSW_JAVA_Defines;

import com.sun.jna.Structure;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;

/**
 *
 * @author andrea
 */
public class GBFRSW_CodeHeader extends Structure {
	
	@Override
    public Pointer getPointer() {
        this.write();
        Pointer ret = super.getPointer();
        return ret;
    }
	
	public GBFRSW_CodeHeader() {
        super();
        setAlignType(ALIGN_NONE);
    }
	
	public GBFRSW_CodeHeader(Pointer p) {
        this();
        if (p == Pointer.NULL) {
            return;
        }
        setAlignType(ALIGN_NONE);
        useMemory(p);
        read();
    }

	/**
	 * Native is an uint32
	 * see point (1) of ReadME.txt file
	 */
	public int CRC;					// checksum, internally used to check the code integrity
	/**
	 * Native is an uint32
	 * see point (1) of ReadME.txt file
	 */
	public int Size;					// size of fingerrpint code, in bytes, 
	//   including GBFRSW_CODE_HEADER size
	/**
	 * Native is an uint32
	 * see point (1) of ReadME.txt file
	 */
	public int SizeAfterConversion;  // size of this fingerrpint code after Convert() 
	//    operation, in bytes, informative scope
	public byte Type;					// fingerrpint code type, see definitions below
	public byte Version;				// fingerrpint code version, numerical (1,2,3,..)
	public byte ImageFlags;			// reports the value of <ImageFlags> parameter of 
	//   GBFRSW_Coding() function used for code generation
	public byte Quality;				// fingerprint pattern quality, in range [0-100]: 
	//	 0 - lowest quality, 100-highest quality
	public float InformationCapacity;	// reflectf informational capacity of fingerprint code;
	//   normally, in practice, this value denotes the highest 
	//   achievable matching score value for the given  
	//   fingerprint code; so, it could be used as a certain 
	//   "predictor" for further matching success,
	//   (see recommendations for use below). 
	/**
	 * Native is an unsigned short
	 * see point (1) of ReadME.txt file
	 */
	public short ImageSizeX;			// VER1310, width of original fingerprint image 
	/**
	 * Native is an unsigned short
	 * see point (1) of ReadME.txt file
	 */
	public short ImageSizeY;			// VER1310, height of original fingerprint image 
	public byte[] Reserve = new byte[4];			// reserved for future use, initialized by 0
	/**
	 * Native is an uint32
	 * see point (1) of ReadME.txt file
	 */
	public int SizeConfirm;			// must contain the same value as <Size> field
}
