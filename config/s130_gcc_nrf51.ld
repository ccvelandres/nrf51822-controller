/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

/* Memory sizes for NRF51822 - 256kB, 16kB */
_FLASH_START = 0x0;
_FLASH_SIZE = 0x40000;
_RAM_START = 0x20000000;
_RAM_SIZE = 0x4000;

_S130_RAM_SIZE = 0x1fe8;
_S130_FLASH_SIZE = 0x1b000;

_APP_RAM_START = _RAM_START + _S130_RAM_SIZE;
_APP_RAM_SIZE = _RAM_SIZE - _S130_RAM_SIZE;
_APP_FLASH_START = _FLASH_START + _S130_FLASH_SIZE;
_APP_FLASH_SIZE = _FLASH_SIZE - _APP_FLASH_START;

MEMORY
{
  FLASH (rx) : ORIGIN = _APP_FLASH_START, LENGTH = _APP_FLASH_SIZE
  RAM (rwx) :  ORIGIN = _APP_RAM_START, LENGTH = _APP_RAM_SIZE
}

SECTIONS
{
  .fs_data :
  {
    PROVIDE(__start_fs_data = .);
    KEEP(*(.fs_data))
    PROVIDE(__stop_fs_data = .);
  } > RAM
  .pwr_mgmt_data :
  {
    PROVIDE(__start_pwr_mgmt_data = .);
    KEEP(*(.pwr_mgmt_data))
    PROVIDE(__stop_pwr_mgmt_data = .);
  } > RAM
} INSERT AFTER .data;

INCLUDE "nrf5x_common.ld"
