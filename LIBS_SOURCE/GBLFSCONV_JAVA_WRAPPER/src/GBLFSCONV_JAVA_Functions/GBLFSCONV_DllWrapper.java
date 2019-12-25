/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package GBLFSCONV_JAVA_Functions;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.NativeLong;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.ByteByReference;
import com.sun.jna.ptr.PointerByReference;
import com.sun.jna.ptr.DoubleByReference;
import com.sun.jna.ptr.FloatByReference;
import com.sun.jna.ptr.ShortByReference;
import com.sun.jna.WString;
import java.nio.ByteBuffer;

/**
 *
 * @author andrea
 */
public class GBLFSCONV_DllWrapper {

    public interface GBLfsConv_Library extends Library {

        GBLfsConv_Library INSTANCE =
                (GBLfsConv_Library) Native.loadLibrary("LfsConv", GBLfsConv_Library.class);

        /**
         * 
         * @param FMR 
         * @param FormatType @see GBFRSW_JAVA_Defines.GBFRSW_FMR_Formats
         * @param Minutiae MINUTIAE **
         * @return @see GBLFSCONV_JAVA_Defines.GBLFSCONV_ReturnCodes
         */
        int FMRMinutiaeToLfs(byte[] FMR,
                byte FormatType,
                Pointer Minutiae);
        
        /**
         * 
         * @param FMC
         * @param FMCLength
         * @param FormatType @see GBFRSW_JAVA_Defines.GBFRSW_FMC_FormatTypes
         * @param Minutiae MINUTIAE **
         * @return @see GBLFSCONV_JAVA_Defines.GBLFSCONV_ReturnCodes
         */
        int FMCMinutiaeToLfs(byte[] FMC,
                short FMCLength,
                byte FormatType,
                Pointer Minutiae);
        
        /**
         * 
         * @param Minutiae MINUTIAE ** 
         */
        void FreeMinutiae(Pointer Minutiae);
    }
}
