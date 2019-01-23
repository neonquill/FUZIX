#include <kernel.h>
#include <timer.h>
#include <kdata.h>
#include <printf.h>
#include <devtty.h>

void platform_idle(void)
{
}

uint8_t need_resched;

uint8_t platform_param(char *p)
{
	return 0;
}

void platform_discard(void)
{
}

void memzero(void *p, usize_t len)
{
	memset(p, 0, len);
}

void platform_copyright(void)
{
}

void do_beep(void)
{
}

void map_init(void)
{
}

u_block uarea_block[PTABSIZE];

void *screenbase;

void pagemap_init(void)
{
	/* FIXME: 512K hackish setup to get going */
	/* Linker provided end of kernel */
	extern uint8_t _end;
	uint32_t e = (uint32_t)&_end;
	kprintf("Kernel end %p\n", e);
	/* Allocate the rest of memory to the userspace */
	kmemaddblk((void *)e, 0x78000 - e);
}

/* Udata and kernel stacks */
/* We need an initial kernel stack and udata so the slot for init is
   set up at compile time */
u_block udata_block0;
static u_block *udata_block[PTABSIZE] = {&udata_block0,};

/* This will belong in the core 68K code once finalized */

void install_vdso(void)
{
	extern uint8_t vdso[];
	/* Should be uput etc */
	memcpy((void *)udata.u_codebase, &vdso, 0x40);
}

uint8_t platform_udata_set(ptptr p)
{
	u_block **up = &udata_block[p - ptab];
	if (*up == NULL) {
		*up = kmalloc(sizeof(struct u_block));
		if (*up == NULL)
			return ENOMEM;
	}
	p->p_udata = &(*up)->u_d;
	return 0;
}
