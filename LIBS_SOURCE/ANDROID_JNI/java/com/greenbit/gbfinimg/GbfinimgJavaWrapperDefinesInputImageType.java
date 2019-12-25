package com.greenbit.gbfinimg;

public class GbfinimgJavaWrapperDefinesInputImageType {
    /**
     * Added in the wrapper for comfortability
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_NO_TYPE = 0;
    /**
     * LEFT HAND SLAP 4:  INDEX+MIDDLE+RING+LITTLE
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_4 = 1;
    /**
     * RIGHT HAND SLAP 4: INDEX+MIDDLE+RING+LITTLE
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_4 = 2;
    /**
     * THUMB LEFT + THUMB RIGHT
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_THUMBS_2 = 3;
    /**
     * FLAT OR ROLLED
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER = 4;
    /**
     * LEFT HAND SLAP 2:  INDEX+MIDDLE
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_2 = 5;
    /**
     * RIGHT HAND SLAP 2: INDEX+MIDDLE
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_2 = 6;
    /**
     * LEFT LOWER HALF PALM
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM = 7;
    /**
     * RIGHT LOWER HALF PALM
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM = 8;
    /**
     * LEFT WRITER'S PALM
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_WRITER_PALM = 9;
    /**
     * RIGHT WRITER'S PALM
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_WRITER_PALM = 10;
    /**
     * use only for FLAT single fingers to obtain correct
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER_FLAT = 11;
    /**
     *
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_ROLLED_THENAR = 12;
    /**
     *
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_ROLLED_THENAR = 13;
    /**
     * FV1, Full Finger Rolled View
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_JOINT_FINGER_FV1 = 14;
    /**
     * FV2, Full Finger Plain Image – left side
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV2 = 15;
    /**
     * FV3, Full Finger Plain Image – center, then vertically rolled onto fingertip
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV3 = 16;
    /**
     * FV4, Full Finger Plain Image – right side
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV4 = 17;
    /**
     *
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_TIP = 18;
    /**
     * LEFT UPPER HALF PALM
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM_UPPER = 19;
    /**
     * RIGHT UPPER HALF PALM
     */
    public static final int GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM_UPPER = 20;

    public static String GetStringFromInputImageType(int inputImageType)
    {
        switch (inputImageType)
        {
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV2:
                return "FLAT JF FV2";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV3:
                return "FLAT JF FV3";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_FLAT_JOINT_FINGER_FV4:
                return "FLAT JF FV4";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_2:
                return "LEFT-2";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_4:
                return "LEFT-4";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM:
                return "LEFT LOWER PALM";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_HALF_PALM_UPPER:
                return "LEFT UPPER PALM";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_ROLLED_THENAR:
                return "LEFT ROLLED THENAR";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_LEFT_HAND_WRITER_PALM:
                return "LEFT WRITER PALM";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_NO_TYPE:
                return "NO TYPE";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_2:
                return "RIGHT-2";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_4:
                return "RIGHT-4";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM:
                return "RIGHT LOWER PALM";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_HALF_PALM_UPPER:
                return "RIGHT UPPER PALM";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_ROLLED_THENAR:
                return "RIGHT ROLLED THENAR";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_RIGHT_HAND_WRITER_PALM:
                return "RIGHT WRITER PALM";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_JOINT_FINGER_FV1:
                return "ROLLED JF FV1";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_ROLLED_TIP:
                return "ROLLED TIP";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER:
                return "ROLLED SINGLE FINGER";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_SINGLE_FINGER_FLAT:
                return "FLAT SINGLE FINGER";
            case GbfinimgJavaWrapperDefinesInputImageType.GBFINIMG_INPUT_IMAGE_TYPE_THUMBS_2:
                return "THUMBS-2";
                default:
                    return "NOT RECOGNIZED";
        }
    }
}
