#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>
#include <stddef.h>

#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/debug.h"
#include "mbedtls/net.h"

static mbedtls_entropy_context entropy;
static mbedtls_ctr_drbg_context ctr_drbg;
static mbedtls_ssl_context ssl;
static mbedtls_ssl_config conf;
static mbedtls_x509_crt cacert;
static mbedtls_ctr_drbg_context ctr_drbg;

/*---------HEAP_3 calloc --------------------------------------------------*/

void *pvPortCalloc(size_t num, size_t size)
{
    void *pvReturn;

    vTaskSuspendAll();
    {
        pvReturn = calloc(num, size);
        traceMALLOC(pvReturn, xWantedSize);
    }
    (void)xTaskResumeAll();

#if (configUSE_MALLOC_FAILED_HOOK == 1)
    {
        if (pvReturn == NULL)
        {
            extern void vApplicationMallocFailedHook(void);
            vApplicationMallocFailedHook();
        }
    }
#endif

    return pvReturn;
}
static void my_debug( void *ctx, int level,
                      const char *file, int line, const char *str )
{
    ((void) level);
    fprintf( (FILE *) ctx, "%s:%04d: %s", file, line, str );
    fflush(  (FILE *) ctx  );
}

static int tls_init()
{
    int ret;
    const char *pers = "nguyen tuan cuong";
    /*
     * 0. Initialize the RNG and the session data
     */
    mbedtls_ssl_init( &ssl );
    mbedtls_ssl_config_init( &conf );
    mbedtls_x509_crt_init( &cacert );
    mbedtls_ctr_drbg_init( &ctr_drbg );
    mbedtls_entropy_init( &entropy );
    mbedtls_printf( "\n  . Seeding the random number generator...\n" );
    mbedtls_entropy_init( &entropy );
    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                             (const unsigned char *) pers,
                             strlen(pers ) ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret );
        return -1;
    }
      /*
       * First prepare the SSL configuration by setting the endpoint and transport type, and loading reasonable
       * defaults for security parameters. The endpoint determines if the SSL/TLS layer will act as a server (MBEDTLS_SSL_IS_SERVER)
       * or a client (MBEDTLS_SSL_IS_CLIENT). The transport type determines if we are using TLS (MBEDTLS_SSL_TRANSPORT_STREAM)
       * or DTLS (MBEDTLS_SSL_TRANSPORT_DATAGRAM).
       */
      if( ( ret = mbedtls_ssl_config_defaults( &conf,
                      MBEDTLS_SSL_IS_CLIENT,
                      MBEDTLS_SSL_TRANSPORT_STREAM,
                      MBEDTLS_SSL_PRESET_DEFAULT ) ) != 0 )
      {
          mbedtls_printf(" failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret );
          return -1;
      }
      /* The authentication mode determines how strict the certificates that are presented are checked.  */
      mbedtls_printf("  . Loading the CA root certificate ...\n");
      ret = mbedtls_x509_crt_parse(&cacert, (const unsigned char *)mbedtls_azure_ca_crt, mbedtls_azure_ca_crt_len );
    if(ret != 0)
    {
        mbedtls_printf( " failed\n  !  mbedtls_x509_crt_parse returned -0x%x\n\n", -ret );
    }

    mbedtls_ssl_conf_ca_chain( &conf, &cacert, NULL );
    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_REQUIRED);
    /* The library needs to know which random engine to use and which debug function to use as callback. */
    mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );
    mbedtls_ssl_conf_dbg( &conf, my_debug, stdout );
    mbedtls_ssl_setup(&ssl, &conf);
}