package com.greenbit.ansinistitl;

public class GBANJavaWrapperDefinesFingersAlternateQualityDescription
{
    public String FingID;
    public String Quality;
    public String VendorId;
    public String AlgorithmId;

    public void Build (String fingID,String quality, String vendorId, String algorithmId)
    {
        FingID = fingID;
        Quality = quality;
        VendorId = vendorId;
        AlgorithmId = algorithmId;
    }
}
