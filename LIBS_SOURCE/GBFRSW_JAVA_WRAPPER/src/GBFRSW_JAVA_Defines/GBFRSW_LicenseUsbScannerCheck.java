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
public class GBFRSW_LicenseUsbScannerCheck extends Structure {

    @Override
    public Pointer getPointer() {
        this.write();
        Pointer ret = super.getPointer();
        return ret;
    }

    public GBFRSW_LicenseUsbScannerCheck() {
        super();
        setAlignType(ALIGN_NONE);
    }

    public GBFRSW_LicenseUsbScannerCheck(Pointer p) {
        this();
        if (p == Pointer.NULL) {
            return;
        }
        setAlignType(ALIGN_NONE);
        useMemory(p);
        read();
    }

    /**
     * @see GBFRSW_UsbScannerLicenseCheckResults
     */
    public int Result;
    /**
     * Native is an int32 see point (1) of ReadME.txt file
     */
    public int UsbDriverErrorCodeDetail;
}
