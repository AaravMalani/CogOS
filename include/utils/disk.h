#ifndef __DISK
#define __DISK
#include <utils/types.h>
struct CHSAddress {
    uint8_t head;
    uint8_t sector : 6;
    uint16_t cylinder : 10;
} __attribute__((packed)); 
struct PartitionTableEntry {
    uint8_t attributes;
    struct CHSAddress start;
    uint8_t systemId;
    struct CHSAddress end;
    uint32_t relativeSector;
    uint32_t totalSectors;
} __attribute__((packed));

struct MBR {
    uint8_t bootstrap[440];
    uint32_t uniqueDiskId;
    uint8_t reserved[2];
    struct PartitionTableEntry partTableEntries[4];
    uint8_t signature[2];
} __attribute__((packed));


#endif
