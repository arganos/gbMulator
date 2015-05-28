/*
 *  gb_cpu.h
 *  Game Boy Emulator
 *
 *  Created by Englebert Gilles on 5/1/15.
 *  Copyright 2015. All rights reserved.
 *
 */

#include "gb_mem.h"
#include "gb_interrupt.h"

#include <string>

using namespace std;


class gb_cpu_state {
    	//Registers
    	unsigned char a, b, c, d, e, f, h, l;
    	unsigned short sp, pc;
	
	//Clocks
    	unsigned short t, m;
	
    	//Interrupt Master Enable Flag
    	bool ime;
    	unsigned char int_e, int_f;
    
	//CPU State
    	bool halted;
    
    	//Memory
	gb_mem_state *gb_mem;
    
    	//Interrupts
    	gb_interrupt_state *gb_int;
	
	//Command Clock cycles
	int cycles[0x100];
	int ext_cycles[0x100];  
	
	//Opcodes
	typedef  void (gb_cpu_state::*cmd)();
	
	cmd opcode_map[0x100];
	cmd ext_opcode_map[0x100];
    
    	string opc_name[0x100];
	string ext_opc_name[0x100];
	
	
public:
	gb_cpu_state();
	
	//Executes 1 CPU Cycle
	int execute();
	
	//Load external components
	int loadMEM(gb_mem_state &_gb_mem);
	int loadINT(gb_interrupt_state &_gb_int);
	int loadBIOS(string filename); 
	int loadROM(string filename);
    
	//Getters/Setters 
	void core_dump();  //Debugging
	
	void set_pc(short val) {pc = val;};
    	unsigned short get_pc() {return pc;};
	
    	bool isHalted() {return halted;};
    	bool intEnabled() {return gb_mem->rb(IO_REG_IE);};
    
private:
	//Util
	unsigned short mk_word(unsigned char x,unsigned char y);
	
	//Flags
	void inline clear_f(){f=0;};
	
	void inline set_zf(){ f|=(1<<7);};
	void inline set_nf(){ f|=(1<<6);};
	void inline set_hf(){ f|=(1<<5);};
	void inline set_cf(){ f|=(1<<4);};
	
	void inline clear_zf(){f&=((1<<8)-1-(1<<7));};
	void inline clear_nf(){f&=((1<<8)-1-(1<<6));};
	void inline clear_hf(){f&=((1<<8)-1-(1<<5));};
	void inline clear_cf(){f&=((1<<8)-1-(1<<4));};
	
	bool zf_set(){return f&(1<<7);};
	bool nf_set(){return f&(1<<6);};
	bool hf_set(){return f&(1<<5);};
	bool cf_set(){return f&(1<<4);};
	
	//Commands
	
	//8-Bit Loads
	void ld_a_m();  //8   3E
	void ld_a_mm(); //16  FA
	void ld_b_m();  //8   06
	void ld_c_m();  //8   0E
	void ld_d_m();  //8   16
	void ld_e_m();  //8   1E
	void ld_h_m();  //8   26
	void ld_l_m();  //8   2E
	void ld_mhl_m(); //12  36
	
	void ld_a_a();  //4   7F
	void ld_a_b();	//4   78
	void ld_a_c();  //4   79
	void ld_a_d();  //4   7A
	void ld_a_e();  //4   7B
	void ld_a_h();  //4   7C
	void ld_a_l();  //4   7D
	void ld_a_mbc(); //8   0A
	void ld_a_mde(); //8   1A
	void ld_a_mhl(); //8   7E
	
	void ld_b_a();  //4   47
	void ld_b_b();  //4   40
	void ld_b_c();  //4   41
	void ld_b_d();  //4   42
	void ld_b_e();  //4   43
	void ld_b_h();  //4   44
	void ld_b_l();  //4   45
	void ld_b_mhl(); //8   46
	
	void ld_c_a();  //4   4F
	void ld_c_b();  //4   48
	void ld_c_c();  //4   49
	void ld_c_d();  //4   4A
	void ld_c_e();  //4   4B
	void ld_c_h();  //4   4C
	void ld_c_l();  //4   4D
	void ld_c_mhl(); //8   4E
	
	void ld_d_a();  //4   57
	void ld_d_b();  //4   50
	void ld_d_c();  //4   51
	void ld_d_d();  //4   52
	void ld_d_e();  //4   53
	void ld_d_h();  //4   54
	void ld_d_l();  //4   55
	void ld_d_mhl(); //8   56
	
	void ld_e_a();  //4   5F
	void ld_e_b();  //4   58
	void ld_e_c();  //4   59
	void ld_e_d();  //4   5A
	void ld_e_e();  //4   5B
	void ld_e_h();  //4   5C
	void ld_e_l();  //4   5D
	void ld_e_mhl(); //8   5E
	
	void ld_h_a();  //4   67
	void ld_h_b();  //4   60
	void ld_h_c();  //4   61
	void ld_h_d();  //4   62
	void ld_h_e();  //4   63
	void ld_h_h();  //4   64
	void ld_h_l();  //4   65
	void ld_h_mhl(); //8   66
	
	void ld_l_a();  //4   6F
	void ld_l_b();  //4   68
	void ld_l_c();  //4   69
	void ld_l_d();  //4   6A
	void ld_l_e();  //4   6B
	void ld_l_h();  //4   6C
	void ld_l_l();  //4   6D
	void ld_l_mhl(); //8   6E
	
	void ld_mbc_a(); //8   02
	
	void ld_mde_a(); //8   12 
	
	void ld_mhl_a(); //8   77
	void ld_mhl_b(); //8   70
	void ld_mhl_c(); //8   71
	void ld_mhl_d(); //8   72
	void ld_mhl_e(); //8   73
	void ld_mhl_h(); //8   74
	void ld_mhl_l(); //8   75
	
	void ld_mm_a(); //16  EA
	
	
	//Write memory
	void ld_a_mc(); //8   F2
	void ld_mc_a(); //8   E2
	
	
	//LDD
	void ldd_a_mhl();//8   3A
	void ldd_mhl_a();//8   32
	
	//LDI
	void ldi_a_mhl();//8   2A
	void ldi_mhl_a();//8   22
	
	//LDH
	void ldh_m_a();	 //12  E0
	void ldh_a_m();  //12  F0
	
	//16-bit Loads
	
	void ld_bc_mm(); //12  01
	void ld_de_mm(); //12  11
	void ld_hl_mm(); //12  21
	void ld_sp_mm(); //12  31
	
	void ld_sp_hl(); //8   F9
	
	void ldhl_sp_n(); //12 F8
	
	void ld_mm_sp(); //20  08
	
	//PUSH
	
	void push_af(); //16 F5
	void push_bc(); //16 C5
	void push_de(); //16 D5
	void push_hl(); //16 E5
	
	//POP
	
	void pop_af(); //12 F1
	void pop_bc(); //12 C1
	void pop_de(); //12 D1
	void pop_hl(); //12 E1
	
	
	//8-Bit ALU
	
	//ADD
	void add_to_a(unsigned short val);
	
	void add_a_a(); //4  87
	void add_a_b(); //4  80
	void add_a_c(); //4  81
	void add_a_d(); //4  82
	void add_a_e(); //4  83
	void add_a_h(); //4  84
	void add_a_l(); //4  85
	void add_a_mhl(); //8  86
	void add_a_m();   //8  C6
	
	//ADC
    	void adc_to_a(unsigned short val);
    
	void adc_a_a(); //4  8F
	void adc_a_b(); //4  88
	void adc_a_c(); //4  89
	void adc_a_d(); //4  8A
	void adc_a_e(); //4  8B
	void adc_a_h(); //4  8C
	void adc_a_l(); //4  8D
	void adc_a_mhl(); //8  8E
	void adc_a_m();   //8  CE
	
	//SUB
	void sub_from_a(unsigned short val);
	
	void sub_a(); //4  97
	void sub_b(); //4  90
	void sub_c(); //4  91
	void sub_d(); //4  92
	void sub_e(); //4  93
	void sub_h(); //4  94
	void sub_l(); //4  95
	void sub_mhl(); //8  96
	void sub_m();   //8  D6
	
	//SBC
	void sbc_from_a(unsigned short val);
    
    	void sbc_a(); //4  9F
	void sbc_b(); //4  98
	void sbc_c(); //4  99
	void sbc_d(); //4  9A
	void sbc_e(); //4  9B
	void sbc_h(); //4  9C
	void sbc_l(); //4  9D
	void sbc_mhl(); //8  9E
	void sbc_m();   //8  DE
	
	//LOGIC
	//AND
	void and_with_a(short val);
	
	void and_a();  //4  A7
	void and_b();  //4  A0
	void and_c();  //4  A1
	void and_d();  //4  A2
	void and_e();  //4  A3
	void and_h();  //4  A4
	void and_l();  //4  A5
	void and_mhl();//8  A6
	void and_m();  //8  E6	
	
	//OR
	void or_with_a(short val);
	
	void or_a();  //4  B7
	void or_b();  //4  B0
	void or_c();  //4  B1
	void or_d();  //4  B2
	void or_e();  //4  B3
	void or_h();  //4  B4
	void or_l();  //4  B5
	void or_mhl();//8  B6
	void or_m();  //8  F6
	
	//XOR
	void xor_with_a(short val);
	
	void xor_a();  //4  AF
	void xor_b();  //4  A8
	void xor_c();  //4  A9
	void xor_d();  //4  AA
	void xor_e();  //4  AB
	void xor_h();  //4  AC
	void xor_l();  //4  AD
	void xor_mhl();//8  AE
	void xor_m();  //8  EE
	
	
	//CP
	void cp_with_a(short val);
	
	void cp_a();  //4  BF
	void cp_b();  //4  B8
	void cp_c();  //4  B9
	void cp_d();  //4  BA
	void cp_e();  //4  BB
	void cp_h();  //4  BC
	void cp_l();  //4  BD
	void cp_mhl();//8  BE
	void cp_m();  //8  FE
	
	
	//INC
	void inc_reg_8bit(unsigned char& reg);
	
	void inc_a();   //4 3C
	void inc_b();   //4 04
	void inc_c();   //4 0C
	void inc_d();   //4 14
	void inc_e();   //4 1C
	void inc_h();   //4 24
	void inc_l();   //4 2C
	void inc_mhl(); //12 34
	
	//DEC
	void dec_reg_8bit(unsigned char& reg);
	
	void dec_a();   //4 3D
	void dec_b();   //4 05
	void dec_c();   //4 0D
	void dec_d();   //4 15
	void dec_e();   //4 1D
	void dec_h();   //4 25
	void dec_l();   //4 2D
	void dec_mhl(); //12 35
	
	//16-Bit ALU
	
	//ADD HL,n
	void add_to_hl(unsigned short val);
	
	void add_hl_bc(); //8 09
	void add_hl_de(); //8 19
	void add_hl_hl(); //8 29
	void add_hl_sp(); //8 39
	
	void add_sp_m();  //16 E8
	
	void inc_bc();	//8  03
	void inc_de();  //8  13
	void inc_hl();  //8  23
	void inc_sp();  //8  33
	
	void dec_bc();	//8  0B
	void dec_de();  //8  1B
	void dec_hl();  //8  2B
	void dec_sp();  //8  3B

    
    	//Other
    	//SWAP
    	void swap_reg(unsigned char& reg);
    
    	void swap_a(); //8 CB 37
    	void swap_b(); //8 CB 30
    	void swap_c(); //8 CB 31
    	void swap_d(); //8 CB 32
    	void swap_e(); //8 CB 33
    	void swap_h(); //8 CB 34
    	void swap_l(); //8 CB 35
    	void swap_mhl(); //16 CB 36
    
    	//Other operations
    	void daa(); //4 27
    	void cpl(); //4 2F
    	void ccf(); //4 3F
    	void scf(); //4 37
    
    	void nop();  //4 00
    	void halt(); //4 76
    	void stop(); //4 10 00
    
    	void di();  //4 F3
    	void ei();  //4 FB
    
    	//ROTATES
    	void rlca(); //4 07
    	void rla();  //4 17
    
    	void rrca(); //4 0F
    	void rra();  //4 1F
    
    	void rlc(unsigned char &reg);
    
    	void rlc_a(); //8 CB 07
    	void rlc_b(); //8 CB 00
    	void rlc_c(); //8 CB 01
    	void rlc_d(); //8 CB 02
    	void rlc_e(); //8 CB 03
    	void rlc_h(); //8 CB 04
    	void rlc_l(); //8 CB 05
    	void rlc_mhl(); //16 CB 06
    
    	void rl(unsigned char &reg);
    
    	void rl_a(); //8 CB 17
    	void rl_b(); //8 CB 10
    	void rl_c(); //8 CB 11
    	void rl_d(); //8 CB 12
    	void rl_e(); //8 CB 13
    	void rl_h(); //8 CB 14
    	void rl_l(); //8 CB 15
    	void rl_mhl(); //16 CB 16
    
    	void rrc(unsigned char &reg);
    
    	void rrc_a(); //8 CB 0F
    	void rrc_b(); //8 CB 08
    	void rrc_c(); //8 CB 09
    	void rrc_d(); //8 CB 0A
    	void rrc_e(); //8 CB 0B
    	void rrc_h(); //8 CB 0C
    	void rrc_l(); //8 CB 0D
    	void rrc_mhl(); //16 CB 0E
    
    	void rr(unsigned char &reg);
    
    	void rr_a(); //8 CB 1F
    	void rr_b(); //8 CB 18
    	void rr_c(); //8 CB 19
    	void rr_d(); //8 CB 1A
    	void rr_e(); //8 CB 1B
    	void rr_h(); //8 CB 1C
    	void rr_l(); //8 CB 1D
    	void rr_mhl(); //16 CB 1E
    
    	//SHIFTS
    
    	void sla(unsigned char &reg);
    
    	void sla_a(); //8 CB 27
    	void sla_b(); //8 CB 20
    	void sla_c(); //8 CB 21
    	void sla_d(); //8 CB 22
    	void sla_e(); //8 CB 23
    	void sla_h(); //8 CB 24
    	void sla_l(); //8 CB 25
    	void sla_mhl(); //16 CB 26
    
    	void sra(unsigned char &reg);
    
    	void sra_a(); //8 CB 2F
    	void sra_b(); //8 CB 28
    	void sra_c(); //8 CB 29
    	void sra_d(); //8 CB 2A
	void sra_e(); //8 CB 2B
    	void sra_h(); //8 CB 2C
    	void sra_l(); //8 CB 2D
    	void sra_mhl(); //16 CB 2E
    
    	void srl(unsigned char &reg);
    
    	void srl_a(); //8 CB 3F
    	void srl_b(); //8 CB 38
    	void srl_c(); //8 CB 39
    	void srl_d(); //8 CB 3A
    	void srl_e(); //8 CB 3B
    	void srl_h(); //8 CB 3C
    	void srl_l(); //8 CB 3D
    	void srl_mhl(); //16 CB 3E
    
	//BIT opcodes
    	void bit(unsigned char& reg, unsigned char b);
    
    	void bit_0_a(); //8 CB 47
    	void bit_1_a(); //8 CB 4f
    	void bit_2_a(); //8 CB 57
    	void bit_3_a(); //8 CB 5f
    	void bit_4_a(); //8 CB 67
    	void bit_5_a(); //8 CB 6f
    	void bit_6_a(); //8 CB 77
    	void bit_7_a(); //8 CB 7f
    
    	void bit_0_b(); //8 CB 40
    	void bit_1_b(); //8 CB 48
    	void bit_2_b(); //8 CB 50
    	void bit_3_b(); //8 CB 58
    	void bit_4_b(); //8 CB 60
    	void bit_5_b(); //8 CB 68
    	void bit_6_b(); //8 CB 70
    	void bit_7_b(); //8 CB 78
    
    	void bit_0_c(); //8 CB 41
    	void bit_1_c(); //8 CB 49
    	void bit_2_c(); //8 CB 51
    	void bit_3_c(); //8 CB 59
    	void bit_4_c(); //8 CB 61
    	void bit_5_c(); //8 CB 69
    	void bit_6_c(); //8 CB 71
    	void bit_7_c(); //8 CB 79
    
    	void bit_0_d(); //8 CB 42
    	void bit_1_d(); //8 CB 4a
    	void bit_2_d(); //8 CB 52
    	void bit_3_d(); //8 CB 5a
    	void bit_4_d(); //8 CB 62
    	void bit_5_d(); //8 CB 6a
    	void bit_6_d(); //8 CB 72
    	void bit_7_d(); //8 CB 7a
    
    	void bit_0_e(); //8 CB 43
    	void bit_1_e(); //8 CB 4b
    	void bit_2_e(); //8 CB 53
    	void bit_3_e(); //8 CB 5b
    	void bit_4_e(); //8 CB 63
    	void bit_5_e(); //8 CB 6b
    	void bit_6_e(); //8 CB 73
    	void bit_7_e(); //8 CB 7b
    
    	void bit_0_h(); //8 CB 44
    	void bit_1_h(); //8 CB 4c
    	void bit_2_h(); //8 CB 54
    	void bit_3_h(); //8 CB 5c
    	void bit_4_h(); //8 CB 64
    	void bit_5_h(); //8 CB 6c
    	void bit_6_h(); //8 CB 74
    	void bit_7_h(); //8 CB 7c
    
    	void bit_0_l(); //8 CB 45
    	void bit_1_l(); //8 CB 4d
    	void bit_2_l(); //8 CB 55
    	void bit_3_l(); //8 CB 5d
	void bit_4_l(); //8 CB 65
	void bit_5_l(); //8 CB 6d
    	void bit_6_l(); //8 CB 75
    	void bit_7_l(); //8 CB 7d
    
    	void bit_0_mhl(); //16 CB 46
    	void bit_1_mhl(); //16 CB 4e
    	void bit_2_mhl(); //16 CB 56
    	void bit_3_mhl(); //16 CB 5e
    	void bit_4_mhl(); //16 CB 66
    	void bit_5_mhl(); //16 CB 6e
    	void bit_6_mhl(); //16 CB 76
    	void bit_7_mhl(); //16 CB 7e
    
    	//SET
    
    	void set(unsigned char& reg, unsigned char b);
    
    	void set_0_a(); //8 CB c7
    	void set_1_a(); //8 CB cf
    	void set_2_a(); //8 CB d7
    	void set_3_a(); //8 CB df
    	void set_4_a(); //8 CB e7
    	void set_5_a(); //8 CB ef
    	void set_6_a(); //8 CB f7
    	void set_7_a(); //8 CB ff
	 
    	void set_0_b(); //8 CB c0
    	void set_1_b(); //8 CB c8
    	void set_2_b(); //8 CB d0
    	void set_3_b(); //8 CB d8
    	void set_4_b(); //8 CB e0
    	void set_5_b(); //8 CB e8
    	void set_6_b(); //8 CB f0
    	void set_7_b(); //8 CB f8
    
    	void set_0_c(); //8 CB c1
    	void set_1_c(); //8 CB c9
    	void set_2_c(); //8 CB d1
    	void set_3_c(); //8 CB d9
    	void set_4_c(); //8 CB e1
    	void set_5_c(); //8 CB e9
    	void set_6_c(); //8 CB f1
    	void set_7_c(); //8 CB f9
    
    	void set_0_d(); //8 CB c2
    	void set_1_d(); //8 CB ca
    	void set_2_d(); //8 CB d2
    	void set_3_d(); //8 CB da
    	void set_4_d(); //8 CB e2
    	void set_5_d(); //8 CB ea
    	void set_6_d(); //8 CB f2
    	void set_7_d(); //8 CB fa
    
    	void set_0_e(); //8 CB c3
    	void set_1_e(); //8 CB cb
    	void set_2_e(); //8 CB d3
    	void set_3_e(); //8 CB db
    	void set_4_e(); //8 CB e3
    	void set_5_e(); //8 CB eb
	void set_6_e(); //8 CB f3
    	void set_7_e(); //8 CB fb
    
    	void set_0_h(); //8 CB c4
    	void set_1_h(); //8 CB cc
    	void set_2_h(); //8 CB d4
    	void set_3_h(); //8 CB dc
    	void set_4_h(); //8 CB e4
    	void set_5_h(); //8 CB ec
    	void set_6_h(); //8 CB f4
    	void set_7_h(); //8 CB fc
    
    	void set_0_l(); //8 CB c5
    	void set_1_l(); //8 CB cd
    	void set_2_l(); //8 CB d5
    	void set_3_l(); //8 CB dd
    	void set_4_l(); //8 CB e5
    	void set_5_l(); //8 CB ed
    	void set_6_l(); //8 CB f5
    	void set_7_l(); //8 CB fd
    
    	void set_0_mhl(); //16 CB c6
    	void set_1_mhl(); //16 CB ce
    	void set_2_mhl(); //16 CB d6
    	void set_3_mhl(); //16 CB de
    	void set_4_mhl(); //16 CB e6
    	void set_5_mhl(); //16 CB ee
    	void set_6_mhl(); //16 CB f6
    	void set_7_mhl(); //16 CB fe
    
    	//RES
    
    	void res(unsigned char& reg, unsigned char b);
    
    	void res_0_a(); //8 CB 87
    	void res_1_a(); //8 CB 8f
    	void res_2_a(); //8 CB 97
    	void res_3_a(); //8 CB 9f
    	void res_4_a(); //8 CB a7
    	void res_5_a(); //8 CB af
    	void res_6_a(); //8 CB b7
    	void res_7_a(); //8 CB bf
    
    	void res_0_b(); //8 CB 80
    	void res_1_b(); //8 CB 88
    	void res_2_b(); //8 CB 90
    	void res_3_b(); //8 CB 98
	void res_4_b(); //8 CB a0
    	void res_5_b(); //8 CB a8
    	void res_6_b(); //8 CB b0
    	void res_7_b(); //8 CB b8
    
    	void res_0_c(); //8 CB 81
    	void res_1_c(); //8 CB 89
    	void res_2_c(); //8 CB 91
    	void res_3_c(); //8 CB 99
    	void res_4_c(); //8 CB a1
    	void res_5_c(); //8 CB a9
	void res_6_c(); //8 CB b1
	void res_7_c(); //8 CB b9
    
    	void res_0_d(); //8 CB 82
    	void res_1_d(); //8 CB 8a
    	void res_2_d(); //8 CB 92
    	void res_3_d(); //8 CB 9a
    	void res_4_d(); //8 CB a2
    	void res_5_d(); //8 CB aa
    	void res_6_d(); //8 CB b2
    	void res_7_d(); //8 CB ba
    
    	void res_0_e(); //8 CB 83
    	void res_1_e(); //8 CB 8b
    	void res_2_e(); //8 CB 93
    	void res_3_e(); //8 CB 9b
    	void res_4_e(); //8 CB a3
    	void res_5_e(); //8 CB ab
    	void res_6_e(); //8 CB b3
    	void res_7_e(); //8 CB bb
    
    	void res_0_h(); //8 CB 84
    	void res_1_h(); //8 CB 8c
    	void res_2_h(); //8 CB 94
    	void res_3_h(); //8 CB 9c
    	void res_4_h(); //8 CB a4
    	void res_5_h(); //8 CB ac
    	void res_6_h(); //8 CB b4
    	void res_7_h(); //8 CB bc
    
    	void res_0_l(); //8 CB 85
    	void res_1_l(); //8 CB 8d
    	void res_2_l(); //8 CB 95
    	void res_3_l(); //8 CB 9d
    	void res_4_l(); //8 CB a5
    	void res_5_l(); //8 CB ad
    	void res_6_l(); //8 CB b5
    	void res_7_l(); //8 CB bd
    
    	void res_0_mhl(); //16 CB 86
    	void res_1_mhl(); //16 CB 8e
    	void res_2_mhl(); //16 CB 96
    	void res_3_mhl(); //16 CB 9e
	void res_4_mhl(); //16 CB a6
    	void res_5_mhl(); //16 CB ae
    	void res_6_mhl(); //16 CB b6
    	void res_7_mhl(); //16 CB be
    
    	//JUMPS
    	void jp_mm();  //12 C3
    
    	void jp_nz();  //12 C2
    	void jp_z();   //12 CA
    	void jp_nc();  //12 D2
    	void jp_c();   //12 DA
    
    	void jp_mhl(); //4  E9
    
    	void jr_m();   //8  18
    
    	void jr_nz();  //8  20
    	void jr_z();   //8  28
    	void jr_nc();  //8  30
    	void jr_c();   //8  38
    
    	//CALLS
    
    	void call();     //12 CD
    
    	void call_nz();  //12 C4
    	void call_z();   //12 CC
    	void call_nc();  //12 D4
    	void call_c();   //12 DC
    
    	//RESTARTS
    
    	void rst(unsigned char addr);
    
    	void rst_00();  //32 C7
    	void rst_10();  //32 D7
    	void rst_20();  //32 E7
    	void rst_30();  //32 F7

    	void rst_08();  //32 CF
    	void rst_18();  //32 DF
    	void rst_28();  //32 EF
    	void rst_38();  //32 FF
    
    	//RETURNS
    
    	void ret();     //8 C9
    
    	void ret_nz();  //8 C0
    	void ret_z();   //8 C8
    	void ret_nc();  //8 D0
    	void ret_c();   //8 D8
    
    	void reti();    //8 D9
    
};

