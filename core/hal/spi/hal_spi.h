#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

#include <ioal.h>
#include "hal_spi_impl.h"

/*** SPI SPECIFIC API ***/

/* set configuration on SPI device */
int ua_spi_configure(ua_spi_device_p device,
                     ua_spi_configuration_p cfg);

/* send data then receive data from SPI device */
int ua_spi_send_then_recv(ua_spi_device_p device,
                          const void    *send_buf,
                          ua_size_t      send_len,
                          void          *recv_buf,
                          ua_size_t      recv_len);

int ua_spi_send_then_send(ua_spi_device_p device,
                          const void    *buf1,
                          ua_size_t      len1,
                          const void    *buf2,
                          ua_size_t      len2);

ua_size_t ua_spi_transfer(ua_spi_device_p device,
                          const void    *send_buf,
                          void          *recv_buf,
                          ua_size_t      len);

UA_INLINE ua_size_t ua_spi_recv(ua_spi_device_p device,
                                void        *recv_buf,
                                ua_size_t    len)
{
    return ua_spi_transfer(device, UA_NULL, recv_buf, len);
}

UA_INLINE ua_size_t ua_spi_send(ua_spi_device_p device,
                                const void  *send_buf,
                                ua_size_t    len)
{
    return ua_spi_transfer(device, send_buf, UA_NULL, len);
}

#endif //




