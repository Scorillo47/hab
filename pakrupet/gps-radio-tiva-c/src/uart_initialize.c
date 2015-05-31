#include "uart.h"
#include "uart_impl.h"

#include <string.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>

#include <driverlib/rom.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/rom_map.h>

struct UartChannelData uartChannelData[UART_NUMBER_OF_CHANNELS];
struct UartChannelData* uart2UartChannelData[UART_COUNT];

void initializeUart(void)
{
    memset(uartChannelData, 0, sizeof(uartChannelData));
    memset(uart2UartChannelData, 0, sizeof(uart2UartChannelData));
}

bool initializeUartChannel(uint8_t channel,
                           uint8_t uartPort,
                           uint32_t baudRate,
                           uint32_t cpuSpeedHz,
                           uint32_t flags)
{
    if (channel >= UART_NUMBER_OF_CHANNELS ||
        uartPort >= UART_COUNT)
    {
        return false;
    }

    if (uart2UartChannelData[uartPort] != 0)
    {
        return false;
    }

    if (!(flags & UART_FLAGS_RECEIVE) && !(flags & UART_FLAGS_SEND))
    {
        return false;
    }

    uint32_t uartBase;
    uint32_t uartInterruptId;
    uint32_t uartPeripheralSysCtl;

    switch (uartPort)
    {
        case UART_1:
        {
            uartBase = UART1_BASE;
            uartInterruptId = INT_UART1;
            uartPeripheralSysCtl = SYSCTL_PERIPH_UART1;
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
            ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
            ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
            ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
            break;
        }
        case UART_2:
        {
            uartBase = UART2_BASE;
            uartInterruptId = INT_UART2;
            uartPeripheralSysCtl = SYSCTL_PERIPH_UART2;
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
            ROM_GPIOPinConfigure(GPIO_PD6_U2RX);
            ROM_GPIOPinConfigure(GPIO_PD7_U2TX);
            ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
            break;
        }
        case UART_3:
        {
            uartBase = UART3_BASE;
            uartInterruptId = INT_UART3;
            uartPeripheralSysCtl = SYSCTL_PERIPH_UART3;
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
            ROM_GPIOPinConfigure(GPIO_PC6_U3RX);
            ROM_GPIOPinConfigure(GPIO_PC7_U3TX);
            ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
            break;
        }
        case UART_4:
        {
            uartBase = UART4_BASE;
            uartInterruptId = INT_UART4;
            uartPeripheralSysCtl = SYSCTL_PERIPH_UART4;
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
            ROM_GPIOPinConfigure(GPIO_PC4_U4RX);
            ROM_GPIOPinConfigure(GPIO_PC5_U4TX);
            ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
            break;
        }
        default:
        {
            return false;
        }
    }

    if(!MAP_SysCtlPeripheralPresent(uartPeripheralSysCtl))
    {
        return false;
    }

    MAP_SysCtlPeripheralEnable(uartPeripheralSysCtl);

    UARTClockSourceSet(uartBase, UART_CLOCK_PIOSC);
    
    MAP_UARTConfigSetExpClk(uartBase,
                            cpuSpeedHz,
                            baudRate, 
                            (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    MAP_UARTFIFOLevelSet(uartBase, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    MAP_UARTIntDisable(uartBase, 0xFFFFFFFF);

    if (flags & UART_FLAGS_RECEIVE)
    {
        MAP_UARTIntEnable(uartBase, UART_INT_RX | UART_INT_RT);
        MAP_IntEnable(uartInterruptId);
    }

    MAP_UARTEnable(uartBase);

    uartChannelData[channel].base = uartBase;
    uartChannelData[channel].interruptId = uartInterruptId;
    uart2UartChannelData[uartPort] = &uartChannelData[channel];

    return true;
}

void Uart1IntHandler(void)
{
    const uint32_t status = MAP_UARTIntStatus(uart2UartChannelData[UART_1]->base, true);
    MAP_UARTIntClear(uart2UartChannelData[UART_1]->base, status);

    if (status & (UART_INT_RX | UART_INT_RT))
    {
        UartReadIntHandler(uart2UartChannelData[UART_1]);
    }
    if (status & UART_INT_TX)
    {
        UartWriteIntHandler(uart2UartChannelData[UART_1]);
    }
}

void Uart2IntHandler(void)
{
    const uint32_t status = MAP_UARTIntStatus(uart2UartChannelData[UART_2]->base, true);
    MAP_UARTIntClear(uart2UartChannelData[UART_2]->base, status);

    if (status & (UART_INT_RX | UART_INT_RT))
    {
        UartReadIntHandler(uart2UartChannelData[UART_2]);
    }
    if (status & UART_INT_TX)
    {
        UartWriteIntHandler(uart2UartChannelData[UART_2]);
    }
}

void Uart3IntHandler(void)
{
    const uint32_t status = MAP_UARTIntStatus(uart2UartChannelData[UART_3]->base, true);
    MAP_UARTIntClear(uart2UartChannelData[UART_3]->base, status);

    if (status & (UART_INT_RX | UART_INT_RT))
    {
        UartReadIntHandler(uart2UartChannelData[UART_3]);
    }
    if (status & UART_INT_TX)
    {
        UartWriteIntHandler(uart2UartChannelData[UART_3]);
    }
}

void Uart4IntHandler(void)
{
    const uint32_t status = MAP_UARTIntStatus(uart2UartChannelData[UART_4]->base, true);
    MAP_UARTIntClear(uart2UartChannelData[UART_4]->base, status);

    if (status & (UART_INT_RX | UART_INT_RT))
    {
        UartReadIntHandler(uart2UartChannelData[UART_4]);
    }
    if (status & UART_INT_TX)
    {
        UartWriteIntHandler(uart2UartChannelData[UART_4]);
    }
}

uint8_t advanceIndex(uint8_t currentValue)
{
    ++currentValue;
    if (currentValue >= UART_BUFFER_MAX_LEN)
    {
        currentValue = 0;
    }
    return currentValue;
}
