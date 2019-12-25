package com.greenbit.gbmsapi;

public interface IGbmsapiAcquisitionManagerCallback {
    boolean AcquisitionEventsManagerCallback(
            int OccurredEventCode, int GetFrameErrorCode, int EventInfo,
            byte[] FramePtr,
            int FrameSizeX, int FrameSizeY,
            double CurrentFrameRate, double NominalFrameRate,
            int GB_Diagnostic,
            Object UserDefinedParameters
    );
}
