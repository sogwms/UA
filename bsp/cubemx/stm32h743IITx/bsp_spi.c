// TODO 数据大小 超过 65535 的发送处理
#include <hal_spi_impl.h>
#include <spi.h>
#include "bsp_spi.h"
#include <string.h>

extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

#define BSP_SPI_USING_IRQ_FREERTOS
#ifdef BSP_SPI_USING_IRQ_FREERTOS

#include <FreeRTOS.h>
#include <semphr.h>
static SemaphoreHandle_t g_sem_txrx_complete = 0;

void __transimit_complete_cb(SPI_HandleTypeDef *hspi)
{
    if ( hspi == &hspi2 ){
        printf("spi cb\n");
        BaseType_t xHigherPriorityTaskWoken;
        xSemaphoreGiveFromISR(g_sem_txrx_complete, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    __transimit_complete_cb(hspi);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    __transimit_complete_cb(hspi);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    __transimit_complete_cb(hspi);
}

#endif 

static int configure(struct ua_spi_device *device, struct ua_spi_configuration *configuration)
{
    return UA_EOK;
}

static int transfer(struct ua_spi_device *device, struct ua_spi_message *message)
{
    //TODO
    struct bsp_spi_cs *cs = (struct bsp_spi_cs *)device->parent.user_data;
    SPI_HandleTypeDef *hspix = &hspi2;

    if (message->cs_take && !(device->config.mode & UA_SPI_NO_CS))
    {
        HAL_GPIO_WritePin(cs->GPIOx, cs->GPIO_Pin, GPIO_PIN_RESET);
    }

    uint32_t message_length;
    uint8_t *recv_buf;
    const uint8_t *send_buf;
    int state;

    message_length = message->length;
    recv_buf = message->recv_buf;
    send_buf = message->send_buf;

    /* start once data exchange in DMA mode */
    if (message->send_buf && message->recv_buf)
    {
#ifdef BSP_SPI_USING_IRQ_FREERTOS         
        taskENTER_CRITICAL();     
#endif           
        state = HAL_SPI_TransmitReceive_IT(hspix, (uint8_t *)send_buf, (uint8_t *)recv_buf, message_length);
#ifdef BSP_SPI_USING_IRQ_FREERTOS         
        taskEXIT_CRITICAL();     
#endif
    }
    else if (message->send_buf)
    {
#ifdef BSP_SPI_USING_IRQ_FREERTOS         
        taskENTER_CRITICAL();     
#endif   
        state = HAL_SPI_Transmit_IT(hspix, (uint8_t *)send_buf, message_length);
#ifdef BSP_SPI_USING_IRQ_FREERTOS          
        taskEXIT_CRITICAL();
#endif        
        // if (message->cs_release && (device->config.mode & UA_SPI_3WIRE))
        // {
        //     /* release the CS by disable SPI when using 3 wires SPI */
        //     __HAL_SPI_DISABLE(&hspi3);
        // }
    }
    else
    {
        memset((uint8_t *)recv_buf, 0xff, message_length);

        /* clear the old error flag */
        __HAL_SPI_CLEAR_OVRFLAG(hspix);
#ifdef BSP_SPI_USING_IRQ_FREERTOS         
        taskENTER_CRITICAL();     
#endif           
        state = HAL_SPI_Receive_IT(hspix, (uint8_t *)recv_buf, message_length);
#ifdef BSP_SPI_USING_IRQ_FREERTOS          
        taskEXIT_CRITICAL();
#endif           
    }

    if (state != HAL_OK)
    {
        // LOG_I("spi transfer error : %d", state);
        message->length = 0;
        hspix->State = HAL_SPI_STATE_READY;
        return UA_ERROR;
    }
    else
    {
        // LOG_D("%s transfer done", spi_drv->config->bus_name);
    }

#ifdef BSP_SPI_USING_IRQ_FREERTOS 
    xSemaphoreTake(g_sem_txrx_complete, portMAX_DELAY);
#else
    /* For simplicity reasons, this example is just waiting till the end of the
           transfer, but application may perform other tasks while transfer operation
           is ongoing. */
    while (HAL_SPI_GetState(hspix) != HAL_SPI_STATE_READY);
#endif

    if (message->cs_release && !(device->config.mode & UA_SPI_NO_CS))
    {
        HAL_GPIO_WritePin(cs->GPIOx, cs->GPIO_Pin, GPIO_PIN_SET);
    }

    return UA_EOK;
}

static struct ua_spi_ops ops = {
    .configure = configure,
    .transfer = transfer,
};

static ua_spi_bus_t bus;

int bsp_spi_init(void)
{
    printf("bsp_spi_init\r\n");

    MX_SPI2_Init();
    ua_spi_bus_register(&bus, "spi2", &ops);

    // MX_SPI3_Init();
    // ua_spi_bus_register(&bus, "spi3", &ops);

    return 0;
}

#include <module_init.h>
bsp_init(bsp_spi_init);

#ifdef BSP_SPI_USING_IRQ_FREERTOS
int bsp_spi_sem_init(void)
{
    static int isInit = 0;

    printf("enter init bsp spi sem\n");
    if (isInit) {
        return 0;
    }
    isInit = 1;
    printf("init bsp spi sem\n");
    g_sem_txrx_complete = xSemaphoreCreateBinary();
    return 0;
}
#endif

// #include <rtthread.h>
// INIT_APP_EXPORT(bsp_spi_init);

static ua_spi_device_t spi_device;

static int _init_cs(struct bsp_spi_cs *cs)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (cs->GPIOx == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if (cs->GPIOx == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if (cs->GPIOx == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }
    else {
        printf("unknown GPIO\n");
        return -1;
    }

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(cs->GPIOx, cs->GPIO_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = cs->GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(cs->GPIOx, &GPIO_InitStruct);

    return 0;
}

int bsp_hw_spi_attach_device(const char *name, const char *bus_name, struct bsp_spi_cs *cs)
{
    _init_cs(cs);
#ifdef BSP_SPI_USING_IRQ_FREERTOS
    bsp_spi_sem_init();
#endif    
    return ua_spi_bus_attach_device(&spi_device, name, bus_name, cs);
}
