/*
 * Copyright 2020 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file
 *
 * API for the HTTP / HTTPS Web Server
 *
 * Web pages and other resources are provided via an array which gets passed as an argument when starting the web server.
 * The array is constructed using the START_OF_HTTP_PAGE_DATABASE() and END_OF_HTTP_PAGE_DATABASE() macros, and optionally the ROOT_HTTP_PAGE_REDIRECT() macro.
 * The following is an example of a list of web pages (taken from one of the demo apps).
 *
 * START_OF_HTTP_PAGE_DATABASE(web_pages)
 *     ROOT_HTTP_PAGE_REDIRECT("/apps/temp_control/main.html"),
 *     { "/apps/temp_control/main.html",    "text/html",                CY_RESOURCE_URL_CONTENT,  .url_content.resource_data  = &resources_apps_DIR_temp_control_DIR_main_html, },
 *     { "/temp_report.html",               "text/html",                CY_DYNAMIC_URL_CONTENT,   .url_content.dynamic_data   = {process_temperature_update, 0 }, },
 *     { "/temp_up",                        "text/html",                CY_DYNAMIC_URL_CONTENT,   .url_content.dynamic_data   = {process_temperature_up, 0 }, },
 *     { "/temp_down",                      "text/html",                CY_DYNAMIC_URL_CONTENT,   .url_content.dynamic_data   = {process_temperature_down, 0 }, },
 *     { "/images/favicon.ico",             "image/vnd.microsoft.icon", CY_RESOURCE_URL_CONTENT,  .url_content.resource_data  = &resources_images_DIR_favicon_ico, },
 *     { "/scripts/general_ajax_script.js", "application/javascript",   CY_RESOURCE_URL_CONTENT,  .url_content.resource_data  = &resources_scripts_DIR_general_ajax_script_js, },
 *     { "/images/cypresslogo.png",         "image/png",                CY_RESOURCE_URL_CONTENT,  .url_content.resource_data  = &resources_images_DIR_cypresslogo_png, },
 *     { "/images/cypressogo_line.png",     "image/png",                CY_RESOURCE_URL_CONTENT,  .url_content.resource_data  = &resources_images_DIR_cypresslogo_line_png, },
 *     { "/styles/buttons.css",             "text/css",                 CY_RESOURCE_URL_CONTENT,  .url_content.resource_data  = &resources_styles_DIR_buttons_css, },
 * END_OF_HTTP_PAGE_DATABASE();
 */


/**
********************************************************************************
* \mainpage Overview
********************************************************************************
* This library provides the HTTP Server implementation that can work on the PSoC 6 MCU platforms with Wi-Fi connectivity.
* It supports RESTful methods such as GET, PUT, and POST for the client to communicate with this HTTP Server library.
*
* \section section_features Features
* Following are the features supported by this library:
* * Secure [with TLS security] and non-secure modes of connection.
* * RESTful HTTP methods: GET, PUT, and POST.
* * Various resource content types such as HTML, Plain, and JSON.
* * Content payload greater than the MTU size using the Content-Length HTTP header. This feature is supported only for CY_RAW_DYNAMIC_URL_CONTENT and CY_DYNAMIC_URL_CONTENT content types.
* * Chunked encoding for GET and POST methods.
*   > Note: For POST requests, chunked encoding is supported only for data that is less than a single MTU; Content-Length headers are recommended for larger data.
* * Server-Sent Events (SSE). SSE is a server push technology, enabling an HTTP client (for example, a browser or any device running an HTTP client) to receive automatic updates from the HTTP server via the HTTP connection.
*
* \section section_platforms Supported Platforms
* * [PSoC 6 WiFi-BT Prototyping Kit (CY8CPROTO-062-4343W)](https://www.cypress.com/documentation/development-kitsboards/psoc-6-wi-fi-bt-prototyping-kit-cy8cproto-062-4343w)
* * [PSoC 6 WiFi-BT Pioneer Kit (CY8CKIT-062-WiFi-BT)](https://www.cypress.com/documentation/development-kitsboards/psoc-6-wifi-bt-pioneer-kit-cy8ckit-062-wifi-bt)
* * [PSoC 62S2 Wi-Fi BT Pioneer Kit (CY8CKIT-062S2-43012)](https://www.cypress.com/documentation/development-kitsboards/psoc-62s2-wi-fi-bt-pioneer-kit-cy8ckit-062s2-43012)
*
* \section section_frameworks Supported Frameworks
* This middleware library supports the following frameworks:
* * AnyCloud Framework : AnyCloud is a FreeRTOS-based solution. HTTP server library uses the [abstraction-rtos](https://github.com/cypresssemiconductorco/abstraction-rtos) library that provides the RTOS abstraction API and uses the [secure-sockets](https://github.com/cypresssemiconductorco/secure-sockets) library for implementing socket functions.
* * Mbed framework: Mbed framework is a Mbed OS-based solution. HTTP server library uses the [abstraction-rtos](https://github.com/cypresssemiconductorco/abstraction-rtos) library that provides RTOS abstraction API and uses the Mbed socket API for implementing socket functions.
* 
* \section section_dependencies Dependencies
* This HTTP Server library depends on the following libraries. All these libraries are included by default.
* \subsection section_anycloud AnyCloud
* * [Wi-Fi Middleware Core](https://github.com/cypresssemiconductorco/wifi-mw-core)
*
* \subsection section_mbed Mbed OS
* * [Arm Mbed OS stack version 5.15.3](https://os.mbed.com/mbed-os/releases)
* * [Connectivity Utilities Library](https://github.com/cypresssemiconductorco/connectivity-utilities)
*
* \section section_quick_start Quick Start
* This library is supported on both AnyCloud and Mbed OS frameworks. The following section provides information on how to build the library in those framework
* \subsection framework_1 AnyCloud
* * A set of pre-defined configuration files have been bundled with the wifi-mw-core library for FreeRTOS, lwIP, and mbed TLS. You should review the configuration and make the required adjustments. See the "Quick Start" section in <a href="https://github.com/cypresssemiconductorco/wifi-mw-core/blob/master/README.md">README.md</a>
* * A set of COMPONENTS must be defined in the application's makefile for the HTTP Server Library. See the "Quick Start" section in <a href="https://github.com/cypresssemiconductorco/wifi-mw-core/blob/master/README.md">README.md</a>
* * HTTP Server Library disables all the debug log messages by default. To enable log messages, the application must perform the following:
*      -# Add `ENABLE_HTTP_SERVER_LOGS` macro to the *DEFINES* in the code example's Makefile. The Makefile entry would look like as follows:
*         \code
*           DEFINES+=ENABLE_HTTP_SERVER_LOGS
*         \endcode
*      -# Call the `cy_log_init()` function provided by the *cy-log* module. cy-log is part of the *connectivity-utilities* library. See [connectivity-utilities library API documentation](https://cypresssemiconductorco.github.io/connectivity-utilities/api_reference_manual/html/group__logging__utils.html) for cy-log details.
* * Define the below macro in application Makefile to configure maximum number of HTTP server resource :
*   ```
*   MAX_NUMBER_OF_HTTP_SERVER_RESOURCES
*   ```
*
* \subsection framework_2 Mbed OS
* * HTTP Server Library disables all the debug log messages by default. To enable log messages, the application must perform the following:
*      -# Add `ENABLE_HTTP_SERVER_LOGS` macro to the *DEFINES* in the code example's JSON file. The JSON file entry would look like as follows:
*         \code
*           "macros": ["ENABLE_HTTP_SERVER_LOGS"],
*         \endcode
*      -# Call the `cy_log_init()` function provided by the *cy-log* module. cy-log is part of the *connectivity-utilities* library. See [connectivity-utilities library API documentation](https://cypresssemiconductorco.github.io/connectivity-utilities/api_reference_manual/html/group__logging__utils.html) for cy-log details.
* * Define the following macro in the application JSON file to configure the maximum number of HTTP server resources:
*   ```
*   MAX_NUMBER_OF_HTTP_SERVER_RESOURCES
*   ```
*
********************************************************************************
* \section section_code_snippet Code Snippets
********************************************************************************
* This section provides code snippets for this library on AnyCloud and Mbed OS frameworks. The code snippets given under the AnyCloud section uses C APIs, whereas the snippets
* given under Mbed OS use C++ Class to demonstrate the library usage.  In general, the library features are tested on Mbed OS using C++ Class interface, and tested on AnyCloud using C APIs.
* * AnyCloud - Snippets for HTTP Server create, start, stop, and delete (C implementation).
* * Mbed OS - Snippets for HTTP server create, start, stop, and delete (C++ implementation).
*
********************************************************************************
* \section section_code_snippet_AnyCloud AnyCloud
********************************************************************************
********************************************************************************
* These code snippets demonstrate the initialization of the configuration structures required for the operation such as creating a secure or non-secure connection and registering a static or dynamic resource with the HTTP server.
* \subsection snip1 Code Snippet 1: Creation and Initialization of Handle for Non-Secure HTTP Server
* Creates an HTTP server handle for a non-secure HTTP connection using the \ref cy_http_server_create API function.
* \snippet doxygen_http_code_snippet.c snippet_http_non_secure_create
* \subsection snip2 Code Snippet 2:  Creation and Initialization of Handle for Secure HTTP Server
* Creates an HTTP server handle for a secure HTTP connection using the \ref cy_http_server_create API function.
* \snippet doxygen_http_code_snippet.c snippet_http_secure_create
* \subsection snip3 Code Snippet 3: Registering a Static Resource
* Registers a static resource with the HTTP server using the \ref cy_http_server_register_resource API function.
* \snippet doxygen_http_code_snippet.c snippet_http_static_reg_resource
* \subsection snip4 Code Snippet 4: Registering a Dynamic Resource
* Registers a dynamic resource with the HTTP server using the \ref cy_http_server_register_resource API function.
* \snippet doxygen_http_code_snippet.c snippet_http_dynamic_reg_resource
* \subsection snip5 Code Snippet 5: Registering a Server-Sent Event (SSE)
* Registers a Server-Sent Event using the \ref cy_http_server_register_resource API function.
* \snippet doxygen_http_code_snippet.c snippet_http_server_event_reg
* \subsection snip6 Code Snippet 6: HTTP Server Start
* Starts the HTTP server using the \ref cy_http_server_start API function.
* \snippet doxygen_http_code_snippet.c snippet_http_server_start
* \subsection snip7 Code Snippet 7: Sending HTTP Server Events
* Sends HTTP server events using the \ref cy_http_server_response_stream_write_header API function.
* \snippet doxygen_http_code_snippet.c snippet_http_server_send_events
* \subsection snip8 Code Snippet 8: HTTP Server Stop
* Stops the HTTP server using the \ref cy_http_server_stop API function.
* \snippet doxygen_http_code_snippet.c snippet_http_server_stop
* \subsection snip9 Code Snippet 9: HTTP URL query
* Search a key-value pair, fetch the value of a key, and get the number of queries from the given URL using URL-processing functions.
* \snippet doxygen_http_code_snippet.c snippet_http_server_url_query
********************************************************************************
********************************************************************************
* \section section_code_snippet_Mbed Mbed OS
********************************************************************************
********************************************************************************
* These code snippets demonstrate the initialization of the configuration structures required for the operation such as creating a secure or non-secure connection and registering a static or dynamic resource with the HTTP server.
* \subsection snip10 Code Snippet 1: Creation and initialization of a non-secure HTTP server object
* Creates a non-secure HTTP connection using the HTTPServer() constructor.
* \snippet doxygen_Mbed_http_code_snippet.cpp snippet_http_non_secure_new
* \subsection snip11 Code Snippet 2: Creation and initialization of a secure HTTP server object
* Creates a secure HTTP connection using the HTTPServer() constructor.
* \snippet doxygen_Mbed_http_code_snippet.cpp snippet_http_secure_new
* \subsection snip12 Code Snippet 3: Registering a static resource
* Registers a static resource with the HTTP server using the register_resource() API function.
* \snippet doxygen_Mbed_http_code_snippet.cpp snippet_mbed_http_static_reg_resource
* \subsection snip13 Code Snippet 4: Registering a dynamic resource
* Registers a dynamic resource with the HTTP server using the register_resource() API function.
* \snippet doxygen_Mbed_http_code_snippet.cpp snippet_mbed_http_dynamic_reg_resource
* \subsection snip14 Code Snippet 5: Registering a Server-Sent Events
* Registers a Server-Sent Events using the register_resource() API function.
* \snippet doxygen_Mbed_http_code_snippet.cpp snippet_mbed_http_server_event_reg
* \subsection snip15 Code Snippet 6: HTTP server start
* Starts an HTTP server using the start() API function.
* \snippet doxygen_Mbed_http_code_snippet.cpp snippet_mbed_http_server_start
* \subsection snip16 Code Snippet 7: Sending HTTP server events
* Sends HTTP server events using the http_response_stream_write() API function.
* \snippet doxygen_Mbed_http_code_snippet.cpp snippet_mbed_http_server_send_events
* \subsection snip17 Code Snippet 8: HTTP server stop
* Stops an HTTP server using the stop() API function.
* \snippet doxygen_Mbed_http_code_snippet.cpp snippet_mbed_http_server_stop
* \subsection snip18 Code Snippet 9: HTTP URL query
* Search a key-value pair, fetch the value of a key, and get the number of queries from the given URL using URL-processing functions
* \snippet doxygen_Mbed_http_code_snippet.cpp snippet_mbed_http_server_url_query
********************************************************************************
*/
/**
 * \defgroup group_c_api C API
 * \defgroup http_server_struct Structures & Enumerations
 * @ingroup group_c_api
 * \defgroup http_server_defines Macros
 * @ingroup group_c_api
 * \defgroup group_c_api_functions Functions
 * @ingroup group_c_api
 */
#pragma once

#include <stdbool.h>
#include "stdio.h"
#include "cy_tcpip_port.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/
/**
 * @addtogroup http_server_defines
 *
 * HTTP server preprocessor directives such as results and error codes.
 *
 * Middleware APIs return results of type cy_rslt_t and are composed of three parts:
 * - module base
 * - type
 * - error code
 *
 * \par Result Format
 *
   \verbatim
              Module base         Type    Library specific error code
      +---------------------------+------+------------------------------+
      |CY_RSLT_MODULE_HTTP_SERVER | 0x2  |           Error Code         |
      +---------------------------+------+------------------------------+
                14-bits            2-bits            16-bits

   See the macro section of this document for library-specific error codes.
   \endverbatim
 *
 * The cy_rslt_t data structure is a part of cy_result.h in Mbed OS PSoC 6 MCU target platform, located in <mbed-os/targets/TARGET_Cypress/TARGET_PSOC6/psoc6csp/core_lib/include>
 *
 * Module base: This base is derived from CY_RSLT_MODULE_MIDDLEWARE_BASE (defined in cy_result.h) and is an offset of the CY_RSLT_MODULE_MIDDLEWARE_BASE.
 *              The details of the offset and the middleware base are defined in cy_result_mw.h, which is part of [Github connectivity-utilities] (https://github.com/cypresssemiconductorco/connectivity-utilities)
 *              For example, the HTTP server uses CY_RSLT_MODULE_HTTP_SERVER as the module base.
 *
 * Type: This type is defined in cy_result.h and can be one of CY_RSLT_TYPE_FATAL, CY_RSLT_TYPE_ERROR, CY_RSLT_TYPE_WARNING or CY_RSLT_TYPE_INFO. AWS library error codes are of type CY_RSLT_TYPE_ERROR.
 *
 * Library-specific error code: These error codes are library-specific and defined in the macro section.
 *
 * Helper macros used for creating library-specific results are provided as part of cy_result.h.
 *
 *  @{
 */
/**
 * Results returned by HTTP Server Library
 */
#define CY_RSLT_MODULE_HTTP_SERVER_ERR_CODE_START       (0)

/** HTTP server error code base */        
#define CY_RSLT_HTTP_SERVER_ERR_BASE                    CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_HTTP_SERVER, CY_RSLT_MODULE_HTTP_SERVER_ERR_CODE_START)

/** Out of memory */
#define CY_RSLT_HTTP_SERVER_ERROR_NO_MEMORY             ((cy_rslt_t)(CY_RSLT_HTTP_SERVER_ERR_BASE + 1))
/** Error initializing thread */
#define CY_RSLT_HTTP_SERVER_ERROR_THREAD_INIT           ((cy_rslt_t)(CY_RSLT_HTTP_SERVER_ERR_BASE + 2))
/** Error initializing queue */
#define CY_RSLT_HTTP_SERVER_ERROR_QUEUE_INIT            ((cy_rslt_t)(CY_RSLT_HTTP_SERVER_ERR_BASE + 3))
/** Error initializing mutex */
#define CY_RSLT_HTTP_SERVER_ERROR_MUTEX_INIT            ((cy_rslt_t)(CY_RSLT_HTTP_SERVER_ERR_BASE + 4))
/** Failed to start TCP server */
#define CY_RSLT_HTTP_SERVER_ERROR_TCP_SERVER_START      ((cy_rslt_t)(CY_RSLT_HTTP_SERVER_ERR_BASE + 5))
/** Feature not supported */        
#define CY_RSLT_HTTP_SERVER_ERROR_UNSUPPORTED           ((cy_rslt_t)(CY_RSLT_HTTP_SERVER_ERR_BASE + 6))
/** Bad argument/parameter */        
#define CY_RSLT_HTTP_SERVER_ERROR_BADARG                ((cy_rslt_t)(CY_RSLT_HTTP_SERVER_ERR_BASE + 7))
/** Resource not found */        
#define CY_RSLT_HTTP_SERVER_ERROR_NOT_FOUND             ((cy_rslt_t)(CY_RSLT_HTTP_SERVER_ERR_BASE + 8))
/** Partially processed. Returned by application's receive callback */        
#define CY_RSLT_HTTP_SERVER_ERROR_PARTIAL_RESULTS       ((cy_rslt_t)(CY_RSLT_HTTP_SERVER_ERR_BASE + 9))
/** Exceeded maximum number of resources */
#define CY_RSLT_HTTP_SERVER_PAGE_DATABASE_FULL          ((cy_rslt_t)(CY_RSLT_HTTP_SERVER_ERR_BASE + 10))
/** HTTP server generic error */        
#define CY_RSLT_ERROR                                   ((cy_rslt_t)(CY_RSLT_HTTP_SERVER_ERR_BASE + 11))

/**
 * Max number of resources supported by the HTTP server.
 * \note Change this macro to support more resources.
 */
#ifndef MAX_NUMBER_OF_HTTP_SERVER_RESOURCES
#define MAX_NUMBER_OF_HTTP_SERVER_RESOURCES            (10)
#endif

/**
 * @}
 */

/**
 * @addtogroup http_server_struct
 *
 * HTTP server data structures and type definitions.
 *
 * @{
 */
/******************************************************
 *                    Constants
 ******************************************************/
/** Macro to expand the enumeration */
#define EXPAND_AS_ENUMERATION(a,b)   a,

/** MIME table */
#define MIME_TABLE( ENTRY ) \
    ENTRY( MIME_TYPE_TLV = 0 ,                "application/x-tlv8"               ) \
    ENTRY( MIME_TYPE_APPLE_BINARY_PLIST,      "application/x-apple-binary-plist" ) \
    ENTRY( MIME_TYPE_APPLE_PROXY_AUTOCONFIG,  "application/x-ns-proxy-autoconfig") \
    ENTRY( MIME_TYPE_BINARY_DATA,             "application/octet-stream"         ) \
    ENTRY( MIME_TYPE_JAVASCRIPT,              "application/javascript"           ) \
    ENTRY( MIME_TYPE_JSON,                    "application/json"                 ) \
    ENTRY( MIME_TYPE_HAP_JSON,                "application/hap+json"             ) \
    ENTRY( MIME_TYPE_HAP_PAIRING,             "application/pairing+tlv8"         ) \
    ENTRY( MIME_TYPE_HAP_VERIFY,              "application/hap+verify"           ) \
    ENTRY( MIME_TYPE_TEXT_HTML,               "text/html"                        ) \
    ENTRY( MIME_TYPE_TEXT_PLAIN,              "text/plain"                       ) \
    ENTRY( MIME_TYPE_TEXT_EVENT_STREAM,       "text/event-stream"                ) \
    ENTRY( MIME_TYPE_TEXT_CSS,                "text/css"                         ) \
    ENTRY( MIME_TYPE_IMAGE_PNG,               "image/png"                        ) \
    ENTRY( MIME_TYPE_IMAGE_GIF,               "image/gif"                        ) \
    ENTRY( MIME_TYPE_IMAGE_MICROSOFT,         "image/vnd.microsoft.icon"         ) \
    ENTRY( MIME_TYPE_ALL,                     "*/*"                              ) /* This must always be the last mime*/

/******************************************************
 *                   Enumerations
 ******************************************************/
/**
 * HTTP request type
 */
typedef enum
{
    CY_HTTP_REQUEST_GET,       /**< HTTP server GET request type */
    CY_HTTP_REQUEST_POST,      /**< HTTP server POST request type */
    CY_HTTP_REQUEST_PUT,       /**< HTTP server PUT request type*/
    CY_HTTP_REQUEST_UNDEFINED  /**< HTTP server undefined request type */
} cy_http_request_type_t;

/**
 * HTTP cache
 */
typedef enum
{
    CY_HTTP_CACHE_DISABLED, /**< Do not cache previously fetched resources */
    CY_HTTP_CACHE_ENABLED   /**< Allow caching of previously fetched resources  */
} cy_http_cache_t;

/**
 * HTTP MIME type.
 * \note Refer to the 1st argument of \ref MIME_TABLE for the list of MIME types supported. Example: MIME_TYPE_TLV.
 */
typedef enum
{
    /**
     * \cond
     */
    MIME_TABLE( EXPAND_AS_ENUMERATION )  /**< HTTP MIME type */
    MIME_UNSUPPORTED                     /**< HTTP unsupported MIME type */
    /**
     * \endcond
     */
} cy_http_mime_type_t;

/**
 * HTTP status code
 */
typedef enum
{
    CY_HTTP_200_TYPE, /**< OK */
    CY_HTTP_204_TYPE, /**< No Content */
    CY_HTTP_207_TYPE, /**< Multi-Status */
    CY_HTTP_301_TYPE, /**< Moved Permanently */
    CY_HTTP_400_TYPE, /**< Bad Request */
    CY_HTTP_403_TYPE, /**< Forbidden */
    CY_HTTP_404_TYPE, /**< Not Found */
    CY_HTTP_405_TYPE, /**< Method Not Allowed */
    CY_HTTP_406_TYPE, /**< Not Acceptable */
    CY_HTTP_412_TYPE, /**< Precondition Failed */
    CY_HTTP_415_TYPE, /**< Unsupported Media Type */
    CY_HTTP_429_TYPE, /**< Too Many Requests */
    CY_HTTP_444_TYPE, /**< No Response */
    CY_HTTP_470_TYPE, /**< Connection Authorization Required */
    CY_HTTP_500_TYPE, /**< Internal Server Error */
    CY_HTTP_504_TYPE  /**< Gateway Timeout */
} cy_http_status_codes_t;

/**
 * HTTP server resource type
 */
typedef enum
{
    CY_STATIC_URL_CONTENT,                 /**< Page is constant data in memory-addressable area. */
    CY_DYNAMIC_URL_CONTENT,                /**< Page is dynamically generated by a @ref url_processor_t type function. */
    CY_RESOURCE_URL_CONTENT,               /**< Page data is provided by a Resource which may reside off-chip. Currently this URL type is not supported in Mbed OS and AnyCloud frameworks. */
    CY_RAW_STATIC_URL_CONTENT,             /**< Same as @ref CY_STATIC_URL_CONTENT, but the HTTP header must be supplied as part of the content. */
    CY_RAW_DYNAMIC_URL_CONTENT,            /**< Same as @ref CY_DYNAMIC_URL_CONTENT, but the HTTP header must be supplied as part of the content. */
    CY_RAW_RESOURCE_URL_CONTENT            /**< Same as @ref CY_RESOURCE_URL_CONTENT, but the HTTP header must be supplied as part of the content. */
} cy_url_resource_type;

/******************************************************
 *                 Type Definitions
 ******************************************************/
/** HTTP server handle */
typedef void* cy_http_server_t;

/******************************************************
 *                    Structures
 ******************************************************/
/**
 * HTTP message structure that gets passed to dynamic URL processor functions
 */
typedef struct
{
    const uint8_t             *data;                        /**< Packet data in message body      */
    uint16_t                  data_length;                  /**< Data length in current packet    */
    uint32_t                  data_remaining;               /**< Data yet to be consumed          */
    bool                      is_chunked_transfer;          /**< Chunked data format              */
    cy_http_mime_type_t       mime_type;                    /**< MIME type                        */
    cy_http_request_type_t    request_type;                 /**< Request type                     */
} cy_http_message_body_t;

/**
 * Context structure for HTTP server stream
 * Users should not access these values - they are provided here only
 * to provide the compiler with datatype size information that allows static declarations.
 */
typedef struct
{
    cy_tcp_stream_t tcp_stream;                /**< TCP stream handle */
    bool            chunked_transfer_enabled;  /**< Flag to indicate whether chunked transfer is enabled */
} cy_http_response_stream_t;

/**
 * Prototype for URL processor functions
 *
 * @param[in] url_path           : URL path.
 * @param[in] url_query_string   : NULL terminated URL query string.
 * @param[in] stream             : HTTP stream on which data was received.
 * @param[in] arg                : Arguments passed along with callback function.
 * @param[in] http_data          : Buffer having HTTP data
 *
 */
typedef int32_t (*url_processor_t)( const char *url_path, const char *url_query_string, cy_http_response_stream_t *stream, void *arg, cy_http_message_body_t *http_data );

/** HTTP server security info */
typedef struct
{
    uint8_t     *private_key;               /**< HTTP server private key (base64 encoded) */
    uint16_t    key_length;                 /**< HTTP server private key length excluding 'null' termination character */
    uint8_t     *certificate;               /**< HTTP server certificate */
    uint16_t    certificate_length;         /**< HTTP server certificate length excluding 'null' termination character */
    uint8_t     *root_ca_certificate;       /**< Root CA certificate to verify client certificate */
    uint16_t    root_ca_certificate_length; /**< Root CA certificate length excluding 'null' termination character */
} cy_https_server_security_info_t;

/** Dynamic HTTP resource info */
typedef struct cy_resource_dynamic_data_s
{
    url_processor_t resource_handler;       /**< The function that will handle requests for this page */
    void            *arg;                   /**< Argument to be passed to the generator function    */
} cy_resource_dynamic_data_t;

/** Static HTTP resource info */
typedef struct cy_resource_static_data_s
{
    const void  *data;                       /**< A pointer to the data for the page/file resource */
    uint32_t    length;                     /**< The length in bytes of the page/file */
} cy_resource_static_data_t;

/**
 * @}
 */
/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/
/*****************************************************************************/
/**
 *
 * @addtogroup group_c_api_functions
 *
 * C API provided by HTTP server library.
 *
 * @{
 */
/*****************************************************************************/
/**
 * One-time initialization function for network sockets implementation.
 * <b>It must be called once (and only once) before calling any other function in this library.</b>
 * @return cy_rslt_t             : CY_RSLT_SUCCESS on success; error codes in @ref http_server_defines otherwise.
**/
cy_rslt_t cy_http_server_network_init( void );

/**
 * One-time deinitialization function for Secure Sockets implementation.
 * It should be called after destroying all network socket connections.
 * @return cy_rslt_t             : CY_RSLT_SUCCESS on success; error codes in @ref http_server_defines otherwise.
**/
cy_rslt_t cy_http_server_network_deinit( void );

/**
 * Creates a HTTP server instance and initializes its members based on the supplied arguments.
 * Handle to the HTTP server instance is returned via the handle pointer supplied by the user on successful return.
 * Handle to the HTTP server instance is used for server start, stop, and delete. This API function must be called before using any other HTTP Server API.
 *
 * @param[in] interface          : Pointer to the network interface information structure (included as part of cy_nw_helper.h). Used for server start.
 *                                 \note Refer code snippet \ref snip6 to understand how to initialize the 'interface' before calling this function.
 * @param[in] port               : Port number on which the server listens for client connection requests. Usually port number 443 is used for the HTTPS server, and port number 80 is used for the HTTP server.
 * @param[in] max_connection     : Maximum number of client connections that can be accepted.
 * @param[in] security_info      : Security info containing the certificate, private key, and rootCA certificate.
 *                                 For a non-secured connection, this parameter should be NULL.
 * @param[out] server_handle     : Pointer to store the HTTP sever handle allocated by this function on successful return.
 *
 * @return cy_rslt_t             : CY_RSLT_SUCCESS on success; error codes in @ref http_server_defines otherwise.
**/
cy_rslt_t cy_http_server_create( cy_network_interface_t *interface,
                                 uint16_t port,
                                 uint16_t max_connection,
                                 cy_https_server_security_info_t *security_info,
                                 cy_http_server_t *server_handle );

/**
 * Deletes the given HTTP server instance and resources allocated for the instance by the \ref cy_http_server_create function.
 * Before calling this API function, the HTTP server associated with server_handle must be stopped.
 *
 * @param[in] server_handle      : HTTP server handle
 * @return cy_rslt_t             : CY_RSLT_SUCCESS on success; error codes in @ref http_server_defines otherwise.
**/
cy_rslt_t cy_http_server_delete( cy_http_server_t server_handle );

/**
 * Starts a HTTP server daemon (web server)
 *
 * The web server implements HTTP 1.1 using a non-blocking architecture which allows
 * multiple sockets to be served simultaneously.
 * Web pages and other files can be served either dynamically from a function or
 * from static data in memory or internal/external flash resources.
 * Prior to calling this API, API \ref cy_http_server_create must be called for creating HTTP{ server instance.
 *
 * @param[in] server_handle      : HTTP server handle created using \ref cy_http_server_create.
 * @return cy_rslt_t             : CY_RSLT_SUCCESS on success; error codes in @ref http_server_defines otherwise.
**/
cy_rslt_t cy_http_server_start( cy_http_server_t server_handle );

/**
 * Stops a HTTP server daemon (web server)
 * Before calling this API function, API \ref cy_http_server_start must be called to start HTTP server.
 *
 * @param[in] server_handle      : HTTP server handle created using \ref cy_http_server_create.
 * @return cy_rslt_t             : CY_RSLT_SUCCESS on success; error codes in @ref http_server_defines otherwise.
**/
cy_rslt_t cy_http_server_stop( cy_http_server_t server_handle );

/**
 * Used to register a resource(static/dynamic) with the HTTP server.
 * All static resources must have been registered before calling \ref cy_http_server_start.
 *
 * @param[in] server_handle       : HTTP server handle created using \ref cy_http_server_create.
 * @param[in] url                 : URL of the resource. The application should reserve memory for the URL.
 * @param[in] mime_type           : MIME type of the resource. The application should reserve memory for the MIME type.
 * @param[in] url_resource_type   : Content type of the resource.
 * @param[in] resource_data       : Pointer to the static or dynamic resource type structure.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS on success; error codes in @ref http_server_defines otherwise.
 */
cy_rslt_t cy_http_server_register_resource( cy_http_server_t server_handle, uint8_t *url, uint8_t *mime_type, cy_url_resource_type url_resource_type, void *resource_data );

/**
 * Enables chunked transfer encoding on the HTTP stream.
 *
 * @param[in] stream              : Pointer to the HTTP stream.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS on success; error codes from @ref http_server_defines otherwise.
 */
cy_rslt_t cy_http_server_response_stream_enable_chunked_transfer( cy_http_response_stream_t *stream );

/**
 * Disables chunked transfer encoding on the HTTP stream.
 *
 * @param[in] stream              : Pointer to the HTTP stream.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS on success; error codes from @ref http_server_defines otherwise.
 */
cy_rslt_t cy_http_server_response_stream_disable_chunked_transfer( cy_http_response_stream_t *stream );

/**
 * Writes HTTP header to the HTTP stream provided.
 *
 * @param[in] stream              : Pointer to the HTTP stream.
 * @param[in] status_code         : HTTP status code.
 * @param[in] content_length      : HTTP content length to follow, in bytes.
 * @param[in] cache_type          : HTTP cache type (enabled or disabled).
 *                                  In AnyCloud and Mbed frameworks, the caching feature is not supported; therefore, this parameter should be always \ref CY_HTTP_CACHE_DISABLED.
 * @param[in] mime_type           : HTTP MIME type.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS on success; error codes from @ref http_server_defines otherwise.
 */
cy_rslt_t cy_http_server_response_stream_write_header( cy_http_response_stream_t *stream, cy_http_status_codes_t status_code, uint32_t content_length, cy_http_cache_t cache_type, cy_http_mime_type_t mime_type );

/**
 * Writes data to the HTTP stream.
 *
 * @param[in] stream              : HTTP stream to write the data into.
 * @param[in] data                : data to write.
 * @param[in] length              : data length in bytes.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS on success; error codes from @ref http_server_defines otherwise.
 */
cy_rslt_t cy_http_server_response_stream_write_payload( cy_http_response_stream_t *stream, const void *data, uint32_t length );

/**
 * Writes resource to HTTP stream.
 * Currently not supported in AnyCloud and Mbed frameworks.
 *
 * @param[in] stream              : HTTP stream to write the resource into.
 * @param[in] resource            : Pointer to resource.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS on success; error codes from @ref http_server_defines otherwise.
 */
cy_rslt_t cy_http_server_response_stream_write_resource( cy_http_response_stream_t *stream, const void *resource );

/**
 * Flushes the HTTP stream.
 *
 * @param[in] stream              : HTTP stream to flush.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS on success; error codes from @ref http_server_defines otherwise.
 */
cy_rslt_t cy_http_server_response_stream_flush( cy_http_response_stream_t *stream );

/**
 * Queues a disconnect request to the HTTP server.
 *
 * @param[in] stream              : Pointer to the HTTP stream.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS on success; error codes from @ref http_server_defines otherwise.
 */
cy_rslt_t cy_http_server_response_stream_disconnect( cy_http_response_stream_t *stream );

/**
 * Disconnects all the HTTP streams associated with the given server.
 *
 * @param[in] server_handle       : HTTP server handle created using \ref cy_http_server_create.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS on success; error codes from @ref http_server_defines otherwise.
 */
cy_rslt_t cy_http_server_response_stream_disconnect_all( cy_http_server_t server_handle );

/**
 * Searches for a parameter (key-value pair) in a URL query string and returns a pointer to the value.
 *
 * @param[in]  url_query          : NULL-terminated URL query string.
 * @param[in]  parameter_key      : NULL-terminated Key or name of the parameter to find in the URL query string.
 * @param[out] parameter_value    : If the parameter with the given key is found, this pointer will point to the parameter value upon return; NULL otherwise.
 * @param[out] value_length       : This variable will contain the length of the parameter value upon return; 0 otherwise.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS if found; CY_RSLT_NOT_FOUND otherwise.
 */
cy_rslt_t cy_http_server_get_query_parameter_value( const char *url_query, const char *parameter_key, char **parameter_value, uint32_t *value_length );

/**
 * Returns the number of parameters found in the URL query string.
 *
 * @param[in] url_query           : NULL terminated URL query string.
 * @param[out] count              : Parameter count.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS on success; error codes from @ref http_server_defines otherwise.
 */
cy_rslt_t cy_http_server_get_query_parameter_count( const char *url_query, uint32_t *count );

/**
 * Checks whether the given parameter key-value pair is present in the given URL query.
 *
 * @param[in]  url_query          : NULL-terminated URL query string.
 * @param[in]  parameter_key      : NULL-terminated key or name of the parameter to find in the URL query string.
 * @param[out] parameter_value    : NULL-terminated value of the parameter to find in the URL query string.
 *
 * @return cy_rslt_t              : CY_RSLT_SUCCESS if matched; CY_RSLT_NOT_FOUND if matching parameter is not found.
 */
cy_rslt_t cy_http_server_match_query_parameter( const char *url_query, const char *parameter_key, const char *parameter_value );
/**
 * @}
 */
#ifdef __cplusplus
} /* extern "C" */
#endif