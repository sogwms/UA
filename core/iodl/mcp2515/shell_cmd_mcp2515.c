#include <rtthread.h>
#include "mcp2515.h"

static struct mcp2515 mcp2515;

extern int htoi(char s[]);

static void rx_cb(struct mcp2515 *ins, uint32_t id, uint8_t *data, uint8_t len)
{
    rt_kprintf("Recv echo: id: 0x%x(%d) len:%d\nData:", id, id, len);
    for (int i=0; i<len; i++) {
        rt_kprintf(" 0x%x", data[i]);
    }
    rt_kprintf("\n");
}

void pin_irq_entry(void *param)
{
    // static int cnt = 0;
    // rt_kprintf("irq_cnt: %d\n", cnt++);
    mcp2515_can_polling_read(&mcp2515);

}

#include <rtdevice.h>
#include <board.h>
#define INT_PIN    GET_PIN(B, 7)

static void irq_pin_init()
{
    rt_pin_mode(INT_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_attach_irq(INT_PIN, PIN_IRQ_MODE_FALLING, pin_irq_entry, &mcp2515);
    rt_pin_irq_enable(INT_PIN, PIN_IRQ_ENABLE);
}

int polling_thread_entry(void *param)
{
    rt_kprintf("Enter polling therad\n");
    while(1) {
        mcp2515_can_polling_read(&mcp2515);
        rt_thread_mdelay(5000);
    }
}

static void therad_init()
{
    rt_thread_t tid;
    tid = rt_thread_create("polling", polling_thread_entry, RT_NULL, 1024, 18, 10);
    rt_thread_startup(tid);
}

static int mcp(int argc, char **argv)
{
    if (argc < 2)
    {
        rt_kprintf("Usage: mcp [OPTION]\n");
        rt_kprintf("Options:\n");
        rt_kprintf("    init <dev_name>\n");
        rt_kprintf("    testSend <id>\n");

        return 0;
    }

    if (argc < 3) {
        return 0;
    }

    if (!rt_strcmp(argv[1], "init"))
    {
         char *dev_name = argv[2];
         int sta;
         rt_kprintf("Echo: dev_name:%s\n", dev_name);
         sta = mcp2515_init(&mcp2515, rx_cb, dev_name);
         if (sta != RT_EOK) {
             rt_kprintf("mcp2515 init failed\n");
         }
         else {
             rt_kprintf("mcp2515 init successfully\n");
             irq_pin_init();
             therad_init();
         }
    }
    else if (!rt_strcmp(argv[1], "testSend"))
    {
        uint32_t id = htoi(argv[2]);
        uint8_t send_buffer[8] = {1, 2, 3, 4, 5, 6, 7, 0};

        rt_kprintf("Echo: id:%d (0x%x) \n", id, id);
        mcp2515_can_send(&mcp2515, id, send_buffer, 8);
    }
    return 0;
}

MSH_CMD_EXPORT(mcp, mcp2515 cli)

#include <rtdevice.h>
#include <board.h>

static int rt_hw_spi_mcp2515_init(void)
{
    struct rt_spi_configuration cfg;
    rt_device_t dev;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    rt_hw_spi_device_attach("spi2", "spi20", GPIOB, GPIO_PIN_12);

    cfg.data_width = 8;
    cfg.mode =  RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
    cfg.max_hz = 8 * 1000 *1000;
    dev = rt_device_find("spi20");
    if (RT_EOK != rt_spi_configure((struct rt_spi_device *)dev, &cfg)) {
        rt_kprintf("Faile to config spi20 dev\n");
    }

    return RT_EOK;
}
/* 导出到自动初始化 */
// INIT_COMPONENT_EXPORT(rt_hw_spi_mcp2515_init);

extern int htoi(char s[]);

static int spibus(int argc, char **argv)
{
    static struct rt_spi_device *dev = RT_NULL;

    if (argc < 2)
    {
        rt_kprintf("Usage: spibus [OPTION]\n");
        rt_kprintf("Options:\n");
        rt_kprintf("    probe <dev_name>\n");
        rt_kprintf("    send_then_recv <recv_data_num> <send-data0> .. <send-dataN>\n");

        return 0;
    }

    if (argc < 3) {
        return 0;
    }

    if (!rt_strcmp(argv[1], "probe"))
    {
         char *dev_name = argv[2];
         
         rt_kprintf("Echo: dev_name:%s\n", dev_name);
         dev = (struct rt_spi_device *)rt_device_find(dev_name);
         if (dev == RT_NULL) {
             rt_kprintf("Find device failed\n");
             return 0;
         }
         else {
             rt_kprintf("Find device successfully\n");
         }
    }
    else if (!rt_strcmp(argv[1], "send_then_recv"))
    {
        uint16_t recv_len = atoi(argv[2]);
        uint8_t send_buffer[32] = {0};
        uint8_t recv_buffer[32] = {0};
        uint8_t send_len = argc - 3;

        rt_kprintf("Echo: rl:%d tl:%d\n", recv_len, send_len);
        rt_kprintf("Echo: Data: ");
        for (int i=0; i<send_len; i++) {
            send_buffer[i] = htoi(argv[i+3]);
            rt_kprintf("0x%x ", send_buffer[i]);
        }
        rt_kprintf("\r\n");

        rt_spi_send_then_recv(dev, send_buffer, send_len, recv_buffer, recv_len);

        rt_kprintf("Received data: ");
        for (int i=0; i<recv_len; i++) {
            rt_kprintf("0x%x ", recv_buffer[i]);
        }
        rt_kprintf("\r\n");

    }
    
    return 0;
}

MSH_CMD_EXPORT(spibus, spibus tools)
