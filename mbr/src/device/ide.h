#ifndef	__IDE_H
#define	__IDE_H
#include "stdint.h"
#include "list.h"
#include "bitmap.h"
#include "sync.h"
#include "global.h"
#include "superblock.h"
#include "console.h"
struct partition {
	uint32_t start_lba;
	uint32_t sec_cnt;
	struct disk * my_disk;
	struct list_elem part_tag;
	char name[8];
	struct super_block * sb;
	struct bitmap block_bitmap;
	struct bitmap inode_bitmap;
	struct list open_inodes;
};

struct disk{
	char name[8];
	struct ide_channel * my_channel;
	uint8_t dev_no;	
	struct partition prim_parts[4];	
	struct partition logic_parts[8];
};

struct ide_channel{
	char name[8];
	uint16_t port_base;
	uint8_t irq_no;	
	struct lock lock;
	uint32_t expecting_intr;
	struct semaphore disk_done;
	struct disk devices[2];
};

struct partition_table_entry{
	uint8_t bootable;
	uint8_t	start_head;	
	uint8_t start_sec;
	uint8_t	start_chs;	
	uint8_t fs_type;
	uint8_t	end_head;	
	uint8_t end_sec;
	uint8_t	end_chs;
	uint32_t start_lba;
	uint32_t sec_cnt;
}__attribute__((packed));

struct boot_sector{
	uint8_t other[446];
	struct partition_table_entry partition_table[4];
	uint16_t signature;
}__attribute__((packed));


extern uint8_t channel_cnt;
extern struct list partition_list;
extern struct ide_channel channels[2];
static void swap_pairs_bytes(const char * dst, char *buf, uint32_t len);

static void identify_disk(struct disk * hd);


void ide_init();
static void select_disk(struct disk* hd);
static void select_sector(struct disk * hd, uint32_t lba, uint8_t sec_cnt);
static void cmd_out(struct ide_channel * channel, uint8_t cmd);
static uint32_t busy_wait(struct disk * hd);
static void read_from_sector(struct disk * hd, void * buf, uint8_t sec_cnt);
void intr_hd_handler(uint8_t irq_no);
void ide_write(struct disk * hd, uint32_t lba, void * buf, uint32_t sec_cnt);
static uint32_t busy_wait(struct disk * hd);
void ide_read(struct disk * hd, uint32_t lba, void * buf, uint32_t sec_cnt);
static void partition_scan(struct disk * hd, uint32_t ext_lba);
static uint32_t partition_info(struct list_elem * pelem, int arg);
void write2sector(struct disk * hd, void * buf, uint8_t sec_cnt);
#endif
