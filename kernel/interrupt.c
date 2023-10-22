#include <stdint.h>
#include <stddef.h>

#include "arch/amd64/amd64.h"
#include "interrupt.h"
#include "descriptor.h"
#include "stream.h"

#if defined(__x86_64__)
#include "memory.h"

idt_t idt[256];

void * apic_base = NULL;

void interrupt_register(uint8_t vector, void * interrupt_handler, uint8_t flags)
{
	uint64_t handler_interrupt = (uint64_t)interrupt_handler;

	idt[vector].offset_low = (uint16_t)handler_interrupt;
	idt[vector].offset_middle = (uint16_t)(handler_interrupt >> 16);
	idt[vector].offset_high = (uint32_t)(handler_interrupt >> 32);
	idt[vector].selector = 0x28;
	idt[vector].stack_table = 0;
	idt[vector].flags = flags;
	idt[vector].reserved = 0;
}

void interrupt_flush()
{
	descriptor_t descriptor;
	descriptor.limit = sizeof(idt) - 1;
	descriptor.base = (uint64_t)&idt;

	asm volatile ("lidt %0" :: "m"(descriptor) : "memory");
}

void interrupt0(frame_t * frame)
{
	(void)frame;
	stream_printf(current_stream, "Exception triggered!\r\nDivision error occured in kernel!\r\n");

	hang();
}

void interrupt2(frame_t * frame)
{
	(void)frame;
	stream_printf(current_stream, "Exception triggered!\r\nNMI (Non Maskable Interrupt) occured!\r\n");

	hang();
}

void interrupt4(frame_t * frame)
{
	(void)frame;
	stream_printf(current_stream, "Exception triggered!\r\nOverflow occured in kernel!\r\n");

	hang();
}

void interrupt5(frame_t * frame)
{
	(void)frame;
	stream_printf(current_stream, "Exception triggered!\r\nUnauthorized access in memory!\r\n");

	hang();
}

void interrupt6(frame_t * frame)
{
	(void)frame;
	stream_printf(current_stream, "Exception triggered!\r\nInvalid opcode!\r\n");

	hang();
}

void interrupt7(frame_t * frame)
{
	(void)frame;
	stream_printf(current_stream, "Exception triggered!\r\nFPU (Floating-Point Unit) fault!\r\n");

	hang();
}

#endif