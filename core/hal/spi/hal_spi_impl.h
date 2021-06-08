#ifndef __HAL_SPI_IMPL_H__
#define __HAL_SPI_IMPL_H__

#include <ioal.h>

/**
 * At CPOL=0 the base value of the clock is zero
 *  - For CPHA=0, data are captured on the clock's rising edge (low->high transition)
 *    and data are propagated on a falling edge (high->low clock transition).
 *  - For CPHA=1, data are captured on the clock's falling edge and data are
 *    propagated on a rising edge.
 * At CPOL=1 the base value of the clock is one (inversion of CPOL=0)
 *  - For CPHA=0, data are captured on clock's falling edge and data are propagated
 *    on a rising edge.
 *  - For CPHA=1, data are captured on clock's rising edge and data are propagated
 *    on a falling edge.
 */
#define UA_SPI_CPHA     (1<<0)                             /* bit[0]:CPHA, clock phase */
#define UA_SPI_CPOL     (1<<1)                             /* bit[1]:CPOL, clock polarity */

#define UA_SPI_LSB      (0<<2)                             /* bit[2]: 0-LSB */
#define UA_SPI_MSB      (1<<2)                             /* bit[2]: 1-MSB */

#define UA_SPI_MASTER   (0<<3)                             /* SPI master device */
#define UA_SPI_SLAVE    (1<<3)                             /* SPI slave device */

#define UA_SPI_CS_HIGH  (1<<4)                             /* Chipselect active high */
#define UA_SPI_NO_CS    (1<<5)                             /* No chipselect */
#define UA_SPI_3WIRE    (1<<6)                             /* SI/SO pin shared */
#define UA_SPI_READY    (1<<7)                             /* Slave pulls low to pause */

#define UA_SPI_MODE_MASK    (UA_SPI_CPHA | UA_SPI_CPOL | UA_SPI_MSB | UA_SPI_SLAVE | UA_SPI_CS_HIGH | UA_SPI_NO_CS | UA_SPI_3WIRE | UA_SPI_READY)

#define UA_SPI_MODE_0       (0 | 0)                        /* CPOL = 0, CPHA = 0 */
#define UA_SPI_MODE_1       (0 | UA_SPI_CPHA)              /* CPOL = 0, CPHA = 1 */
#define UA_SPI_MODE_2       (UA_SPI_CPOL | 0)              /* CPOL = 1, CPHA = 0 */
#define UA_SPI_MODE_3       (UA_SPI_CPOL | UA_SPI_CPHA)    /* CPOL = 1, CPHA = 1 */

// #define UA_SPI_BUS_MODE_SPI         (1<<0)
// #define UA_SPI_BUS_MODE_QSPI        (1<<1)

typedef struct ua_spi_message
{
    const void *send_buf;
    void *recv_buf;
    ua_size_t length;

    uint8_t cs_take    : 1;
    uint8_t cs_release : 1;
}ua_spi_message_t;
typedef ua_spi_message_t* ua_spi_message_p;

typedef struct ua_spi_configuration
{
    uint8_t mode;
    uint8_t data_width;
    uint16_t reserved;

    uint32_t max_hz;
}ua_spi_configuration_t;
typedef ua_spi_configuration_t* ua_spi_configuration_p;

struct ua_spi_ops;
typedef struct ua_spi_bus 
{
    struct ua_device parent;
    uint8_t mode;
    const struct ua_spi_ops *ops;
    // struct ua_spi_device *owner;
}ua_spi_bus_t;
typedef ua_spi_bus_t* ua_spi_bus_p;

typedef struct ua_spi_device
{
    struct ua_device parent;
    struct ua_spi_bus *bus;

    struct ua_spi_configuration config;

    /* API */
    int (*send_then_recv)(struct ua_spi_device *device, const void *send_buf, ua_size_t send_len, void *recv_buf, ua_size_t recv_len);
    int (*send_then_send)(struct ua_spi_device *device, const void *buf1, ua_size_t len1, const void *buf2, ua_size_t len2);
    ua_size_t (*transfer)(struct ua_spi_device *device, const void *send_buf, void *recv_buf, ua_size_t len);

    // void *user_data;
}ua_spi_device_t;
typedef ua_spi_device_t* ua_spi_device_p;

typedef struct ua_spi_ops 
{
    int (*configure)(struct ua_spi_device *device, struct ua_spi_configuration *configuration);
    int (*transfer)(struct ua_spi_device *device, struct ua_spi_message *message);
}ua_spi_ops_t;
typedef ua_spi_ops_t* ua_spi_ops_p;

/*** ***/
int ua_spi_bus_register(ua_spi_bus_p bus, const char *name, ua_spi_ops_p ops);

int ua_spi_bus_attach_device(ua_spi_device_p device, 
                             const char *name,
                             const char *bus_name,
                             void *user_data);

#endif // 
