#ifndef opcodes_h
#define opcodes_h

#include "tyson.h"

#define OPCOUNT      213

#define DIE            0
#define NOP            1
#define JMP            2
#define CALL           3
#define RET            4
#define SWCH           5

#define JEQ_B          6
#define JNEQ_B         7
#define JEQ_W          8
#define JNEQ_W         9
#define JGEQ_U        10
#define JLEQ_U        11
#define JGT_U         12
#define JLT_U         13
#define JGEQ_I        14
#define JLEQ_I        15
#define JGT_I         16
#define JLT_I         17
#define JGEQ_R        18
#define JLEQ_R        19
#define JGT_R         20
#define JLT_R         21

#define JMP_C1        22
#define JMP_C2        23
#define JMP_C3        24
#define JMP_C4        25
#define SET_C1        26
#define SET_C2        27
#define SET_C3        28
#define SET_C4        29

#define EQ            30
#define NEQ           31
#define AND           32
#define NOT           33
#define OR            34
#define XOR           35
#define LSH           36
#define RSH           37

#define INC_B         38
#define INC_U         39
#define INC_I         40
#define DEC_B         41
#define DEC_U         42
#define DEC_I         43
#define ADD_B         44
#define ADD_U         45
#define ADD_I         46
#define ADD_R         47
#define SUB_B         48
#define SUB_U         49
#define SUB_I         50
#define SUB_R         51
#define MUL_B         52
#define MUL_U         53
#define MUL_I         54
#define MUL_R         55
#define DIV_B         56
#define DIV_U         57
#define DIV_I         58
#define DIV_R         59
#define MOD_B         60
#define MOD_U         61
#define MOD_I         62

#define B2U           63
#define B2I           64
#define B2R           65
#define U2B           66
#define U2I           67
#define U2R           68
#define I2B           69
#define I2U           70
#define I2R           71
#define R2B           72
#define R2U           73
#define R2I           74

#define LSTART        75
#define LTEST         76
#define LCONT         77
#define LSTOP         78

#define BREAKPOINT    79

#define PUT_B         80
#define PUT_NB        81
#define PUT_HW        82
#define PUT_W         83
#define PUT_NW        84
#define PUT_DW        85
#define PUT_QW        86
#define PUT_S         87
#define CPY_B         88
#define CPY_NB        89
#define CPY_HW        90
#define CPY_W         91
#define CPY_NW        92
#define CPY_DW        93
#define CPY_QW        94
#define CPY_S         95
#define XCH_B         96
#define XCH_NB        97
#define XCH_HW        98
#define XCH_W         99
#define XCH_NW       100
#define XCH_DW       101
#define XCH_QW       102
#define XCH_S        103

#define STR_CMP      104
#define STR_NCMP     105
#define JMP_STR_CMP  106
#define JMP_STR_NCMP 107
#define STR_CHR      108
#define STR_CSPN     109
#define STR_STR      110
#define STR_CAT      111
#define STR_NCAT     112
#define STR_LEN      113

#define RSTK_UP      114
#define RSTK_DWN     115
#define RSTK_RST     116

#define PUT_B_FS     117
#define PUT_W_FS     118
#define CPY_B_FS     119
#define CPY_W_FS     120
#define XCH_B_FS     121
#define XCH_W_FS     122
#define SET_TDX_FC   123
#define SET_TDX_FH   124
#define SET_TDX_FS   125
#define T_FD_PUTB    126
#define T_BK_PUTB    127
#define T_FD_PUTW    128
#define T_BK_PUTW    129
#define T_FD_CPYB    130
#define T_BK_CPYB    131
#define T_FD_CPYW    132
#define T_BK_CPYW    133
#define T_FD_POPB    134
#define T_BK_POPB    135
#define T_FD_POPW    136
#define T_BK_POPW    137
#define T_FD_PSHB    138
#define T_BK_PSHB    139
#define T_FD_PSHW    140
#define T_BK_PSHW    141

#define STK_SPOFFS   142
#define STK_SAVE     143
#define STK_LOAD     144
#define STK_UP       145
#define STK_DWN      146
#define STK_RST      147
#define STK_CLR      148
#define STK_SET      149
#define STK_SETN     150
#define STK_SETC     151
#define STK_SETCN    152
#define STK_CPY      153
#define STK_CPYN     154
#define STK_XCH      155
#define STK_XCHN     156
#define STK_HXCH     157
#define STK_HXCHN    158
#define STK_MOV      159
#define STK_MOVN     160
#define STK_DEL      161
#define STK_DELN     162
#define STK_GET      163
#define STK_GETN     164
#define STK_INS      165
#define STK_INSN     166
#define STK_2TOP     167
#define STK_TT_DUP   168
#define STK_XT_DUP   169
#define STK_TX_DUP   170
#define STK_TOP_DUP  171
#define STK_TOP_DUP2 172
#define STK_DUP      173
#define STK_TAPSH    174
#define STK_PSH      175
#define STK_PSHC     176 
#define STK_PSH0     177
#define STK_PSH1     178
#define STK_PSH2     179
#define STK_OVWR     180
#define STK_STOR     181
#define STK_POP      182
#define STK_XCHT     183
#define STK_GCOL     184

#define OPENF        185
#define RSV_SYS2     186
#define RSV_SYS3     187
#define RSV_SYS4     188
#define RSV_SYS5     189
#define RSV_SYS6     190
#define RSV_SYS7     191
#define RSV_SYS8     192
#define RSV_SYS9     193
#define RSV_SYS10    194
#define RSV_SYS11    195
#define RSV_SYS12    196
#define RSV_SYS13    197
#define RSV_SYS14    198
#define RSV_SYS15    199

#define SHOW_TOP_B   200
#define SHOW_TOP_U   201
#define SHOW_TOP_I   202
#define SHOW_TOP_R   203
#define SHOW_MEM_B   204
#define SHOW_MEM_U   205
#define SHOW_MEM_I   206
#define SHOW_MEM_R   207
#define SHOW_MEM_S   208

#define TDX_B_UP     209
#define TDX_B_DWN    210
#define TDX_W_UP     211
#define TDX_W_DWN    212


#define build_optable()                  			  \
	static void* optable[OPCOUNT]= {&&die,            \
		                            &&nop,            \
		                            &&jmp,            \
                                    &&call,           \
                                    &&ret,            \
                                    &&swch,       \
									&&jeq_b, \
									&&jneq_b, \
									&&jeq_w, \
									&&jneq_w, \
									&&jgeq_u, \
									&&jleq_u, \
									&&jgt_u, \
									&&jlt_u, \
									&&jgeq_i, \
									&&jleq_i, \
									&&jgt_i, \
									&&jlt_i, \
									&&jgeq_r, \
									&&jleq_r, \
									&&jgt_r, \
									&&jlt_r, \
									&&jmp_c1, \
									&&jmp_c2, \
									&&jmp_c3, \
									&&jmp_c4, \
									&&set_c1, \
									&&set_c2, \
									&&set_c3, \
									&&set_c4, \
									&&eq, \
									&&neq, \
									&&and, \
									&&not, \
									&&or, \
									&&xor, \
									&&lsh, \
									&&rsh, \
									&&inc_b, \
									&&inc_u, \
									&&inc_i, \
									&&dec_b, \
									&&dec_u, \
									&&dec_i, \
									&&add_b, \
									&&add_u, \
									&&add_i, \
									&&add_r, \
									&&sub_b, \
									&&sub_u, \
									&&sub_i, \
									&&sub_r, \
									&&mul_b, \
									&&mul_u, \
									&&mul_i, \
									&&mul_r, \
									&&div_b, \
									&&div_u, \
									&&div_i, \
									&&div_r, \
									&&mod_b, \
									&&mod_u, \
									&&mod_i, \
									&&b2u, \
									&&b2i, \
									&&b2r, \
									&&u2b, \
									&&u2i, \
									&&u2r, \
									&&i2b, \
									&&i2u, \
									&&i2r, \
									&&r2b, \
									&&r2u, \
									&&r2i, \
									&&lstart, \
									&&ltest, \
									&&lcont, \
									&&lstop, \
									&&breakpoint, \
									&&put_b, \
									&&put_nb, \
									&&put_hw, \
									&&put_w, \
									&&put_nw, \
									&&put_dw, \
									&&put_qw, \
									&&put_s, \
									&&cpy_b, \
									&&cpy_nb, \
									&&cpy_hw, \
									&&cpy_w, \
									&&cpy_nw, \
									&&cpy_dw, \
									&&cpy_qw, \
									&&cpy_s, \
									&&xch_b, \
									&&xch_nb, \
									&&xch_hw, \
									&&xch_w, \
									&&xch_nw, \
									&&xch_dw, \
									&&xch_qw, \
									&&xch_s, \
									&&str_cmp,      \
									&&str_ncmp,     \
									&&jmp_str_cmp,  \
									&&jmp_str_ncmp, \
									&&str_chr,      \
									&&str_cspn,     \
									&&str_str,      \
									&&str_cat,      \
									&&str_ncat,     \
									&&str_len,      \
									&&rstk_up, \
									&&rstk_dwn, \
									&&rstk_rst, \
									&&put_b_fs, \
									&&put_w_fs, \
									&&cpy_b_fs, \
									&&cpy_w_fs, \
									&&xch_b_fs, \
									&&xch_w_fs, \
									&&set_tdx_fc, \
									&&set_tdx_fh, \
									&&set_tdx_fs, \
									&&t_fd_putb, \
									&&t_bk_putb, \
									&&t_fd_putw, \
									&&t_bk_putw, \
									&&t_fd_cpyb, \
									&&t_bk_cpyb, \
									&&t_fd_cpyw, \
									&&t_bk_cpyw, \
									&&t_fd_popb, \
									&&t_bk_popb, \
									&&t_fd_popw, \
									&&t_bk_popw, \
									&&t_fd_pshb, \
									&&t_bk_pshb, \
									&&t_fd_pshw, \
									&&t_bk_pshw, \
									&&stk_spoffs, \
									&&stk_save, \
									&&stk_load, \
									&&stk_up, \
									&&stk_dwn, \
									&&stk_rst, \
									&&stk_clr, \
									&&stk_set, \
									&&stk_setn, \
									&&stk_setc, \
									&&stk_setcn, \
									&&stk_cpy, \
									&&stk_cpyn, \
									&&stk_xch, \
									&&stk_xchn, \
									&&stk_hxch, \
									&&stk_hxchn, \
									&&stk_mov, \
									&&stk_movn, \
									&&stk_del, \
									&&stk_deln, \
									&&stk_get, \
									&&stk_getn, \
									&&stk_ins, \
									&&stk_insn, \
									&&stk_2top, \
									&&stk_tt_dup, \
									&&stk_xt_dup, \
									&&stk_tx_dup, \
									&&stk_top_dup, \
									&&stk_top_dup2, \
									&&stk_dup, \
									&&stk_tapsh, \
									&&stk_psh, \
									&&stk_pshc, \
									&&stk_psh0, \
									&&stk_psh1, \
									&&stk_psh2, \
									&&stk_ovwr, \
									&&stk_stor, \
									&&stk_pop, \
									&&stk_xcht, \
									&&stk_gcol, \
									&&openf, \
									&&rsv_sys2, \
									&&rsv_sys3, \
									&&rsv_sys4, \
									&&rsv_sys5, \
									&&rsv_sys6, \
									&&rsv_sys7, \
									&&rsv_sys8, \
									&&rsv_sys9, \
									&&rsv_sys10,\
									&&rsv_sys11, \
									&&rsv_sys12, \
									&&rsv_sys13, \
									&&rsv_sys14, \
									&&rsv_sys15,\
									&&show_top_b, \
									&&show_top_u, \
									&&show_top_i, \
									&&show_top_r, \
									&&show_mem_b, \
									&&show_mem_u, \
									&&show_mem_i, \
									&&show_mem_r, \
                                    &&show_mem_s, \
                                    &&tdx_b_up,   \
                                    &&tdx_b_dwn,  \
                                    &&tdx_w_up,   \
                                    &&tdx_w_dwn}



char* opcode_strmap[OPCOUNT];

u8  lookup_opcode(const u64, char*);
s64 lookup_mneumonic(const char*);
u8  is_mneumonic(const char*);
u8  is_opcode(const u64);

#endif

