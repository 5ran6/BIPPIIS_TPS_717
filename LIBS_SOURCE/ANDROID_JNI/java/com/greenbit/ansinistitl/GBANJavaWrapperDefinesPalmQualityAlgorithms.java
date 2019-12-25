package com.greenbit.ansinistitl;

public class GBANJavaWrapperDefinesPalmQualityAlgorithms
{
    public String PalmID;
    public String Quality;
    public String VendorId;
    public String AlgorithmId;

    public void Build (String palmID,String quality, String vendorId, String algorithmId)
    {
        PalmID = palmID;
        Quality = quality;
        VendorId = vendorId;
        AlgorithmId = algorithmId;
    }
}
