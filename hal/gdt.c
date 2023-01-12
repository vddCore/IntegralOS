/*
 * File name: gdt.c
 * Description: x86-specific GDT implementation
 *
 * * * */

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <hal/gdt.h>

gdt_entry_t create_gdt_entry(uint32_t base_addr, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt_entry_t entry;
    memset(&entry, 0, sizeof(gdt_entry_t));

    entry.base_low = base_addr & 0xFFFF;
    entry.base_mid = (base_addr >> 16) & 0xFF;
    entry.base_high = (base_addr >> 24) & 0xFF;

    entry.limit_low = limit & 0xFFFF;
    entry.access = access;
    entry.granularity = (limit >> 16) & 0x0F;
    entry.granularity |= granularity & 0xF0;
    return entry;
}

gdt_entry_t create_gdt_null_entry(void) {
    return create_gdt_entry(0, 0, 0, 0);
}

gdt_entry_t create_kernelspace_entry(uint32_t base_addr, uint32_t limit, bool is_code) {
    uint16_t can_execute = 0;

    if(is_code) {
        can_execute = ACCESS_DESC_EXECUTE;
    }

    return create_gdt_entry(
        base_addr,
        limit,
        ACCESS_DESC_READWRITE | can_execute | ACCESS_DESC_RESERVED | ACCESS_DESC_PRESENT,
        FLAGS_SIZE_32BIT | FLAGS_GRANULARITY_4K | 0x0F
    );
}

gdt_entry_t create_userspace_entry(uint32_t base_addr, uint32_t limit, bool is_code) {
    uint16_t can_execute = 0;

    if(is_code) {
        can_execute = ACCESS_DESC_EXECUTE;
    }

    return create_gdt_entry(
        base_addr,
        limit,
        ACCESS_DESC_READWRITE | can_execute | ACCESS_DESC_RESERVED | ACCESS_DESC_PRESENT | ACCESS_DESC_PRIVILEGE_RING_3,
        FLAGS_SIZE_32BIT | FLAGS_GRANULARITY_4K | 0x0F
    );
}