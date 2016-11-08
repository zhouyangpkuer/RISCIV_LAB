#ifndef _LOAD_H
#define _LOAD_H
#include "params.h"

char buff[10000000];
uchar *load(int fd)
{

	Elf64_Ehdr *p_ehdr;
	read(fd, buff, 64);
	p_ehdr = (Elf64_Ehdr *)buff;



	//program head table off(byte)
	ulint e_phoff = p_ehdr->e_phoff;
	//program head item size(byte)
	uint e_phentsize = p_ehdr->e_phentsize;
	//program head item num
	uint e_phnum = p_ehdr->e_phnum;

	//section head off(byte)
	ulint e_shoff = p_ehdr->e_shoff;
	//section head size(byte)
	uint e_shentsize = p_ehdr->e_shentsize;
	//section head num
	uint e_shnum = p_ehdr->e_shnum;

	uchar * p_entry = (uchar *)(p_ehdr->e_entry);



	// lseek(fd, e_phoff, SEEK_SET);
	Elf64_Phdr *p_phdr;
	for(int i = 0; i < e_phnum; i++)
	{
		lseek(fd, e_phoff + i * e_phentsize, SEEK_SET);
		read(fd, buff, e_phentsize);
		p_phdr = (Elf64_Phdr *)buff;

		uint p_type = p_phdr->p_type;
		ulint p_offset = p_phdr->p_offset;
		ulint p_vaddr = p_phdr->p_vaddr;
		ulint p_filesz = p_phdr->p_filesz;
		ulint p_memsz = p_phdr->p_memsz;

		// printf("%x\n", p_offset);
		// printf("%x\n", p_vaddr);
		// printf("%x\n", p_filesz);
		// printf("%x\n", p_memsz);

		if(p_type != PT_LOAD)
			continue;

		lseek(fd, p_offset, SEEK_SET);
		read(fd, buff, p_filesz);
		
		if(p_vaddr >= VM_SIZE)
		{
			printf("In proghdr, Vm memory is too samll!\n");
			exit(0);
		}

		memcpy(vm + p_vaddr, buff, p_filesz);
		memset(vm + p_vaddr + p_filesz, 0, p_memsz - p_filesz);
	}

	Elf64_Shdr * p_shdr;
	for(int i = 0; i < e_shnum; i++)
	{
		lseek(fd, e_shoff + i * e_shentsize, SEEK_SET);
		read(fd, buff, e_shentsize);
		p_shdr = (Elf64_Shdr *)buff;

		ulint sh_addr = p_shdr->sh_addr;
		ulint sh_offset = p_shdr->sh_offset;
		ulint sh_size = p_shdr->sh_size;

		// printf("%x\n", sh_addr);
		// printf("%x\n", sh_offset);
		// printf("%x\n", sh_size);

		if(sh_addr == 0)
			continue;

		lseek(fd, sh_offset, SEEK_SET);
		read(fd, buff, sh_size);

		if(sh_addr >= VM_SIZE)
		{
			printf("In shdr, Vm memory is too samll!\n");
			exit(0);
		}

		memcpy(vm + sh_addr, buff, sh_size);
	}
	return p_entry;
}



#endif //_LOAD_H