package com.greenbit.gbfrsw;

public class GbfrswJavaWrapperDefinesISOFMRGBProprietaryData {
    /**
     * no proprietary data section will be stored
     * into FMR record; in this case the FMR will be the smallest
     * but reverse conversion back to Green Bit Template code
     * is not more possible;
     */
    public static final int GBFRSW_ISO_FMR_GB_PROPR_DATA_NONE = 0;
    /**
     * proprietary data section will be stored
     * into FMR record in uncompressed format; in this case the FMR will
     * be bigger and reverse conversion back to Green Bit Template code
     * will be performed in a faster way;
     */
    public static final int GBFRSW_ISO_FMR_GB_PROPR_DATA_FAST = 1;
    /**
     * proprietary data section will be stored
     * into FMR record in compresseed format; in this case the FMR will
     * be smaller respect to uncompressed form and reverse conversion back
     * to Green Bit Template code will require more time;
     */
    public static final int GBFRSW_ISO_FMR_GB_PROPR_DATA_COMPRESSED = 2;
    /**
     * no Extended Data Block (proprietary data, Zonal quality and Core
     * and Delta data) is included in the record, but only minutiae;
     * reverse conversion back to Green Bit Template code is not possible;
     */
    public static final int GBFRSW_ISO_FMR_EXCLUDE_EXTENDED_DATA_BLOCK = 3;
}
