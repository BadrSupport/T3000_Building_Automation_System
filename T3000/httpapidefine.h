#pragma once
const int VALUE_TYPE_ANALYSIS_ERROR = -1;
const int VALUE_TYPE_INVALID = 0;
const int VALUE_TYPE_CSTRING = 1;
const int VALUE_TYPE_INT = 2;
const int VALUE_TYPE_FLOAT = 3;

typedef struct
{
    CString  cs_value;
    int      n_value;
    float    f_value;

}ValueType;

typedef struct
{
    CString KeyName;
    int nStatus;  //״̬ -1 ��    
    ValueType tValue;
}Http_key;

typedef struct
{
    Http_key k_productID;
    Http_key k_serialNumber;
    Http_key k_ipAddress;
    Http_key k_ipPort;
    Http_key k_hardwareVer;
    Http_key k_softwareVer;
    Http_key k_lastCommunicationDate;
    Http_key k_createdAt;
    Http_key k_updatedAt;
}StrGetProductID;

typedef struct
{
    Http_key k_serialNumber;
    Http_key k_ipAddress;
    Http_key k_ipPort;
    Http_key k_hardwareVer;
    Http_key k_softwareVer;
}StrPostProductID;




