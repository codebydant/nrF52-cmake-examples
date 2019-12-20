#include "random_number_service.h"
#include "nrf_log.h"

/**************************************************/
/*** 		     BLE HANDLE	CONNECTION		   	***/
/**************************************************/
/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_random_number_service   	RANDOM NUMBER Service structure.
 * @param[in]   p_ble_evt        			Event received from the BLE stack.
 */
static void on_connect(ble_random_number_service_t *p_random_number_service, ble_evt_t const *p_ble_evt) {
  p_random_number_service->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**************************************************/
/*** 		   BLE HANDLE DISCONNECTION		   	***/
/**************************************************/
/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_random_number_service  RANDOM NUMBER Service structure.
 * @param[in]   p_ble_evt       		 Event received from the BLE stack.
 */
static void on_disconnect(ble_random_number_service_t *p_random_number_service, ble_evt_t const *p_ble_evt) {
  UNUSED_PARAMETER(p_ble_evt);
  p_random_number_service->conn_handle = BLE_CONN_HANDLE_INVALID;
}

/**************************************************/
/*** 		   ATT BLE HANDLE				   	***/
/**************************************************/
// void ble_random_number_service_on_ble_evt(ble_random_number_service_t *p_random_number_service,
//                                           ble_evt_t const *p_ble_evt) {
void ble_random_number_service_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context) {
  ble_random_number_service_t *p_random_number_service = (ble_random_number_service_t *)p_context;

  // NRF_LOG_INFO("BLE event received. Event type = %d\r\n", p_ble_evt->header.evt_id);

  // if (p_random_number_service == NULL || p_ble_evt == NULL) {
  //   return;
  // }

  switch (p_ble_evt->header.evt_id) {
  case BLE_GAP_EVT_CONNECTED:
    on_connect(p_random_number_service, p_ble_evt);
    break;

  case BLE_GAP_EVT_DISCONNECTED:
    on_disconnect(p_random_number_service, p_ble_evt);
    break;

  // case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
  //   NRF_LOG_INFO("BLE read event received in service.c\r\n");
  //   break;

  // case BLE_GATTC_EVT_READ_RSP:
  //   NRF_LOG_INFO("BLE read event received\r\n");
  //   break;

  // case BLE_GATTS_EVT_WRITE:
  //     on_write(p_midi_service, p_ble_evt);
  //     break;

  default:
    // No implementation needed.
    break;
  }
}

/**************************************************/
/*** 		        ATT CHARACTERISTIC		   	***/
/**************************************************/
/**@brief Function for adding the Data I/O characteristic.
 *
 */
static uint32_t data_io_char_add(ble_random_number_service_t *p_random_number_service) {
  ble_gatts_char_md_t char_md;
  ble_gatts_attr_t attr_char_value;
  ble_gatts_attr_md_t attr_md;
  ble_uuid_t ble_uuid;

  memset(&char_md, 0, sizeof(char_md));
  memset(&attr_md, 0, sizeof(attr_md));
  memset(&attr_char_value, 0, sizeof(attr_char_value));

  char_md.char_props.read = 1;
  char_md.char_props.write = 0;
  char_md.char_props.notify = 0;
  char_md.p_char_user_desc = NULL;
  // char_md.char_user_desc_size = sizeof(LED2CharName);
  // char_md.char_user_desc_max_size = sizeof(LED2CharName);
  char_md.p_char_pf = NULL;
  char_md.p_user_desc_md = NULL;
  char_md.p_cccd_md = NULL;
  char_md.p_sccd_md = NULL;

  // Define the LED 2 Characteristic UUID
  ble_uuid.type = p_random_number_service->uuid_type;
  ble_uuid.uuid = BLE_UUID_RANDOM_NUMBER_DATA_OFFSET;

  // Set permissions on the Characteristic value
  // BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);

  // Attribute Metadata settings
  attr_md.vloc = BLE_GATTS_VLOC_STACK;
  attr_md.wr_auth = 0;
  attr_md.vlen = 0;
  attr_md.rd_auth = 1;

  // Attribute Value settings
  attr_char_value.p_uuid = &ble_uuid;
  attr_char_value.p_attr_md = &attr_md;
  attr_char_value.init_len = sizeof(uint8_t);
  attr_char_value.init_offs = 0;
  attr_char_value.max_len = sizeof(uint8_t);
  uint8_t val_default = 58;
  attr_char_value.p_value = &val_default;
  /*
    uint32_t err_code;                // error code
    ble_gatts_char_md_t char_md;      // GATT characteristic metadata
    ble_gatts_attr_md_t cccd_md;      // GATT client characteristic config descriptor metadata
    ble_gatts_attr_t attr_char_value; // Characteristic value att
    ble_uuid_t ble_uuid;              // UUID
    ble_gatts_attr_md_t attr_md;      // Attribute metadata

    memset(&cccd_md, 0, sizeof(cccd_md));
    memset(&char_md, 0, sizeof(char_md));
    memset(&attr_md, 0, sizeof(attr_md));
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm); // Read (enabled)
    // BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK; // Location ATT on stack RAM

    char_md.char_props.read = 1;
    char_md.char_props.write_wo_resp = 0;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = &cccd_md;
    char_md.p_sccd_md = NULL;

    NRF_LOG_INFO("ADDING CHAR1\r\n");

    // Add the RANDOM NUMBER Data I/O Characteristic UUID
    ble_uuid128_t base_uuid = {BLE_UUID_RANDOM_NUMBER_SERVICE_BASE_UUID};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_random_number_service->uuid_type);
    if (err_code != NRF_SUCCESS) {
      return err_code;
    }

    NRF_LOG_INFO("ADDING CHAR2\r\n");

    ble_uuid.type = p_random_number_service->uuid_type;
    ble_uuid.uuid = BLE_UUID_RANDOM_NUMBER_DATA_OFFSET;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    // BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 0;

    // Configure the characteristic value
    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = sizeof(uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = sizeof(uint8_t);
    attr_char_value.p_value = NULL;*/

  // NRF_LOG_INFO("ERROR:%lu\n", (unsigned long)sd_ble_gatts_characteristic_add(
  //                                 p_random_number_service->service_handle, &char_md, &attr_char_value,
  //                                 &p_random_number_service->data_io_char_handles));

  return sd_ble_gatts_characteristic_add(p_random_number_service->service_handle, &char_md, &attr_char_value,
                                         &p_random_number_service->data_io_char_handles);
}

/**************************************************/
/*** 		        ATT SERVICES     		   	***/
/**************************************************/

uint32_t ble_random_number_service_init(ble_random_number_service_t *p_random_number_service,
                                        const ble_random_number_service_init_t *p_random_number_service_init) {
  uint32_t err_code;
  ble_uuid_t ble_uuid;

  // Initialize service structure
  p_random_number_service->conn_handle = BLE_CONN_HANDLE_INVALID;
  p_random_number_service->evt_handler = p_random_number_service_init->evt_handler;

  // NRF_LOG_INFO("RANDOM NUMBER started20.");
  // Add service
  ble_uuid128_t base_uuid = {BLE_UUID_RANDOM_NUMBER_SERVICE_BASE_UUID};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_random_number_service->uuid_type);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  // NRF_LOG_INFO("RANDOM NUMBER started21.");

  ble_uuid.type = p_random_number_service->uuid_type;
  ble_uuid.uuid = BLE_UUID_RANDOM_NUMBER_SERVICE_BASE_OFFSET;

  err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_random_number_service->service_handle);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  // NRF_LOG_INFO("RANDOM NUMBER started22.");

  err_code = data_io_char_add(p_random_number_service);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  // NRF_LOG_INFO("RANDOM NUMBER started23.");

  return NRF_SUCCESS;
}

uint32_t ble_random_number_data_io_value_update(ble_random_number_service_t *p_random_number_service,
                                                uint8_t data_io_value) {

  // if (p_random_number_service == NULL) {
  //   return NRF_ERROR_NULL;
  // }

  uint32_t err_code = NRF_SUCCESS;
  ble_gatts_value_t gatts_value;

  // Initialize value struct.
  memset(&gatts_value, 0, sizeof(gatts_value));

  gatts_value.len = sizeof(uint8_t);
  gatts_value.offset = 0;
  gatts_value.p_value = &data_io_value;

  // Update database.
  err_code = sd_ble_gatts_value_set(p_random_number_service->conn_handle,
                                    p_random_number_service->data_io_char_handles.value_handle, &gatts_value);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }
  /*

  ble_gatts_hvx_params_t hvx_params;

  memset(&hvx_params, 0, sizeof(hvx_params));

  hvx_params.handle = p_random_number_service->data_io_char_handles.value_handle;
  hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
  hvx_params.offset = gatts_value.offset;
  hvx_params.p_len = &gatts_value.len;
  hvx_params.p_data = gatts_value.p_value;

  err_code = sd_ble_gatts_hvx(p_random_number_service->conn_handle, &hvx_params);
  NRF_LOG_INFO("sd_ble_gatts_hvx result: %x. \r\n", err_code);

  // // Send value if connected and notifying.
  // if ((p_random_number_service->conn_handle != BLE_CONN_HANDLE_INVALID)) {
  //   ble_gatts_hvx_params_t hvx_params;

  //   memset(&hvx_params, 0, sizeof(hvx_params));

  //   hvx_params.handle = p_random_number_service->data_io_char_handles.value_handle;
  //   hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
  //   hvx_params.offset = gatts_value.offset;
  //   hvx_params.p_len = &gatts_value.len;
  //   hvx_params.p_data = gatts_value.p_value;

  //   err_code = sd_ble_gatts_hvx(p_random_number_service->conn_handle, &hvx_params);
  //   NRF_LOG_INFO("sd_ble_gatts_hvx result: %x. \r\n", err_code);
  // } else {
  //   err_code = NRF_ERROR_INVALID_STATE;
  //   NRF_LOG_INFO("sd_ble_gatts_hvx result: NRF_ERROR_INVALID_STATE. \r\n");
  // }

  return NRF_SUCCESS;*/

  // Send value if connected and notifying.
  if ((p_random_number_service->conn_handle != BLE_CONN_HANDLE_INVALID)) {
    ble_gatts_hvx_params_t hvx_params;

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_random_number_service->data_io_char_handles.value_handle;
    hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.offset = gatts_value.offset;
    hvx_params.p_len = &gatts_value.len;
    hvx_params.p_data = gatts_value.p_value;

    err_code = sd_ble_gatts_hvx(p_random_number_service->conn_handle, &hvx_params);
    // NRF_LOG_INFO("sd_ble_gatts_hvx result: %x. \r\n", err_code);
    NRF_LOG_INFO("Randomic number (dec): %u", data_io_value);
    NRF_LOG_INFO("Randomic number (hex): 0x%02X", data_io_value);
  } else {
    err_code = NRF_ERROR_INVALID_STATE;
    // NRF_LOG_INFO("sd_ble_gatts_hvx result: NRF_ERROR_INVALID_STATE. \r\n");
    // NRF_LOG_INFO("Characteristic updated --> [NOT]\r\n");
    NRF_LOG_INFO("Couldn't generated a randomic number:\n");
  }

  return err_code;
}