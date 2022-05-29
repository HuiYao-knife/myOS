#include "stdint.h"
#include "bitmap.h"
#include "debug.h"
void bitmap_init(struct bitmap * btmp)
{
	memset(btmp->bits, 0, btmp->btmp_bytes_len);
}
bool bitmap_scan_test(struct bitmap* btmp, uint32_t bit_idx)
{
	uint32_t byte_idx = bit_idx / 8;
	uint32_t offset = bit_idx % 8;
	return (btmp->bits[byte_idx] & (BITMAP_MASK << offset));
}
int bitmap_scan(struct bitmap* btmp, uint32_t cnt)
{
	uint32_t idx_byte = 0;
	while((0xff == btmp->bits[idx_byte]) && (idx_byte < btmp->btmp_bytes_len)){
		idx_byte++;
	}
	ASSERT(idx_byte < btmp->btmp_bytes_len);
	if(idx_byte == btmp->btmp_bytes_len){
		return -1;
	}
	int offset = 0;
	while((uint8_t)(btmp->bits[idx_byte] & BITMAP_MASK << offset)){
		offset++;
	}
	int bit_idx_start = idx_byte * 8 + offset;
	if(cnt == 1){
		return bit_idx_start;
	}
	uint32_t bit_left = (btmp->btmp_bytes_len * 8 - bit_idx_start);
	uint32_t next_bit = bit_idx_start + 1;
	uint32_t num = 1;
	bit_idx_start = -1;
	while(bit_left--){
		if(!(bitmap_scan_test(btmp, next_bit))){
			num++;
		}else{
			num = 0;
		}
		if(num == cnt){
			bit_idx_start = next_bit - cnt + 1;
			break;
		}
		next_bit++;
	}
	return bit_idx_start;
}

void bitmap_set(struct bitmap* btmp, uint32_t bit_idx, int8_t value)
{
	ASSERT((value == 0) || (value == 1));
	uint32_t byte_idx = bit_idx / 8;
	uint32_t offset = bit_idx % 8;
	if(value){
		btmp->bits[byte_idx] |= (BITMAP_MASK << offset);
	}else{
		btmp->bits[byte_idx] &= ~(BITMAP_MASK << offset);
	}
}
