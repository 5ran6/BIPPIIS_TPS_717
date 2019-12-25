package com.greenbit.jpeg;

import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;

public class GbjpegJavaWrapperLibrary {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("GBJPEG_JNI");
    }

    public native String GetLastErrorString();

    public native int Load();

    public native int Unload();

    public native int Jp2GetParamsOfEncodedBuffer(
            // INPUT
            byte[] Source, int Sx, int Sy, int BitsPerPixel, int Rate,
            // OUTPUT
            GBJavaWrapperUtilIntForJavaToCExchange DestinationLen
    );

    public native int Jp2Encode(
            // INPUT
            byte[] Source, int Sx, int Sy, int BitsPerPixel, int Rate,
            // OUTPUT
            byte [] Destination
    );

    public native int Jp2GetParamsOfEncodedBufferFP(
            // INPUT
            byte[] Source, int Sx, int Sy, int BitsPerPixel, double Rate,
            // OUTPUT
            GBJavaWrapperUtilIntForJavaToCExchange DestinationLen
    );

    public native int Jp2EncodeFP(
            // INPUT
            byte[] Source, int Sx, int Sy, int BitsPerPixel, double Rate,
            // OUTPUT
            byte [] Destination
    );

    public native int Jp2GetParametersOfDecodedBuffer(
            // INPUT
            byte[] Source,
            // OUTPUT
            GBJavaWrapperUtilIntForJavaToCExchange Sx,
            GBJavaWrapperUtilIntForJavaToCExchange Sy,
            GBJavaWrapperUtilIntForJavaToCExchange BitsPerPixel
    );

    public native int Jp2Decode(
            // INPUT
            byte[] Source,
            // OUTPUT
            byte [] Destination,
            GBJavaWrapperUtilIntForJavaToCExchange Sx,
            GBJavaWrapperUtilIntForJavaToCExchange Sy,
            GBJavaWrapperUtilIntForJavaToCExchange BitsPerPixel
    );

    public native int JpegGetParamsOfEncodedBuffer(
            // INPUT
            byte[] Source, int Sx, int Sy, int BitsPerPixel, int Dpi, int Quality,
            // OUTPUT
            GBJavaWrapperUtilIntForJavaToCExchange DestinationLen
    );

    public native int JpegEncode(
            // INPUT
            byte[] Source, int Sx, int Sy, int BitsPerPixel, int Dpi, int Quality,
            // OUTPUT
            byte [] Destination
    );

    public native int JpegGetParametersOfDecodedBuffer(
            // INPUT
            byte[] Source,
            // OUTPUT
            GBJavaWrapperUtilIntForJavaToCExchange Sx,
            GBJavaWrapperUtilIntForJavaToCExchange Sy,
            GBJavaWrapperUtilIntForJavaToCExchange BitsPerPixel
    );

    public native int JpegDecode(
            // INPUT
            byte[] Source,
            // OUTPUT
            byte [] Destination,
            GBJavaWrapperUtilIntForJavaToCExchange Sx,
            GBJavaWrapperUtilIntForJavaToCExchange Sy,
            GBJavaWrapperUtilIntForJavaToCExchange BitsPerPixel
    );

    public native int Jp2SetDpi(
            // INPUT
            int Dpi
    );
}
