package com.greenbit.gbfinimg;

public class GbfinimgJavaWrapperDefinesMissingDeltaMask {
    /**
     * left delta (information is availavle only for "Whorl" pattern )
     */
    public static final int GBFINIMG_MISSING_DELTA_MASK_LEFT = 0x01;
    /**
     * right delta (information is availavle only for "Whorl" pattern )
     */
    public static final int GBFINIMG_MISSING_DELTA_MASK_RIGHT = 0x02;
    /**
     * unknown delta type (information is availavle for other pattern types)
     */
    public static final int GBFINIMG_MISSING_DELTA_MASK_UNKNOWN = 0x04;
}
