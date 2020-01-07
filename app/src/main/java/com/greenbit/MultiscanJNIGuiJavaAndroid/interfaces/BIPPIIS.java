package com.greenbit.MultiscanJNIGuiJavaAndroid.interfaces;

import com.greenbit.MultiscanJNIGuiJavaAndroid.models.FingerprintRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.FingerprintResponse;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.LoginRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.LoginResponse;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.PassportRequest;
import com.greenbit.MultiscanJNIGuiJavaAndroid.models.PassportResponse;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.POST;

public interface BIPPIIS {
//    POST ->  https://bippiis.com/api/v1/user/uploadPassport
//    data -> passport, type base64 string
//
//    POST -> https://bippiis.com/api/v1/enroll
//    data{ fingerprints, type array, bippiis_number, type string}
//
//    GET -> https://bippiis.com/api/v1/user
//    data{ fingerprints, type array, bippiis_number, type string}

    @POST("enroll")
    Call<FingerprintResponse> getFingerprintResponse(@Body FingerprintRequest fingerprintRequest);

    @POST("user/uploadPassport")
    Call<PassportResponse> getPassportResponse(@Body PassportRequest passportRequest);
//    Call<PassportResponse> getPassportResponse(@Body PassportRequest passportRequest, @Header("Authorization") String token);

    @POST("auth")
    Call<LoginResponse> getLoginResponse(@Body LoginRequest loginRequest);

    @GET("user")
    Call<LoginResponse> getUserInformation(@Body LoginRequest loginRequest);

}
