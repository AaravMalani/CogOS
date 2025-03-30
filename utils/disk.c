#include <utils/types.h>
uint32_t chsToLba(uint32_t cylinder, uint32_t head, uint32_t sector, uint32_t sectors_per_track, uint32_t tracks_per_head) {
    return (sector - 1) + cylinder*sectors_per_track + head*sectors_per_track*tracks_per_head;
}