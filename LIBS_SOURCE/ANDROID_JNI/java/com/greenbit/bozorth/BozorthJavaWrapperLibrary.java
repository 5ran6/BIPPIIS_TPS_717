package com.greenbit.bozorth;

import com.greenbit.lfs.LfsJavaWrapperDefinesMinutia;
import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;

public class BozorthJavaWrapperLibrary {
    // Used to load the 'native-lib' library on application startup.

    static {
        System.loadLibrary("BOZORTH_JNI");
    }

    public static final int BOZORTH_MAX_MINUTIAE = 1000;

    public static final int BOZORTH_NO_ERROR = 0;

    public static final int BOZORTH_ERROR = 1;

    public native String GetLastErrorString();

    public native int Load();

    public native int Unload();

    /****************************
     *
     * @param maxminutiae : max number of minutiae that are to be considered for matching
     * @param probe : pointer to the minutiae of the first fingerprint
     * @param gallery : pointer to the minutiae of the second fingerprint
     * @return
     */
    public native int bozDirectCall(
            int maxminutiae,
            LfsJavaWrapperDefinesMinutia[] probe,
            int probeLength,
            LfsJavaWrapperDefinesMinutia[] gallery,
            int galleryLength,
            GBJavaWrapperUtilIntForJavaToCExchange score
    );
}
