#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/elf.h"

void print_usage(char* program) {
  fprintf(stderr, "usage: %s elf_file\n", program);
}

int main(int argc, char* argv[]) {
  char* program = argv[0];
  if(argc < 2) {
    print_usage(program);
    return 1;
  }

  FILE* fp = fopen(argv[1], "rb");
  if(fp == NULL) {
    fprintf(stderr, "%s: %s: could not open file\n", program, argv[1]);
    return 1;
  }
  size_t filesize;
  fseek(fp, 0L, SEEK_END);
  filesize = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  uint8_t* buf = malloc(filesize);
  if(buf == NULL) {
    fprintf(stderr, "%s: malloc() failed\n", program);
    fclose(fp);
    return 2;
  }
  fread(buf, filesize, 1, fp);
  fclose(fp);

  Elf64_Ehdr* elf = (Elf64_Ehdr*)buf;
  char* magic_buf = malloc(5);
  if(magic_buf == NULL) {
    fprintf(stderr, "%s: malloc() failed\n", program);
    return 2;
  }
  memcpy(magic_buf, buf, 4);
  magic_buf[4] = '\0';
  if(strcmp(ELF_MAGIC, magic_buf)) {
    fprintf(stderr, "%s: %s: not an elf formatted file\n", program, argv[1]);
    free(buf);
    free(magic_buf);
    return 1;
  }
  free(magic_buf);
  
  printf("bitness: ");
  switch(elf->e_ident[4]) {
    case BITS_32:
      puts("32");
      break;
    case BITS_64:
      puts("64");
      break;
    default:
      puts("??");
break;
  }

  printf("os:      ");
  switch(elf->e_ident[7]) {
    case ABI_SYSV:
      puts("sysv");
      break;
    case ABI_HPUX:
      puts("hp-ux");
      break;
    case ABI_NETBSD:
      puts("netbsd");
      break;
    case ABI_LINUX:
      puts("linux");
      break;
    case ABI_HURD:
      puts("hurd");
      break;
    case ABI_86OPEN:
      puts("86open");
      break;
    case ABI_SOLARIS:
      puts("solaris");
      break;
    case ABI_AIX:
      puts("hp aix");
      break;
    case ABI_IRIS:
      puts("iris");
      break;
    case ABI_FREEBSD:
      puts("freebsd");
      break;
    case ABI_TRU64:
      puts("tru64");
      break;
    case ABI_MODESTO:
      puts("modesto");
      break;
    case ABI_OPENBSD:
      puts("openbsd");
      break;
    default:
      puts("unknown");
  }

  printf("arch:    ");
  switch(elf->e_machine) {
    case EM_NONE:
      puts("none");
      break;
    case EM_386:
      puts("i386");
      break;
    case EM_PPC:   // fall
    case EM_PPC64:
      puts("powerpc");
      break;
    case EM_ARM:
      puts("arm");
      break;
    case EM_X86_64:
      puts("x86_64");
      break;
    case EM_AVR:
      puts("avr");
      break;
    case EM_AARCH64:
      puts("aarch64");
      break;
    case EM_RISCV:
      puts("risc-v");
      break;
    case EM_LOONG:
      puts("loongarch");
      break;
    default:
      puts("unknown");
  }

  int is_dyn_and_exec = 0;
  char* interp;
  Elf64_Phdr* phdr = (Elf64_Phdr*)(buf + elf->e_phoff);
  for(int i = 0; i < elf->e_phnum; i++) {
    if(phdr->p_type == 3) {
      is_dyn_and_exec = 1;
      interp = (char*)(buf + phdr->p_offset);
    }
    phdr = (Elf64_Phdr*)((uint8_t*)phdr + elf->e_phentsize);
  }
  
  printf("type:    ");
  switch(elf->e_type) {
    case ET_REL:
      puts("relocatable");
      break;
    case ET_EXEC:
      puts("executable");
      break;
    case ET_DYN:
      if(is_dyn_and_exec)
        printf("dynamic executable [%s]\n", interp);
      else
        puts("shared lib");
      break;
    case ET_CORE:
      puts("core dump");
      break;
  }

  free(buf);
  return 0;
}
