package com.greenbit.ansinistitl;

/***********************************************************************/
/* COMPRESSION ALGORITHMS DEFINITIONS */
// Values preceded by a "BIN_" are to be used with type-4 records

/***********************************************************************/
public class GBANJavaWrapperDefinesCompressionAlgorithmsStrings
{
    // no compression
    public static final String COMP_NONE = "NONE";
    public static final String BIN_COMP_NONE = "0";
    // wsq 2.0
    public static final String COMP_WSQ = "WSQ20";
    public static final String BIN_COMP_WSQ = "1";
    // jpegb (lossy)
    public static final String COMP_JPEGB = "JPEGB";
    public static final String BIN_COMP_JPEGB = "2";
    // jpegl (lossless)
    public static final String COMP_JPEGL = "JPEGL";
    public static final String BIN_COMP_JPEGL = "3";
    // jpeg2000 (lossy)
    public static final String JPG2K_COMP = "JP2";
    public static final String BIN_COMP_JPEG2K = "4";
    // jpeg2000 (lossless)
    public static final String JPG2KL_COMP = "JP2L";
    public static final String BIN_COMP_JPEG2KL = "5";
    // png
    public static final String PNG_COMP = "PNG";
    public static final String BIN_COMP_PNG = "6";
}
