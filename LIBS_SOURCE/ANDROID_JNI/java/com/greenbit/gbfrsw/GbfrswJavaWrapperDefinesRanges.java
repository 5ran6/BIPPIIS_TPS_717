package com.greenbit.gbfrsw;

public class GbfrswJavaWrapperDefinesRanges {
    // Quality
    public static final int GBFRSW_FINGER_CODE_MAX_QUALITY = 100;
    public static final int GBFRSW_FINGER_CODE_MIN_QUALITY = 0;

    // matching score
    public static final double GBFRSW_MIN_MATCHING_SCORE = (-10000);
    public static final double GBFRSW_MAX_MATCHING_SCORE = (3.4E+36F);

    // rotation angles: range
    public static final int GBFRSW_ROTATION_ANGLE_NO_LIMITS = 0;
    public static final int GBFRSW_ROTATION_ANGLE_MINIMAL_ACCEPTABLE = 20;
    public static final int GBFRSW_ROTATION_ANGLE_MAXIMAL_ACCEPTABLE = 180;
    // rotation angles: recommended
    public static final int GBFRSW_ROTATION_ANGLE_MIN_RECOMMENDED = 40;
    public static final int GBFRSW_ROTATION_ANGLE_MAX_RECOMMENDED = 60;

}
