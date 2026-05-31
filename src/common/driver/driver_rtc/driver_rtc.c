// DRIVER_RTC
// MAY 31, 2026

// Header Includes
#include <time.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "driver_rtc.h"
#include "util_logging.h"
#include "bsp.h"

// Defines
#define DRIVER_RTC_I2C_ADDRESS      (0x68)
#define DRIVER_RTC_REGISTER_SECOND  (0x00)
#define DRIVER_RTC_REGISTER_MINUTE  (0x01)
#define DRIVER_RTC_REGISTER_HOUR    (0x02)
#define DRIVER_RTC_REGISTER_WEEKDAY (0x03)  //1-7
#define DRIVER_RTC_REGISTER_DATE    (0x04)
#define DRIVER_RTC_REGISTER_MONTH   (0x05)
#define DRIVER_RTC_REGISTER_YEAR    (0x06)  //0-99, Offset From 2000
#define DRIVER_RTC_REGISTER_CONTROL (0x0E)
#define DRIVER_RTC_MASK_SECOND      (0x7F)
#define DRIVER_RTC_MASK_HOUR        (0x3F)
#define DRIVER_RTC_MASK_MONTH       (0x1F)

// Local Variables
static driver_rtc_tick_cb_t s_tick_cb = NULL;

// Local Functions
static uint8_t s_bcd(uint8_t v);
static uint8_t s_unbcd(uint8_t u);
static void s_rtc_to_tm(uint8_t* d, struct tm* ptr);
static void s_tm_to_rtc(uint8_t* d, struct tm* ptr);
static void s_irq_cb(uint gpio, uint32_t events);

// External Functions

// DRIVER_RTC_Init: Initialise The RTC.
//   returns: True On Success, False Otherwise
bool DRIVER_RTC_Init(void)
{
    uint8_t buf[2];

    // Initialize I2C
    i2c_init(i2c1, 100000);
    gpio_set_function(BSP_RTC_SDA, GPIO_FUNC_I2C);
    gpio_set_function(BSP_RTC_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(BSP_RTC_SDA);
    gpio_pull_up(BSP_RTC_SCL);

    // Setup 1 Second Output On RTC
    buf[0] = DRIVER_RTC_REGISTER_CONTROL;
    buf[1] = 0x00;
    i2c_write_blocking(i2c1, DRIVER_RTC_I2C_ADDRESS, buf, 2, false);

    // Enable Interrupt
    gpio_init(BSP_RTC_INT);
    gpio_set_dir(BSP_RTC_INT, GPIO_IN);
    gpio_pull_up(BSP_RTC_INT);
    gpio_set_irq_enabled_with_callback(
        BSP_RTC_INT,
        GPIO_IRQ_EDGE_FALL,
        true,
        s_irq_cb
    );

    return true;
}

// DRIVER_RTC_GetTime: Read The Current Time From The RTC.
//   ptr    : Destination Struct Populated With The Read Time
//   returns: True On Success, False Otherwise
bool DRIVER_RTC_GetTime(struct tm* ptr)
{
    uint8_t reg = DRIVER_RTC_REGISTER_SECOND;
    uint8_t out[7];

    i2c_write_blocking(i2c1, DRIVER_RTC_I2C_ADDRESS, &reg, 1, true);
    i2c_read_blocking(i2c1, DRIVER_RTC_I2C_ADDRESS, out, 7, false);
    
    s_rtc_to_tm(out, ptr);

    return true;
}

// DRIVER_RTC_SetTime: Write The Current Time To The RTC.
//   ptr    : Source Struct Holding The Time To Write
//   returns: True On Success, False Otherwise
bool DRIVER_RTC_SetTime(struct tm* ptr)
{
    uint8_t out[8];

    out[0] = DRIVER_RTC_REGISTER_SECOND;
    s_tm_to_rtc(&out[1], ptr);
    i2c_write_blocking(i2c1, DRIVER_RTC_I2C_ADDRESS, out, 8, false);

    return true;
}

// DRIVER_RTC_SetTickCallback: Register A Callback Invoked On Each RTC Tick.
//   cb     : Callback Function Called From The RTC Interrupt (NULL To Disable)
//   returns: True On Success, False Otherwise
bool DRIVER_RTC_SetTickCallback(driver_rtc_tick_cb_t cb)
{
    s_tick_cb = cb;

    return true;
}

// s_bcd: Convert A Binary Value To BCD.
//   v      : Binary Value (0-99)
//   returns: BCD Value
static uint8_t s_bcd(uint8_t v)
{
    return (v/10)<<4 | (v%10);
}

// s_unbcd: Convert A BCD Value To Binary.
//   u      : BCD Value
//   returns: Binary Value
static uint8_t s_unbcd(uint8_t u)
{
    return (u>>4)*10 + (u&0x0F);
}

// s_rtc_to_tm: Decode A Raw RTC Register Buffer Into A tm Struct.
//   d      : Raw RTC Register Bytes (Seconds Through Year)
//   ptr    : Destination Struct Populated With The Decoded Time
//   returns: None
static void s_rtc_to_tm(uint8_t* d, struct tm* ptr)
{
    ptr->tm_sec = s_unbcd(d[0] & DRIVER_RTC_MASK_SECOND);
    ptr->tm_min = s_unbcd(d[1]);
    ptr->tm_hour = s_unbcd(d[2] & DRIVER_RTC_MASK_HOUR);
    ptr->tm_wday = s_bcd(d[3]) - 1;                             //0-6
    ptr->tm_mday = s_unbcd(d[4]);
    ptr->tm_mon = s_unbcd(d[5] & DRIVER_RTC_MASK_MONTH) - 1;    //0-11
    ptr->tm_year = s_unbcd(d[6]) + 100;                         //Years Since 1900
    ptr->tm_isdst = -1;
    mktime(ptr);
}

// s_tm_to_rtc: Encode A tm Struct Into A Raw RTC Register Buffer.
//   d      : Destination Buffer For RTC Register Bytes (Seconds Through Year)
//   ptr    : Source Struct Holding The Time To Encode
//   returns: None
static void s_tm_to_rtc(uint8_t* d, struct tm* ptr)
{
    d[0] = s_bcd(ptr->tm_sec);
    d[1] = s_bcd(ptr->tm_min);
    d[2] = s_bcd(ptr->tm_hour);
    d[3] = s_bcd(ptr->tm_wday + 1);
    d[4] = s_bcd(ptr->tm_mday);
    d[5] = s_bcd(ptr->tm_mon + 1);
    d[6] = s_bcd(ptr->tm_year - 100);
}

// s_irq_cb: GPIO Interrupt Callback For The RTC 1 Second Output.
//   gpio   : GPIO Pin That Triggered The Interrupt
//   events : Bitmask Of The Triggering Edge/Level Events
//   returns: None
static void s_irq_cb(uint gpio, uint32_t events)
{
    LOG_DEBUG("1 sec tick...");

    if (s_tick_cb != NULL)
    {
        s_tick_cb();
    }
}

