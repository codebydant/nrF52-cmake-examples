#ifndef RANDOM_NUMBER_SERVICE_H
#define RANDOM_NUMBER_SERVICE_H

#include "ble.h"
#include "ble_srv_common.h"
#include "boards.h"
#include "nrf_sdh_ble.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef BLE_RANDOM_NUMBER_BLE_OBSERVER_PRIO
#define BLE_RANDOM_NUMBER_BLE_OBSERVER_PRIO 2
#endif

#define BLE_RANDOM_NUMBER_DEF(_name)                                                                                   \
  static ble_random_number_service_t _name;                                                                            \
  NRF_SDH_BLE_OBSERVER(_name##_obs, BLE_RANDOM_NUMBER_BLE_OBSERVER_PRIO, ble_random_number_service_on_ble_evt, &_name)

// RANDOM NUMBER Service UUID: 0000-0001-5000-47D4-A9B6-FA44-E9C1-5422
// RANDOM NUMBER Data /Out Characteristic UUID: 0000-0002-5000-47D4-A9B6-FA44-E9C1-5422
// write (encryption recommended, write without response is required)
// read (encryption recommended, respond with no payload)
// notify (encryption recommended)

// The bytes need to be in reverse order to match the UUID from the spec
// 0000-0001-5000-47D4-A9B6-FA44-E9C1-5422
// 22-54-C1-E9-44-FA-B6-A9-D4-47-00-50-01-00-00-00

/**************************************************/
/*** 		        ATT TYPE			    	***/
/**************************************************/

#define BLE_UUID_RANDOM_NUMBER_SERVICE_BASE_UUID                                                                       \
  { 0x22, 0x54, 0xC1, 0xE9, 0x44, 0xFA, 0xB6, 0xA9, 0xD4, 0x47, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00 }
#define BLE_UUID_RANDOM_NUMBER_SERVICE_BASE_OFFSET 0x01
#define BLE_UUID_RANDOM_NUMBER_DATA_OFFSET 0x02

/**************************************************/
/*** 		    	ATT DECLARATION    			***/
/**************************************************/

// Forward declaration of the ble_random_number_service_t type.
typedef struct ble_random_number_service_s ble_random_number_service_t;

/**************************************************/
/*** 		        ATT EVENT			    	***/
/**************************************************/

/**@brief RANDOM NUMBER Service event type. */
typedef enum {
  BLE_DATA_IO_EVT_NOTIFICATION_ENABLED,  /**&lt; Custom value notification enabled event. */
  BLE_DATA_IO_EVT_NOTIFICATION_DISABLED, /**&lt; Custom value notification disabled event. */
} ble_random_number_evt_type_t;

/**@brief RANDOM NUMBER Service event. */
typedef struct { ble_random_number_evt_type_t evt_type; } ble_random_number_evt_t;

/**************************************************/
/*** 		        ATT HANDLE			    	***/
/**************************************************/
typedef void (*ble_random_number_evt_handler_t)(ble_random_number_service_t *p_random_number_service,
                                                ble_random_number_evt_t *p_evt);

typedef struct {
  ble_random_number_evt_handler_t evt_handler; /**&lt; Event handler to be called when a RANDOM NUMBER event occurs. */
} ble_random_number_service_init_t;

/**************************************************/
/*** 		        ATT DEFINITION			   	***/
/**************************************************/
/**@brief RANDOM NUMBER Service structure. This contains various status information for the service. */
typedef struct ble_random_number_service_s {
  uint16_t service_handle;
  ble_gatts_char_handles_t data_io_char_handles;
  uint8_t uuid_type;
  uint16_t conn_handle;
  ble_random_number_evt_handler_t evt_handler;
} ble_random_number_service_t;

/**@brief Function for initializing the RANDOM NUMBER Service.
 *
 * @param[out]  p_random_number_service   		RANDOM NUMBER Service structure. This structure will have to be supplied
 * 												by the application. It will be initialized by this function, and will
 * later
 * be
 * used
 * to
 * identify
 * this
 * particular service instance.
 * @param[in]   p_random_number_service_init 	Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_random_number_service_init(ble_random_number_service_t *p_random_number_service,
                                        const ble_random_number_service_init_t *p_random_number_service_init);

uint32_t ble_random_number_data_io_value_update(ble_random_number_service_t *p_random_number_service,
                                                uint8_t data_io_value);

/**************************************************/
/*** 		        ATT BLE HANDLE			   	***/
/**************************************************/
/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the RANDOM NUMBER Service.
 *
 *
 * @param[in]   p_random_number_service     LED Button Service structure.
 * @param[in]   p_ble_evt  					Event received from the BLE stack.
 */
void ble_random_number_service_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context);
// void ble_random_number_service_on_ble_evt(ble_random_number_service_t *p_random_number_service,
//                                           ble_evt_t const *p_ble_evt);

#endif /* RANDOM_NUMBER_SERVICE_H */