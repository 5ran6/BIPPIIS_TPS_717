package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

import com.greenbit.gbmsapi.*;
import com.greenbit.utils.GBJavaWrapperUtilIntForJavaToCExchange;

import java.util.ArrayList;
import java.util.List;

public class DeviceFeaturesActivity extends AppCompatActivity {
    private Spinner comboScannableObjects, comboDeviceFeatures, comboSupportedScanAreas,
    comboSupportedScanOptions, comboOptionalExternalEquipment;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_features);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        String checkGbmsapi;
        int RetVal;
        GBMSAPIJavaWrapperDefinesDeviceInfoStruct currentDevice = new GBMSAPIJavaWrapperDefinesDeviceInfoStruct();
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetCurrentDevice(currentDevice);
        if (RetVal == GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            checkGbmsapi = GBMSAPIJavaWrapperDefinesDeviceName.DevIDToString(currentDevice.DeviceID) + ", " + currentDevice.DeviceSerialNum;
            this.setTitle(checkGbmsapi);
        } else {
            checkGbmsapi = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            this.setTitle(checkGbmsapi);
            return;
        }

        comboScannableObjects = (Spinner)findViewById(R.id.ScannableObjectsComboBox);
        GBJavaWrapperUtilIntForJavaToCExchange objTypesMask = new GBJavaWrapperUtilIntForJavaToCExchange();
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetScannableTypes(objTypesMask);
        if (RetVal == GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            List<String> objTypes = GBMSAPIJavaWrapperDefinesScannableBiometricType.ScannableTypesToStringList(objTypesMask.Get());
            objTypes.add(0,"- SCANNABLE TYPES");
            ArrayAdapter<String> objectsToAcquireAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item, objTypes);
            objectsToAcquireAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            comboScannableObjects.setAdapter(objectsToAcquireAdapter);
        } else {
            checkGbmsapi = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            List<String> objTypes = new ArrayList<String>();
            objTypes.add(0,checkGbmsapi);
            ArrayAdapter<String> objectsToAcquireAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item, objTypes);
            objectsToAcquireAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            comboScannableObjects.setAdapter(objectsToAcquireAdapter);
            return;
        }

        comboDeviceFeatures = (Spinner)findViewById(R.id.DeviceFeaturesComboBox);
        GBJavaWrapperUtilIntForJavaToCExchange DeviceFeaturesMask = new GBJavaWrapperUtilIntForJavaToCExchange();
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetDeviceFeatures(DeviceFeaturesMask);
        if (RetVal == GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            List<String> devFeaturesList = GBMSAPIJavaWrapperDefinesDeviceFeatures.DeviceFeaturesToStringList(DeviceFeaturesMask.Get());
            devFeaturesList.add(0,"- DEVICE FEATURES");
            ArrayAdapter<String> objectsToAcquireAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item, devFeaturesList);
            objectsToAcquireAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            comboDeviceFeatures.setAdapter(objectsToAcquireAdapter);
        } else {
            checkGbmsapi = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            List<String> objTypes = new ArrayList<String>();
            objTypes.add(0,checkGbmsapi);
            ArrayAdapter<String> objectsToAcquireAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item, objTypes);
            objectsToAcquireAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            comboDeviceFeatures.setAdapter(objectsToAcquireAdapter);
            return;
        }

        comboSupportedScanAreas = (Spinner)findViewById(R.id.SupportedScanAreasComboBox);
        GBJavaWrapperUtilIntForJavaToCExchange ScanAreas = new GBJavaWrapperUtilIntForJavaToCExchange();
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetSupportedScanAreas(ScanAreas);
        if (RetVal == GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            List<String> scanAreasList = GBMSAPIJavaWrapperDefinesScannerAcquisitionAreas.ScannerAcquisitionAreasToStringList(ScanAreas.Get());
            scanAreasList.add(0,"- SCANNER AREAS");
            ArrayAdapter<String> objectsToAcquireAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item, scanAreasList);
            objectsToAcquireAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            comboSupportedScanAreas.setAdapter(objectsToAcquireAdapter);
        } else {
            checkGbmsapi = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            List<String> objTypes = new ArrayList<String>();
            objTypes.add(0,checkGbmsapi);
            ArrayAdapter<String> objectsToAcquireAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item, objTypes);
            objectsToAcquireAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            comboSupportedScanAreas.setAdapter(objectsToAcquireAdapter);
            return;
        }

        comboSupportedScanOptions = (Spinner)findViewById(R.id.SupportedScanOptionsComboBox);
        GBJavaWrapperUtilIntForJavaToCExchange ScanOptions = new GBJavaWrapperUtilIntForJavaToCExchange();
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetSupportedScanOptions(ScanOptions);
        if (RetVal == GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            List<String> scanOptionsList = GBMSAPIJavaWrapperDefinesAcquisitionOptions.ScanOptionsToStringList(ScanOptions.Get());
            scanOptionsList.add(0,"- ACQUISITION OPTIONS");
            ArrayAdapter<String> objectsToAcquireAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item, scanOptionsList);
            objectsToAcquireAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            comboSupportedScanOptions.setAdapter(objectsToAcquireAdapter);
        } else {
            checkGbmsapi = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            List<String> objTypes = new ArrayList<String>();
            objTypes.add(0,checkGbmsapi);
            ArrayAdapter<String> objectsToAcquireAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item, objTypes);
            objectsToAcquireAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            comboSupportedScanOptions.setAdapter(objectsToAcquireAdapter);
            return;
        }

        comboOptionalExternalEquipment = (Spinner)findViewById(R.id.OptionalExternalEquipmentComboBox);
        GBJavaWrapperUtilIntForJavaToCExchange optExtEquip = new GBJavaWrapperUtilIntForJavaToCExchange();
        RetVal = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetOptionalExternalEquipment(optExtEquip);
        if (RetVal == GBMSAPIJavaWrapperDefinesReturnCodes.GBMSAPI_ERROR_CODE_NO_ERROR) {
            List<String> optExtEquipList = GBMSAPIJavaWrapperDefinesOptionalExternalEquipment.OptionalExternalEquipmentToStringList(optExtEquip.Get());
            optExtEquipList.add(0,"- EXTERNAL EQUIPMENT");
            ArrayAdapter<String> objectsToAcquireAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item, optExtEquipList);
            objectsToAcquireAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            comboOptionalExternalEquipment.setAdapter(objectsToAcquireAdapter);
        } else {
            checkGbmsapi = GB_AcquisitionOptionsGlobals.GBMSAPI_Jw.GetLastErrorString();
            List<String> objTypes = new ArrayList<String>();
            objTypes.add(0,checkGbmsapi);
            ArrayAdapter<String> objectsToAcquireAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item, objTypes);
            objectsToAcquireAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            comboOptionalExternalEquipment.setAdapter(objectsToAcquireAdapter);
            return;
        }

    }

}
