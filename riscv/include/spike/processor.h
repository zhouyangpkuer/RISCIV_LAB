// See LICENSE for license details.
#ifndef _RISCV_PROCESSOR_H
#define _RISCV_PROCESSOR_H

#include "decode.h"
#include "config.h"
#include "devices.h"
#include <string>
#include <vector>
#include <map>

class processor_t;
class mmu_t;
typedef reg_t (*insn_func_t)(processor_t*, insn_t, reg_t);
class sim_t;
class trap_t;
class extension_t;
class disassembler_t;

struct insn_desc_t
{
  insn_bits_t match;
  insn_bits_t mask;
  insn_func_t rv32;
  insn_func_t rv64;
};

struct commit_log_reg_t
{
  reg_t addr;
  reg_t data;
};

// architectural state of a RISC-V hart
struct state_t
{
  void reset();

  reg_t pc;
  regfile_t<reg_t, NXPR, true> XPR;
  regfile_t<freg_t, NFPR, false> FPR;

  // control and status registers
  reg_t mstatus;
  reg_t mepc;
  reg_t mbadaddr;
  reg_t mtimecmp;
  reg_t mscratch;
  reg_t mcause;
  reg_t minstret;
  reg_t mie;
  reg_t mip;
  reg_t sepc;
  reg_t sbadaddr;
  reg_t sscratch;
  reg_t stvec;
  reg_t sptbr;
  reg_t scause;
  reg_t sutime_delta;
  reg_t suinstret_delta;
  reg_t tohost;
  reg_t fromhost;
  uint32_t fflags;
  uint32_t frm;
  bool serialized; // whether timer CSRs are in a well-defined state

  reg_t load_reservation;

#ifdef RISCV_ENABLE_COMMITLOG
  commit_log_reg_t log_reg_write;
  reg_t last_inst_priv;
#endif
};

// this class represents one processor in a RISC-V machine.
class processor_t : public abstract_device_t
{
public:
  processor_t(const char* isa, sim_t* sim, uint32_t id);
  ~processor_t();

  void set_debug(bool value);
  void set_histogram(bool value);
  void reset(bool value);
  void step(size_t n); // run for n cycles
  bool running() { return run; }
  void set_csr(int which, reg_t val);
  void raise_interrupt(reg_t which);
  reg_t get_csr(int which);
  mmu_t* get_mmu() { return mmu; }
  state_t* get_state() { return &state; }
  extension_t* get_extension() { return ext; }
  bool supports_extension(unsigned char ext) {
    if (ext >= 'a' && ext <= 'z') ext += 'A' - 'a';
    return ext >= 'A' && ext <= 'Z' && ((cpuid >> (ext - 'A')) & 1);
  }
  void push_privilege_stack();
  void pop_privilege_stack();
  void yield_load_reservation() { state.load_reservation = (reg_t)-1; }
  void update_histogram(reg_t pc);

  void register_insn(insn_desc_t);
  void register_extension(extension_t*);

  // MMIO slave interface
  bool load(reg_t addr, size_t len, uint8_t* bytes);
  bool store(reg_t addr, size_t len, const uint8_t* bytes);

private:
  sim_t* sim;
  mmu_t* mmu; // main memory is always accessed via the mmu
  extension_t* ext;
  disassembler_t* disassembler;
  state_t state;
  reg_t cpuid;
  uint32_t id;
  int max_xlen;
  int xlen;
  std::string isa;
  bool run; // !reset
  bool debug;
  bool histogram_enabled;

  std::vector<insn_desc_t> instructions;
  std::map<reg_t,uint64_t> pc_histogram;

  static const size_t OPCODE_CACHE_SIZE = 8191;
  insn_desc_t opcode_cache[OPCODE_CACHE_SIZE];

  void check_timer();
  void take_interrupt(); // take a trap if any interrupts are pending
  void take_trap(trap_t& t, reg_t epc); // take an exception
  void disasm(insn_t insn); // disassemble and print an instruction

  friend class sim_t;
  friend class mmu_t;
  friend class extension_t;

  void parse_isa_string(const char* isa);
  void build_opcode_map();
  void register_base_instructions();
  insn_func_t decode_insn(insn_t insn);
};

reg_t illegal_instruction(processor_t* p, insn_t insn, reg_t pc);

#define REGISTER_INSN(proc, name, match, mask) \
  extern reg_t rv32_##name(processor_t*, insn_t, reg_t); \
  extern reg_t rv64_##name(processor_t*, insn_t, reg_t); \
  proc->register_insn((insn_desc_t){match, mask, rv32_##name, rv64_##name});

#endif
