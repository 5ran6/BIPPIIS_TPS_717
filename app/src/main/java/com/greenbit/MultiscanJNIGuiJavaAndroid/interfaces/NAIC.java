package com.greenbit.MultiscanJNIGuiJavaAndroid.interfaces;


import com.greenbit.MultiscanJNIGuiJavaAndroid.models.ArmyUserResponse;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.FingerprintRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.FingerprintResponse;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.LoginRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.PassportRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.UserRegisterRequest;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.POST;

public interface NAIC {
//    POST ->  https://bippiis.com/api/v1/user/uploadPassport
//    data -> passport, type base64 string
//
//    POST -> https://bippiis.com/api/v1/enroll
//    data{ fingerprints, type array, bippiis_number, type string}
//
//    GET -> https://bippiis.com/api/v1/user
//    data{ fingerprints, type array, bippiis_number, type string}

    @POST("enrollFingerPrint")
    Call<FingerprintResponse> getFingerprintResponse(@Body FingerprintRequest fingerprintRequest);

    @POST("uploadPassport")
    Call<ResponseBody> getPassportResponse(@Body PassportRequest passportRequest);

    @POST("verifyUser")
    Call<ArmyUserResponse> getLoginResponse(@Body LoginRequest loginRequest);

    @POST("signup")
    Call<ResponseBody> getSignUpResponse(@Body UserRegisterRequest userRequest);

//    @GET("user")
//    Call<ResponseBody> getUserResponse();
}
