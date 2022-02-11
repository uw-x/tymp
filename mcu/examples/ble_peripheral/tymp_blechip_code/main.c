
 /** @file
 *
 *  Created on : Dec 26, 2019
 *      Author: Ali Najafi
 */


#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "app_timer.h"
#include "ble_image_transfer_service.h"
#include "app_uart.h"
#include "app_util_platform.h"
#include "bsp_btn_ble.h"
#include "nrf_pwr_mgmt.h"

#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

//#include "ArducamMini2MP.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define PCA10056_USE_FRONT_HEADER       0                                            /**< Use the front header (P24) for the camera module. Requires SB10-15 and SB20-25 to be soldered/cut, as described in the readme. */

#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */

//#define DEVICE_NAME                     "Camera Demo v2"                          /**< Name of device. Will be included in the advertising data. */
#define DEVICE_NAME                     "Tympanometer"                                 /**< Name of device. Will be included in the advertising data. */
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

#define APP_BLE_OBSERVER_PRIO           3                                           /**< Application's BLE observer priority. You shouldn't need to modify this value. */

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */

//#define APP_ADV_INTERVAL                1000                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */

#define APP_ADV_DURATION                18000                                       /**< The advertising duration (180 seconds) in units of 10 milliseconds. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(15, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(15, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
//#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(400, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
//#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(650, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */



#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                       /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                      /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define UART_TX_BUF_SIZE                256                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                256                                         /**< UART RX buffer size. */


/*********************************************************************************************/
/*Added by Ali*/
/* TWI instance ID. */
//#define TWI_INSTANCE_ID     0
extern uint8_t m_new_move;

static uint8_t m_new_angle;
static bool                             stream_first_image_done = false;
static bool                             img_info_sent = false;
static uint8_t m_new_device;
static uint8_t m_new_reg_address_msb;
static uint8_t m_new_reg_address_lsb;
static uint8_t m_new_reg_value;





#include "app_pwm.h"

//Tymp code variables
#include "TYMP_DEFINES.h"
#include "MOTOR.h"
double init_press_human;
uint8_t max_press[4];
uint8_t min_press[4];
uint32_t mtr_pwm_timer_val;
uint32_t mtr_seal_check_threshold;
uint8_t mtr_dir ;
uint8_t max_range[3];
uint8_t min_range[3];
uint32_t press_measurement_cnt;
uint32_t ble_bytes_sent_counter;
uint8_t measured_press[4];
uint8_t motor_loc_status;

uint8_t mtr_move_step;
uint8_t mtr_move_dir;

//COMPRESSION
//uint8_t       jpeg_imag_mem[jpeg_imag_mem_size+1];
//uint32_t      compressed_size;

////newly added for nrf52832
#if defined(BOARD_PCA10040)
//uint32_t CAM_SPI_CS;
////NRF_GPIO_Type * gpio_p_reg = nrf_gpio_pin_port_decode(&CAM_SPI_CS);
////nrf_gpio_pin_port_decode(&CAM_SPI_CS);
//NRF_GPIO_Type * gpio_p_reg;
//uint32_t CAM_SPI_PIN_MASK;
uint32_t line_count;
uint32_t CAM_LINE_VLD;
//uint32_t rxd_ptr; 
#endif

//uint32_t CAM_SPI_CS =  16;

//comments: - don't initialize gpiote twice!  -care should be taken when having both SPI and I2C!(I have removed SPI stuff here)

/*********************************************************************************************/

//BLE_ITS_DEF(m_its, NRF_SDH_BLE_TOTAL_LINK_COUNT);                                   /**< BLE NUS service instance. */
NRF_BLE_GATT_DEF(m_gatt);                                                           /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);                                                             /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising);                                                 /**< Advertising module instance. */

extern uint8_t                          m_new_command_received = 0;
static uint8_t                          m_new_resolution, m_new_phy;

extern bool                             m_stream_mode_active = false;

static ble_its_ble_params_info_t        m_ble_params_info = {20, 50, 1, 1};

static uint16_t   m_conn_handle          = BLE_CONN_HANDLE_INVALID;                 /**< Handle of the current connection. */
static uint16_t   m_ble_its_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3;            /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */
static ble_uuid_t m_adv_uuids[]          =                                          /**< Universally unique service identifier. */
{
    {BLE_UUID_ITS_SERVICE, NUS_SERVICE_UUID_TYPE}
};



//Commands needed for tymp: set max pressure, set min pressure, set pressure step value, sweep duration, start, stop 
enum {
      APP_CMD_NOCOMMAND = 0, 
      APP_CMD_INIT_TYMP, //first 3 byte is max_range, 2nd 3 byte is min_range
      APP_CMD_START_TYMP, 
      APP_CMD_STOP_TYMP,
      APP_CMD_MOVE_TO_INIT,
      APP_CMD_MIC_CHECK,
      APP_CMD_UPDATE_START,
      APP_CMD_QUERY_TURNS,
      APP_CMD_ACK
      };






/************************************************************************************************************/
/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyse
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the timer module.
 */
static void timers_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for the GAP initialization.
 *
 * @details This function will set up all the necessary GAP (Generic Access Profile) parameters of
 *          the device. It also sets the permissions and appearance.
 */
static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *) DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
    
#if 0
    ble_gap_addr_t ble_address = {.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_STATIC,
                                  .addr_id_peer = 0,
                                  .addr = {0xC3,0x11,0x99,0x33,0x44,0xFF}};
    err_code = sd_ble_gap_addr_set(&ble_address);
    APP_ERROR_CHECK(err_code);
#endif
}


/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for handling the data from the ITS Service.
 *
 * @details This function will process the data received from the ITS BLE Service.
 *
 * @param[in] p_its    ITS Service structure.
 * @param[in] p_data   Data received.
 * @param[in] length   Length of the data.
 */
/**@snippet [Handling the data received over BLE] */
static void its_data_handler(ble_its_t * p_its, uint8_t const * p_data, uint16_t length)
{
    switch(p_data[0])
    {
        // Setting the positive max pressure; pressure is 24bit number
        //p_data[1:3] is setting the max pressure
        case APP_CMD_INIT_TYMP:
            if (!start_cmd_in_progress && !stop_cmd_in_progress) {
                m_new_command_received = APP_CMD_INIT_TYMP;
                
                max_range[0] = p_data[1];
                max_range[1] = p_data[2];
                max_range[2] = p_data[3];

                min_range[0] = p_data[4];
                min_range[1] = p_data[5];
                min_range[2] = p_data[6];
                
                calculate_max_min_press(init_press, max_range, min_range, max_press, min_press);

                mtr_pwm_timer_val = (uint32_t) p_data[7];
    //            mtr_seal_check_threshold = (uint32_t) p_data[8];
                mtr_seal_check_threshold = (uint32_t) p_data[8]*100;
                sweep_dir = (uint32_t) p_data[9];
                keep_num_val = (uint32_t) (p_data[10]*4*MICROSTEPS);//It should be multiple of 4*MICROSTEPS because we would want to be
                // at the max or min pressure for multiple of some pressure values!
                
                MICROSTEPS = (uint8_t)p_data[11];
                if (MICROSTEPS == 8) {
                  microstepcurve = microstepcurve8;
                }
                else if (MICROSTEPS == 16) {
                  microstepcurve = microstepcurve16;
                }
                else if (MICROSTEPS == 32) {
                  microstepcurve = microstepcurve32;
                }
                else if (MICROSTEPS == 64) {
                  microstepcurve = microstepcurve64;
                }
                MOVE_TO_INIT = (uint8_t)p_data[12];

                mtr_fast_speed = (uint32_t)p_data[13];
            }
            break;

        case APP_CMD_START_TYMP:
            m_new_command_received = APP_CMD_START_TYMP;
            break;

        case APP_CMD_STOP_TYMP:
            m_new_command_received = APP_CMD_STOP_TYMP;
            break;
            
        case APP_CMD_MOVE_TO_INIT:
            m_new_command_received = APP_CMD_MOVE_TO_INIT;
            mtr_move_step = p_data[1];
            mtr_move_dir = p_data[2];
            MICROSTEPS = (uint8_t)p_data[3];
            if (MICROSTEPS == 8) {
              microstepcurve = microstepcurve8;
            }
            else if (MICROSTEPS == 16) {
              microstepcurve = microstepcurve16;
            }
            else if (MICROSTEPS == 32) {
              microstepcurve = microstepcurve32;
            }
            else if (MICROSTEPS == 64) {
              microstepcurve = microstepcurve64;
            }
            break;

        case APP_CMD_MIC_CHECK:
            m_new_command_received = APP_CMD_MIC_CHECK;
            mic_check = p_data[1];
            break;
        
        case APP_CMD_UPDATE_START:
            m_new_command_received = APP_CMD_UPDATE_START;
            break;
        case APP_CMD_QUERY_TURNS:
            m_new_command_received = APP_CMD_QUERY_TURNS;
            break;
        case APP_CMD_ACK:
            m_new_command_received = APP_CMD_ACK;
            break;
//        case APP_CMD_TYMP_SPEED:
//            m_new_command_received = APP_CMD_TYMP_SPEED;
//            mtr_speed = p_data[1];
//            break;
            
        default: 
            NRF_LOG_ERROR("Unknown command!!");
            break;
    }
}
/**@snippet [Handling the data received over BLE] */


/**@brief Function for initializing services that will be used by the application.
 */
static void services_init(void)
{
    uint32_t           err_code;
    ble_its_init_t     its_init;
    nrf_ble_qwr_init_t qwr_init = {0};

    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    // Initialize ITS.
    memset(&its_init, 0, sizeof(its_init));

    its_init.data_handler = its_data_handler;

    err_code = ble_its_init(&m_its, &its_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling an event from the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module
 *          which are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    switch(p_evt->evt_type)
    {
        case BLE_CONN_PARAMS_EVT_SUCCEEDED:
            NRF_LOG_INFO("BLE_CONN_PARAMS_EVT_SUCCEEDED...");
            break;
            
        case BLE_CONN_PARAMS_EVT_FAILED:
            //err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
            //APP_ERROR_CHECK(err_code);
            NRF_LOG_ERROR("Conn params failed. Keep the connection anyway..");
            break;
    }
}


/**@brief Function for handling errors from the Connection Parameters module.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}



/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            NRF_LOG_INFO("BLE_ADV_EVT_FAST...");
            #if (BLE_LED_ON)
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            #endif
            break;
        case BLE_ADV_EVT_IDLE:
            NRF_LOG_INFO("BLE_ADV_EVT_IDLE...");
            err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
            APP_ERROR_CHECK(err_code);
            break;
        default:
            break;
    }
}


/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            #if (RELEASE_CODE ==0)
                NRF_LOG_INFO("Connected");
            #endif

            
            #if (BLE_LED_ON)
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);
            #endif
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            #if (RELEASE_CODE ==0)
                NRF_LOG_INFO("Disconnected");
            #endif
            // LED indication will be changed when advertising starts.

            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            break;
            
        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
        {
            uint16_t max_con_int = p_ble_evt->evt.gap_evt.params.conn_param_update.conn_params.max_conn_interval;
            uint16_t min_con_int = p_ble_evt->evt.gap_evt.params.conn_param_update.conn_params.min_conn_interval;

            m_ble_params_info.con_interval = max_con_int;
            ble_its_ble_params_info_send(&m_its, &m_ble_params_info);
            #if (RELEASE_CODE ==0)
                NRF_LOG_INFO("Con params updated: CI %i, %i", (int)min_con_int, (int)max_con_int);
            #endif
        } break;
            
        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            #if (RELEASE_CODE ==0)
                NRF_LOG_DEBUG("PHY update request.");
            #endif
            
            ble_gap_phys_t const phys =
            {
                BLE_GAP_PHY_AUTO,
                BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GAP_EVT_PHY_UPDATE:
            m_ble_params_info.tx_phy = p_ble_evt->evt.gap_evt.params.phy_update.tx_phy;
            m_ble_params_info.rx_phy = p_ble_evt->evt.gap_evt.params.phy_update.rx_phy;    
            ble_its_ble_params_info_send(&m_its, &m_ble_params_info);
            #if (RELEASE_CODE ==0)
                NRF_LOG_INFO("Phy update: %i, %i", (int)m_ble_params_info.tx_phy, (int)m_ble_params_info.rx_phy);
            #endif
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            // No implementation needed.
            //NRF_LOG_INFO("BLE event not handled by app: %i", p_ble_evt->header.evt_id);
            break;
    }
}


/**@brief Function for the SoftDevice initialization.
 *
 * @details This function initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}


/**@brief Function for handling events from the GATT library. */
void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)
{
    uint32_t data_length;
    if ((m_conn_handle == p_evt->conn_handle) && (p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED))
    {
        data_length = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH;
        //m_ble_params_info.mtu = m_ble_its_max_data_len;
        
        NRF_LOG_INFO("gatt_event: ATT MTU is set to 0x%X (%d)", data_length, data_length);
    }
    else if ((m_conn_handle == p_evt->conn_handle) && (p_evt->evt_id == NRF_BLE_GATT_EVT_DATA_LENGTH_UPDATED))
    {
        data_length = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH - 4;
        m_ble_its_max_data_len = data_length;
        m_ble_params_info.mtu = m_ble_its_max_data_len;
        ble_its_ble_params_info_send(&m_its, &m_ble_params_info);
        
        NRF_LOG_INFO("gatt_event: Data len is set to 0x%X (%d)", data_length, data_length);
    }
    //NRF_LOG_DEBUG("ATT MTU exchange completed. central 0x%x peripheral 0x%x",
      //            p_gatt->att_mtu_desired_central,
        //        p_gatt->att_mtu_desired_periph);
}


/**@brief Function for initializing the GATT library. */
void gatt_init(void)
{
    ret_code_t err_code;

    err_code = nrf_ble_gatt_init(&m_gatt, gatt_evt_handler);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_ble_gatt_data_length_set(&m_gatt, BLE_CONN_HANDLE_INVALID, NRF_SDH_BLE_GAP_DATA_LENGTH);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling events from the BSP module.
 *
 * @param[in]   event   Event generated by button press.
 */
void bsp_event_handler(bsp_event_t event)
{
    uint32_t err_code;
    switch (event)
    {
        case BSP_EVENT_SLEEP:
            break;

        case BSP_EVENT_DISCONNECT:
            err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BSP_EVENT_WHITELIST_OFF:
            if (m_conn_handle == BLE_CONN_HANDLE_INVALID)
            {
                err_code = ble_advertising_restart_without_whitelist(&m_advertising);
                if (err_code != NRF_ERROR_INVALID_STATE)
                {
                    APP_ERROR_CHECK(err_code);
                }
            }
            break;

        default:
            break;
    }
}


/**@brief   Function for handling app_uart events.
 *
 * @details This function will receive a single character from the app_uart module and append it to
 *          a string. The string will be be sent over BLE when the last character received was a
 *          'new line' '\n' (hex 0x0A) or if the string has reached the maximum data length.
 */
/**@snippet [Handling the data received over UART] */
void uart_event_handle(app_uart_evt_t * p_event)
{

}
/**@snippet [Handling the data received over UART] */


/**@brief  Function for initializing the UART module.
 */
/**@snippet [UART Initialization] */
static void uart_init(void)
{
    uint32_t                     err_code;
    app_uart_comm_params_t const comm_params =
    {
        RX_PIN_NUMBER,
        TX_PIN_NUMBER,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
#if defined (UART_PRESENT)
        NRF_UART_BAUDRATE_115200
#else
        NRF_UARTE_BAUDRATE_115200
#endif
    };

    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_event_handle,
                       APP_IRQ_PRIORITY_LOWEST,
                       err_code);
    APP_ERROR_CHECK(err_code);
}
/**@snippet [UART Initialization] */


/**@brief Function for initializing the Advertising functionality.
 */
static void advertising_init(void)
{
    uint32_t               err_code;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    init.advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    init.advdata.include_appearance = false;
    init.advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;

    init.srdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.srdata.uuids_complete.p_uuids  = m_adv_uuids;

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;
    init.evt_handler = on_adv_evt;

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}


/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
static void buttons_leds_init(void)
{
    bsp_event_t startup_event;

    uint32_t err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(err_code);

    err_code = bsp_btn_ble_init(NULL, &startup_event);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    while(NRF_LOG_PROCESS());
//    sd_app_evt_wait();
    sd_app_evt_wait();
//    nrf_pwr_mgmt_run();
//__WFE();
}


/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    uint32_t err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
}


void conn_evt_len_ext_set(bool status)
{
    ret_code_t err_code;
    ble_opt_t  opt;

    memset(&opt, 0x00, sizeof(opt));
    opt.common_opt.conn_evt_ext.enable = status ? 1 : 0;

    err_code = sd_ble_opt_set(BLE_COMMON_OPT_CONN_EVT_EXT, &opt);
    APP_ERROR_CHECK(err_code);
}

void data_len_set(uint8_t value)
{
    ret_code_t err_code;
    err_code = nrf_ble_gatt_data_length_set(&m_gatt, BLE_CONN_HANDLE_INVALID, value);
    APP_ERROR_CHECK(err_code);
}


/**@brief Application main function.
 */
int main(void) {
    uint32_t press_data_length = 0;
    uint8_t press_data_buffer[255];

    //test code to move motor to desired postion
//    tymp_init();
//    mtr_move(100, 0);
    log_init();
    timers_init();
//    buttons_leds_init();
    power_management_init();
    ble_stack_init();
    gap_params_init();
    gatt_init();
    services_init();
    advertising_init();
    
    conn_params_init();
    sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);

//    printf("APP_CMD_INIT_TYMP start");

    memset(min_range,0x04, 1);
    memset(min_range+1,0xB3, 1);
    memset(min_range+2,0x20, 1);

    memset(max_range,0x02, 1);
    memset(max_range+1,0x59, 1);
    memset(max_range+2,0x90, 1);

    memset(press_mesearements, 0x00, MAX_STEP_TIMES*4);
    tymp_init();

    //We read the initial pressure right when we turn on the system
    sweep_dir = 0;
    press_measurement_cnt = 0;
//    memcpy(press_mesearements, init_press, sizeof(init_press));
//    press_measurement_cnt = press_measurement_cnt + 4;//update the memory address to copy the measured pressure 

//    calculate_max_min_press(init_press, max_range, min_range, max_press, min_press);
//    double max_press_human = convert_to_human(max_press);
//    double min_press_human = convert_to_human(min_press);

    //read_pressure_data(measured_press);

//    init_press[0] = 0x40;
//    init_press[1] = 0x93;
//    init_press[2] = 0x2a;
//    init_press[3] = 0xac;
//    motor_loc_status = INIT_PRESS_SEND;
//    status_sent = false;
    
//     mtr_move(50, 0);
//    mtr_move(10, 0);
//    read_pressure_data(measured_press);
//    read_pressure_data(measured_press);
//    read_pressure_data(measured_press);
//    read_pressure_data_interrupt();
//    read_pressure_data_interrupt();
//    read_pressure_data_interrupt();
//    read_pressure_data_interrupt();
//    MICROSTEPS = 8;
//    nrf_delay_ms(500);
    read_pressure_data(init_press);
   init_press_human = convert_to_human(init_press);

//    mic_check = true;
//   tymp_start();
//   seal_flag=true;
//   tymp_move_to_init();
    
    //test to force sending data
//    tymp_done = true;
//    press_measurement_cnt = 2000;
//    memset(press_mesearements, 0xAB, press_measurement_cnt);

//    while(!tymp_done);
//mtr_step_counter8=65312;
//mtr_step_counter64=66816;
//    tymp_stop();
//    mtr_move(100,1);
//    while(1);
    advertising_start();
    // Enter main loop.
    for (;;) {
        ble_gap_phys_t gap_phys_settings;    
        uint32_t new_command = m_new_command_received;
        
        uint32_t ret_code;
        if (!status_sent) {
            ret_code = ble_its_seal_info_send(&m_its, motor_loc_status, mtr_step_counter8, pressure_index);
            status_sent = true;
        }

        if(m_new_command_received != APP_CMD_NOCOMMAND) {
            
            m_new_command_received = APP_CMD_NOCOMMAND;

            switch(new_command) {
                case APP_CMD_INIT_TYMP:
                    NRF_LOG_INFO("APP_CMD_INIT_TYMP start");
//                    tymp_init();
                    NRF_LOG_INFO("APP_CMD_INIT_TYMP end");
                    break;

                case APP_CMD_START_TYMP:
                    NRF_LOG_INFO("APP_CMD_START_TYMP start");

                    ret_code = ble_its_seal_info_send(&m_its, SEND_START_ACK, 0, 0);
                    if (!start_cmd_in_progress && !stop_cmd_in_progress) {
                        start_cmd_in_progress = true;
                        tymp_start();
                    }
                    NRF_LOG_INFO("APP_CMD_START_TYMP end");
                    break;

                case APP_CMD_STOP_TYMP:
                    NRF_LOG_INFO("APP_CMD_STOP_TYMP start");
                    if (!stop_cmd_in_progress) {
                        stop_cmd_in_progress = true;
                        tymp_stop();
                    }
                    NRF_LOG_INFO("APP_CMD_STOP_TYMP end");
                    break;

                case APP_CMD_MOVE_TO_INIT:
                    NRF_LOG_INFO("APP_CMD_MOVE_TO_INIT start");
                    mtr_pwm_timer_val = 50;
                    mtr_move(mtr_move_step, mtr_move_dir);
                    NRF_LOG_INFO("APP_CMD_MOVE_TO_INIT end");
                    break;

                case APP_CMD_MIC_CHECK:
                    NRF_LOG_INFO("APP_CMD_MOVE_TO_INIT start");
                    NRF_LOG_INFO("APP_CMD_MOVE_TO_INIT end");
                    break;
                case APP_CMD_UPDATE_START:
                    mtr_step_counter8=mtr_start_point;
                    mtr_step_counter16=mtr_start_point;
                    mtr_step_counter32=mtr_start_point;
                    mtr_step_counter64=mtr_start_point;
                    read_pressure_data(init_press);
                    init_press_human = convert_to_human(init_press);
                    break;
                case APP_CMD_QUERY_TURNS:
                    ret_code = ble_its_seal_info_send(&m_its, SEND_TURN_INFO, mtr_step_counter8, counter_before_seal);
                    break;
                case APP_CMD_ACK:
                    // send ack
                    ret_code = ble_its_seal_info_send(&m_its, SEND_START_ACK, 0, 0);
                    
                    break;
                default: 
                    NRF_LOG_ERROR("Unknown command!!");
                    break;
              }
        }  

//        if(new_command != APP_CMD_NOCOMMAND){          
        if(tymp_done) {
            
            uint32_t press_len = press_measurement_cnt-pressure_seal_marker;
            uint8_t press_mesearements_to_send[press_len];
            memset(press_mesearements_to_send, 0x00, press_len);

            memcpy(press_mesearements_to_send, init_press, sizeof(press_mesearements[0])*4);
            memcpy(press_mesearements_to_send+4, &press_mesearements[pressure_seal_marker], press_len-4);

            ble_its_img_info_t pressure_array_info;
            pressure_array_info.file_size_bytes = press_len;

//            pressure_array_info.file_size_bytes = press_measurement_cnt;

            uint32_t ret_code;
            ret_code = ble_its_img_info_send(&m_its, &pressure_array_info, tymp_aborted);

            do {
                if(press_data_length == 0) {
                    press_data_length = ((press_len - ble_bytes_sent_counter) > m_ble_its_max_data_len ? 
                        m_ble_its_max_data_len : 
                        (press_len - ble_bytes_sent_counter));
                }

                ret_code = ble_its_send_file_fragment(&m_its, press_mesearements_to_send + ble_bytes_sent_counter, press_data_length);
                if(ret_code == NRF_SUCCESS) {
                    ble_bytes_sent_counter = ble_bytes_sent_counter + press_data_length;
                    press_data_length = 0;
                }
            } while(ret_code == NRF_SUCCESS); 

            //setting parameters for the next tympanogram 
            tymp_done = false;
//
            #if (INIT_GO == 0)
                ble_bytes_sent_counter = 0;
                press_measurement_cnt = 4;
                mtr_dir = 0;
    //            seal_flag = true;
                currentstep = CURRENT_STEP_INIT;
                memset(press_mesearements + 4, 0x00, (MAX_STEP_TIMES - 1)*4);//erase the press_measurements memory!
            #else
                nrf_delay_ms(10000);
                tymp_move_to_init();
            #endif
            nrf_delay_ms(1000);
            tymp_move_to_init();
        }
//        }

        if(m_new_command_received == APP_CMD_NOCOMMAND) {
            idle_state_handle();
        }
     }
}
