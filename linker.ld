OUTPUT_FORMAT(binary)
SECTIONS
{
  . = 0xC0008000;
  .text  ALIGN (4K) : AT (ADDR (.text) - 0xC0000000) { *(.text) }
  .data  ALIGN (4K) : AT (ADDR (.data) - 0xC0000000) { *(.data) }
  .bss  ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000) { *(.bss) }
}