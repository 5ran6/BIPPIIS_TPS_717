/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package GBFRSW_JAVA_Defines;

import com.sun.jna.Structure;
import com.sun.jna.Pointer;

/**
 *
 * @author andrea
 */
public class GBFRSW_FileLicenseCheck extends Structure {

    @Override
    public Pointer getPointer() {
        this.write();
        Pointer ret = super.getPointer();
        return ret;
    }

    public GBFRSW_FileLicenseCheck() {
        super();
        setAlignType(ALIGN_NONE);
    }

    public GBFRSW_FileLicenseCheck(Pointer p) {
        this();
        if (p == Pointer.NULL) {
            return;
        }
        setAlignType(ALIGN_NONE);
        useMemory(p);
        read();
    }
    public int ErrorCode; // see LM-X document <enduser.pdf>, section <9. Error messages> 
    public int Phase;     // internal phase number
    /**
     * see point (2) of ReadME.txt file
     */
    public byte[] ErrorDetailString
            = new byte[GBFRSW_FileLicenseCheckGlobals.GBFRSW_LICENSE_ERROR_DETAIL_STRING_LENGTH];
}
