package com.greenbit.gbfrsw;

public class GbfrswJavaWrapperDefinesISOFMRFormat {
    /**
     * FMR in <ISO/IEC 19794-2> format will be created
     */
    public static final int GBFRSW_ISO_FMR_FORMAT_ISO = 1;
    /**
     * FMR in <INCITS 378> format will be created
     */
    public static final int GBFRSW_ISO_FMR_FORMAT_INCITS = 2;
    /**
     * FMR in <INCITS 378> format will be created, with special
     * requirements for MINEX test; note that GBProprietaryData
     * must be set to GBFRSW_ISO_FMR_GB_PROPR_DATA_NONE for this format type.
     * @see {@link GbfrswJavaWrapperDefinesISOFMRGBProprietaryData}
     */
    public static final int GBFRSW_ISO_FMR_FORMAT_MINEX = 3;
//					    - F
}
