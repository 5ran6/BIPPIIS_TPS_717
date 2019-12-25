package com.greenbit.wsq;

import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;

public class WsqJavaWrapperLibrary {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("WSQ_JNI");
    }

    public native int Load();

    public native int Unload();

    public native String GetLastErrorString();

    public native int GetCompressedFileSize(
            double Bitrate,
            byte [] Source, int Width, int Height,
            int BitsPerPixel, int Ppi,
            String Comment,
            GBJavaWrapperUtilIntForJavaToCExchange CompressedFileSize
    );

    /**
     *
     * @param Bitrate: recommended 0.75 (15:1), alternative 2.25 (5:1)
     */
    public native int Compress (
            byte [] Destination, double Bitrate,
            byte [] Source, int Width, int Height,
            int BitsPerPixel, int Ppi,
            String Comment
    );

    public native int GetUncompressedOutputParameters(
            GBJavaWrapperUtilIntForJavaToCExchange Width, GBJavaWrapperUtilIntForJavaToCExchange Height,
            GBJavaWrapperUtilIntForJavaToCExchange BitsPerPixel, GBJavaWrapperUtilIntForJavaToCExchange Ppi,
            GBJavaWrapperUtilIntForJavaToCExchange LossyFlag,
            byte [] Source
    );

    public native int Uncompress(
            byte [] Destination,
            GBJavaWrapperUtilIntForJavaToCExchange Width, GBJavaWrapperUtilIntForJavaToCExchange Height,
            GBJavaWrapperUtilIntForJavaToCExchange BitsPerPixel, GBJavaWrapperUtilIntForJavaToCExchange Ppi,
            GBJavaWrapperUtilIntForJavaToCExchange LossyFlag,
            byte [] Source
    );
}
