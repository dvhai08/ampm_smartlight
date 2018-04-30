/**
 ******************************************************************************
  * File Name          : mbedtls.c
  * Description        : This file provides code for the configuration
  *                      of the mbedtls instances.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mbedtls.h"

/* USER CODE BEGIN 0 */


const char AWS_IOT_ROOT_CA [] = \
"-----BEGIN CERTIFICATE-----\n"\
"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\n"\
"yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\n"\
"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\n"\
"U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\n"\
"ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\n"\
"aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\n"\
"MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\n"\
"ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\n"\
"biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\n"\
"U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\n"\
"aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\n"\
"nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\n"\
"t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\n"\
"SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\n"\
"BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\n"\
"rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\n"\
"NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\n"\
"BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\n"\
"BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\n"\
"aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\n"\
"MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\n"\
"p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\n"\
"5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\n"\
"WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\n"\
"4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\n"\
"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\n"\
"-----END CERTIFICATE-----\0";

const char AWS_IOT_CERTIFICATE [] = \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDWjCCAkKgAwIBAgIVAOVxVIBiCr0971qltlrDyaTZP/cxMA0GCSqGSIb3DQEB\n"\
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"\
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xNzExMDIwNDU0\n"\
"MzlaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"\
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCNpzn3fDPV2Cw0v1xl\n"\
"m41TRQMC+w0HnL+fsM5kiBCWN1KOfOQ79mhXYOpD84aHFsknUrmPfZO5WB8jB3dz\n"\
"/3qbJOhV44gRxn3Czw/7anmUZmmf9Kp7AUkOONQPJuUrmP3+zq0/NJxPNUIzpFEi\n"\
"ehpgwoQmKMtW2Q6F3bq5mi1pjhFJ7zLe1Z0JF9jHvTbwlPi2nBRkkjbl/ZJ18+65\n"\
"h/t8OVKqyfgPZ17xPnlZ0TnJ2WKTJRR4OIWpsT5Lb8KGZUCMITmOe2iDJdEiCHxR\n"\
"ctzccLoT1IIjQsNd0vhp6zNV+cnqfWKJLJ3rIWeIaqx9q2108vv0vJZEa5Vijdh5\n"\
"z/dHAgMBAAGjYDBeMB8GA1UdIwQYMBaAFDgC6SHUEbNqN+YU7t9VnFmAoe0LMB0G\n"\
"A1UdDgQWBBRzpMxCIs8u7a2sg7F22nunvI15KTAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"\
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAkWyPspT4ICdzcUq7d0m+HTOH\n"\
"2mCdEheaU5S0hTfQ4kjCXbJZjUceeombM5lZXvm12r/o/4GhypgbgROJrcXgEECc\n"\
"3/IBz5uc4DcdSuQkxveFWeiiVR7Um+6UxP8Q/11JfPPWc+nJkmnz0tL3g8uckDUt\n"\
"jk1fIIzkQeswZICvLnipBve19druRU/IYEctSeTabrKn9mydAWP75WiFNWR60Cs0\n"\
"grOlOts5jEMEcX2+uDsEdheSM/xWgGlEGcIQ4lAvZNkRyfkr7Q7lmMMRodaH4Jj3\n"\
"Y485jT9BhAIsl6Ae+xxUEXLqlXs9g0AQB40nErMfs4qv0uLeoo3OwBFpn9SglQ==\n"\
"-----END CERTIFICATE-----\0";

const char AWS_IOT_PRIVATE_KEY [] = \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEogIBAAKCAQEAjac593wz1dgsNL9cZZuNU0UDAvsNB5y/n7DOZIgQljdSjnzk\n"\
"O/ZoV2DqQ/OGhxbJJ1K5j32TuVgfIwd3c/96myToVeOIEcZ9ws8P+2p5lGZpn/Sq\n"\
"ewFJDjjUDyblK5j9/s6tPzScTzVCM6RRInoaYMKEJijLVtkOhd26uZotaY4RSe8y\n"\
"3tWdCRfYx7028JT4tpwUZJI25f2SdfPuuYf7fDlSqsn4D2de8T55WdE5ydlikyUU\n"\
"eDiFqbE+S2/ChmVAjCE5jntogyXRIgh8UXLc3HC6E9SCI0LDXdL4aeszVfnJ6n1i\n"\
"iSyd6yFniGqsfattdPL79LyWRGuVYo3Yec/3RwIDAQABAoIBAHXWJNyBz8UWvT9r\n"\
"6PsFeX1JrYWvPlmUxbucIzVyEwpDH0Q1FM/k3UvC7V51mPWLuO4jXQXG2zqV2eET\n"\
"9wTiBCZHf5bjXnAbBMcn5Q2qt1UCFN/hcmg8238fOxkXrBCmhrSPTYOEynsBvP6k\n"\
"tHHPzV2ssq3jxbOLgQ9PMmPvo9NoAcZiXiBJvxm1pmRx0+Mg0swdB0ndzPNOSy1E\n"\
"vd7Kw75rlLwHblfH6Ox71mGtueSPmhUKitbw4TBy/86xdJK7jrajnNiJYT3T+JRJ\n"\
"ZeGExAJ0XrAbt+Y7prpnagPbw4ym6nMSa4bTT5s45xOYFEXFe0t3L7CH55TKI85n\n"\
"sWSYWfECgYEA2Z7eUajqiD8/tXfXQVr4UgX0E7v3rJcLiIWWyvjdp7UmkqX24gxR\n"\
"g/3hIA7MtNFfl25bwIBc3ko11NDSQMe35wSYMXix7JMNsGHvaix5tYXrjpp7XO4r\n"\
"wlq6EHvUajCit4z2rG1GMgvgoF9fsO8QgN7bTbWSW1Xpr4mgrWkmFwUCgYEApqKV\n"\
"t/Xl0yxgEYij2gtbnbeFrgnPEmc8M0z5BaVe99vGaVG/qa+fdGUQC08mPI1eiWQ7\n"\
"VKf8TDRYtanUSk56QPXrkYluffEj8L2IwKan7gHWEkGBTYssaWzkAv10UUDdNpaH\n"\
"I/uBVvutq3Z4Mh8rBUzMwC0d2hDJjScJeY6YDtsCgYAeGQLWeG43ecft6S00NMiz\n"\
"T296azzN+fX2Y8N/Pvwo/bzf+2Qxz1JW1v4T2Pgm6e42m2TFMvulIs5fKKJrozOk\n"\
"F9aUwQSam5wBEQpzvML4gBUcG/GD4MzcjH+/V+EA7skSacSUBGj48vuWMpqhde7q\n"\
"Y+ggz0P2reMsC/TVBv3loQKBgCUoYCZzaNpcLNf8Fj2LBtT2lZKnkUtTRfNSmL0P\n"\
"kUvH8EiP+CNIjFASMS+0aJxcEdalSS2pf/MmQRnRUwt20IAn1m3PrZnb6d3HBfWf\n"\
"tBa4xhmwDbtS64Zex8F67zMnOazwiBYYAVF6MdHoSwkyNg+qfoXkCaIU2DznlY9L\n"\
"otC9AoGAJO27t9IFX/goRLkzU2AC9jLrXEUdfFuxLxsUR01MoylVhdf1YwKRVuzC\n"\
"9+8+K5AE4Hc97lARlFCGkvqnW9YPasXykqu14bUDZ2d/LGWlXz1sQPFJDLWRfcbp\n"\
"JziHmWu+S2yXxHckTkr5wO2j1tvQdbwEcRxGZS/4DzaD8+VUppA=\n"\
"-----END RSA PRIVATE KEY-----\0";


char PASSWORD [] = "";
/* USER CODE END 0 */

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */

/* Global variables ---------------------------------------------------------*/

/* USER CODE BEGIN 2 */
/* USER CODE END 2 */

/* init function */
void MX_MBEDTLS_Init(void)
{
  /** 
  */

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */

}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @}
  */
 
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
