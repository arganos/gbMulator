/*
 *  gb_cpu.cpp
 *  Game Boy Emulator
 *
 *  Created by Englebert Gilles on 5/1/15.
 *  Copyright 2015. All rights reserved.
 *
 */

#include "gb_cpu.h"
#include <iostream>
#include <bitset>
#include <iomanip>


using namespace std;

//Init everything

gb_cpu_state::gb_cpu_state() {
    //Reset of Registers
    a = b = c = d = e = f = h = l = 0;
	
    sp = 0;
    pc = 0;
	
    //Reset of clocks
    t = m = 0;
    
    //Init Params
    halted  = 0;
    ime = 0;
    
    //Init OC-Tables
    
    opcode_map[0x3E] = &gb_cpu_state::ld_a_m;  cycles[0x3E] = 8;   opc_name[62] = string("ld_a_m");
    opcode_map[0xFA] = &gb_cpu_state::ld_a_mm; cycles[0xFA] = 16;   opc_name[250] = string("ld_a_mm");
    opcode_map[0x06] = &gb_cpu_state::ld_b_m; cycles[0x06] = 8;   opc_name[6] = string("ld_b_m");
    opcode_map[0x0E] = &gb_cpu_state::ld_c_m; cycles[0x0E] = 8;   opc_name[14] = string("ld_c_m");
    opcode_map[0x16] = &gb_cpu_state::ld_d_m; cycles[0x16] = 8;   opc_name[22] = string("ld_d_m");
    opcode_map[0x1E] = &gb_cpu_state::ld_e_m; cycles[0x1E] = 8;   opc_name[30] = string("ld_e_m");
    opcode_map[0x26] = &gb_cpu_state::ld_h_m; cycles[0x26] = 8;   opc_name[38] = string("ld_h_m");
    opcode_map[0x2E] = &gb_cpu_state::ld_l_m; cycles[0x2E] = 8;   opc_name[46] = string("ld_l_m");
    opcode_map[0x36] = &gb_cpu_state::ld_mhl_m; cycles[0x36] = 12;   opc_name[54] = string("ld_mhl_m");
    
    opcode_map[0x7F] = &gb_cpu_state::ld_a_a; cycles[0x7F] = 4;   opc_name[127] = string("ld_a_a");
    opcode_map[0x78] = &gb_cpu_state::ld_a_b; cycles[0x78] = 4;   opc_name[120] = string("ld_a_b");
    opcode_map[0x79] = &gb_cpu_state::ld_a_c; cycles[0x79] = 4;   opc_name[121] = string("ld_a_c");
    opcode_map[0x7A] = &gb_cpu_state::ld_a_d; cycles[0x7A] = 4;   opc_name[122] = string("ld_a_d");
    opcode_map[0x7B] = &gb_cpu_state::ld_a_e; cycles[0x7B] = 4;   opc_name[123] = string("ld_a_e");
    opcode_map[0x7C] = &gb_cpu_state::ld_a_h; cycles[0x7C] = 4;   opc_name[124] = string("ld_a_h");
    opcode_map[0x7D] = &gb_cpu_state::ld_a_l; cycles[0x7D] = 4;   opc_name[125] = string("ld_a_l");
    opcode_map[0x0A] = &gb_cpu_state::ld_a_mbc; cycles[0x0A] = 8;   opc_name[10] = string("ld_a_mbc");
    opcode_map[0x1A] = &gb_cpu_state::ld_a_mde; cycles[0x1A] = 8;   opc_name[26] = string("ld_a_mde");
    opcode_map[0x7E] = &gb_cpu_state::ld_a_mhl; cycles[0x7E] = 8;   opc_name[126] = string("ld_a_mhl");
    
    opcode_map[0x47] = &gb_cpu_state::ld_b_a; cycles[0x47] = 4;   opc_name[71] = string("ld_b_a");
    opcode_map[0x40] = &gb_cpu_state::ld_b_b; cycles[0x40] = 4;   opc_name[64] = string("ld_b_b");
    opcode_map[0x41] = &gb_cpu_state::ld_b_c; cycles[0x41] = 4;   opc_name[65] = string("ld_b_c");
    opcode_map[0x42] = &gb_cpu_state::ld_b_d; cycles[0x42] = 4;   opc_name[66] = string("ld_b_d");
    opcode_map[0x43] = &gb_cpu_state::ld_b_e; cycles[0x43] = 4;   opc_name[67] = string("ld_b_e");
    opcode_map[0x44] = &gb_cpu_state::ld_b_h; cycles[0x44] = 4;   opc_name[68] = string("ld_b_h");
    opcode_map[0x45] = &gb_cpu_state::ld_b_l; cycles[0x45] = 4;   opc_name[69] = string("ld_b_l");
    opcode_map[0x46] = &gb_cpu_state::ld_b_mhl; cycles[0x46] = 8;   opc_name[70] = string("ld_b_mhl");
    
    opcode_map[0x4F] = &gb_cpu_state::ld_c_a; cycles[0x4F] = 4;   opc_name[79] = string("ld_c_a");
    opcode_map[0x48] = &gb_cpu_state::ld_c_b; cycles[0x48] = 4;   opc_name[72] = string("ld_c_b");
    opcode_map[0x49] = &gb_cpu_state::ld_c_c; cycles[0x49] = 4;   opc_name[73] = string("ld_c_c");
    opcode_map[0x4A] = &gb_cpu_state::ld_c_d; cycles[0x4A] = 4;   opc_name[74] = string("ld_c_d");
    opcode_map[0x4B] = &gb_cpu_state::ld_c_e; cycles[0x4B] = 4;   opc_name[75] = string("ld_c_e");
    opcode_map[0x4C] = &gb_cpu_state::ld_c_h; cycles[0x4C] = 4;   opc_name[76] = string("ld_c_h");
    opcode_map[0x4D] = &gb_cpu_state::ld_c_l; cycles[0x4D] = 4;   opc_name[77] = string("ld_c_l");
    opcode_map[0x4E] = &gb_cpu_state::ld_c_mhl; cycles[0x4E] = 8;   opc_name[78] = string("ld_c_mhl");
    
    opcode_map[0x57] = &gb_cpu_state::ld_d_a; cycles[0x57] = 4;   opc_name[87] = string("ld_d_a");
    opcode_map[0x50] = &gb_cpu_state::ld_d_b; cycles[0x50] = 4;   opc_name[80] = string("ld_d_b");
    opcode_map[0x51] = &gb_cpu_state::ld_d_c; cycles[0x51] = 4;   opc_name[81] = string("ld_d_c");
    opcode_map[0x52] = &gb_cpu_state::ld_d_d; cycles[0x52] = 4;   opc_name[82] = string("ld_d_d");
    opcode_map[0x53] = &gb_cpu_state::ld_d_e; cycles[0x53] = 4;   opc_name[83] = string("ld_d_e");
    opcode_map[0x54] = &gb_cpu_state::ld_d_h; cycles[0x54] = 4;   opc_name[84] = string("ld_d_h");
    opcode_map[0x55] = &gb_cpu_state::ld_d_l; cycles[0x55] = 4;   opc_name[85] = string("ld_d_l");
    opcode_map[0x56] = &gb_cpu_state::ld_d_mhl; cycles[0x56] = 8;   opc_name[86] = string("ld_d_mhl");
    
    opcode_map[0x5F] = &gb_cpu_state::ld_e_a; cycles[0x5F] = 4;   opc_name[95] = string("ld_e_a");
    opcode_map[0x58] = &gb_cpu_state::ld_e_b; cycles[0x58] = 4;   opc_name[88] = string("ld_e_b");
    opcode_map[0x59] = &gb_cpu_state::ld_e_c; cycles[0x59] = 4;   opc_name[89] = string("ld_e_c");
    opcode_map[0x5A] = &gb_cpu_state::ld_e_d; cycles[0x5A] = 4;   opc_name[90] = string("ld_e_d");
    opcode_map[0x5B] = &gb_cpu_state::ld_e_e; cycles[0x5B] = 4;   opc_name[91] = string("ld_e_e");
    opcode_map[0x5C] = &gb_cpu_state::ld_e_h; cycles[0x5C] = 4;   opc_name[92] = string("ld_e_h");
    opcode_map[0x5D] = &gb_cpu_state::ld_e_l; cycles[0x5D] = 4;   opc_name[93] = string("ld_e_l");
    opcode_map[0x5E] = &gb_cpu_state::ld_e_mhl; cycles[0x5E] = 8;   opc_name[94] = string("ld_e_mhl");
    
    opcode_map[0x67] = &gb_cpu_state::ld_h_a; cycles[0x67] = 4;   opc_name[103] = string("ld_h_a");
    opcode_map[0x60] = &gb_cpu_state::ld_h_b; cycles[0x60] = 4;   opc_name[96] = string("ld_h_b");
    opcode_map[0x61] = &gb_cpu_state::ld_h_c; cycles[0x61] = 4;   opc_name[97] = string("ld_h_c");
    opcode_map[0x62] = &gb_cpu_state::ld_h_d; cycles[0x62] = 4;   opc_name[98] = string("ld_h_d");
    opcode_map[0x63] = &gb_cpu_state::ld_h_e; cycles[0x63] = 4;   opc_name[99] = string("ld_h_e");
    opcode_map[0x64] = &gb_cpu_state::ld_h_h; cycles[0x64] = 4;   opc_name[100] = string("ld_h_h");
    opcode_map[0x65] = &gb_cpu_state::ld_h_l; cycles[0x65] = 4;   opc_name[101] = string("ld_h_l");
    opcode_map[0x66] = &gb_cpu_state::ld_h_mhl; cycles[0x66] = 8;   opc_name[102] = string("ld_h_mhl");
    
    opcode_map[0x6F] = &gb_cpu_state::ld_l_a; cycles[0x6F] = 4;   opc_name[111] = string("ld_l_a");
    opcode_map[0x68] = &gb_cpu_state::ld_l_b; cycles[0x68] = 4;   opc_name[104] = string("ld_l_b");
    opcode_map[0x69] = &gb_cpu_state::ld_l_c; cycles[0x69] = 4;   opc_name[105] = string("ld_l_c");
    opcode_map[0x6A] = &gb_cpu_state::ld_l_d; cycles[0x6A] = 4;   opc_name[106] = string("ld_l_d");
    opcode_map[0x6B] = &gb_cpu_state::ld_l_e; cycles[0x6B] = 4;   opc_name[107] = string("ld_l_e");
    opcode_map[0x6C] = &gb_cpu_state::ld_l_h; cycles[0x6C] = 4;   opc_name[108] = string("ld_l_h");
    opcode_map[0x6D] = &gb_cpu_state::ld_l_l; cycles[0x6D] = 4;   opc_name[109] = string("ld_l_l");
    opcode_map[0x6E] = &gb_cpu_state::ld_l_mhl; cycles[0x6E] = 8;   opc_name[110] = string("ld_l_mhl");
    
    opcode_map[0x77] = &gb_cpu_state::ld_mhl_a; cycles[0x77] = 8;   opc_name[119] = string("ld_mhl_a");
    opcode_map[0x70] = &gb_cpu_state::ld_mhl_b; cycles[0x70] = 8;   opc_name[112] = string("ld_mhl_b");
    opcode_map[0x71] = &gb_cpu_state::ld_mhl_c; cycles[0x71] = 8;   opc_name[113] = string("ld_mhl_c");
    opcode_map[0x72] = &gb_cpu_state::ld_mhl_d; cycles[0x72] = 8;   opc_name[114] = string("ld_mhl_d");
    opcode_map[0x73] = &gb_cpu_state::ld_mhl_e; cycles[0x73] = 8;   opc_name[115] = string("ld_mhl_e");
    opcode_map[0x74] = &gb_cpu_state::ld_mhl_h; cycles[0x74] = 8;   opc_name[116] = string("ld_mhl_h");
    opcode_map[0x75] = &gb_cpu_state::ld_mhl_l; cycles[0x75] = 8;   opc_name[117] = string("ld_mhl_l");
    
    
    opcode_map[0x02] = &gb_cpu_state::ld_mbc_a; cycles[0x02] = 8;   opc_name[2] = string("ld_mbc_a");
    opcode_map[0x12] = &gb_cpu_state::ld_mde_a; cycles[0x12] = 8;   opc_name[18] = string("ld_mde_a");
    opcode_map[0xEA] = &gb_cpu_state::ld_mm_a; cycles[0xEA] = 16;   opc_name[234] = string("ld_mm_a");
    
    opcode_map[0xF2] = &gb_cpu_state::ld_a_mc; cycles[0xF2] = 8;   opc_name[242] = string("ld_a_mc");
    opcode_map[0xE2] = &gb_cpu_state::ld_mc_a; cycles[0xE2] = 8;   opc_name[226] = string("ld_mc_a");
    
    opcode_map[0x3A] = &gb_cpu_state::ldd_a_mhl; cycles[0x3A] = 8;   opc_name[58] = string("ldd_a_mhl");
    opcode_map[0x32] = &gb_cpu_state::ldd_mhl_a; cycles[0x32] = 8;   opc_name[50] = string("ldd_mhl_a");
    
    opcode_map[0x2A] = &gb_cpu_state::ldi_a_mhl; cycles[0x2A] = 8;   opc_name[42] = string("ldi_a_mhl");
    opcode_map[0x22] = &gb_cpu_state::ldi_mhl_a; cycles[0x22] = 8;   opc_name[34] = string("ldi_mhl_a");
    
    opcode_map[0xE0] = &gb_cpu_state::ldh_m_a; cycles[0xE0] = 12;   opc_name[224] = string("ldh_m_a");
    opcode_map[0xF0] = &gb_cpu_state::ldh_a_m; cycles[0xF0] = 12;   opc_name[240] = string("ldh_a_m");
    
    //16-Bit loads
    opcode_map[0x01] = &gb_cpu_state::ld_bc_mm; cycles[0x01] = 12;   opc_name[1] = string("ld_bc_mm");
    opcode_map[0x11] = &gb_cpu_state::ld_de_mm; cycles[0x11] = 12;   opc_name[17] = string("ld_de_mm");
    opcode_map[0x21] = &gb_cpu_state::ld_hl_mm; cycles[0x21] = 12;   opc_name[33] = string("ld_hl_mm");
    opcode_map[0x31] = &gb_cpu_state::ld_sp_mm; cycles[0x31] = 12;   opc_name[49] = string("ld_sp_mm");
    
    opcode_map[0xF9] = &gb_cpu_state::ld_sp_hl; cycles[0xF9] = 4;   opc_name[249] = string("ld_sp_hl");
    opcode_map[0xF8] = &gb_cpu_state::ldhl_sp_n; cycles[0xF8] = 12;   opc_name[248] = string("ldhl_sp_n");
    
    opcode_map[0x08] = &gb_cpu_state::ld_mm_sp; cycles[0x08] = 20;   opc_name[8] = string("ld_mm_sp");
    
    //PUSH
    opcode_map[0xF5] = &gb_cpu_state::push_af; cycles[0xF5] = 16;   opc_name[245] = string("push_af");
    opcode_map[0xC5] = &gb_cpu_state::push_bc; cycles[0xC5] = 16;   opc_name[197] = string("push_bc");
    opcode_map[0xD5] = &gb_cpu_state::push_de; cycles[0xD5] = 16;   opc_name[213] = string("push_de");
    opcode_map[0xE5] = &gb_cpu_state::push_hl; cycles[0xE5] = 16;   opc_name[229] = string("push_hl");
    
    //POP
    opcode_map[0xF1] = &gb_cpu_state::pop_af; cycles[0xF1] = 12;   opc_name[241] = string("pop_af");
    opcode_map[0xC1] = &gb_cpu_state::pop_bc; cycles[0xC1] = 12;   opc_name[193] = string("pop_bc");
    opcode_map[0xD1] = &gb_cpu_state::pop_de; cycles[0xD1] = 12;   opc_name[209] = string("pop_de");
    opcode_map[0xE1] = &gb_cpu_state::pop_hl; cycles[0xE1] = 12;   opc_name[225] = string("pop_hl");
    
    //8-Bit ALU
    
    //ADD
    opcode_map[0x87] = &gb_cpu_state::add_a_a; cycles[0x87] = 4;   opc_name[135] = string("add_a_a");
    opcode_map[0x80] = &gb_cpu_state::add_a_b; cycles[0x80] = 4;   opc_name[128] = string("add_a_b");
    opcode_map[0x81] = &gb_cpu_state::add_a_c; cycles[0x81] = 4;   opc_name[129] = string("add_a_c");
    opcode_map[0x82] = &gb_cpu_state::add_a_d; cycles[0x82] = 4;   opc_name[130] = string("add_a_d");
    opcode_map[0x83] = &gb_cpu_state::add_a_e; cycles[0x83] = 4;   opc_name[131] = string("add_a_e");
    opcode_map[0x84] = &gb_cpu_state::add_a_h; cycles[0x84] = 4;   opc_name[132] = string("add_a_h");
    opcode_map[0x85] = &gb_cpu_state::add_a_l; cycles[0x85] = 4;   opc_name[133] = string("add_a_l");
    opcode_map[0x86] = &gb_cpu_state::add_a_mhl; cycles[0x86] = 8;   opc_name[134] = string("add_a_mhl");
    opcode_map[0xC6] = &gb_cpu_state::add_a_m; cycles[0xC6] = 8;   opc_name[198] = string("add_a_m");
    
    //ADC
    opcode_map[0x8F] = &gb_cpu_state::adc_a_a; cycles[0x8F] = 4;   opc_name[143] = string("adc_a_a");
    opcode_map[0x88] = &gb_cpu_state::adc_a_b; cycles[0x88] = 4;   opc_name[136] = string("adc_a_b");
    opcode_map[0x89] = &gb_cpu_state::adc_a_c; cycles[0x89] = 4;   opc_name[137] = string("adc_a_c");
    opcode_map[0x8A] = &gb_cpu_state::adc_a_d; cycles[0x8A] = 4;   opc_name[138] = string("adc_a_d");
    opcode_map[0x8B] = &gb_cpu_state::adc_a_e; cycles[0x8B] = 4;   opc_name[139] = string("adc_a_e");
    opcode_map[0x8C] = &gb_cpu_state::adc_a_h; cycles[0x8C] = 4;   opc_name[140] = string("adc_a_h");
    opcode_map[0x8D] = &gb_cpu_state::adc_a_l; cycles[0x8D] = 4;   opc_name[141] = string("adc_a_l");
    opcode_map[0x8E] = &gb_cpu_state::adc_a_mhl; cycles[0x8E] = 8;   opc_name[142] = string("adc_a_mhl");
    opcode_map[0xCE] = &gb_cpu_state::adc_a_m; cycles[0xCE] = 8;   opc_name[206] = string("adc_a_m");
    
    //SUB
    opcode_map[0x97] = &gb_cpu_state::sub_a; cycles[0x97] = 4;   opc_name[151] = string("sub_a");
    opcode_map[0x90] = &gb_cpu_state::sub_b; cycles[0x90] = 4;   opc_name[144] = string("sub_b");
    opcode_map[0x91] = &gb_cpu_state::sub_c; cycles[0x91] = 4;   opc_name[145] = string("sub_c");
    opcode_map[0x92] = &gb_cpu_state::sub_d; cycles[0x92] = 4;   opc_name[146] = string("sub_d");
    opcode_map[0x93] = &gb_cpu_state::sub_e; cycles[0x93] = 4;   opc_name[147] = string("sub_e");
    opcode_map[0x94] = &gb_cpu_state::sub_h; cycles[0x94] = 4;   opc_name[148] = string("sub_h");
    opcode_map[0x95] = &gb_cpu_state::sub_l; cycles[0x95] = 4;   opc_name[149] = string("sub_l");
    opcode_map[0x96] = &gb_cpu_state::sub_mhl; cycles[0x96] = 8;   opc_name[150] = string("sub_mhl");
    opcode_map[0xD6] = &gb_cpu_state::sub_m; cycles[0xD6] = 8;   opc_name[214] = string("sub_m");
    
    //SBC
    opcode_map[0x9F] = &gb_cpu_state::sbc_a; cycles[0x9F] = 4;   opc_name[159] = string("sbc_a");
    opcode_map[0x98] = &gb_cpu_state::sbc_b; cycles[0x98] = 4;   opc_name[152] = string("sbc_b");
    opcode_map[0x99] = &gb_cpu_state::sbc_c; cycles[0x99] = 4;   opc_name[153] = string("sbc_c");
    opcode_map[0x9A] = &gb_cpu_state::sbc_d; cycles[0x9A] = 4;   opc_name[154] = string("sbc_d");
    opcode_map[0x9B] = &gb_cpu_state::sbc_e; cycles[0x9B] = 4;   opc_name[155] = string("sbc_e");
    opcode_map[0x9C] = &gb_cpu_state::sbc_h; cycles[0x9C] = 4;   opc_name[156] = string("sbc_h");
    opcode_map[0x9D] = &gb_cpu_state::sbc_l; cycles[0x9D] = 4;   opc_name[157] = string("sbc_l");
    opcode_map[0x9E] = &gb_cpu_state::sbc_mhl; cycles[0x9E] = 8;   opc_name[158] = string("sbc_mhl");
    opcode_map[0xDE] = &gb_cpu_state::sbc_m; cycles[0xDE] = 8;   opc_name[222] = string("sbc_m");
    
    //LOGIC
    
    //AND
    opcode_map[0xA7] = &gb_cpu_state::and_a; cycles[0xA7] = 4;   opc_name[167] = string("and_a");
    opcode_map[0xA0] = &gb_cpu_state::and_b; cycles[0xA0] = 4;   opc_name[160] = string("and_b");
    opcode_map[0xA1] = &gb_cpu_state::and_c; cycles[0xA1] = 4;   opc_name[161] = string("and_c");
    opcode_map[0xA2] = &gb_cpu_state::and_d; cycles[0xA2] = 4;   opc_name[162] = string("and_d");
    opcode_map[0xA3] = &gb_cpu_state::and_e; cycles[0xA3] = 4;   opc_name[163] = string("and_e");
    opcode_map[0xA4] = &gb_cpu_state::and_h; cycles[0xA4] = 4;   opc_name[164] = string("and_h");
    opcode_map[0xA5] = &gb_cpu_state::and_l; cycles[0xA5] = 4;   opc_name[165] = string("and_l");
    opcode_map[0xA6] = &gb_cpu_state::and_mhl; cycles[0xA6] = 8;   opc_name[166] = string("and_mhl");
    opcode_map[0xE6] = &gb_cpu_state::and_m; cycles[0xE6] = 8;   opc_name[230] = string("and_m");
    
    //OR
    opcode_map[0xB7] = &gb_cpu_state::or_a; cycles[0xB7] = 4;   opc_name[183] = string("or_a");
    opcode_map[0xB0] = &gb_cpu_state::or_b; cycles[0xB0] = 4;   opc_name[176] = string("or_b");
    opcode_map[0xB1] = &gb_cpu_state::or_c; cycles[0xB1] = 4;   opc_name[177] = string("or_c");
    opcode_map[0xB2] = &gb_cpu_state::or_d; cycles[0xB2] = 4;   opc_name[178] = string("or_d");
    opcode_map[0xB3] = &gb_cpu_state::or_e; cycles[0xB3] = 4;   opc_name[179] = string("or_e");
    opcode_map[0xB4] = &gb_cpu_state::or_h; cycles[0xB4] = 4;   opc_name[180] = string("or_h");
    opcode_map[0xB5] = &gb_cpu_state::or_l; cycles[0xB5] = 4;   opc_name[181] = string("or_l");
    opcode_map[0xB6] = &gb_cpu_state::or_mhl; cycles[0xB6] = 8;   opc_name[182] = string("or_mhl");
    opcode_map[0xF6] = &gb_cpu_state::or_m; cycles[0xF6] = 8;   opc_name[246] = string("or_m");
    
    //XOR
    opcode_map[0xAF] = &gb_cpu_state::xor_a; cycles[0xAF] = 4;   opc_name[175] = string("xor_a");
    opcode_map[0xA8] = &gb_cpu_state::xor_b; cycles[0xA8] = 4;   opc_name[168] = string("xor_b");
    opcode_map[0xA9] = &gb_cpu_state::xor_c; cycles[0xA9] = 4;   opc_name[169] = string("xor_c");
    opcode_map[0xAA] = &gb_cpu_state::xor_d; cycles[0xAA] = 4;   opc_name[170] = string("xor_d");
    opcode_map[0xAB] = &gb_cpu_state::xor_e; cycles[0xAB] = 4;   opc_name[171] = string("xor_e");
    opcode_map[0xAC] = &gb_cpu_state::xor_h; cycles[0xAC] = 4;   opc_name[172] = string("xor_h");
    opcode_map[0xAD] = &gb_cpu_state::xor_l; cycles[0xAD] = 4;   opc_name[173] = string("xor_l");
    opcode_map[0xAE] = &gb_cpu_state::xor_mhl; cycles[0xAE] = 8;   opc_name[174] = string("xor_mhl");
    opcode_map[0xEE] = &gb_cpu_state::xor_m; cycles[0xEE] = 8;   opc_name[238] = string("xor_m");
    
    //CP
    opcode_map[0xBF] = &gb_cpu_state::cp_a; cycles[0xBF] = 4;   opc_name[191] = string("cp_a");
    opcode_map[0xB8] = &gb_cpu_state::cp_b; cycles[0xB8] = 4;   opc_name[184] = string("cp_b");
    opcode_map[0xB9] = &gb_cpu_state::cp_c; cycles[0xB9] = 4;   opc_name[185] = string("cp_c");
    opcode_map[0xBA] = &gb_cpu_state::cp_d; cycles[0xBA] = 4;   opc_name[186] = string("cp_d");
    opcode_map[0xBB] = &gb_cpu_state::cp_e; cycles[0xBB] = 4;   opc_name[187] = string("cp_e");
    opcode_map[0xBC] = &gb_cpu_state::cp_h; cycles[0xBC] = 4;   opc_name[188] = string("cp_h");
    opcode_map[0xBD] = &gb_cpu_state::cp_l; cycles[0xBD] = 4;   opc_name[189] = string("cp_l");
    opcode_map[0xBE] = &gb_cpu_state::cp_mhl; cycles[0xBE] = 8;   opc_name[190] = string("cp_mhl");
    opcode_map[0xFE] = &gb_cpu_state::cp_m; cycles[0xFE] = 8;   opc_name[254] = string("cp_m");
    
    //INC
    opcode_map[0x3C] = &gb_cpu_state::inc_a; cycles[0x3C] = 4;   opc_name[60] = string("inc_a");
    opcode_map[0x04] = &gb_cpu_state::inc_b; cycles[0x04] = 4;   opc_name[4] = string("inc_b");
    opcode_map[0x0C] = &gb_cpu_state::inc_c; cycles[0x0C] = 4;   opc_name[12] = string("inc_c");
    opcode_map[0x14] = &gb_cpu_state::inc_d; cycles[0x14] = 4;   opc_name[20] = string("inc_d");
    opcode_map[0x1C] = &gb_cpu_state::inc_e; cycles[0x1C] = 4;   opc_name[28] = string("inc_e");
    opcode_map[0x24] = &gb_cpu_state::inc_h; cycles[0x24] = 4;   opc_name[36] = string("inc_h");
    opcode_map[0x2C] = &gb_cpu_state::inc_l; cycles[0x2C] = 4;   opc_name[44] = string("inc_l");
    opcode_map[0x34] = &gb_cpu_state::inc_mhl; cycles[0x34] = 12;   opc_name[52] = string("inc_mhl");
    
    //DEC
    opcode_map[0x3D] = &gb_cpu_state::dec_a; cycles[0x3D] = 4;   opc_name[61] = string("dec_a");
    opcode_map[0x05] = &gb_cpu_state::dec_b; cycles[0x05] = 4;   opc_name[5] = string("dec_b");
    opcode_map[0x0D] = &gb_cpu_state::dec_c; cycles[0x0D] = 4;   opc_name[13] = string("dec_c");
    opcode_map[0x15] = &gb_cpu_state::dec_d; cycles[0x15] = 4;   opc_name[21] = string("dec_d");
    opcode_map[0x1D] = &gb_cpu_state::dec_e; cycles[0x1D] = 4;   opc_name[29] = string("dec_e");
    opcode_map[0x25] = &gb_cpu_state::dec_h; cycles[0x25] = 4;   opc_name[37] = string("dec_h");
    opcode_map[0x2D] = &gb_cpu_state::dec_l; cycles[0x2D] = 4;   opc_name[45] = string("dec_l");
    opcode_map[0x35] = &gb_cpu_state::dec_mhl; cycles[0x35] = 12;   opc_name[53] = string("dec_mhl");
    
    
    //16-bit ALU
    
    //ADD
    opcode_map[0x09] = &gb_cpu_state::add_hl_bc; cycles[0x09] = 8;   opc_name[9] = string("add_hl_bc");
    opcode_map[0x19] = &gb_cpu_state::add_hl_de; cycles[0x19] = 8;   opc_name[25] = string("add_hl_de");
    opcode_map[0x29] = &gb_cpu_state::add_hl_hl; cycles[0x29] = 8;   opc_name[41] = string("add_hl_hl");
    opcode_map[0x39] = &gb_cpu_state::add_hl_sp; cycles[0x39] = 8;   opc_name[57] = string("add_hl_sp");
    
    opcode_map[0xE8] = &gb_cpu_state::add_sp_m; cycles[0xE8] = 16;   opc_name[232] = string("add_sp_m");
    
    //INC
    opcode_map[0x03] = &gb_cpu_state::inc_bc; cycles[0x03] = 8;   opc_name[3] = string("inc_bc");
    opcode_map[0x13] = &gb_cpu_state::inc_de; cycles[0x13] = 8;   opc_name[19] = string("inc_de");
    opcode_map[0x23] = &gb_cpu_state::inc_hl; cycles[0x23] = 8;   opc_name[35] = string("inc_hl");
    opcode_map[0x33] = &gb_cpu_state::inc_sp; cycles[0x33] = 8;   opc_name[51] = string("inc_sp");
    //DEC
    opcode_map[0x0B] = &gb_cpu_state::dec_bc; cycles[0x0B] = 8;   opc_name[11] = string("dec_bc");
    opcode_map[0x1B] = &gb_cpu_state::dec_de; cycles[0x1B] = 8;   opc_name[27] = string("dec_de");
    opcode_map[0x2B] = &gb_cpu_state::dec_hl; cycles[0x2B] = 8;   opc_name[43] = string("dec_hl");
    opcode_map[0x3B] = &gb_cpu_state::dec_sp; cycles[0x3B] = 8;   opc_name[59] = string("dec_sp");
    
    
    //DAA
    opcode_map[0x27] = &gb_cpu_state::daa; cycles[0x27] = 4;   opc_name[39] = string("daa");
    
    opcode_map[0x2F] = &gb_cpu_state::cpl; cycles[0x2F] = 4;   opc_name[47] = string("cpl");
    opcode_map[0x3F] = &gb_cpu_state::ccf; cycles[0x3F] = 4;   opc_name[63] = string("ccf");
    opcode_map[0x37] = &gb_cpu_state::scf; cycles[0x37] = 4;   opc_name[55] = string("scf");
    
    
    
    //STUFF
    opcode_map[0x00] = &gb_cpu_state::nop; cycles[0x00] = 4;   opc_name[0] = string("nop");
    opcode_map[0x76] = &gb_cpu_state::halt; cycles[0x76] = 4;   opc_name[118] = string("halt");
    opcode_map[0x10] = &gb_cpu_state::stop; cycles[0x10] = 4;   opc_name[16] = string("stop");
    
    
    opcode_map[0xF3] = &gb_cpu_state::di; cycles[0xF3] = 4;   opc_name[243] = string("di");
    opcode_map[0xFB] = &gb_cpu_state::ei; cycles[0xFB] = 4;   opc_name[251] = string("ei");
    
    
    //ROTATES
    opcode_map[0x07] = &gb_cpu_state::rlca; cycles[0x07] = 4;   opc_name[7] = string("rlca");
    opcode_map[0x17] = &gb_cpu_state::rla; cycles[0x17] = 4;   opc_name[23] = string("rla");
    opcode_map[0x0F] = &gb_cpu_state::rrca; cycles[0x0F] = 4;   opc_name[15] = string("rrca");
    opcode_map[0x1F] = &gb_cpu_state::rra; cycles[0x1F] = 4;   opc_name[31] = string("rra");
    
    
    //JUMPS
    
    opcode_map[0xC3] = &gb_cpu_state::jp_mm; cycles[0xC3] = 16;   opc_name[195] = string("jp_mm");
    
    opcode_map[0xC2] = &gb_cpu_state::jp_nz; cycles[0xC2] = 12;   opc_name[194] = string("jp_nz");
    opcode_map[0xCA] = &gb_cpu_state::jp_z; cycles[0xCA] = 12;   opc_name[202] = string("jp_z");
    opcode_map[0xD2] = &gb_cpu_state::jp_nc; cycles[0xD2] = 12;   opc_name[210] = string("jp_nc");
    opcode_map[0xDA] = &gb_cpu_state::jp_c; cycles[0xDA] = 12;   opc_name[218] = string("jp_c");
    
    opcode_map[0xE9] = &gb_cpu_state::jp_mhl; cycles[0xE9] = 4;   opc_name[233] = string("jp_mhl");
    
    
    opcode_map[0x18] = &gb_cpu_state::jr_m; cycles[0x18] = 12;   opc_name[24] = string("jr_m");
    
    opcode_map[0x20] = &gb_cpu_state::jr_nz; cycles[0x20] = 8;   opc_name[32] = string("jr_nz");
    opcode_map[0x28] = &gb_cpu_state::jr_z; cycles[0x28] = 8;   opc_name[40] = string("jr_z");
    opcode_map[0x30] = &gb_cpu_state::jr_nc; cycles[0x30] = 8;   opc_name[48] = string("jr_nc");
    opcode_map[0x38] = &gb_cpu_state::jr_c; cycles[0x38] = 8;   opc_name[56] = string("jr_c");
    
    
    //CALLS
    
    opcode_map[0xCD] = &gb_cpu_state::call; cycles[0xCD] = 24;   opc_name[205] = string("call");
    
    opcode_map[0xC4] = &gb_cpu_state::call_nz; cycles[0xC4] = 12;   opc_name[196] = string("call_nz");
    opcode_map[0xCC] = &gb_cpu_state::call_z; cycles[0xCC] = 12;   opc_name[204] = string("call_z");
    opcode_map[0xD4] = &gb_cpu_state::call_nc; cycles[0xD4] = 12;   opc_name[212] = string("call_nc");
    opcode_map[0xDC] = &gb_cpu_state::call_c; cycles[0xDC] = 12;   opc_name[220] = string("call_c");
    
    
    //RESTARTS
    
    opcode_map[0xC7] = &gb_cpu_state::rst_00; cycles[0xC7] = 16;   opc_name[199] = string("rst_00");
    opcode_map[0xD7] = &gb_cpu_state::rst_10; cycles[0xD7] = 16;   opc_name[215] = string("rst_10");
    opcode_map[0xE7] = &gb_cpu_state::rst_20; cycles[0xE7] = 16;   opc_name[231] = string("rst_20");
    opcode_map[0xF7] = &gb_cpu_state::rst_30; cycles[0xF7] = 16;   opc_name[247] = string("rst_30");
    
    opcode_map[0xCF] = &gb_cpu_state::rst_08; cycles[0xCF] = 16;   opc_name[207] = string("rst_08");
    opcode_map[0xDF] = &gb_cpu_state::rst_18; cycles[0xDF] = 16;   opc_name[223] = string("rst_18");
    opcode_map[0xEF] = &gb_cpu_state::rst_28; cycles[0xEF] = 16;   opc_name[239] = string("rst_28");
    opcode_map[0xFF] = &gb_cpu_state::rst_38; cycles[0xFF] = 16;   opc_name[255] = string("rst_38");
    
    
    //RETURNS
    
    opcode_map[0xC9] = &gb_cpu_state::ret; cycles[0xC9] = 16;   opc_name[201] = string("ret");
    
    opcode_map[0xC0] = &gb_cpu_state::ret_nz; cycles[0xC0] = 8;   opc_name[192] = string("ret_nz");
    opcode_map[0xC8] = &gb_cpu_state::ret_z; cycles[0xC8] = 8;   opc_name[200] = string("ret_z");
    opcode_map[0xD0] = &gb_cpu_state::ret_nc; cycles[0xD0] = 8;   opc_name[208] = string("ret_nc"); 
    opcode_map[0xD8] = &gb_cpu_state::ret_c; cycles[0xD8] = 8;   opc_name[216] = string("ret_c"); 
    
    opcode_map[0xD9] = &gb_cpu_state::reti; cycles[0xD9] = 16;   opc_name[217] = string("reti");
    
    //Extended Opcodes
    
    //SWAP
    ext_opcode_map[0x37] = &gb_cpu_state::swap_a; ext_cycles[0x37] = 8;   ext_opc_name[55] = string("swap_a");
    ext_opcode_map[0x30] = &gb_cpu_state::swap_b; ext_cycles[0x30] = 8;   ext_opc_name[48] = string("swap_b");
    ext_opcode_map[0x31] = &gb_cpu_state::swap_c; ext_cycles[0x31] = 8;   ext_opc_name[49] = string("swap_c");
    ext_opcode_map[0x32] = &gb_cpu_state::swap_d; ext_cycles[0x32] = 8;   ext_opc_name[50] = string("swap_d");
    ext_opcode_map[0x33] = &gb_cpu_state::swap_e; ext_cycles[0x33] = 8;   ext_opc_name[51] = string("swap_e");
    ext_opcode_map[0x34] = &gb_cpu_state::swap_h; ext_cycles[0x34] = 8;   ext_opc_name[52] = string("swap_h");
    ext_opcode_map[0x35] = &gb_cpu_state::swap_l; ext_cycles[0x35] = 8;   ext_opc_name[53] = string("swap_l");
    ext_opcode_map[0x36] = &gb_cpu_state::swap_mhl; ext_cycles[0x36] = 16;   ext_opc_name[54] = string("swap_mhl");
    
    //RLC
    ext_opcode_map[0x07] = &gb_cpu_state::rlc_a; ext_cycles[0x07] = 8;   ext_opc_name[7] = string("rlc_a");
    ext_opcode_map[0x00] = &gb_cpu_state::rlc_b; ext_cycles[0x00] = 8;   ext_opc_name[0] = string("rlc_b");
    ext_opcode_map[0x01] = &gb_cpu_state::rlc_c; ext_cycles[0x01] = 8;   ext_opc_name[1] = string("rlc_c");
    ext_opcode_map[0x02] = &gb_cpu_state::rlc_d; ext_cycles[0x02] = 8;   ext_opc_name[2] = string("rlc_d");
    ext_opcode_map[0x03] = &gb_cpu_state::rlc_e; ext_cycles[0x03] = 8;   ext_opc_name[3] = string("rlc_e");
    ext_opcode_map[0x04] = &gb_cpu_state::rlc_h; ext_cycles[0x04] = 8;   ext_opc_name[4] = string("rlc_h");
    ext_opcode_map[0x05] = &gb_cpu_state::rlc_l; ext_cycles[0x05] = 8;   ext_opc_name[5] = string("rlc_l");
    ext_opcode_map[0x06] = &gb_cpu_state::rlc_mhl; ext_cycles[0x06] = 16;   ext_opc_name[6] = string("rlc_mhl");
    
    //RL
    ext_opcode_map[0x17] = &gb_cpu_state::rl_a; ext_cycles[0x17] = 8;   ext_opc_name[23] = string("rl_a");
    ext_opcode_map[0x10] = &gb_cpu_state::rl_b; ext_cycles[0x10] = 8;   ext_opc_name[16] = string("rl_b");
    ext_opcode_map[0x11] = &gb_cpu_state::rl_c; ext_cycles[0x11] = 8;   ext_opc_name[17] = string("rl_c");
    ext_opcode_map[0x12] = &gb_cpu_state::rl_d; ext_cycles[0x12] = 8;   ext_opc_name[18] = string("rl_d");
    ext_opcode_map[0x13] = &gb_cpu_state::rl_e; ext_cycles[0x13] = 8;   ext_opc_name[19] = string("rl_e");
    ext_opcode_map[0x14] = &gb_cpu_state::rl_h; ext_cycles[0x14] = 8;   ext_opc_name[20] = string("rl_h");
    ext_opcode_map[0x15] = &gb_cpu_state::rl_l; ext_cycles[0x15] = 8;   ext_opc_name[21] = string("rl_l");
    ext_opcode_map[0x16] = &gb_cpu_state::rl_mhl; ext_cycles[0x16] = 16;   ext_opc_name[22] = string("rl_mhl");
    
    //RRC
    ext_opcode_map[0x0F] = &gb_cpu_state::rrc_a; ext_cycles[0x0F] = 8;   ext_opc_name[15] = string("rrc_a");
    ext_opcode_map[0x08] = &gb_cpu_state::rrc_b; ext_cycles[0x08] = 8;   ext_opc_name[8] = string("rrc_b");
    ext_opcode_map[0x09] = &gb_cpu_state::rrc_c; ext_cycles[0x09] = 8;   ext_opc_name[9] = string("rrc_c");
    ext_opcode_map[0x0A] = &gb_cpu_state::rrc_d; ext_cycles[0x0A] = 8;   ext_opc_name[10] = string("rrc_d");
    ext_opcode_map[0x0B] = &gb_cpu_state::rrc_e; ext_cycles[0x0B] = 8;   ext_opc_name[11] = string("rrc_e");
    ext_opcode_map[0x0C] = &gb_cpu_state::rrc_h; ext_cycles[0x0C] = 8;   ext_opc_name[12] = string("rrc_h");
    ext_opcode_map[0x0D] = &gb_cpu_state::rrc_l; ext_cycles[0x0D] = 8;   ext_opc_name[13] = string("rrc_l");
    ext_opcode_map[0x0E] = &gb_cpu_state::rrc_mhl; ext_cycles[0x0E] = 16;   ext_opc_name[14] = string("rrc_mhl");
    
    //RR
    ext_opcode_map[0x1F] = &gb_cpu_state::rr_a; ext_cycles[0x1F] = 8;   ext_opc_name[31] = string("rr_a");
    ext_opcode_map[0x18] = &gb_cpu_state::rr_b; ext_cycles[0x18] = 8;   ext_opc_name[24] = string("rr_b");
    ext_opcode_map[0x19] = &gb_cpu_state::rr_c; ext_cycles[0x19] = 8;   ext_opc_name[25] = string("rr_c");
    ext_opcode_map[0x1A] = &gb_cpu_state::rr_d; ext_cycles[0x1A] = 8;   ext_opc_name[26] = string("rr_d");
    ext_opcode_map[0x1B] = &gb_cpu_state::rr_e; ext_cycles[0x1B] = 8;   ext_opc_name[27] = string("rr_e");
    ext_opcode_map[0x1C] = &gb_cpu_state::rr_h; ext_cycles[0x1C] = 8;   ext_opc_name[28] = string("rr_h");
    ext_opcode_map[0x1D] = &gb_cpu_state::rr_l; ext_cycles[0x1D] = 8;   ext_opc_name[29] = string("rr_l");
    ext_opcode_map[0x1E] = &gb_cpu_state::rr_mhl; ext_cycles[0x1E] = 16;   ext_opc_name[30] = string("rr_mhl");
    
    //SLA
    ext_opcode_map[0x27] = &gb_cpu_state::sla_a; ext_cycles[0x27] = 8;   ext_opc_name[39] = string("sla_a");
    ext_opcode_map[0x20] = &gb_cpu_state::sla_b; ext_cycles[0x20] = 8;   ext_opc_name[32] = string("sla_b");
    ext_opcode_map[0x21] = &gb_cpu_state::sla_c; ext_cycles[0x21] = 8;   ext_opc_name[33] = string("sla_c");
    ext_opcode_map[0x22] = &gb_cpu_state::sla_d; ext_cycles[0x22] = 8;   ext_opc_name[34] = string("sla_d");
    ext_opcode_map[0x23] = &gb_cpu_state::sla_e; ext_cycles[0x23] = 8;   ext_opc_name[35] = string("sla_e");
    ext_opcode_map[0x24] = &gb_cpu_state::sla_h; ext_cycles[0x24] = 8;   ext_opc_name[36] = string("sla_h");
    ext_opcode_map[0x25] = &gb_cpu_state::sla_l; ext_cycles[0x25] = 8;   ext_opc_name[37] = string("sla_l");
    ext_opcode_map[0x26] = &gb_cpu_state::sla_mhl; ext_cycles[0x26] = 16;   ext_opc_name[38] = string("sla_mhl");
    
    //SRA
    ext_opcode_map[0x2F] = &gb_cpu_state::sra_a; ext_cycles[0x2F] = 8;   ext_opc_name[47] = string("sra_a");
    ext_opcode_map[0x28] = &gb_cpu_state::sra_b; ext_cycles[0x28] = 8;   ext_opc_name[40] = string("sra_b");
    ext_opcode_map[0x29] = &gb_cpu_state::sra_c; ext_cycles[0x29] = 8;   ext_opc_name[41] = string("sra_c");
    ext_opcode_map[0x2A] = &gb_cpu_state::sra_d; ext_cycles[0x2A] = 8;   ext_opc_name[42] = string("sra_d");
    ext_opcode_map[0x2B] = &gb_cpu_state::sra_e; ext_cycles[0x2B] = 8;   ext_opc_name[43] = string("sra_e");
    ext_opcode_map[0x2C] = &gb_cpu_state::sra_h; ext_cycles[0x2C] = 8;   ext_opc_name[44] = string("sra_h");
    ext_opcode_map[0x2D] = &gb_cpu_state::sra_l; ext_cycles[0x2D] = 8;   ext_opc_name[45] = string("sra_l");
    ext_opcode_map[0x2E] = &gb_cpu_state::sra_mhl; ext_cycles[0x2E] = 16;   ext_opc_name[46] = string("sra_mhl");
    
    //SRL
    ext_opcode_map[0x3F] = &gb_cpu_state::srl_a; ext_cycles[0x3F] = 8;   ext_opc_name[63] = string("srl_a");
    ext_opcode_map[0x38] = &gb_cpu_state::srl_b; ext_cycles[0x38] = 8;   ext_opc_name[56] = string("srl_b");
    ext_opcode_map[0x39] = &gb_cpu_state::srl_c; ext_cycles[0x39] = 8;   ext_opc_name[57] = string("srl_c");
    ext_opcode_map[0x3A] = &gb_cpu_state::srl_d; ext_cycles[0x3A] = 8;   ext_opc_name[58] = string("srl_d");
    ext_opcode_map[0x3B] = &gb_cpu_state::srl_e; ext_cycles[0x3B] = 8;   ext_opc_name[59] = string("srl_e");
    ext_opcode_map[0x3C] = &gb_cpu_state::srl_h; ext_cycles[0x3C] = 8;   ext_opc_name[60] = string("srl_h");
    ext_opcode_map[0x3D] = &gb_cpu_state::srl_l; ext_cycles[0x3D] = 8;   ext_opc_name[61] = string("srl_l");
    ext_opcode_map[0x3E] = &gb_cpu_state::srl_mhl; ext_cycles[0x3E] = 16;   ext_opc_name[62] = string("srl_mhl");
    
    //BIT
    ext_opcode_map[0x47] = &gb_cpu_state::bit_0_a; ext_cycles[0x47] = 8;   ext_opc_name[71] = string("bit_0_a");
    ext_opcode_map[0x4f] = &gb_cpu_state::bit_1_a; ext_cycles[0x4f] = 8;   ext_opc_name[79] = string("bit_1_a");
    ext_opcode_map[0x57] = &gb_cpu_state::bit_2_a; ext_cycles[0x57] = 8;   ext_opc_name[87] = string("bit_2_a");
    ext_opcode_map[0x5f] = &gb_cpu_state::bit_3_a; ext_cycles[0x5f] = 8;   ext_opc_name[95] = string("bit_3_a");
    ext_opcode_map[0x67] = &gb_cpu_state::bit_4_a; ext_cycles[0x67] = 8;   ext_opc_name[103] = string("bit_4_a");
    ext_opcode_map[0x6f] = &gb_cpu_state::bit_5_a; ext_cycles[0x6f] = 8;   ext_opc_name[111] = string("bit_5_a");
    ext_opcode_map[0x77] = &gb_cpu_state::bit_6_a; ext_cycles[0x77] = 8;   ext_opc_name[119] = string("bit_6_a");
    ext_opcode_map[0x7f] = &gb_cpu_state::bit_7_a; ext_cycles[0x7f] = 8;   ext_opc_name[127] = string("bit_7_a");
    
    ext_opcode_map[0x40] = &gb_cpu_state::bit_0_b; ext_cycles[0x40] = 8;   ext_opc_name[64] = string("bit_0_b");
    ext_opcode_map[0x48] = &gb_cpu_state::bit_1_b; ext_cycles[0x48] = 8;   ext_opc_name[72] = string("bit_1_b");
    ext_opcode_map[0x50] = &gb_cpu_state::bit_2_b; ext_cycles[0x50] = 8;   ext_opc_name[80] = string("bit_2_b");
    ext_opcode_map[0x58] = &gb_cpu_state::bit_3_b; ext_cycles[0x58] = 8;   ext_opc_name[88] = string("bit_3_b");
    ext_opcode_map[0x60] = &gb_cpu_state::bit_4_b; ext_cycles[0x60] = 8;   ext_opc_name[96] = string("bit_4_b");
    ext_opcode_map[0x68] = &gb_cpu_state::bit_5_b; ext_cycles[0x68] = 8;   ext_opc_name[104] = string("bit_5_b");
    ext_opcode_map[0x70] = &gb_cpu_state::bit_6_b; ext_cycles[0x70] = 8;   ext_opc_name[112] = string("bit_6_b");
    ext_opcode_map[0x78] = &gb_cpu_state::bit_7_b; ext_cycles[0x78] = 8;   ext_opc_name[120] = string("bit_7_b");
    
    ext_opcode_map[0x41] = &gb_cpu_state::bit_0_c; ext_cycles[0x41] = 8;   ext_opc_name[65] = string("bit_0_c");
    ext_opcode_map[0x49] = &gb_cpu_state::bit_1_c; ext_cycles[0x49] = 8;   ext_opc_name[73] = string("bit_1_c");
    ext_opcode_map[0x51] = &gb_cpu_state::bit_2_c; ext_cycles[0x51] = 8;   ext_opc_name[81] = string("bit_2_c");
    ext_opcode_map[0x59] = &gb_cpu_state::bit_3_c; ext_cycles[0x59] = 8;   ext_opc_name[89] = string("bit_3_c");
    ext_opcode_map[0x61] = &gb_cpu_state::bit_4_c; ext_cycles[0x61] = 8;   ext_opc_name[97] = string("bit_4_c");
    ext_opcode_map[0x69] = &gb_cpu_state::bit_5_c; ext_cycles[0x69] = 8;   ext_opc_name[105] = string("bit_5_c");
    ext_opcode_map[0x71] = &gb_cpu_state::bit_6_c; ext_cycles[0x71] = 8;   ext_opc_name[113] = string("bit_6_c");
    ext_opcode_map[0x79] = &gb_cpu_state::bit_7_c; ext_cycles[0x79] = 8;   ext_opc_name[121] = string("bit_7_c");
    
    ext_opcode_map[0x42] = &gb_cpu_state::bit_0_d; ext_cycles[0x42] = 8;   ext_opc_name[66] = string("bit_0_d");
    ext_opcode_map[0x4a] = &gb_cpu_state::bit_1_d; ext_cycles[0x4a] = 8;   ext_opc_name[74] = string("bit_1_d");
    ext_opcode_map[0x52] = &gb_cpu_state::bit_2_d; ext_cycles[0x52] = 8;   ext_opc_name[82] = string("bit_2_d");
    ext_opcode_map[0x5a] = &gb_cpu_state::bit_3_d; ext_cycles[0x5a] = 8;   ext_opc_name[90] = string("bit_3_d");
    ext_opcode_map[0x62] = &gb_cpu_state::bit_4_d; ext_cycles[0x62] = 8;   ext_opc_name[98] = string("bit_4_d");
    ext_opcode_map[0x6a] = &gb_cpu_state::bit_5_d; ext_cycles[0x6a] = 8;   ext_opc_name[106] = string("bit_5_d");
    ext_opcode_map[0x72] = &gb_cpu_state::bit_6_d; ext_cycles[0x72] = 8;   ext_opc_name[114] = string("bit_6_d");
    ext_opcode_map[0x7a] = &gb_cpu_state::bit_7_d; ext_cycles[0x7a] = 8;   ext_opc_name[122] = string("bit_7_d");
    
    ext_opcode_map[0x43] = &gb_cpu_state::bit_0_e; ext_cycles[0x43] = 8;   ext_opc_name[67] = string("bit_0_e");
    ext_opcode_map[0x4b] = &gb_cpu_state::bit_1_e; ext_cycles[0x4b] = 8;   ext_opc_name[75] = string("bit_1_e");
    ext_opcode_map[0x53] = &gb_cpu_state::bit_2_e; ext_cycles[0x53] = 8;   ext_opc_name[83] = string("bit_2_e");
    ext_opcode_map[0x5b] = &gb_cpu_state::bit_3_e; ext_cycles[0x5b] = 8;   ext_opc_name[91] = string("bit_3_e");
    ext_opcode_map[0x63] = &gb_cpu_state::bit_4_e; ext_cycles[0x63] = 8;   ext_opc_name[99] = string("bit_4_e");
    ext_opcode_map[0x6b] = &gb_cpu_state::bit_5_e; ext_cycles[0x6b] = 8;   ext_opc_name[107] = string("bit_5_e");
    ext_opcode_map[0x73] = &gb_cpu_state::bit_6_e; ext_cycles[0x73] = 8;   ext_opc_name[115] = string("bit_6_e");
    ext_opcode_map[0x7b] = &gb_cpu_state::bit_7_e; ext_cycles[0x7b] = 8;   ext_opc_name[123] = string("bit_7_e");
    
    ext_opcode_map[0x44] = &gb_cpu_state::bit_0_h; ext_cycles[0x44] = 8;   ext_opc_name[68] = string("bit_0_h");
    ext_opcode_map[0x4c] = &gb_cpu_state::bit_1_h; ext_cycles[0x4c] = 8;   ext_opc_name[76] = string("bit_1_h");
    ext_opcode_map[0x54] = &gb_cpu_state::bit_2_h; ext_cycles[0x54] = 8;   ext_opc_name[84] = string("bit_2_h");
    ext_opcode_map[0x5c] = &gb_cpu_state::bit_3_h; ext_cycles[0x5c] = 8;   ext_opc_name[92] = string("bit_3_h");
    ext_opcode_map[0x64] = &gb_cpu_state::bit_4_h; ext_cycles[0x64] = 8;   ext_opc_name[100] = string("bit_4_h");
    ext_opcode_map[0x6c] = &gb_cpu_state::bit_5_h; ext_cycles[0x6c] = 8;   ext_opc_name[108] = string("bit_5_h");
    ext_opcode_map[0x74] = &gb_cpu_state::bit_6_h; ext_cycles[0x74] = 8;   ext_opc_name[116] = string("bit_6_h");
    ext_opcode_map[0x7c] = &gb_cpu_state::bit_7_h; ext_cycles[0x7c] = 8;   ext_opc_name[124] = string("bit_7_h");
    
    ext_opcode_map[0x45] = &gb_cpu_state::bit_0_l; ext_cycles[0x45] = 8;   ext_opc_name[69] = string("bit_0_l");
    ext_opcode_map[0x4d] = &gb_cpu_state::bit_1_l; ext_cycles[0x4d] = 8;   ext_opc_name[77] = string("bit_1_l");
    ext_opcode_map[0x55] = &gb_cpu_state::bit_2_l; ext_cycles[0x55] = 8;   ext_opc_name[85] = string("bit_2_l");
    ext_opcode_map[0x5d] = &gb_cpu_state::bit_3_l; ext_cycles[0x5d] = 8;   ext_opc_name[93] = string("bit_3_l");
    ext_opcode_map[0x65] = &gb_cpu_state::bit_4_l; ext_cycles[0x65] = 8;   ext_opc_name[101] = string("bit_4_l");
    ext_opcode_map[0x6d] = &gb_cpu_state::bit_5_l; ext_cycles[0x6d] = 8;   ext_opc_name[109] = string("bit_5_l");
    ext_opcode_map[0x75] = &gb_cpu_state::bit_6_l; ext_cycles[0x75] = 8;   ext_opc_name[117] = string("bit_6_l");
    ext_opcode_map[0x7d] = &gb_cpu_state::bit_7_l; ext_cycles[0x7d] = 8;   ext_opc_name[125] = string("bit_7_l");
    
    ext_opcode_map[0x46] = &gb_cpu_state::bit_0_mhl; ext_cycles[0x46] = 12;   ext_opc_name[70] = string("bit_0_mhl");
    ext_opcode_map[0x4e] = &gb_cpu_state::bit_1_mhl; ext_cycles[0x4e] = 12;   ext_opc_name[78] = string("bit_1_mhl");
    ext_opcode_map[0x56] = &gb_cpu_state::bit_2_mhl; ext_cycles[0x56] = 12;   ext_opc_name[86] = string("bit_2_mhl");
    ext_opcode_map[0x5e] = &gb_cpu_state::bit_3_mhl; ext_cycles[0x5e] = 12;   ext_opc_name[94] = string("bit_3_mhl");
    ext_opcode_map[0x66] = &gb_cpu_state::bit_4_mhl; ext_cycles[0x66] = 12;   ext_opc_name[102] = string("bit_4_mhl");
    ext_opcode_map[0x6e] = &gb_cpu_state::bit_5_mhl; ext_cycles[0x6e] = 12;   ext_opc_name[110] = string("bit_5_mhl");
    ext_opcode_map[0x76] = &gb_cpu_state::bit_6_mhl; ext_cycles[0x76] = 12;   ext_opc_name[118] = string("bit_6_mhl");
    ext_opcode_map[0x7e] = &gb_cpu_state::bit_7_mhl; ext_cycles[0x7e] = 12;   ext_opc_name[126] = string("bit_7_mhl");
    
    
    //SET
    ext_opcode_map[0xc7] = &gb_cpu_state::set_0_a; ext_cycles[0xc7] = 8;   ext_opc_name[199] = string("set_0_a");
    ext_opcode_map[0xcf] = &gb_cpu_state::set_1_a; ext_cycles[0xcf] = 8;   ext_opc_name[207] = string("set_1_a");
    ext_opcode_map[0xd7] = &gb_cpu_state::set_2_a; ext_cycles[0xd7] = 8;   ext_opc_name[215] = string("set_2_a");
    ext_opcode_map[0xdf] = &gb_cpu_state::set_3_a; ext_cycles[0xdf] = 8;   ext_opc_name[223] = string("set_3_a");
    ext_opcode_map[0xe7] = &gb_cpu_state::set_4_a; ext_cycles[0xe7] = 8;   ext_opc_name[231] = string("set_4_a");
    ext_opcode_map[0xef] = &gb_cpu_state::set_5_a; ext_cycles[0xef] = 8;   ext_opc_name[239] = string("set_5_a");
    ext_opcode_map[0xf7] = &gb_cpu_state::set_6_a; ext_cycles[0xf7] = 8;   ext_opc_name[247] = string("set_6_a");
    ext_opcode_map[0xff] = &gb_cpu_state::set_7_a; ext_cycles[0xff] = 8;   ext_opc_name[255] = string("set_7_a");
    
    ext_opcode_map[0xc0] = &gb_cpu_state::set_0_b; ext_cycles[0xc0] = 8;   ext_opc_name[192] = string("set_0_b");
    ext_opcode_map[0xc8] = &gb_cpu_state::set_1_b; ext_cycles[0xc8] = 8;   ext_opc_name[200] = string("set_1_b");
    ext_opcode_map[0xd0] = &gb_cpu_state::set_2_b; ext_cycles[0xd0] = 8;   ext_opc_name[208] = string("set_2_b");
    ext_opcode_map[0xd8] = &gb_cpu_state::set_3_b; ext_cycles[0xd8] = 8;   ext_opc_name[216] = string("set_3_b");
    ext_opcode_map[0xe0] = &gb_cpu_state::set_4_b; ext_cycles[0xe0] = 8;   ext_opc_name[224] = string("set_4_b");
    ext_opcode_map[0xe8] = &gb_cpu_state::set_5_b; ext_cycles[0xe8] = 8;   ext_opc_name[232] = string("set_5_b");
    ext_opcode_map[0xf0] = &gb_cpu_state::set_6_b; ext_cycles[0xf0] = 8;   ext_opc_name[240] = string("set_6_b");
    ext_opcode_map[0xf8] = &gb_cpu_state::set_7_b; ext_cycles[0xf8] = 8;   ext_opc_name[248] = string("set_7_b");
    
    ext_opcode_map[0xc1] = &gb_cpu_state::set_0_c; ext_cycles[0xc1] = 8;   ext_opc_name[193] = string("set_0_c");
    ext_opcode_map[0xc9] = &gb_cpu_state::set_1_c; ext_cycles[0xc9] = 8;   ext_opc_name[201] = string("set_1_c");
    ext_opcode_map[0xd1] = &gb_cpu_state::set_2_c; ext_cycles[0xd1] = 8;   ext_opc_name[209] = string("set_2_c");
    ext_opcode_map[0xd9] = &gb_cpu_state::set_3_c; ext_cycles[0xd9] = 8;   ext_opc_name[217] = string("set_3_c");
    ext_opcode_map[0xe1] = &gb_cpu_state::set_4_c; ext_cycles[0xe1] = 8;   ext_opc_name[225] = string("set_4_c");
    ext_opcode_map[0xe9] = &gb_cpu_state::set_5_c; ext_cycles[0xe9] = 8;   ext_opc_name[233] = string("set_5_c");
    ext_opcode_map[0xf1] = &gb_cpu_state::set_6_c; ext_cycles[0xf1] = 8;   ext_opc_name[241] = string("set_6_c");
    ext_opcode_map[0xf9] = &gb_cpu_state::set_7_c; ext_cycles[0xf9] = 8;   ext_opc_name[249] = string("set_7_c");
    
    ext_opcode_map[0xc2] = &gb_cpu_state::set_0_d; ext_cycles[0xc2] = 8;   ext_opc_name[194] = string("set_0_d");
    ext_opcode_map[0xca] = &gb_cpu_state::set_1_d; ext_cycles[0xca] = 8;   ext_opc_name[202] = string("set_1_d");
    ext_opcode_map[0xd2] = &gb_cpu_state::set_2_d; ext_cycles[0xd2] = 8;   ext_opc_name[210] = string("set_2_d");
    ext_opcode_map[0xda] = &gb_cpu_state::set_3_d; ext_cycles[0xda] = 8;   ext_opc_name[218] = string("set_3_d");
    ext_opcode_map[0xe2] = &gb_cpu_state::set_4_d; ext_cycles[0xe2] = 8;   ext_opc_name[226] = string("set_4_d");
    ext_opcode_map[0xea] = &gb_cpu_state::set_5_d; ext_cycles[0xea] = 8;   ext_opc_name[234] = string("set_5_d");
    ext_opcode_map[0xf2] = &gb_cpu_state::set_6_d; ext_cycles[0xf2] = 8;   ext_opc_name[242] = string("set_6_d");
    ext_opcode_map[0xfa] = &gb_cpu_state::set_7_d; ext_cycles[0xfa] = 8;   ext_opc_name[250] = string("set_7_d");
    
    ext_opcode_map[0xc3] = &gb_cpu_state::set_0_e; ext_cycles[0xc3] = 8;   ext_opc_name[195] = string("set_0_e");
    ext_opcode_map[0xcb] = &gb_cpu_state::set_1_e; ext_cycles[0xcb] = 8;   ext_opc_name[203] = string("set_1_e");
    ext_opcode_map[0xd3] = &gb_cpu_state::set_2_e; ext_cycles[0xd3] = 8;   ext_opc_name[211] = string("set_2_e");
    ext_opcode_map[0xdb] = &gb_cpu_state::set_3_e; ext_cycles[0xdb] = 8;   ext_opc_name[219] = string("set_3_e");
    ext_opcode_map[0xe3] = &gb_cpu_state::set_4_e; ext_cycles[0xe3] = 8;   ext_opc_name[227] = string("set_4_e");
    ext_opcode_map[0xeb] = &gb_cpu_state::set_5_e; ext_cycles[0xeb] = 8;   ext_opc_name[235] = string("set_5_e");
    ext_opcode_map[0xf3] = &gb_cpu_state::set_6_e; ext_cycles[0xf3] = 8;   ext_opc_name[243] = string("set_6_e");
    ext_opcode_map[0xfb] = &gb_cpu_state::set_7_e; ext_cycles[0xfb] = 8;   ext_opc_name[251] = string("set_7_e");
    
    ext_opcode_map[0xc4] = &gb_cpu_state::set_0_h; ext_cycles[0xc4] = 8;   ext_opc_name[196] = string("set_0_h");
    ext_opcode_map[0xcc] = &gb_cpu_state::set_1_h; ext_cycles[0xcc] = 8;   ext_opc_name[204] = string("set_1_h");
    ext_opcode_map[0xd4] = &gb_cpu_state::set_2_h; ext_cycles[0xd4] = 8;   ext_opc_name[212] = string("set_2_h");
    ext_opcode_map[0xdc] = &gb_cpu_state::set_3_h; ext_cycles[0xdc] = 8;   ext_opc_name[220] = string("set_3_h");
    ext_opcode_map[0xe4] = &gb_cpu_state::set_4_h; ext_cycles[0xe4] = 8;   ext_opc_name[228] = string("set_4_h");
    ext_opcode_map[0xec] = &gb_cpu_state::set_5_h; ext_cycles[0xec] = 8;   ext_opc_name[236] = string("set_5_h");
    ext_opcode_map[0xf4] = &gb_cpu_state::set_6_h; ext_cycles[0xf4] = 8;   ext_opc_name[244] = string("set_6_h");
    ext_opcode_map[0xfc] = &gb_cpu_state::set_7_h; ext_cycles[0xfc] = 8;   ext_opc_name[252] = string("set_7_h");
    
    ext_opcode_map[0xc5] = &gb_cpu_state::set_0_l; ext_cycles[0xc5] = 8;   ext_opc_name[197] = string("set_0_l");
    ext_opcode_map[0xcd] = &gb_cpu_state::set_1_l; ext_cycles[0xcd] = 8;   ext_opc_name[205] = string("set_1_l");
    ext_opcode_map[0xd5] = &gb_cpu_state::set_2_l; ext_cycles[0xd5] = 8;   ext_opc_name[213] = string("set_2_l");
    ext_opcode_map[0xdd] = &gb_cpu_state::set_3_l; ext_cycles[0xdd] = 8;   ext_opc_name[221] = string("set_3_l");
    ext_opcode_map[0xe5] = &gb_cpu_state::set_4_l; ext_cycles[0xe5] = 8;   ext_opc_name[229] = string("set_4_l");
    ext_opcode_map[0xed] = &gb_cpu_state::set_5_l; ext_cycles[0xed] = 8;   ext_opc_name[237] = string("set_5_l");
    ext_opcode_map[0xf5] = &gb_cpu_state::set_6_l; ext_cycles[0xf5] = 8;   ext_opc_name[245] = string("set_6_l");
    ext_opcode_map[0xfd] = &gb_cpu_state::set_7_l; ext_cycles[0xfd] = 8;   ext_opc_name[253] = string("set_7_l");
    
    ext_opcode_map[0xc6] = &gb_cpu_state::set_0_mhl; ext_cycles[0xc6] = 16;   ext_opc_name[198] = string("set_0_mhl");
    ext_opcode_map[0xce] = &gb_cpu_state::set_1_mhl; ext_cycles[0xce] = 16;   ext_opc_name[206] = string("set_1_mhl");
    ext_opcode_map[0xd6] = &gb_cpu_state::set_2_mhl; ext_cycles[0xd6] = 16;   ext_opc_name[214] = string("set_2_mhl");
    ext_opcode_map[0xde] = &gb_cpu_state::set_3_mhl; ext_cycles[0xde] = 16;   ext_opc_name[222] = string("set_3_mhl");
    ext_opcode_map[0xe6] = &gb_cpu_state::set_4_mhl; ext_cycles[0xe6] = 16;   ext_opc_name[230] = string("set_4_mhl");
    ext_opcode_map[0xee] = &gb_cpu_state::set_5_mhl; ext_cycles[0xee] = 16;   ext_opc_name[238] = string("set_5_mhl");
    ext_opcode_map[0xf6] = &gb_cpu_state::set_6_mhl; ext_cycles[0xf6] = 16;   ext_opc_name[246] = string("set_6_mhl");
    ext_opcode_map[0xfe] = &gb_cpu_state::set_7_mhl; ext_cycles[0xfe] = 16;   ext_opc_name[254] = string("set_7_mhl");
    
    //RES
    
    ext_opcode_map[0x87] = &gb_cpu_state::res_0_a; ext_cycles[0x87] = 8;   ext_opc_name[135] = string("res_0_a");
    ext_opcode_map[0x8f] = &gb_cpu_state::res_1_a; ext_cycles[0x8f] = 8;   ext_opc_name[143] = string("res_1_a");
    ext_opcode_map[0x97] = &gb_cpu_state::res_2_a; ext_cycles[0x97] = 8;   ext_opc_name[151] = string("res_2_a");
    ext_opcode_map[0x9f] = &gb_cpu_state::res_3_a; ext_cycles[0x9f] = 8;   ext_opc_name[159] = string("res_3_a");
    ext_opcode_map[0xa7] = &gb_cpu_state::res_4_a; ext_cycles[0xa7] = 8;   ext_opc_name[167] = string("res_4_a");
    ext_opcode_map[0xaf] = &gb_cpu_state::res_5_a; ext_cycles[0xaf] = 8;   ext_opc_name[175] = string("res_5_a");
    ext_opcode_map[0xb7] = &gb_cpu_state::res_6_a; ext_cycles[0xb7] = 8;   ext_opc_name[183] = string("res_6_a");
    ext_opcode_map[0xbf] = &gb_cpu_state::res_7_a; ext_cycles[0xbf] = 8;   ext_opc_name[191] = string("res_7_a");
    
    ext_opcode_map[0x80] = &gb_cpu_state::res_0_b; ext_cycles[0x80] = 8;   ext_opc_name[128] = string("res_0_b");
    ext_opcode_map[0x88] = &gb_cpu_state::res_1_b; ext_cycles[0x88] = 8;   ext_opc_name[136] = string("res_1_b");
    ext_opcode_map[0x90] = &gb_cpu_state::res_2_b; ext_cycles[0x90] = 8;   ext_opc_name[144] = string("res_2_b");
    ext_opcode_map[0x98] = &gb_cpu_state::res_3_b; ext_cycles[0x98] = 8;   ext_opc_name[152] = string("res_3_b");
    ext_opcode_map[0xa0] = &gb_cpu_state::res_4_b; ext_cycles[0xa0] = 8;   ext_opc_name[160] = string("res_4_b");
    ext_opcode_map[0xa8] = &gb_cpu_state::res_5_b; ext_cycles[0xa8] = 8;   ext_opc_name[168] = string("res_5_b");
    ext_opcode_map[0xb0] = &gb_cpu_state::res_6_b; ext_cycles[0xb0] = 8;   ext_opc_name[176] = string("res_6_b");
    ext_opcode_map[0xb8] = &gb_cpu_state::res_7_b; ext_cycles[0xb8] = 8;   ext_opc_name[184] = string("res_7_b");
    
    ext_opcode_map[0x81] = &gb_cpu_state::res_0_c; ext_cycles[0x81] = 8;   ext_opc_name[129] = string("res_0_c");
    ext_opcode_map[0x89] = &gb_cpu_state::res_1_c; ext_cycles[0x89] = 8;   ext_opc_name[137] = string("res_1_c");
    ext_opcode_map[0x91] = &gb_cpu_state::res_2_c; ext_cycles[0x91] = 8;   ext_opc_name[145] = string("res_2_c");
    ext_opcode_map[0x99] = &gb_cpu_state::res_3_c; ext_cycles[0x99] = 8;   ext_opc_name[153] = string("res_3_c");
    ext_opcode_map[0xa1] = &gb_cpu_state::res_4_c; ext_cycles[0xa1] = 8;   ext_opc_name[161] = string("res_4_c");
    ext_opcode_map[0xa9] = &gb_cpu_state::res_5_c; ext_cycles[0xa9] = 8;   ext_opc_name[169] = string("res_5_c");
    ext_opcode_map[0xb1] = &gb_cpu_state::res_6_c; ext_cycles[0xb1] = 8;   ext_opc_name[177] = string("res_6_c");
    ext_opcode_map[0xb9] = &gb_cpu_state::res_7_c; ext_cycles[0xb9] = 8;   ext_opc_name[185] = string("res_7_c");
    
    ext_opcode_map[0x82] = &gb_cpu_state::res_0_d; ext_cycles[0x82] = 8;   ext_opc_name[130] = string("res_0_d");
    ext_opcode_map[0x8a] = &gb_cpu_state::res_1_d; ext_cycles[0x8a] = 8;   ext_opc_name[138] = string("res_1_d");
    ext_opcode_map[0x92] = &gb_cpu_state::res_2_d; ext_cycles[0x92] = 8;   ext_opc_name[146] = string("res_2_d");
    ext_opcode_map[0x9a] = &gb_cpu_state::res_3_d; ext_cycles[0x9a] = 8;   ext_opc_name[154] = string("res_3_d");
    ext_opcode_map[0xa2] = &gb_cpu_state::res_4_d; ext_cycles[0xa2] = 8;   ext_opc_name[162] = string("res_4_d");
    ext_opcode_map[0xaa] = &gb_cpu_state::res_5_d; ext_cycles[0xaa] = 8;   ext_opc_name[170] = string("res_5_d");
    ext_opcode_map[0xb2] = &gb_cpu_state::res_6_d; ext_cycles[0xb2] = 8;   ext_opc_name[178] = string("res_6_d");
    ext_opcode_map[0xba] = &gb_cpu_state::res_7_d; ext_cycles[0xba] = 8;   ext_opc_name[186] = string("res_7_d");
    
    ext_opcode_map[0x83] = &gb_cpu_state::res_0_e; ext_cycles[0x83] = 8;   ext_opc_name[131] = string("res_0_e");
    ext_opcode_map[0x8b] = &gb_cpu_state::res_1_e; ext_cycles[0x8b] = 8;   ext_opc_name[139] = string("res_1_e");
    ext_opcode_map[0x93] = &gb_cpu_state::res_2_e; ext_cycles[0x93] = 8;   ext_opc_name[147] = string("res_2_e");
    ext_opcode_map[0x9b] = &gb_cpu_state::res_3_e; ext_cycles[0x9b] = 8;   ext_opc_name[155] = string("res_3_e");
    ext_opcode_map[0xa3] = &gb_cpu_state::res_4_e; ext_cycles[0xa3] = 8;   ext_opc_name[163] = string("res_4_e");
    ext_opcode_map[0xab] = &gb_cpu_state::res_5_e; ext_cycles[0xab] = 8;   ext_opc_name[171] = string("res_5_e");
    ext_opcode_map[0xb3] = &gb_cpu_state::res_6_e; ext_cycles[0xb3] = 8;   ext_opc_name[179] = string("res_6_e");
    ext_opcode_map[0xbb] = &gb_cpu_state::res_7_e; ext_cycles[0xbb] = 8;   ext_opc_name[187] = string("res_7_e");
    
    ext_opcode_map[0x84] = &gb_cpu_state::res_0_h; ext_cycles[0x84] = 8;   ext_opc_name[132] = string("res_0_h");
    ext_opcode_map[0x8c] = &gb_cpu_state::res_1_h; ext_cycles[0x8c] = 8;   ext_opc_name[140] = string("res_1_h");
    ext_opcode_map[0x94] = &gb_cpu_state::res_2_h; ext_cycles[0x94] = 8;   ext_opc_name[148] = string("res_2_h");
    ext_opcode_map[0x9c] = &gb_cpu_state::res_3_h; ext_cycles[0x9c] = 8;   ext_opc_name[156] = string("res_3_h");
    ext_opcode_map[0xa4] = &gb_cpu_state::res_4_h; ext_cycles[0xa4] = 8;   ext_opc_name[164] = string("res_4_h");
    ext_opcode_map[0xac] = &gb_cpu_state::res_5_h; ext_cycles[0xac] = 8;   ext_opc_name[172] = string("res_5_h");
    ext_opcode_map[0xb4] = &gb_cpu_state::res_6_h; ext_cycles[0xb4] = 8;   ext_opc_name[180] = string("res_6_h");
    ext_opcode_map[0xbc] = &gb_cpu_state::res_7_h; ext_cycles[0xbc] = 8;   ext_opc_name[188] = string("res_7_h");
    
    ext_opcode_map[0x85] = &gb_cpu_state::res_0_l; ext_cycles[0x85] = 8;   ext_opc_name[133] = string("res_0_l");
    ext_opcode_map[0x8d] = &gb_cpu_state::res_1_l; ext_cycles[0x8d] = 8;   ext_opc_name[141] = string("res_1_l");
    ext_opcode_map[0x95] = &gb_cpu_state::res_2_l; ext_cycles[0x95] = 8;   ext_opc_name[149] = string("res_2_l");
    ext_opcode_map[0x9d] = &gb_cpu_state::res_3_l; ext_cycles[0x9d] = 8;   ext_opc_name[157] = string("res_3_l");
    ext_opcode_map[0xa5] = &gb_cpu_state::res_4_l; ext_cycles[0xa5] = 8;   ext_opc_name[165] = string("res_4_l");
    ext_opcode_map[0xad] = &gb_cpu_state::res_5_l; ext_cycles[0xad] = 8;   ext_opc_name[173] = string("res_5_l");
    ext_opcode_map[0xb5] = &gb_cpu_state::res_6_l; ext_cycles[0xb5] = 8;   ext_opc_name[181] = string("res_6_l");
    ext_opcode_map[0xbd] = &gb_cpu_state::res_7_l; ext_cycles[0xbd] = 8;   ext_opc_name[189] = string("res_7_l");
    
    ext_opcode_map[0x86] = &gb_cpu_state::res_0_mhl; ext_cycles[0x86] = 16;   ext_opc_name[134] = string("res_0_mhl");
    ext_opcode_map[0x8e] = &gb_cpu_state::res_1_mhl; ext_cycles[0x8e] = 16;   ext_opc_name[142] = string("res_1_mhl");
    ext_opcode_map[0x96] = &gb_cpu_state::res_2_mhl; ext_cycles[0x96] = 16;   ext_opc_name[150] = string("res_2_mhl");
    ext_opcode_map[0x9e] = &gb_cpu_state::res_3_mhl; ext_cycles[0x9e] = 16;   ext_opc_name[158] = string("res_3_mhl");
    ext_opcode_map[0xa6] = &gb_cpu_state::res_4_mhl; ext_cycles[0xa6] = 16;   ext_opc_name[166] = string("res_4_mhl");
    ext_opcode_map[0xae] = &gb_cpu_state::res_5_mhl; ext_cycles[0xae] = 16;   ext_opc_name[174] = string("res_5_mhl");
    ext_opcode_map[0xb6] = &gb_cpu_state::res_6_mhl; ext_cycles[0xb6] = 16;   ext_opc_name[182] = string("res_6_mhl");
    ext_opcode_map[0xbe] = &gb_cpu_state::res_7_mhl; ext_cycles[0xbe] = 16;   ext_opc_name[190] = string("res_7_mhl");
     
}



//Execute 1 CPU Fetch /exectute cycle
//Return number of cycles processed
int gb_cpu_state::execute() {
    //Manage Interrupts
    
    if (gb_int->is_int()) {
       // cout << "Interrupt" << endl;
       
        int_e = gb_mem->rb(IO_REG_IE);
        int_f = gb_mem->rb(IO_REG_IF);
        
        if (gb_int->get_vblank()) {
            //VBLANK
            //cout << "VBLANK"<< endl;
       
            halted = 0;
            if (ime) {
                ime = false;
                
                sp--;
                gb_mem->wb(sp, pc/(1<<8));
                sp--;
                gb_mem->wb(sp, pc%(1<<8));
                
                pc = INT_START_VBLANK;
                
                gb_int->clear_vblank();
            }
            
        } else if (gb_int->get_lcdc()) {
            //LCDC
            //cout << "LCDC"<< endl;
            
             halted = 0;
            if (ime) {
                ime = false;
                
                sp--;
                gb_mem->wb(sp, pc/(1<<8));
                sp--;
                gb_mem->wb(sp, pc%(1<<8));
                
                pc = INT_START_LCDC;
                
                gb_int->clear_lcdc();
            }
            
            
        } else if (gb_int->get_timer()) {
            //TIMER
           // cout << "TIMER"<< endl;
           
            halted = 0;
           
            if (ime) {
                ime = false;
                
                sp--;
                gb_mem->wb(sp, pc/(1<<8));
                sp--;
                gb_mem->wb(sp, pc%(1<<8));
                
                pc = INT_START_TIMER;
                
                gb_int->clear_timer();
            }
        
        } else if (gb_int->get_serial()) {
            //SERIAL
            //cout << "SERIAL"<< endl;
            
             halted = 0;
             
            if (ime) {
                ime = false;
                
                sp--;
                gb_mem->wb(sp, pc/(1<<8));
                sp--;
                gb_mem->wb(sp, pc%(1<<8));
                
                pc = INT_START_SERIAL;
                
                gb_int->clear_serial();
            }
         
        } else if (gb_int->get_htl()) {
            //HTL
           // cout << "HTL"<< endl;
            
             halted = 0;
            if ( ime) {
                ime = false; 
                
                sp--;
                gb_mem->wb(sp, pc/(1<<8));
                sp--;
                gb_mem->wb(sp, pc%(1<<8));
                
                pc = INT_START_HTL;
                
                gb_int->clear_htl();
            }
        }
    }
    
    if (halted) 
    	return 4;
    	
    unsigned char instr = gb_mem->rb(pc);
    int prev_t = t;

    pc++;
  
    
    
    if (instr != 0xCB) {
      
        /*if (pc >= 0x000  && pc <= 0x8000) {
             // cout <<"0x"<<hex <<pc-1<<": "<<hex << (((unsigned short)instr)&0xff)<< " "<<opc_name[instr]  <<"   arg = "<< hex << gb_mem->rb(pc)+0l<< endl;
            if ((opc_name[instr] ==string("ld_mhl_a")) || (opc_name[instr] ==string("ld_mm_a"))|| (opc_name[instr] ==string("ret"))|| (opc_name[instr] ==string("reti"))) {
                cout <<"0x"<<hex <<pc-1<<": "<<hex << (((unsigned short)instr)&0xff)<< " "<<opc_name[instr]  <<"   arg = "<< hex << gb_mem->rb(pc)+0l<< endl;
            }
        }*/

		(*this.*opcode_map[instr])();


		t += cycles[instr];
		m += cycles[instr]/4;
    } else {
		unsigned char ext_instr = gb_mem->rb(pc);
		
		pc++;
      /*  if (pc >= 0x0 ) {
        cout <<"0x"<<hex <<pc-1<<": "<<hex << (((unsigned short)ext_instr)&0xff)<< " "<<ext_opc_name[ext_instr]    << "     "<<hex << (((unsigned short)gb_mem->rb(pc))&0xff)<< endl;
        }*/
		(*this.*ext_opcode_map[ext_instr])();
		
		t += ext_cycles[ext_instr];
		m += ext_cycles[ext_instr]/4;
        
	}
    

    
    if (pc >= 0xffff) {
        cout << "Error: PC > 0xffff" << endl;
        while (1);
    }
    
    
    return (t+(1<<16)-prev_t)%(1<<16);
}

void gb_cpu_state::reset() {
    //Reset of Registers
    a = b = c = d = e = f = h = l = 0;

    sp = 0;
    pc = 0;

    //Reset of clocks
    t = m = 0;

    //Init Params
    halted  = 0;
    ime = 0;
}

int gb_cpu_state::loadMEM(gb_mem_state *_gb_mem) {
    gb_mem = _gb_mem;
    return 1;
}

int gb_cpu_state::loadINT(gb_interrupt_state *_gb_int) {
    gb_int = _gb_int;
    return 1;
}

int gb_cpu_state::loadBIOS(string filename) {
	return gb_mem->loadBIOS(filename);
}

int gb_cpu_state::loadROM(string filename) {
    return gb_mem->loadROM(filename);
}


void gb_cpu_state::core_dump() {
	
	cout << "Registers: " << endl;
	cout << "a  = " << (a&0xff) << endl;
	cout << "b  = " << (b&0xff) << endl;
	cout << "c  = " << (c&0xff) << endl;
	cout << "d  = " << (d&0xff) << endl;
	cout << "e  = " << (e&0xff) << endl;
	cout << "h  = " << (h&0xff) << endl;
	cout << "l  = " << (l&0xff) << endl;
	cout << "f  = " << (f&0xff) << endl;
	cout << endl;
	cout << "pc = "<<hex <<pc << endl;
	cout << "sp = "<<hex <<sp << endl<<endl;
	cout << "Clocks: " << endl;
	cout << "m  = " <<dec<< m << endl;
	cout << "t  = " <<dec<< t << endl;
    
    cout << "inBios = "<< hex<<((gb_mem->rb(0)&0xff)==0x31) << endl;
   
}

unsigned short gb_cpu_state::mk_word(unsigned char x,unsigned char y) {
	return (x<<8)+y;
}

//Loads

//LD A,* 
void gb_cpu_state::ld_a_m()  //8   3E
{ 
	a = gb_mem->rb(pc);
	pc++;
}
void gb_cpu_state::ld_a_mm() //16  FA
{ 
	a = gb_mem->rb(mk_word(gb_mem->rb(pc+1), gb_mem->rb(pc)));
	pc += 2;
}
void gb_cpu_state::ld_b_m() { //8 06
	b = gb_mem->rb(pc);
	pc++;
}
void gb_cpu_state::ld_c_m(){  //8 0E
	c = gb_mem->rb(pc);
	pc++;
}
void gb_cpu_state::ld_d_m(){  //8 16
	d = gb_mem->rb(pc);
	pc++;
}
void gb_cpu_state::ld_e_m(){  //8 1E
	e = gb_mem->rb(pc);
	pc++;
}
void gb_cpu_state::ld_h_m(){  //8 26
	h = gb_mem->rb(pc);
	pc++;
}
void gb_cpu_state::ld_l_m(){  //8 2E
	l = gb_mem->rb(pc);
	pc++;
}
void gb_cpu_state::ld_mhl_m(){ //12 36
	gb_mem->wb(mk_word(h,l), gb_mem->rb(pc));
	pc++;
}

//LD A, *
void gb_cpu_state::ld_a_a()  //4   7F
{
	//a = a;
}
void gb_cpu_state::ld_a_b()  //4   78
{
	a = b;
}
void gb_cpu_state::ld_a_c()  //4   79
{
	a = c;
}
void gb_cpu_state::ld_a_d()  //4   7A
{
	a = d;
}
void gb_cpu_state::ld_a_e()  //4   7B
{
	a = e;
}
void gb_cpu_state::ld_a_h()  //4   7C
{
	a = h;
}
void gb_cpu_state::ld_a_l()  //4   7D
{
	a = l;
}
void gb_cpu_state::ld_a_mbc() //8   0A
{
	a = gb_mem->rb(mk_word(b,c));
}
void gb_cpu_state::ld_a_mde() //8   1A
{
	a = gb_mem->rb(mk_word(d,e));
}
void gb_cpu_state::ld_a_mhl() //8   7E
{
	a = gb_mem->rb(mk_word(h,l));
}
//LD B, *
void gb_cpu_state::ld_b_a()  //4   47
{
	b = a;	
}
void gb_cpu_state::ld_b_b() //4   40
{
	//b = b;
}
void gb_cpu_state::ld_b_c() //4   41
{
	b = c;
}
void gb_cpu_state::ld_b_d()  //4   42
{
	b = d;
}
void gb_cpu_state::ld_b_e()  //4   43
{
	b = e;
}
void gb_cpu_state::ld_b_h()  //4   44
{
	b = h;
}
void gb_cpu_state::ld_b_l()  //4   45
{
	b = l;
}
void gb_cpu_state::ld_b_mhl() //8   46
{
	b = gb_mem->rb(mk_word(h,l));
}

//LD C, *

void gb_cpu_state::ld_c_a()  //4   4F
{
	c = a;	
}
void gb_cpu_state::ld_c_b()  //4   48
{
	c = b;
}
void gb_cpu_state::ld_c_c()  //4   49
{
	//c = c;
}
void gb_cpu_state::ld_c_d()  //4   4A
{
	c = d;
}
void gb_cpu_state::ld_c_e()  //4   4B
{
	c = e;
}
void gb_cpu_state::ld_c_h()  //4   4C
{
	c = h;
}
void gb_cpu_state::ld_c_l()  //4   4D
{
	c = l;
}
void gb_cpu_state::ld_c_mhl() //8   4E
{
	c = gb_mem->rb(mk_word(h,l));
}

//LD D, *
void gb_cpu_state::ld_d_a()  //4   57
{
	d = a;	
}
void gb_cpu_state::ld_d_b()  //4   50
{
	d = b;
}
void gb_cpu_state::ld_d_c()  //4   51
{
	d = c;
}
void gb_cpu_state::ld_d_d()  //4   52
{
	//d = d;
}
void gb_cpu_state::ld_d_e()  //4   53
{
	d = e;
}
void gb_cpu_state::ld_d_h()  //4   54
{
	d = h;
}
void gb_cpu_state::ld_d_l()  //4   55
{
	d = l;
}
void gb_cpu_state::ld_d_mhl() //8   56
{
	d = gb_mem->rb(mk_word(h,l));
}

//LD E, *
void gb_cpu_state::ld_e_a()  //4   5F
{
	e = a;	
}
void gb_cpu_state::ld_e_b()  //4   58
{
	e = b;
}
void gb_cpu_state::ld_e_c()  //4   59
{
	e = c;
}
void gb_cpu_state::ld_e_d()  //4   5A
{
	e = d;
}
void gb_cpu_state::ld_e_e()  //4   5B
{
	//e = e;
}
void gb_cpu_state::ld_e_h()  //4   5C
{
	e = h;
}
void gb_cpu_state::ld_e_l()  //4   5D
{
	e = l;
}
void gb_cpu_state::ld_e_mhl() //8   5E
{
	e = gb_mem->rb(mk_word(h,l));
}

//LD H, *
void gb_cpu_state::ld_h_a()  //4   67
{
	h = a;	
}
void gb_cpu_state::ld_h_b()  //4   60
{
	h = b;
}
void gb_cpu_state::ld_h_c()  //4   61
{
	h = c;
}
void gb_cpu_state::ld_h_d()  //4   62
{
	h = d;
}
void gb_cpu_state::ld_h_e()  //4   63
{
	h = e;
}
void gb_cpu_state::ld_h_h()  //4   64
{
	//h = h;
}
void gb_cpu_state::ld_h_l()  //4   65
{
	h = l;
}
void gb_cpu_state::ld_h_mhl() //8   66
{
	h = gb_mem->rb(mk_word(h,l));
}

//LD L, *
void gb_cpu_state::ld_l_a()  //4   6F
{
	l = a;
}
void gb_cpu_state::ld_l_b()  //4   68
{
	l = b;
}
void gb_cpu_state::ld_l_c()  //4   69
{
	l = c;
}
void gb_cpu_state::ld_l_d()  //4   6A
{
	l = d;
}
void gb_cpu_state::ld_l_e()  //4   6B
{
	l = e;
}
void gb_cpu_state::ld_l_h()  //4   6C
{
	l = h;
}
void gb_cpu_state::ld_l_l()  //4   6D
{
	//l = l;
}
void gb_cpu_state::ld_l_mhl() //8   6E
{
	l = gb_mem->rb(mk_word(h,l));
}
//LD BC, *
void gb_cpu_state::ld_mbc_a() //8   02
{
	gb_mem->wb(mk_word(b,c), a);
}

//LD DE, *
void gb_cpu_state::ld_mde_a() //8   12 
{
	gb_mem->wb(mk_word(d,e), a);	
}

//LD HL, *
void gb_cpu_state::ld_mhl_a() //8   77
{
	gb_mem->wb(mk_word(h,l), (unsigned char)a);
}
void gb_cpu_state::ld_mhl_b() //8   70
{
	gb_mem->wb(mk_word(h,l), (unsigned char)b);
}

void gb_cpu_state::ld_mhl_c() //8   71
{
	gb_mem->wb(mk_word(h,l), (unsigned char)c);
}

void gb_cpu_state::ld_mhl_d() //8   72
{
	gb_mem->wb(mk_word(h,l), (unsigned char)d);
}

void gb_cpu_state::ld_mhl_e() //8   73
{
	gb_mem->wb(mk_word(h,l), (unsigned char)e);
}

void gb_cpu_state::ld_mhl_h() //8   74
{
	gb_mem->wb(mk_word(h,l), (unsigned char)h);
}

void gb_cpu_state::ld_mhl_l() //8   75
{
	gb_mem->wb(mk_word(h,l), (unsigned char)l);
}

//LD *, R
void gb_cpu_state::ld_mm_a() //16  EA
{
	gb_mem->wb(mk_word(gb_mem->rb(pc+1),gb_mem->rb(pc)), a);
	pc += 2;
}

//LD A,(C)
void gb_cpu_state::ld_a_mc() //8   F2 
{
	a = gb_mem->rb(0xFF00+(unsigned char)c);
}
void gb_cpu_state::ld_mc_a() //8   E2
{
	gb_mem->wb(0xFF00+(unsigned char)c, a);
}
//LDD *, *
void gb_cpu_state::ldd_a_mhl()//8   3A
{
	a = gb_mem->rb(mk_word(h,l));
    dec_hl();
}

void gb_cpu_state::ldd_mhl_a()//8   32
{
	gb_mem->wb(mk_word(h,l), (unsigned char)a);
    dec_hl();
}

//LDI *, *

void gb_cpu_state::ldi_a_mhl()//8   2A
{
	a = gb_mem->rb(mk_word(h,l));
    inc_hl();
}

void gb_cpu_state::ldi_mhl_a()//8   22
{
	gb_mem->wb(mk_word(h,l), (unsigned)a);
    inc_hl();
}

//LDH
void gb_cpu_state::ldh_m_a()  //12  E0
{
	gb_mem->wb(0xFF00+(unsigned char)gb_mem->rb(pc), a);
	pc++;
}
void gb_cpu_state::ldh_a_m()  //12  F0
{
    a = gb_mem->rb(0xFF00+(unsigned char)gb_mem->rb(pc));
	pc++;
}

//16-Bit loads

void gb_cpu_state::ld_bc_mm() //12  01
{
	c = gb_mem->rb(pc);
	b = gb_mem->rb(pc+1);
    
	pc += 2;
}
void gb_cpu_state::ld_de_mm() //12  11
{
	e = gb_mem->rb(pc);
	d = gb_mem->rb(pc+1);
	pc += 2;
}
void gb_cpu_state::ld_hl_mm() //12  21
{
	l = gb_mem->rb(pc);
	h = gb_mem->rb(pc+1);
	pc += 2;
}
void gb_cpu_state::ld_sp_mm() //12  31
{
	sp = mk_word( gb_mem->rb(pc+1), gb_mem->rb(pc));
	pc += 2;
}

void gb_cpu_state::ld_sp_hl() //8   F9 
{
	sp = mk_word(h,l);
}


void gb_cpu_state::ldhl_sp_n() //12 F8 
{
    unsigned short tsp = sp;
    
    signed char n = gb_mem->rb(pc);
    pc++;
    
    
    int tmp = tsp ^ n ^ ((tsp+n)&0xffff);
    
    if (((tmp & 0x100) == 0x100))
        set_cf();
    else
        clear_cf();
    
    if (n >= 0) {
        if (((tmp & 0x10) == 0x10))
            set_hf();
        else
            clear_hf();
    } else  {
        if (((tmp & 0x10) == 0x10))
            set_hf();
        else
            clear_hf();
    }
    
    tsp = (tsp+n)%(1<<16);
    
	h = tsp/(1<<8);
	l = tsp%(1<<8);
    
   
	clear_zf();
	clear_nf();
}

void gb_cpu_state::ld_mm_sp() //20 08
{
	gb_mem->wb(mk_word(gb_mem->rb(pc+1), gb_mem->rb(pc)), sp%(1<<8));
	gb_mem->wb(mk_word(gb_mem->rb(pc+1), gb_mem->rb(pc))+1, sp/(1<<8));
	
	pc += 2;
}

//PUSH

void gb_cpu_state::push_af() //16 F5
{
	sp--;
	gb_mem->wb(sp, a);
	sp--;
    gb_mem->wb(sp, f);
}
void gb_cpu_state::push_bc() //16 C5
{
	sp--;
	gb_mem->wb(sp, b);
	sp--;
	gb_mem->wb(sp, c);
}
void gb_cpu_state::push_de() //16 D5

{
	sp--;
	gb_mem->wb(sp, d);
	sp--;
	gb_mem->wb(sp, e);
}
void gb_cpu_state::push_hl() //16 E5
{
	sp--;
	gb_mem->wb(sp, h);
	sp--;
	gb_mem->wb(sp, l);
}

//POP

void gb_cpu_state::pop_af() //12 F1
{
	f = gb_mem->rb(sp);
    f &= 0xF0;
	sp++;
	a = gb_mem->rb(sp);
	sp++;
}
void gb_cpu_state::pop_bc() //12 C1
{
	c = gb_mem->rb(sp);
	sp++;
	b = gb_mem->rb(sp);
	sp++;
}
void gb_cpu_state::pop_de() //12 D1
{
	e = gb_mem->rb(sp);
	sp++;
	d = gb_mem->rb(sp);
	sp++;
}
void gb_cpu_state::pop_hl() //12 E1
{
	l = gb_mem->rb(sp);
	sp++;
	h = gb_mem->rb(sp);
	sp++;
}

//ADD

void gb_cpu_state::add_to_a(unsigned short val)
{
	unsigned short sa = (a)&0xFF;
    
	if (sa+val > 255)
		set_cf();
	else
		clear_cf();
	
    if (sa%16+val%16 > 15)
		set_hf();
	else
		clear_hf();
	
	clear_nf();
	if ((sa+val)%(1<<8) == 0)
		set_zf();
	else
		clear_zf();
	
	a = (sa+val)%(1<<8);
}

void gb_cpu_state::add_a_a() //4  87
{
	add_to_a((unsigned short)a);
}
void gb_cpu_state::add_a_b() //4  80
{
	add_to_a((unsigned short)b);
}
void gb_cpu_state::add_a_c() //4  81
{
	add_to_a((unsigned short)c);
}
void gb_cpu_state::add_a_d() //4  82
{
	add_to_a((unsigned short)d);
}
void gb_cpu_state::add_a_e() //4  83
{
	add_to_a((unsigned short)e);
}
void gb_cpu_state::add_a_h() //4  84
{
	add_to_a((unsigned short)h);
}
void gb_cpu_state::add_a_l() //4  85
{
	add_to_a((unsigned short)l);
}
void gb_cpu_state::add_a_mhl() //8  86
{
	add_to_a((unsigned short)gb_mem->rb(mk_word(h, l)));
}
void gb_cpu_state::add_a_m()  //8  C6
{
	add_to_a((unsigned short)gb_mem->rb(pc));
	pc++;
}

void gb_cpu_state::adc_to_a(unsigned short val)
{
    unsigned short sa = (a)&0xFF;
    bool cf = cf_set();
    
    if (sa+val+cf > 255)
        set_cf();
    else
        clear_cf();
    
    
    if (sa%16+val%16+cf > 15)
        set_hf();
    else
        clear_hf();
    
    clear_nf();
    if ((sa+val+cf)%(1<<8) == 0)
        set_zf();
    else
        clear_zf();
    
    a = (sa+val+cf)%(1<<8);
}


void gb_cpu_state::adc_a_a() //4  8F
{
   
	adc_to_a((unsigned short)a);
}
void gb_cpu_state::adc_a_b() //4  88
{
	adc_to_a((unsigned short)b);
}
void gb_cpu_state::adc_a_c() //4  89
{
	adc_to_a((unsigned short)c);
}
void gb_cpu_state::adc_a_d() //4  8A
{
	adc_to_a((unsigned short)d);
}
void gb_cpu_state::adc_a_e() //4  8B
{
	adc_to_a((unsigned short)e);
}
void gb_cpu_state::adc_a_h() //4  8C
{
	adc_to_a((unsigned short)h);
}
void gb_cpu_state::adc_a_l() //4  8D
{
	adc_to_a((unsigned short)l);
}
void gb_cpu_state::adc_a_mhl() //8  8E
{
	adc_to_a((short)gb_mem->rb(mk_word(h, l)));
}
void gb_cpu_state::adc_a_m()  //8  CE
{
	adc_to_a((unsigned short)gb_mem->rb(pc));
	pc++;
}

//SUB

void gb_cpu_state::sub_from_a(unsigned short val) {
		short sa = a;
		if (sa-val < 0)
			set_cf();
		else
			clear_cf();
		
		if (sa%16-val%16 < 0)
			set_hf();
		else
			clear_hf();
		
		set_nf();
		if ((sa-val+(1<<8))%(1<<8) == 0)
			set_zf();
		else
			clear_zf();
		
		a = (sa-val+(1<<8))%(1<<8);
}

void gb_cpu_state::sub_a() //4  97 
{
	sub_from_a((unsigned short)a);
}
void gb_cpu_state::sub_b() //4  90
{
	sub_from_a((unsigned short)b);
}
void gb_cpu_state::sub_c() //4  91
{
	sub_from_a((unsigned short)c);
}
void gb_cpu_state::sub_d() //4  92
{
	sub_from_a((unsigned short)d);
}
void gb_cpu_state::sub_e() //4  93
{
	sub_from_a((unsigned short)e);
}
void gb_cpu_state::sub_h() //4  94
{
	sub_from_a((unsigned short)h);
}
void gb_cpu_state::sub_l() //4  95
{
	sub_from_a((unsigned short)l);
}
void gb_cpu_state::sub_mhl() //8  96
{
	sub_from_a((unsigned short)gb_mem->rb(mk_word(h, l)));
}
void gb_cpu_state::sub_m()   //8  D6
{
	sub_from_a((unsigned short)gb_mem->rb(pc));
	pc++;
}

//SBC
void gb_cpu_state::sbc_from_a(unsigned short val) {
    short sa = a;
    bool cf = cf_set();
    
    if (sa-val-cf < 0)
        set_cf();
    else
        clear_cf();
    
    if (sa%16-val%16-cf < 0)
        set_hf();
    else
        clear_hf();
    
    set_nf();
    if ((sa-val-cf+(1<<8))%(1<<8) == 0)
        set_zf();
    else
        clear_zf();
    
    a = (sa-val-cf+(1<<8))%(1<<8);
}

void gb_cpu_state::sbc_a() //4  9F
{
	sbc_from_a((unsigned short)a);
}
void gb_cpu_state::sbc_b() //4  98
{
	sbc_from_a((unsigned short)b);
}
void gb_cpu_state::sbc_c() //4  99
{
	sbc_from_a((unsigned short)c);
}
void gb_cpu_state::sbc_d() //4  9A
{
	sbc_from_a((unsigned short)d);
}
void gb_cpu_state::sbc_e() //4  9B
{
	sbc_from_a((unsigned short)e);
}
void gb_cpu_state::sbc_h() //4  9C
{
	sbc_from_a((unsigned short)h);
}
void gb_cpu_state::sbc_l() //4  9D
{
	sbc_from_a((unsigned short)l);
}
void gb_cpu_state::sbc_mhl() //8  9E
{
	sbc_from_a((unsigned short)gb_mem->rb(mk_word(h, l)));
}
void gb_cpu_state::sbc_m()  //8  DE
{
	sbc_from_a((unsigned short)gb_mem->rb(pc));
	pc++;
}

//LOGIC
//AND
void gb_cpu_state::and_with_a(short val) {
	short sa = a;
	
	if ((sa&val) == 0)
		set_zf();
	else
		clear_zf();
	
	clear_nf();
	set_hf();
	clear_cf();
	
	a = (sa&val)%(1<<8);
}

void gb_cpu_state::and_a()  //4  A7 
{
	and_with_a((short)a);
}
void gb_cpu_state::and_b()  //4  A0
{
	and_with_a((short)b);
}
void gb_cpu_state::and_c()  //4  A1
{
	and_with_a((short)c);
}
void gb_cpu_state::and_d()  //4  A2
{
	and_with_a((short)d);
}
void gb_cpu_state::and_e()  //4  A3
{
	and_with_a((short)e);
}
void gb_cpu_state::and_h()  //4  A4
{
	and_with_a((short)h);
}
void gb_cpu_state::and_l()  //4  A5
{
	and_with_a((short)l);
}
void gb_cpu_state::and_mhl()//8  A6
{
	and_with_a((short)gb_mem->rb(mk_word(h, l)));
}
void gb_cpu_state::and_m()  //8  E6
{
	and_with_a((short)gb_mem->rb(pc));
	pc++;
}

//OR
void gb_cpu_state::or_with_a(short val){
	short sa = a;
	
	if ((sa|val) == 0)
		set_zf();
	else
		clear_zf();
	
	clear_nf();
	clear_hf();
	clear_cf();
	
	a = (sa|val)%(1<<8);
}

void gb_cpu_state::or_a()  //4  B7
{
	or_with_a((short)a);	
}
void gb_cpu_state::or_b()  //4  B0
{
	or_with_a((short)b);	
}
void gb_cpu_state::or_c()  //4  B1
{
	or_with_a((short)c);	
}
void gb_cpu_state::or_d()  //4  B2
{
	or_with_a((short)d);	
}
void gb_cpu_state::or_e()  //4  B3
{
	or_with_a((short)e);	
}
void gb_cpu_state::or_h()  //4  B4
{
	or_with_a((short)h);	
}
void gb_cpu_state::or_l()  //4  B5
{
	or_with_a((short)l);	
}
void gb_cpu_state::or_mhl()//8  B6
{
	or_with_a((short)gb_mem->rb(mk_word(h, l)));	
}
void gb_cpu_state::or_m()  //8  F6
{
	or_with_a((short)gb_mem->rb(pc));
	pc++;
}

//XOR
void gb_cpu_state::xor_with_a(short val){
	short sa = a;
	
	if ((sa^val) == 0)
		set_zf();
	else
		clear_zf();
	
	clear_nf();
	clear_hf();
	clear_cf();
	
	a = (sa^val)%(1<<8);
}

void gb_cpu_state::xor_a()  //4  AF
{
	xor_with_a((short)a);
}
void gb_cpu_state::xor_b()  //4  A8
{
	xor_with_a((short)b);
}
void gb_cpu_state::xor_c()  //4  A9
{
	xor_with_a((short)c);
}
void gb_cpu_state::xor_d()  //4  AA
{
	xor_with_a((short)d);
}
void gb_cpu_state::xor_e()  //4  AB
{
	xor_with_a((short)e);
}
void gb_cpu_state::xor_h()  //4  AC
{
	xor_with_a((short)h);
}
void gb_cpu_state::xor_l()  //4  AD
{
	xor_with_a((short)l);
}
void gb_cpu_state::xor_mhl()//8  AE
{
	xor_with_a((short)gb_mem->rb(mk_word(h, l)));
}
void gb_cpu_state::xor_m()  //8  EE
{
	xor_with_a((short)gb_mem->rb(pc));
	pc++;
}

//CP

void gb_cpu_state::cp_with_a(short val) {
	unsigned char a_backup = a;
	sub_from_a(val);
	a = a_backup;
    
    
}

void gb_cpu_state::cp_a()  //4  BF
{
	cp_with_a((short)a);
}
void gb_cpu_state::cp_b()  //4  B8
{
	cp_with_a((short)b);
}
void gb_cpu_state::cp_c()  //4  B9
{
	cp_with_a((short)c);
}
void gb_cpu_state::cp_d()  //4  BA
{
	cp_with_a((short)d);
}
void gb_cpu_state::cp_e()  //4  BB
{
	cp_with_a((short)e);
}
void gb_cpu_state::cp_h()  //4  BC
{
	cp_with_a((short)h);
}
void gb_cpu_state::cp_l()  //4  BD
{
	cp_with_a((short)l);
}
void gb_cpu_state::cp_mhl()//8  BE
{
	cp_with_a((short)gb_mem->rb(mk_word(h, l)));
}
void gb_cpu_state::cp_m()  //8  FE
{
	cp_with_a((short)gb_mem->rb(pc));
	pc++;
}

//INC
void gb_cpu_state::inc_reg_8bit(unsigned char& reg) {
	if (reg%16 == 15)
		set_hf();
	else
		clear_hf();
	if ((reg+1)%256 == 0)
		set_zf();
	else
		clear_zf();
	clear_nf();
	reg = (reg+1l)%256;

}

void gb_cpu_state::inc_a()   //4 3C
{
	inc_reg_8bit(a);
}
void gb_cpu_state::inc_b()   //4 04
{
	inc_reg_8bit(b);
}
void gb_cpu_state::inc_c()   //4 0C
{
	inc_reg_8bit(c);
}
void gb_cpu_state::inc_d()   //4 14
{
	inc_reg_8bit(d);
}
void gb_cpu_state::inc_e()   //4 1C
{
	inc_reg_8bit(e);
}
void gb_cpu_state::inc_h()   //4 24
{
	inc_reg_8bit(h);
}
void gb_cpu_state::inc_l()   //4 2C
{
	inc_reg_8bit(l);
}
void gb_cpu_state::inc_mhl() //12 34
{
	unsigned char reg = gb_mem->rb(mk_word(h,l));
	
    inc_reg_8bit(reg);
    
    
	gb_mem->wb(mk_word(h,l), reg);
}

//DEC
void gb_cpu_state::dec_reg_8bit(unsigned char& reg) {
	if (reg%16 == 0)
		set_hf();
	else
		clear_hf();
	if ((reg-1+256)%256 == 0)
		set_zf();
	else
		clear_zf();
	set_nf();
	reg = (256ll+reg-1)%256;
}

void gb_cpu_state::dec_a()  //4 3D
{
	dec_reg_8bit(a);
}
void gb_cpu_state::dec_b()   //4 05
{
	dec_reg_8bit(b);
    
}
void gb_cpu_state::dec_c()   //4 0D
{
	dec_reg_8bit(c);
}
void gb_cpu_state::dec_d()   //4 15
{
	dec_reg_8bit(d);
}
void gb_cpu_state::dec_e()   //4 1D
{
	dec_reg_8bit(e);
}
void gb_cpu_state::dec_h()   //4 25
{
	dec_reg_8bit(h);
}
void gb_cpu_state::dec_l()   //4 2D
{
	dec_reg_8bit(l);
}
void gb_cpu_state::dec_mhl() //12 35
{
	unsigned char reg = gb_mem->rb(mk_word(h,l));
    
	dec_reg_8bit(reg);
	
	gb_mem->wb(mk_word(h,l), reg);
    
}

//16-Bit ALU
//ADD

void gb_cpu_state::add_to_hl(unsigned short val) {
	clear_nf();
	
	unsigned short shl = mk_word(h, l);
	
	if ((0ll+shl+val)>=(1<<16))
		set_cf();
	else
		clear_cf();
	
	if (((shl)%(1<<12)+(val)%(1<<12))>=(1<<12))
		set_hf();
	else
		clear_hf();
    
	shl = (0ll+shl+val)%(1<<16);
	h = (unsigned char)(shl>>8);
	l = (unsigned char)(shl%(1<<8));
}

void gb_cpu_state::add_hl_bc() //8 09
{
	unsigned short sbc = mk_word(b, c);
	add_to_hl(sbc);
}
void gb_cpu_state::add_hl_de() //8 19
{
	unsigned short sde = mk_word(d, e);
	add_to_hl(sde);
}
void gb_cpu_state::add_hl_hl() //8 29
{
	unsigned short shl = mk_word(h, l);
	add_to_hl(shl);
}
void gb_cpu_state::add_hl_sp() //8 39
{
	add_to_hl(sp);
}

void gb_cpu_state::add_sp_m()  //16 E8 
{
	signed char n = gb_mem->rb(pc);
	pc++;
	
    
    int tmp = sp ^ n ^ ((sp+n)&0xffff);

    if (((tmp & 0x100) == 0x100))
        set_cf();
	else
		clear_cf();
    
    if (n >= 0) {
        if (((tmp & 0x10) == 0x10))
            set_hf();
        else
            clear_hf();
    } else  {
        if (((tmp & 0x10) == 0x10))
            set_hf();
        else
            clear_hf();
    }

	sp = (sp+n)%(1<<16);
    
    
    clear_zf();
    clear_nf();
}

void gb_cpu_state::inc_bc()   //8  03
{
	unsigned short sbc = mk_word(b, c);
	
	sbc = (sbc+1ll)%(1<<16);
	
	b = (unsigned char)(sbc>>8);
	c = (unsigned char)(sbc%(1<<8));
}
void gb_cpu_state::inc_de()   //8  13
{
	unsigned short sde = mk_word(d, e);
	
	sde = (sde+1ll)%(1<<16);
	
	d = (unsigned char)(sde>>8);
	e = (unsigned char)(sde%(1<<8));
}
void gb_cpu_state::inc_hl()   //8  23
{
	unsigned short shl = mk_word(h, l);
	
	shl = (shl+1ll)%(1<<16);
	
	h = (unsigned char)(shl>>8);
	l = (unsigned char)(shl%(1<<8));
}
void gb_cpu_state::inc_sp()   //8  33
{
	sp = (sp+1ll)%(1<<16);
}

void gb_cpu_state::dec_bc()   //8  0B
{
	unsigned short sbc = mk_word(b, c);
	
	sbc = (sbc-1ll+(1<<16))%(1<<16);
	
	b = (unsigned char)(sbc>>8);
	c = (unsigned char)(sbc%(1<<8));
}
void gb_cpu_state::dec_de()   //8  1B
{
	unsigned short sde = mk_word(d, e);
	
	sde = (sde-1ll+(1<<16))%(1<<16);
	
	d = (unsigned char)(sde>>8);
	e = (unsigned char)(sde%(1<<8));
}
void gb_cpu_state::dec_hl()   //8  2B
{
	unsigned short shl = mk_word(h, l);
	
	shl = (shl-1ll+(1<<16))%(1<<16);
	
	h = (unsigned char)(shl>>8);
	l = (unsigned char)(shl%(1<<8));
}
void gb_cpu_state::dec_sp()   //8  3B
{
	sp = (sp-1ll+(1<<16))%(1<<16);
}

//SWAP
void gb_cpu_state::swap_reg(unsigned char& reg) {
    unsigned char low = reg%(1<<4), high=reg/(1<<4);
    
    reg = (low<<4)+high;
    
    if (reg == 0)
        set_zf();
    else
        clear_zf();
    clear_nf();
    clear_hf();
    clear_cf();
}

void gb_cpu_state::swap_a() //8 CB 37
{
    swap_reg(a);
}
void gb_cpu_state::swap_b() //8 CB 30
{
    swap_reg(b);
}
void gb_cpu_state::swap_c() //8 CB 31
{
    swap_reg(c);
}
void gb_cpu_state::swap_d() //8 CB 32
{
    swap_reg(d);
}
void gb_cpu_state::swap_e() //8 CB 33
{
    swap_reg(e);
}
void gb_cpu_state::swap_h() //8 CB 34
{
    swap_reg(h);
}
void gb_cpu_state::swap_l() //8 CB 35
{
    swap_reg(l);
}
void gb_cpu_state::swap_mhl() //16 CB 36
{
    unsigned char temp = gb_mem->rb(mk_word(h,l));
    swap_reg(temp);
    gb_mem->wb(mk_word(h,l), temp);
}

//DAA

//DAA copied from DKParott
void gb_cpu_state::daa() //4 27
{
  
    
    int sa = a;
    
    if (!nf_set())
    {
        if (hf_set() || (sa & 0xF) > 9)
            sa += 0x06;
        
        if (cf_set() || sa > 0x9F)
            sa += 0x60;
    }
    else
    {
        if (hf_set())
            sa = (sa - 6) & 0xFF;
        
        if (cf_set())
            sa -= 0x60;
    }
    
    clear_hf();
    clear_zf();
    
    if ((sa & 0x100) == 0x100)
        set_cf();
    
    sa &= 0xFF;
    
    if (sa == 0)
        set_zf();
    
    a = (unsigned char)sa;

}

void gb_cpu_state::cpl() //4 2F
{
    a = ~a;
    set_nf();
    set_hf();
}
void gb_cpu_state::ccf() //4 3F
{
    if (!cf_set()) set_cf();
    else clear_cf();
    clear_nf();
    clear_hf();
}
void gb_cpu_state::scf() //4 37
{
    set_cf();
    clear_nf();
    clear_hf();
}

void gb_cpu_state::nop()  //4 00
{
    
}
void gb_cpu_state::halt() //4 76
{
    halted = 1;
}
void gb_cpu_state::stop() //4 10 00
{
    halted = 1;
}

void gb_cpu_state::di()  //4 F3
{
    //next_ie = 0;
    ime = 0;
}
void gb_cpu_state::ei()  //4 FB
{
    //next_ie = 1;
    ime = 1;
}


//ROTATES
void gb_cpu_state::rlca() //4 07
{
    rlc(a);
    
    clear_zf();

}
void gb_cpu_state::rla()  //4 17
{
    rl(a);
    
    clear_zf();

}
void gb_cpu_state::rrca() //4 0F
{
    rrc(a);
    clear_zf();
}
void gb_cpu_state::rra()  //4 1F
{
    rr(a);
    
    clear_zf();

}


void gb_cpu_state::rlc(unsigned char &reg) {
    unsigned char bit7 = !!(reg&(1<<7));
    if (bit7)
        set_cf();
    else
        clear_cf();
    reg = ((reg<<1)+bit7)%(256);
    
    clear_hf();
    clear_nf();
    if (reg != 0)
        clear_zf();
    else
        set_zf();
}

void gb_cpu_state::rlc_a() //8 CB 07
{
    rlc(a);
}
void gb_cpu_state::rlc_b() //8 CB 00
{
    rlc(b);
}
void gb_cpu_state::rlc_c() //8 CB 01
{
    rlc(c);
}
void gb_cpu_state::rlc_d() //8 CB 02
{
    rlc(d);
}
void gb_cpu_state::rlc_e() //8 CB 03
{
    rlc(e);
}
void gb_cpu_state::rlc_h() //8 CB 04
{
    rlc(h);
}
void gb_cpu_state::rlc_l() //8 CB 05
{
    rlc(l);
}
void gb_cpu_state::rlc_mhl() //16 CB 06
{
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    rlc(r);
    gb_mem->wb(mk_word(h, l), r);
}

void gb_cpu_state::rl(unsigned char &reg)
{
    unsigned char bit7 = !!(reg&(1<<7));
    unsigned char cf = cf_set();
    
    if (bit7)
        set_cf();
    else
        clear_cf();
    reg = ((reg<<1)+cf)%(256);
    
    clear_hf();
    clear_nf();
    if (reg != 0)
        clear_zf();
    else
        set_zf();
}

void gb_cpu_state::rl_a() //8 CB 17
{
    rl(a);
}
void gb_cpu_state::rl_b() //8 CB 10
{
    rl(b);
}
void gb_cpu_state::rl_c() //8 CB 11
{
    rl(c);
}
void gb_cpu_state::rl_d() //8 CB 12
{
    rl(d);
}
void gb_cpu_state::rl_e() //8 CB 13
{
    rl(e);
}
void gb_cpu_state::rl_h() //8 CB 14
{
    rl(h);
}
void gb_cpu_state::rl_l() //8 CB 15
{
    rl(l);
}
void gb_cpu_state::rl_mhl() //16 CB 16
{
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    rl(r);
    gb_mem->wb(mk_word(h, l), r);
}

void gb_cpu_state::rrc(unsigned char &reg)
{
    unsigned char bit0 = reg%2;
    
    if (bit0)
        set_cf();
    else
        clear_cf();
    reg = ((reg>>1)+(bit0<<7))%(256);
    clear_hf();
    clear_nf();
    if (reg != 0)
        clear_zf();
    else
        set_zf();
    
}

void gb_cpu_state::rrc_a() //8 CB 0F
{
    rrc(a);
}
void gb_cpu_state::rrc_b() //8 CB 08
{
    rrc(b);
}
void gb_cpu_state::rrc_c() //8 CB 09
{
    rrc(c);
}
void gb_cpu_state::rrc_d() //8 CB 0A
{
    rrc(d);
}
void gb_cpu_state::rrc_e() //8 CB 0B
{
    rrc(e);
}
void gb_cpu_state::rrc_h() //8 CB 0C
{
    rrc(h);
}
void gb_cpu_state::rrc_l() //8 CB 0D
{
    rrc(l);
}
void gb_cpu_state::rrc_mhl() //16 CB 0E
{
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    rrc(r);
    gb_mem->wb(mk_word(h, l), r);
}

void gb_cpu_state::rr(unsigned char &reg)
{
    
    unsigned char bit0 = !!(reg&(1<<0));
    unsigned char cf = cf_set();
    
    if (bit0)
        set_cf();
    else
        clear_cf();
    reg = ((reg>>1)+(cf<<7))%(256);
    
    clear_hf();
    clear_nf();
    if (reg != 0)
        clear_zf();
    else
        set_zf();
    
}

void gb_cpu_state::rr_a() //8 CB 1F
{
    rr(a);
}
void gb_cpu_state::rr_b() //8 CB 18
{
    rr(b);
}
void gb_cpu_state::rr_c() //8 CB 19
{
    rr(c);
}
void gb_cpu_state::rr_d() //8 CB 1A
{
    rr(d);
}
void gb_cpu_state::rr_e() //8 CB 1B
{
    rr(e);
}
void gb_cpu_state::rr_h() //8 CB 1C
{
    rr(h);
}
void gb_cpu_state::rr_l() //8 CB 1D
{
    rr(l);
}
void gb_cpu_state::rr_mhl() //16 CB 1E
{
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    rr(r);
    gb_mem->wb(mk_word(h, l), r);
}


//SHIFTS

void gb_cpu_state::sla(unsigned char &reg)
{
    if ((reg >> 7) == 1)
        set_cf();
    else
        clear_cf();
    reg <<= 1;
    
    clear_hf();
    clear_nf();
    if (reg != 0)
        clear_zf();
    else
        set_zf();
}

void gb_cpu_state::sla_a() //8 CB 27
{
    sla(a);
}
void gb_cpu_state::sla_b() //8 CB 20
{
    sla(b);
}
void gb_cpu_state::sla_c() //8 CB 21
{
    sla(c);
}
void gb_cpu_state::sla_d() //8 CB 22
{
    sla(d);
}
void gb_cpu_state::sla_e() //8 CB 23
{
    sla(e);
}
void gb_cpu_state::sla_h() //8 CB 24
{
    sla(h);
}
void gb_cpu_state::sla_l() //8 CB 25
{
    sla(l);
}
void gb_cpu_state::sla_mhl() //16 CB 26
{
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    sla(r);
    gb_mem->wb(mk_word(h, l), r);
}

void gb_cpu_state::sra(unsigned char &reg)
{
    if (reg%2 == 1)
        set_cf();
    else
        clear_cf();
    reg = ((reg>>7)<<7)+(reg>>1);
    
    clear_hf();
    clear_nf();
    if (reg != 0)
        clear_zf();
    else
        set_zf();
}

void gb_cpu_state::sra_a() //8 CB 2F
{
    sra(a);
}
void gb_cpu_state::sra_b() //8 CB 28
{
    sra(b);
}
void gb_cpu_state::sra_c() //8 CB 29
{
    sra(c);
}
void gb_cpu_state::sra_d() //8 CB 2A
{
    sra(d);
}
void gb_cpu_state::sra_e() //8 CB 2B
{
    sra(e);
}
void gb_cpu_state::sra_h() //8 CB 2C
{
    sra(h);
}
void gb_cpu_state::sra_l() //8 CB 2D
{
    sra(l);
}
void gb_cpu_state::sra_mhl() //16 CB 2E
{
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    sra(r);
    gb_mem->wb(mk_word(h, l), r);
}


void gb_cpu_state::srl(unsigned char &reg)
{
    if (reg%2 == 1)
        set_cf();
    else
        clear_cf();
    reg >>= 1;
    
    clear_hf();
    clear_nf();
    if (reg != 0)
        clear_zf();
    else
        set_zf();
}

void gb_cpu_state::srl_a() //8 CB 3F
{
    srl(a);
}
void gb_cpu_state::srl_b() //8 CB 38
{
    srl(b);
}
void gb_cpu_state::srl_c() //8 CB 39
{
    srl(c);
}
void gb_cpu_state::srl_d() //8 CB 3A
{
    srl(d);
}
void gb_cpu_state::srl_e() //8 CB 3B
{
    srl(e);
}
void gb_cpu_state::srl_h() //8 CB 3C
{
    srl(h);
}
void gb_cpu_state::srl_l() //8 CB 3D
{
    srl(l);
}
void gb_cpu_state::srl_mhl() //16 CB 3E
{
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    srl(r);
    gb_mem->wb(mk_word(h, l), r);
}

//BIT

void gb_cpu_state::bit(unsigned char& reg, unsigned char b) {
    if (reg & (1<<b)) {
        clear_zf();
    }
    else
        set_zf();
    
    clear_nf();
    set_hf();
}

void gb_cpu_state::bit_0_a() { //8 CB 47
    bit(a, 0);
}
void gb_cpu_state::bit_1_a() { //8 CB 4f
    bit(a, 1);
}
void gb_cpu_state::bit_2_a() { //8 CB 57
    bit(a, 2);
}
void gb_cpu_state::bit_3_a() { //8 CB 5f
    bit(a, 3);
}
void gb_cpu_state::bit_4_a() { //8 CB 67
    bit(a, 4);
}
void gb_cpu_state::bit_5_a() { //8 CB 6f
    bit(a, 5);
}
void gb_cpu_state::bit_6_a() { //8 CB 77
    bit(a, 6);
}
void gb_cpu_state::bit_7_a() { //8 CB 7f
    bit(a, 7);
}

void gb_cpu_state::bit_0_b() { //8 CB 40
    bit(b, 0);
}
void gb_cpu_state::bit_1_b() { //8 CB 48
    bit(b, 1);
}
void gb_cpu_state::bit_2_b() { //8 CB 50
    bit(b, 2);
}
void gb_cpu_state::bit_3_b() { //8 CB 58
    bit(b, 3);
}
void gb_cpu_state::bit_4_b() { //8 CB 60
    bit(b, 4);
}
void gb_cpu_state::bit_5_b() { //8 CB 68
    bit(b, 5);
}
void gb_cpu_state::bit_6_b() { //8 CB 70
    bit(b, 6);
}
void gb_cpu_state::bit_7_b() { //8 CB 78
    bit(b, 7);
}

void gb_cpu_state::bit_0_c() { //8 CB 41
    bit(c, 0);
}
void gb_cpu_state::bit_1_c() { //8 CB 49
    bit(c, 1);
}
void gb_cpu_state::bit_2_c() { //8 CB 51
    bit(c, 2);
}
void gb_cpu_state::bit_3_c() { //8 CB 59
    bit(c, 3);
}
void gb_cpu_state::bit_4_c() { //8 CB 61
    bit(c, 4);
}
void gb_cpu_state::bit_5_c() { //8 CB 69
    bit(c, 5);
}
void gb_cpu_state::bit_6_c() { //8 CB 71
    bit(c, 6);
}
void gb_cpu_state::bit_7_c() { //8 CB 79
    bit(c, 7);
}

void gb_cpu_state::bit_0_d() { //8 CB 42
    bit(d, 0);
}
void gb_cpu_state::bit_1_d() { //8 CB 4a
    bit(d, 1);
}
void gb_cpu_state::bit_2_d() { //8 CB 52
    bit(d, 2);
}
void gb_cpu_state::bit_3_d() { //8 CB 5a
    bit(d, 3);
}
void gb_cpu_state::bit_4_d() { //8 CB 62
    bit(d, 4);
}
void gb_cpu_state::bit_5_d() { //8 CB 6a
    bit(d, 5);
}
void gb_cpu_state::bit_6_d() { //8 CB 72
    bit(d, 6);
}
void gb_cpu_state::bit_7_d() { //8 CB 7a
    bit(d, 7);
}

void gb_cpu_state::bit_0_e() { //8 CB 43
    bit(e, 0);
}
void gb_cpu_state::bit_1_e() { //8 CB 4b
    bit(e, 1);
}
void gb_cpu_state::bit_2_e() { //8 CB 53
    bit(e, 2);
}
void gb_cpu_state::bit_3_e() { //8 CB 5b
    bit(e, 3);
}
void gb_cpu_state::bit_4_e() { //8 CB 63
    bit(e, 4);
}
void gb_cpu_state::bit_5_e() { //8 CB 6b
    bit(e, 5);
}
void gb_cpu_state::bit_6_e() { //8 CB 73
    bit(e, 6);
}
void gb_cpu_state::bit_7_e() { //8 CB 7b
    bit(e, 7);
}

void gb_cpu_state::bit_0_h() { //8 CB 44
    bit(h, 0);
}
void gb_cpu_state::bit_1_h() { //8 CB 4c
    bit(h, 1);
}
void gb_cpu_state::bit_2_h() { //8 CB 54
    bit(h, 2);
}
void gb_cpu_state::bit_3_h() { //8 CB 5c
    bit(h, 3);
}
void gb_cpu_state::bit_4_h() { //8 CB 64
    bit(h, 4);
}
void gb_cpu_state::bit_5_h() { //8 CB 6c
    bit(h, 5);
}
void gb_cpu_state::bit_6_h() { //8 CB 74
    bit(h, 6);
}
void gb_cpu_state::bit_7_h() { //8 CB 7c
    bit(h, 7);
}

void gb_cpu_state::bit_0_l() { //8 CB 45
    bit(l, 0);
}
void gb_cpu_state::bit_1_l() { //8 CB 4d
    bit(l, 1);
}
void gb_cpu_state::bit_2_l() { //8 CB 55
    bit(l, 2);
}
void gb_cpu_state::bit_3_l() { //8 CB 5d
    bit(l, 3);
}
void gb_cpu_state::bit_4_l() { //8 CB 65
    bit(l, 4);
}
void gb_cpu_state::bit_5_l() { //8 CB 6d
    bit(l, 5);
}
void gb_cpu_state::bit_6_l() { //8 CB 75
    bit(l, 6);
}
void gb_cpu_state::bit_7_l() { //8 CB 7d
    bit(l, 7);
}

void gb_cpu_state::bit_0_mhl() { //8 CB 46
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    bit(r, 0);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::bit_1_mhl() { //8 CB 4e
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    bit(r, 1);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::bit_2_mhl() { //8 CB 56
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    bit(r, 2);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::bit_3_mhl() { //8 CB 5e
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    bit(r, 3);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::bit_4_mhl() { //8 CB 66
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    bit(r, 4);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::bit_5_mhl() { //8 CB 6e
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    bit(r, 5);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::bit_6_mhl() { //8 CB 76
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    bit(r, 6);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::bit_7_mhl() { //8 CB 7e
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    bit(r, 7);
    gb_mem->wb(mk_word(h, l), r);
}

//SET

void gb_cpu_state::set(unsigned char& reg, unsigned char b) {
    reg = reg | (1<<b);
}

void gb_cpu_state::set_0_a() { //8 CB c7
    set(a, 0);
}
void gb_cpu_state::set_1_a() { //8 CB cf
    set(a, 1);
}
void gb_cpu_state::set_2_a() { //8 CB d7
    set(a, 2);
}
void gb_cpu_state::set_3_a() { //8 CB df
    set(a, 3);
}
void gb_cpu_state::set_4_a() { //8 CB e7
    set(a, 4);
}
void gb_cpu_state::set_5_a() { //8 CB ef
    set(a, 5);
}
void gb_cpu_state::set_6_a() { //8 CB f7
    set(a, 6);
}
void gb_cpu_state::set_7_a() { //8 CB ff
    set(a, 7);
}

void gb_cpu_state::set_0_b() { //8 CB c0
    set(b, 0);
}
void gb_cpu_state::set_1_b() { //8 CB c8
    set(b, 1);
}
void gb_cpu_state::set_2_b() { //8 CB d0
    set(b, 2);
}
void gb_cpu_state::set_3_b() { //8 CB d8
    set(b, 3);
}
void gb_cpu_state::set_4_b() { //8 CB e0
    set(b, 4);
}
void gb_cpu_state::set_5_b() { //8 CB e8
    set(b, 5);
}
void gb_cpu_state::set_6_b() { //8 CB f0
    set(b, 6);
}
void gb_cpu_state::set_7_b() { //8 CB f8
    set(b, 7);
}

void gb_cpu_state::set_0_c() { //8 CB c1
    set(c, 0);
}
void gb_cpu_state::set_1_c() { //8 CB c9
    set(c, 1);
}
void gb_cpu_state::set_2_c() { //8 CB d1
    set(c, 2);
}
void gb_cpu_state::set_3_c() { //8 CB d9
    set(c, 3);
}
void gb_cpu_state::set_4_c() { //8 CB e1
    set(c, 4);
}
void gb_cpu_state::set_5_c() { //8 CB e9
    set(c, 5);
}
void gb_cpu_state::set_6_c() { //8 CB f1
    set(c, 6);
}
void gb_cpu_state::set_7_c() { //8 CB f9
    set(c, 7);
}

void gb_cpu_state::set_0_d() { //8 CB c2
    set(d, 0);
}
void gb_cpu_state::set_1_d() { //8 CB ca
    set(d, 1);
}
void gb_cpu_state::set_2_d() { //8 CB d2
    set(d, 2);
}
void gb_cpu_state::set_3_d() { //8 CB da
    set(d, 3);
}
void gb_cpu_state::set_4_d() { //8 CB e2
    set(d, 4);
}
void gb_cpu_state::set_5_d() { //8 CB ea
    set(d, 5);
}
void gb_cpu_state::set_6_d() { //8 CB f2
    set(d, 6);
}
void gb_cpu_state::set_7_d() { //8 CB fa
    set(d, 7);
}

void gb_cpu_state::set_0_e() { //8 CB c3
    set(e, 0);
}
void gb_cpu_state::set_1_e() { //8 CB cb
    set(e, 1);
}
void gb_cpu_state::set_2_e() { //8 CB d3
    set(e, 2);
}
void gb_cpu_state::set_3_e() { //8 CB db
    set(e, 3);
}
void gb_cpu_state::set_4_e() { //8 CB e3
    set(e, 4);
}
void gb_cpu_state::set_5_e() { //8 CB eb
    set(e, 5);
}
void gb_cpu_state::set_6_e() { //8 CB f3
    set(e, 6);
}
void gb_cpu_state::set_7_e() { //8 CB fb
    set(e, 7);
}

void gb_cpu_state::set_0_h() { //8 CB c4
    set(h, 0);
}
void gb_cpu_state::set_1_h() { //8 CB cc
    set(h, 1);
}
void gb_cpu_state::set_2_h() { //8 CB d4
    set(h, 2);
}
void gb_cpu_state::set_3_h() { //8 CB dc
    set(h, 3);
}
void gb_cpu_state::set_4_h() { //8 CB e4
    set(h, 4);
}
void gb_cpu_state::set_5_h() { //8 CB ec
    set(h, 5);
}
void gb_cpu_state::set_6_h() { //8 CB f4
    set(h, 6);
}
void gb_cpu_state::set_7_h() { //8 CB fc
    set(h, 7);
}

void gb_cpu_state::set_0_l() { //8 CB c5
    set(l, 0);
}
void gb_cpu_state::set_1_l() { //8 CB cd
    set(l, 1);
}
void gb_cpu_state::set_2_l() { //8 CB d5
    set(l, 2);
}
void gb_cpu_state::set_3_l() { //8 CB dd
    set(l, 3);
}
void gb_cpu_state::set_4_l() { //8 CB e5
    set(l, 4);
}
void gb_cpu_state::set_5_l() { //8 CB ed
    set(l, 5);
}
void gb_cpu_state::set_6_l() { //8 CB f5
    set(l, 6);
}
void gb_cpu_state::set_7_l() { //8 CB fd
    set(l, 7);
}

void gb_cpu_state::set_0_mhl() { //16 CB c6
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    set(r, 0);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::set_1_mhl() { //16 CB ce
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    set(r, 1);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::set_2_mhl() { //16 CB d6
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    set(r, 2);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::set_3_mhl() { //16 CB de
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    set(r, 3);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::set_4_mhl() { //16 CB e6
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    set(r, 4);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::set_5_mhl() { //16 CB ee
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    set(r, 5);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::set_6_mhl() { //16 CB f6
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    set(r, 6);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::set_7_mhl() { //16 CB fe
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    set(r, 7);
    gb_mem->wb(mk_word(h, l), r);
}



//RES

void gb_cpu_state::res(unsigned char& reg, unsigned char b) {
    reg = reg & (((1<<8)-1-(1<<b)));
}

void gb_cpu_state::res_0_a() { //8 CB 87
    res(a, 0);
}
void gb_cpu_state::res_1_a() { //8 CB 8f
    res(a, 1);
}
void gb_cpu_state::res_2_a() { //8 CB 97
    res(a, 2);
}
void gb_cpu_state::res_3_a() { //8 CB 9f
    res(a, 3);
}
void gb_cpu_state::res_4_a() { //8 CB a7
    res(a, 4);
}
void gb_cpu_state::res_5_a() { //8 CB af
    res(a, 5);
}
void gb_cpu_state::res_6_a() { //8 CB b7
    res(a, 6);
}
void gb_cpu_state::res_7_a() { //8 CB bf
    res(a, 7);
}

void gb_cpu_state::res_0_b() { //8 CB 80
    res(b, 0);
}
void gb_cpu_state::res_1_b() { //8 CB 88
    res(b, 1);
}
void gb_cpu_state::res_2_b() { //8 CB 90
    res(b, 2);
}
void gb_cpu_state::res_3_b() { //8 CB 98
    res(b, 3);
}
void gb_cpu_state::res_4_b() { //8 CB a0
    res(b, 4);
}
void gb_cpu_state::res_5_b() { //8 CB a8
    res(b, 5);
}
void gb_cpu_state::res_6_b() { //8 CB b0
    res(b, 6);
}
void gb_cpu_state::res_7_b() { //8 CB b8
    res(b, 7);
}

void gb_cpu_state::res_0_c() { //8 CB 81
    res(c, 0);
}
void gb_cpu_state::res_1_c() { //8 CB 89
    res(c, 1);
}
void gb_cpu_state::res_2_c() { //8 CB 91
    res(c, 2);
}
void gb_cpu_state::res_3_c() { //8 CB 99
    res(c, 3);
}
void gb_cpu_state::res_4_c() { //8 CB a1
    res(c, 4);
}
void gb_cpu_state::res_5_c() { //8 CB a9
    res(c, 5);
}
void gb_cpu_state::res_6_c() { //8 CB b1
    res(c, 6);
}
void gb_cpu_state::res_7_c() { //8 CB b9
    res(c, 7);
}

void gb_cpu_state::res_0_d() { //8 CB 82
    res(d, 0);
}
void gb_cpu_state::res_1_d() { //8 CB 8a
    res(d, 1);
}
void gb_cpu_state::res_2_d() { //8 CB 92
    res(d, 2);
}
void gb_cpu_state::res_3_d() { //8 CB 9a
    res(d, 3);
}
void gb_cpu_state::res_4_d() { //8 CB a2
    res(d, 4);
}
void gb_cpu_state::res_5_d() { //8 CB aa
    res(d, 5);
}
void gb_cpu_state::res_6_d() { //8 CB b2
    res(d, 6);
}
void gb_cpu_state::res_7_d() { //8 CB ba
    res(d, 7);
}

void gb_cpu_state::res_0_e() { //8 CB 83
    res(e, 0);
}
void gb_cpu_state::res_1_e() { //8 CB 8b
    res(e, 1);
}
void gb_cpu_state::res_2_e() { //8 CB 93
    res(e, 2);
}
void gb_cpu_state::res_3_e() { //8 CB 9b
    res(e, 3);
}
void gb_cpu_state::res_4_e() { //8 CB a3
    res(e, 4);
}
void gb_cpu_state::res_5_e() { //8 CB ab
    res(e, 5);
}
void gb_cpu_state::res_6_e() { //8 CB b3
    res(e, 6);
}
void gb_cpu_state::res_7_e() { //8 CB bb
    res(e, 7);
}

void gb_cpu_state::res_0_h() { //8 CB 84
    res(h, 0);
}
void gb_cpu_state::res_1_h() { //8 CB 8c
    res(h, 1);
}
void gb_cpu_state::res_2_h() { //8 CB 94
    res(h, 2);
}
void gb_cpu_state::res_3_h() { //8 CB 9c
    res(h, 3);
}
void gb_cpu_state::res_4_h() { //8 CB a4
    res(h, 4);
}
void gb_cpu_state::res_5_h() { //8 CB ac
    res(h, 5);
}
void gb_cpu_state::res_6_h() { //8 CB b4
    res(h, 6);
}
void gb_cpu_state::res_7_h() { //8 CB bc
    res(h, 7);
}

void gb_cpu_state::res_0_l() { //8 CB 85
    res(l, 0);
}
void gb_cpu_state::res_1_l() { //8 CB 8d
    res(l, 1);
}
void gb_cpu_state::res_2_l() { //8 CB 95
    res(l, 2);
}
void gb_cpu_state::res_3_l() { //8 CB 9d
    res(l, 3);
}
void gb_cpu_state::res_4_l() { //8 CB a5
    res(l, 4);
}
void gb_cpu_state::res_5_l() { //8 CB ad
    res(l, 5);
}
void gb_cpu_state::res_6_l() { //8 CB b5
    res(l, 6);
}
void gb_cpu_state::res_7_l() { //8 CB bd
    res(l, 7);
}

void gb_cpu_state::res_0_mhl() { //16 CB 86
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    res(r, 0);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::res_1_mhl() { //16 CB 8e
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    res(r, 1);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::res_2_mhl() { //16 CB 96
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    res(r, 2);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::res_3_mhl() { //16 CB 9e
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    res(r, 3);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::res_4_mhl() { //16 CB a6
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    res(r, 4);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::res_5_mhl() { //16 CB ae
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    res(r, 5);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::res_6_mhl() { //16 CB b6
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    res(r, 6);
    gb_mem->wb(mk_word(h, l), r);
}
void gb_cpu_state::res_7_mhl() { //16 CB be
    unsigned char r;
    r = gb_mem->rb(mk_word(h, l));
    res(r, 7);
    gb_mem->wb(mk_word(h, l), r);
}

//JUMPS

void gb_cpu_state::jp_mm()  //12 C3
{
    unsigned short addr = mk_word(gb_mem->rb(pc+1), gb_mem->rb(pc));
    pc = addr;
}

void gb_cpu_state::jp_nz()  //12 C2
{
    if (!zf_set()) {
        t += 4;
        m += 1;
        jp_mm();
    }
    
    else
        pc += 2;

}
void gb_cpu_state::jp_z()   //12 CA
{
    if (zf_set()){
        t += 4;
        m += 1;
        jp_mm();
    }

    
    else
        pc += 2;

}
void gb_cpu_state::jp_nc()  //12 D2
{
    if (!cf_set()){
        t += 4;
        m += 1;
        jp_mm();
    }

    else
        pc += 2;

}
void gb_cpu_state::jp_c()   //12 DA
{
    if (cf_set()){
        t += 4;
        m += 1;
        jp_mm();
    }

    else
        pc += 2;

}

void gb_cpu_state::jp_mhl() //4  E9
{
    unsigned addr = mk_word(h, l);
    pc = addr;
}

void gb_cpu_state::jr_m()   //8  18
{
    signed char plus = gb_mem->rb(pc);
    pc++;
    pc += plus;
}

void gb_cpu_state::jr_nz()  //8  20
{
    if (!zf_set()) {
        
        t += 4;
        m += 1;
        jr_m();
    }
    else
        pc += 1;
}
void gb_cpu_state::jr_z()   //8  28
{
    if (zf_set()){
        
        t += 4;
        m += 1;
        jr_m();
    }
    else
        pc += 1;

}
void gb_cpu_state::jr_nc()  //8  30
{
    if (!cf_set()){
        
        t += 4;
        m += 1;
        jr_m();
    }
    
    else
        pc += 1;

}
void gb_cpu_state::jr_c()   //8  38
{
    if (cf_set()){
        
        t += 4;
        m += 1;
        jr_m();
    }
    
    else
        pc += 1;
}

//CALLS

void gb_cpu_state::call()     //12 CD
{
    sp--;
    gb_mem->wb(sp, (pc+2)/256);
    sp--;
    gb_mem->wb(sp, (pc+2)%256);

    unsigned short addr = mk_word(gb_mem->rb(pc+1), gb_mem->rb(pc));
    pc = addr;
}

void gb_cpu_state::call_nz()  //12 C4
{
    if (!zf_set()) {
    
            t += 12;
            m += 3;
        
        call();
    }
    
    else
        pc += 2;
}
void gb_cpu_state::call_z()   //12 CC
{
    if (zf_set()){
        
        t += 12;
        m += 3;
        
        call();
    }
    
    else
        pc += 2;
}
void gb_cpu_state::call_nc()  //12 D4
{
    if (!cf_set()){
        
        t += 12;
        m += 3;
        
        call();
    }
    
    else
        pc += 2;
}
void gb_cpu_state::call_c()   //12 DC
{
    if (cf_set()){
        
        t += 12;
        m += 3;
        
        call();
    }
    
    else
        pc += 2;
}

//RESTARTS

void gb_cpu_state::rst(unsigned char addr)  {
    sp--;
    gb_mem->wb(sp, pc/256);
    sp--;
    gb_mem->wb(sp, pc%256);
    
    pc = addr;
}

void gb_cpu_state::rst_00()  //32 C7
{
    rst(0x00);
}
void gb_cpu_state::rst_10()  //32 D7
{
    rst(0x10);
}
void gb_cpu_state::rst_20()  //32 E7
{
    rst(0x20);
}
void gb_cpu_state::rst_30()  //32 F7
{
    rst(0x30);
}

void gb_cpu_state::rst_08()  //32 CF
{
    rst(0x08);
}
void gb_cpu_state::rst_18()  //32 DF
{
    rst(0x18);
}
void gb_cpu_state::rst_28()  //32 EF
{
    rst(0x28);
}
void gb_cpu_state::rst_38()  //32 FF
{
    rst(0x38);
}

//RETURNS

void gb_cpu_state::ret()     //8 C9
{
    unsigned short addr = mk_word(gb_mem->rb(sp+1), gb_mem->rb(sp));
    sp += 2;
    pc = addr;
}

void gb_cpu_state::ret_nz()  //8 C0
{
    if (!zf_set()) {
        t += 12;
        m += 3;
        ret();
    }
    
    
}
void gb_cpu_state::ret_z()   //8 C8
{
    if (zf_set()){
        t += 12;
        m += 3;
        ret();
    }
    
}
void gb_cpu_state::ret_nc()  //8 D0
{
    if (!cf_set()){
        t += 12;
        m += 3;
        ret();
    }
    
}
void gb_cpu_state::ret_c()   //8 D8
{
    if (cf_set()){
        t += 12;
        m += 3;
        ret();
    }
    
    
}

void gb_cpu_state::reti()    //8 D9
{
    ret();
    
    ime = true;
}


