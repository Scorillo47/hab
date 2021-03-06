#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <sercx\2.0\Sercx.h>

#include "CommonDefinitions.h"

USHORT READ_SERIAL_TX_FIFO_SIZE();
USHORT READ_SERIAL_RX_FIFO_SIZE();

VOID UART_DEVICE_ENABLE(_In_ PUART_DEVICE_EXTENSION pDeviceExtension, _In_ BOOLEAN enable);

// set default values for memory registers
// get status register values from device and cache them in memory
VOID INITIALIZE_AND_BACKUP_REGISTERS_IN_MEMORY(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);
VOID RESTORE_REGISTERS_FROM_MEMORY(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);

VOID FIFO_CONTROL_ENABLE(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);

VOID INTERRUPT_DISABLE_ALL(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);
VOID INTERRUPT_ENABLE_LINE_STATUS_CHANGE(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);
VOID INTERRUPT_DISABLE_LINE_STATUS_CHANGE(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);
VOID INTERRUPT_ENABLE_RECEIVE_DATA_AVAILABLE(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);
// returns true if interrupt was actially disabled
BOOLEAN INTERRUPT_DISABLE_RECEIVE_DATA_AVAILABLE(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);

VOID DISABLE_REQUEST_TO_SEND(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);

BOOLEAN IS_FIFO_DATA_LOSS(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);

BOOLEAN CAN_RECEIVE_CHARACTER(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);
UCHAR RECEIVE_BUFFER_READ(_In_ PUART_DEVICE_EXTENSION pDeviceExtension);
