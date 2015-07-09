MEMORY
{
    FLASH (rx) : ORIGIN = 0x00001800, LENGTH = 0x0003d800
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00010000
}

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > FLASH
	
    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        _data = .;
        *(vtable)
        *(.data*)
        _edata = .;
    } > SRAM
	
	.ARM.exidx : 
	{
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
	} > SRAM		
	
    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
		. = ALIGN (8);
		_end = .;
    } > SRAM
}

/* end of allocated ram _end */
PROVIDE( _HEAP_START = _end );

/* end of the heap -> align 8 byte */ 
PROVIDE ( _HEAP_END = ALIGN(ORIGIN(SRAM) + LENGTH(SRAM),8) );
