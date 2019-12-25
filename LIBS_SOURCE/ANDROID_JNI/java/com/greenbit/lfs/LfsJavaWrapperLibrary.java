package com.greenbit.lfs;

import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;

public class LfsJavaWrapperLibrary {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("LFS_JNI");
    }

    public native String GetLastErrorString();

    public native int Load();

    public native int Unload();

    /****************************
     *
     * @param ImageBuffer : its dimension must be not less than @param ImageSizeX * @param ImageSizeY
     * @param ImageSizeX : > 0
     * @param ImageSizeY : > 0
     * @param BitsPerPixel : for images acquired from Greenbit scanners, it's 8
     * @param PixelPerMillimeter : for 500 dpi images, use 19.68 (1 ppmm =~ 25.4 dpi)
     * @param MinutiaeArray : its size must be >= 1000
     * @param MinutiaeNum : returns the number of detected minutiae
     * @return
     */
    public native int GetMinutiae(
            byte [] ImageBuffer,
            int ImageSizeX, int ImageSizeY,
            int BitsPerPixel, int PixelPerMillimeter,
            LfsJavaWrapperDefinesMinutia [] MinutiaeArray,
            GBJavaWrapperUtilIntForJavaToCExchange MinutiaeNum
    );
}
