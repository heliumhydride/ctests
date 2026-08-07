#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/pe.h"

void print_usage(char* program) {
  fprintf(stderr, "usage: %s pe_file\n", program);
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

  DosStub_t* dos_stub = (DosStub_t*)buf;
  if(dos_stub->Magic != 0x5a4d) {
    fprintf(stderr, "%s: %s: not a pe formatted executable\n", program, argv[1]);
    free(buf);
    return 1;
  }

  uintptr_t pe_offset = dos_stub->e_lfanew;
  PeHeader_t* pe = (PeHeader_t*)(buf + pe_offset);
  if(pe->Magic != 0x00004550) {
    fprintf(stderr, "%s: %s: not a pe formatted executable\n", program, argv[1]);
    free(buf);
    return 1;
  }

  printf("Machine: ");
  switch(pe->Machine) {
    case IMAGE_FILE_MACHINE_UNKNOWN:
      puts("N/A");
      break;
    case IMAGE_FILE_MACHINE_ALPHA:
      puts("Alpha 32");
      break;
    case IMAGE_FILE_MACHINE_ALPHA64:
      puts("Alpha 64");
      break;
    case IMAGE_FILE_MACHINE_AM33:
      puts("Matsushita AM33");
      break;
    case IMAGE_FILE_MACHINE_AMD64:
      puts("AMD64");
      break;
    case IMAGE_FILE_MACHINE_ARM:
      puts("ARM");
      break;
    case IMAGE_FILE_MACHINE_ARM64:
      puts("ARM64");
      break;
    case IMAGE_FILE_MACHINE_ARM64EC:
      puts("ARM64 (Emulation Compatible)");
      break;
    case IMAGE_FILE_MACHINE_ARM64X:
      puts("Combined ARM64/ARM64EC");
      break;
    case IMAGE_FILE_MACHINE_ARMNT:
      puts("ARM (Thumb-2)");
      break;
    case IMAGE_FILE_MACHINE_EBC:
      puts("EFI Byte code");
      break;
    case IMAGE_FILE_MACHINE_I386:
      puts("Intel x86");
      break;
    case IMAGE_FILE_MACHINE_IA64:
      puts("Intel Itanium 64");
      break;
    case IMAGE_FILE_MACHINE_M32R:
      puts("Mitsubishi M32R");
      break;
    case IMAGE_FILE_MACHINE_MIPS16:
      puts("MIPS 16");
      break;
    case IMAGE_FILE_MACHINE_MIPSFPU:
      puts("MIPS (w/ FPU)");
      break;
    case IMAGE_FILE_MACHINE_MIPSFPU16:
      puts("MIPS16 (w/ FPU)");
      break;
    case IMAGE_FILE_MACHINE_POWERPC:
      puts("PowerPC");
      break;
    case IMAGE_FILE_MACHINE_POWERPCFP:
      puts("PowerPC (w/ float support)");
      break;
    case IMAGE_FILE_MACHINE_R4000:
      puts("MIPS little endian");
      break;
    case IMAGE_FILE_MACHINE_RISCV32:
      puts("RISC-V 32");
      break;
    case IMAGE_FILE_MACHINE_RISCV64:
      puts("RISC-V 64");
      break;
    case IMAGE_FILE_MACHINE_RISCV128 :
      puts("RISC-V 128");
      break;
    case IMAGE_FILE_MACHINE_SH3:
      puts("Hitachi SH3");
      break;
    case IMAGE_FILE_MACHINE_SH3DSP:
      puts("Hitachi SH3 DSP");
      break;
    case IMAGE_FILE_MACHINE_SH4:
      puts("Hitachi SH4");
      break;
    case IMAGE_FILE_MACHINE_SH5:
      puts("Hitachi SH5");
      break;
    case IMAGE_FILE_MACHINE_THUMB:
      puts("Thumb");
      break;
    case IMAGE_FILE_MACHINE_WCEMIPSV2:
      puts("MIPS little endian WCE v2");
      break;
    default:
      puts("Unknown");
  }

  printf("Number of Sectors: %d\n", pe->NumberOfSections);

  printf("Characteristics: ");
  if(pe->Characteristics & IMAGE_FILE_RELOCS_STRIPPED)
    printf("relocs_stripped ");
  if(pe->Characteristics & IMAGE_FILE_EXECUTABLE_IMAGE)
    printf("executable ");
  if(pe->Characteristics & IMAGE_FILE_LARGE_ADDRESS_AWARE)
    printf("large_address_aware ");
  if(pe->Characteristics & IMAGE_FILE_32BIT_MACHINE)
    printf("32bit ");
  if(pe->Characteristics & IMAGE_FILE_DEBUG_STRIPPED)
    printf("debug_stripped ");
  if(pe->Characteristics & IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP)
    printf("removable_run_from_swap ");
  if(pe->Characteristics & IMAGE_FILE_NET_RUN_FROM_SWAP)
    printf("net_run_from_swap ");
  if(pe->Characteristics & IMAGE_FILE_SYSTEM)
    printf("system ");
  if(pe->Characteristics & IMAGE_FILE_DLL)
    printf("dll ");
  if(pe->Characteristics & IMAGE_FILE_UP_SYSTEM_ONLY)
    printf("uniprocessor_only ");
  putchar('\n');

  int is_pe_plus = 0;
  Pe32OptHeader_t* pe_opt = (Pe32OptHeader_t*)((uint8_t*)pe + sizeof(PeHeader_t));
  Pe32PlusOptHeader_t* peplus_opt;
  if(pe_opt->Magic == 0x20b) {
    is_pe_plus = 1;
    peplus_opt = (Pe32PlusOptHeader_t*)pe_opt;
  }

  printf("PE Type: PE32%c\n", is_pe_plus ? '+' : '\0');

  printf("Entry at: ");
  if(is_pe_plus)
    printf("%p", peplus_opt->AddressOfEntryPoint);
  else
    printf("%p", pe_opt->AddressOfEntryPoint);
  putchar('\n');

  printf("OS Version: ");
  if(is_pe_plus) {
    printf("%d.%d",
      peplus_opt->MajorOperatingSystemVersion,
      peplus_opt->MinorOperatingSystemVersion
    );
  }
  else {
    printf("%d.%d",
      pe_opt->MajorOperatingSystemVersion,
      pe_opt->MinorOperatingSystemVersion
    );
  }
  putchar('\n');

  printf("Subsystem: ");
  uint16_t subsystem;
  if(is_pe_plus)
    subsystem = peplus_opt->Subsystem;
  else
    subsystem = pe_opt->Subsystem;
  switch(subsystem) {
    case IMAGE_SUBSYSTEM_NATIVE:
      puts("NT Native");
      break;
    case IMAGE_SUBSYSTEM_WINDOWS_GUI:
      puts("Win32 GUI");
      break;
    case IMAGE_SUBSYSTEM_WINDOWS_CUI:
      puts("Win32 Console");
      break;
    case IMAGE_SUBSYSTEM_OS2_CUI:
      puts("OS/2 Console");
      break;
    case IMAGE_SUBSYSTEM_POSIX_CUI:
      puts("POSIX Console");
      break;
    case IMAGE_SUBSYSTEM_NATIVE_WINDOWS:
      puts("Windows 9x VXD");
      break;
    case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
      puts("Windows CE");
      break;
    case IMAGE_SUBSYSTEM_EFI_APPLICATION:
      puts("EFI Application");
      break;
    case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
      puts("EFI Driver (w/ Boot services)");
      break;
    case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
      puts("EFI Driver (w/ Runtime services)");
      break;
    case IMAGE_SUBSYSTEM_EFI_ROM:
      puts("EFI ROM");
      break;
    case IMAGE_SUBSYSTEM_XBOX:
      puts("XBOX");
      break;
    case IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION:
      puts("Windows boot application");
      break;
    default:
      puts("Unknown");
  }

  printf("Subsystem Version: ");
  if(is_pe_plus) {
    printf("%d.%d",
      peplus_opt->MajorSubsystemVersion,
      peplus_opt->MinorSubsystemVersion
    );
  }
  else {
    printf("%d.%d",
      pe_opt->MajorSubsystemVersion,
      pe_opt->MinorSubsystemVersion
    );
  }
  putchar('\n');

  printf("Checksum: ");
  if(is_pe_plus)
    printf("%08x", peplus_opt->CheckSum);
  else
    printf("%08x", pe_opt->CheckSum);
  putchar('\n');

  free(buf);
  return 0;
}
