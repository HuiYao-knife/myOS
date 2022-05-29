#ifndef	__INODE_H
#define	__INODE_H
#include "stdint.h"
#include "list.h"
#include "ide.h"
struct inode_position{
	uint32_t two_sec;
	uint32_t sec_lba;
	uint32_t off_size;
};
struct inode{
	uint32_t i_no;
	uint32_t i_size;
	uint32_t i_open_cnts;
	uint32_t write_deny;
	
	uint32_t i_sectors[13];
	struct list_elem inode_tag;
};
static void inode_locate(struct partition * part, uint32_t inode_no, struct inode_position * inode_pos);
void inode_sync(struct partition * part, struct inode * inode ,void * io_buf);
struct inode * inode_open(struct partition * part, uint32_t inode_no);
void inode_init(uint32_t inode_no, struct inode * new_inode);
void inode_close(struct inode * inode);
void inode_release(struct partition * part, uint32_t inode_no);
void inode_delete(struct partition * part, uint32_t inode_no, void * io_buf);
#endif
