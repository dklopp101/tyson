import struct
import ctypes

READ_MODE          = "r"
WRITE_MODE         = "w"
APPEND_MODE        = "a"
READ_UPDATE_MODE   = "r+"
WRITE_UPDATE_MODE  = "w+"
APPEND_UPDATE_MODE = "a+"

fopen_codemap = { READ_MODE          : 1,
                  WRITE_MODE         : 2,
                  APPEND_MODE        : 3,
                  READ_UPDATE_MODE   : 4,
                  WRITE_UPDATE_MODE  : 5,
                  APPEND_UPDATE_MODE : 6 }

fopen_codes = ( READ_MODE,
                WRITE_MODE,
                APPEND_MODE,
                READ_UPDATE_MODE,
                WRITE_UPDATE_MODE,
                APPEND_UPDATE_MODE )

U8  = 11
U64 = 12
S64 = 13
R64 = 14
STR = 15
ARR = 16

tytypes = ( U8, 
            U64,
            S64,
            R64,
            STR,
            ARR )

typecodes = { U8  : 'u8',
              U64 : 'u64',
			  S64 : 's64',
			  R64 : 'r64',
              STR : 'str',
              ARR : 'arr' }

tytypes_map = { 'u8'  : U8,
                'u64' : U64,
                's64' : S64,
                'r64' : R64,
                'str' : STR,
                'arr' : ARR }

def is_tytype(typ):
	if isinstance(typ, str):
		return typ in tytypes_map.keys()
	else:
		return typ in typecodes.keys()

U8_MIN  = 0
U64_MIN = 0
S64_MIN = -2147483648

U8_MAX  = 255
U64_MAX = 4294967295
S64_MAX = 2147483647
R64_MAX = 1.7976931348623157e+308

OPCOUNT      = 213

DIE          =   0
NOP          =   1
JMP          =   2
CALL         =   3
RET          =   4
SWCH         =   5
JEQ_B        =   6
JNEQ_B       =   7
JEQ_W        =   8
JNEQ_W       =   9
JGEQ_U       =  10
JLEQ_U       =  11
JGT_U        =  12
JLT_U        =  13
JGEQ_I       =  14
JLEQ_I       =  15
JGT_I        =  16
JLT_I        =  17
JGEQ_R       =  18
JLEQ_R       =  19
JGT_R        =  20
JLT_R        =  21
JMP_C1       =  22
JMP_C2       =  23
JMP_C3       =  24
JMP_C4       =  25
SET_C1       =  26
SET_C2       =  27
SET_C3       =  28 
SET_C4       =  29 
EQ           =  30
NEQ          =  31
AND          =  32
NOT          =  33
OR           =  34
XOR          =  35
LSH          =  36
RSH          =  37
INC_B        =  38
INC_U        =  39
INC_I        =  40
DEC_B        =  41
DEC_U        =  42
DEC_I        =  43
ADD_B        =  44
ADD_U        =  45
ADD_I        =  46
ADD_R        =  47
SUB_B        =  48
SUB_U        =  49
SUB_I        =  50
SUB_R        =  51
MUL_B        =  52
MUL_U        =  53
MUL_I        =  54
MUL_R        =  55
DIV_B        =  56
DIV_U        =  57
DIV_I        =  58
DIV_R        =  59
MOD_B        =  60
MOD_U        =  61
MOD_I        =  62
B2U          =  63
B2I          =  64
B2R          =  65
U2B          =  66
U2I          =  67
U2R          =  68
I2B          =  69
I2U          =  70
I2R          =  71
R2B          =  72
R2U          =  73
R2I          =  74
LSTART       =  75
LTEST        =  76
LCONT        =  77
LSTOP        =  78
BREAKPOINT   =  79
PUT_B        =  80
PUT_NB       =  81
PUT_HW       =  82
PUT_W        =  83
PUT_NW       =  84
PUT_DW       =  85
PUT_QW       =  86
PUT_S        =  87
CPY_B        =  88
CPY_NB       =  89
CPY_HW       =  90
CPY_W        =  91
CPY_NW       =  92
CPY_DW       =  93
CPY_QW       =  94
CPY_S        =  95
XCH_B        =  96
XCH_NB       =  97
XCH_HW       =  98
XCH_W        =  99
XCH_NW       = 100
XCH_DW       = 101
XCH_QW       = 102
XCH_S        = 103
STR_CMP      = 104
STR_NCMP     = 105
JMP_STR_CMP  = 106
JMP_STR_NCMP = 107
STR_CHR      = 108
STR_CSPN     = 109
STR_STR      = 110
STR_CAT      = 111
STR_NCAT     = 112
STR_LEN      = 113
RSTK_UP      = 114
RSTK_DWN     = 115
RSTK_RST     = 116
PUT_B_FS     = 117
PUT_W_FS     = 118
CPY_B_FS     = 119
CPY_W_FS     = 120
XCH_B_FS     = 121
XCH_W_FS     = 122
SET_TDX_FC   = 123
SET_TDX_FH   = 124
SET_TDX_FS   = 125
T_FD_PUTB    = 126
T_BK_PUTB    = 127
T_FD_PUTW    = 128
T_BK_PUTW    = 129
T_FD_CPYB    = 130
T_BK_CPYB    = 131
T_FD_CPYW    = 132
T_BK_CPYW    = 133
T_FD_POPB    = 134
T_BK_POPB    = 135
T_FD_POPW    = 136
T_BK_POPW    = 137
T_FD_PSHB    = 138
T_BK_PSHB    = 139
T_FD_PSHW    = 140
T_BK_PSHW    = 141
STK_SPOFFS   = 142
STK_SAVE     = 143
STK_LOAD     = 144
STK_UP       = 145
STK_DWN      = 146
STK_RST      = 147
STK_CLR      = 148
STK_SET      = 149
STK_SETN     = 150
STK_SETC     = 151
STK_SETCN    = 152
STK_CPY      = 153
STK_CPYN     = 154
STK_XCH      = 155
STK_XCHN     = 156
STK_HXCH     = 157
STK_HXCHN    = 158
STK_MOV      = 159
STK_MOVN     = 160
STK_DEL      = 161
STK_DELN     = 162
STK_GET      = 163
STK_GETN     = 164
STK_INS      = 165
STK_INSN     = 166
STK_2TOP     = 167
STK_TT_DUP   = 168
STK_XT_DUP   = 169
STK_TX_DUP   = 170
STK_TOP_DUP  = 171
STK_TOP_DUP2 = 172
STK_DUP      = 173
STK_TAPSH    = 174
STK_PSH      = 175
STK_PSHC     = 176 
STK_PSH0     = 177
STK_PSH1     = 178
STK_PSH2     = 179
STK_OVWR     = 180
STK_STOR     = 181
STK_POP      = 182
STK_XCHT     = 183
STK_GCOL     = 184
OPENF        = 185
RSV_SYS2     = 186
RSV_SYS3     = 187
RSV_SYS4     = 188
RSV_SYS5     = 189
RSV_SYS6     = 190
RSV_SYS7     = 191
RSV_SYS8     = 192
RSV_SYS9     = 193
RSV_SYS10    = 194
RSV_SYS11    = 195
RSV_SYS12    = 196
RSV_SYS13    = 197
RSV_SYS14    = 198
RSV_SYS15    = 199
SHOW_TOP_B   = 200
SHOW_TOP_U   = 201
SHOW_TOP_I   = 202
SHOW_TOP_R   = 203
SHOW_MEM_B   = 204
SHOW_MEM_U   = 205
SHOW_MEM_I   = 206
SHOW_MEM_R   = 207
SHOW_MEM_S   = 208
TDX_B_UP     = 209
TDX_B_DWN    = 210
TDX_W_UP     = 211
TDX_W_DWN    = 212


METADATA_SIZE = 96
TEXT_BASE     = 96

opmap = {'die' : DIE, 
         'nop' : NOP, 
         'jmp' : JMP,
         'call' : CALL, 
         'ret' : RET,
         'swch' : SWCH,
         'jeq_b' : JEQ_B,
         'jneq_b' : JNEQ_B,
         'jeq_w' : JEQ_W,
         'jneq_w' : JNEQ_W,
         'jgeq_u' : JGEQ_U,
         'jleq_u' : JLEQ_U,
         'jgt_u'  : JGT_U,
         'jlt_u' : JLT_U,
         'jgeq_i' : JGEQ_I,
         'jleq_i' : JLEQ_I,
         'jgt_i' : JGT_I,
         'jlt_i' : JLT_I,
         'jgeq_r' : JGEQ_R,
         'jleq_r' : JLEQ_R,
         'jgt_r' : JGT_R,
         'jlt_r' : JLT_R,
         'jmp_c1' : JMP_C1,
         'jmp_c2' : JMP_C2,
         'jmp_c3' : JMP_C3,
         'jmp_c4' : JMP_C4,
         'set_c1' : SET_C1,
         'set_c2' : SET_C2,
         'set_c3' : SET_C3,
         'set_c4' : SET_C4,
         'eq' : EQ,
         'neq' : NEQ,
         'and' : AND,
         'not' : NOT,
         'or' : OR,
         'xor' : XOR,
         'lsh' : LSH,
         'rsh' : RSH,
         'inc_b' : INC_B,
         'inc_u' : INC_U,
         'inc_i' : INC_I,
         'dec_b' : DEC_B,
         'dec_u' : DEC_U,
         'dec_i' : DEC_I,
         'add_b' : ADD_B,
         'add_u' : ADD_U,
         'add_i' : ADD_I,
         'add_r' : ADD_R,
         'sub_b' : SUB_B,
         'sub_u' : SUB_U,
         'sub_i' : SUB_I,
         'sub_r' : SUB_R,
         'mul_b' : MUL_B,
         'mul_u' : MUL_U,
         'mul_i' : MUL_I,
         'mul_r' : MUL_R,
         'div_b' : DIV_B,
         'div_u' : DIV_U,
         'div_i' : DIV_I,
         'div_r' : DIV_R,
         'mod_b' : MOD_B,
         'mod_u' : MOD_U,
         'mod_i' : MOD_I,
         'b2u' : B2U,
         'b2i' : B2I,
         'b2r' : B2R,
         'u2b' : U2B,
         'u2i' : U2I,
         'u2r' : U2R,
         'i2b' : I2B,
         'i2u' : I2U,
         'i2r' : I2R,
         'r2b' : R2B,
         'r2u' : R2U,
         'r2i' : R2I,
         'lstart' : LSTART,
         'ltest' : LTEST,
         'lcont' : LCONT,
         'lstop' : LSTOP,
         'breakpoint' : BREAKPOINT,
         'put_b' : PUT_B,
         'put_nb' : PUT_NB,
         'put_hw' : PUT_HW,
         'put_w' : PUT_W,
         'put_nw' : PUT_NW,
         'put_dw' : PUT_DW,
         'put_qw' : PUT_QW,
         'put_s'  : PUT_S,
         'cpy_b' : CPY_B,
         'cpy_nb' : CPY_NB,
         'cpy_hw' : CPY_HW,
         'cpy_w' : CPY_W,
         'cpy_nw' : CPY_NW,
         'cpy_dw' : CPY_DW,
         'cpy_qw' : CPY_QW,
		 'cpy_s'  : CPY_S,
         'xch_b' : XCH_B,
         'xch_nb' : XCH_NB,
         'xch_hw' : XCH_HW,
         'xch_w' : XCH_W,
         'xch_nw' : XCH_NW,
         'xch_dw' : XCH_DW,
         'xch_qw' : XCH_QW,
		 'xch_s'  : XCH_S,
         'str_cmp' : STR_CMP,
         'str_ncmp' : STR_NCMP,
         'jmp_str_cmp' : JMP_STR_CMP,
         'jmp_str_ncmp' : JMP_STR_NCMP,
         'str_chr' : STR_CHR,
         'str_cspn' : STR_CSPN,
         'str_str' : STR_STR,
         'str_cat' : STR_CAT,
         'str_ncat' : STR_NCAT,
         'str_len' : STR_LEN,
		 'rstk_up' : RSTK_UP,
		 'rstk_dwn' : RSTK_DWN,
		 'rstk_rst' : RSTK_RST,
		 'put_b_fs' : PUT_B_FS,
		 'put_w_fs' : PUT_W_FS,
		 'cpy_b_fs' : CPY_B_FS,
		 'cpy_w_fs' : CPY_W_FS,
		 'xch_b_fs' : XCH_B_FS,
		 'xch_w_fs' : XCH_W_FS,
		 'set_tdx_fc' : SET_TDX_FC,
         'set_tdx_fh' : SET_TDX_FH,
         'set_tdx_fs' : SET_TDX_FS,
         't_fd_putb' : T_FD_PUTB,
         't_bk_putb' : T_BK_PUTB,
         't_fd_putw' : T_FD_PUTW,
         't_bk_putw' : T_BK_PUTW,
         't_fd_cpyb' : T_FD_CPYB,
         't_bk_cpyb' : T_BK_CPYB,
         't_fd_cpyw' : T_FD_CPYW,
         't_bk_cpyw' : T_BK_CPYW,
         't_fd_popb' : T_FD_POPB,
         't_bk_popb' : T_BK_POPB,
         't_fd_popw' : T_FD_POPW,
         't_bk_popw' : T_BK_POPW,
         't_fd_pshb' : T_FD_PSHB,
         't_bk_pshb' : T_BK_PSHB,
         't_fd_pshw' : T_FD_PSHW,
         't_bk_pshw' : T_BK_PSHW,
         'stk_spoffs' : STK_SPOFFS,
         'stk_save' : STK_SAVE,
         'stk_load' : STK_LOAD,
         'stk_up' : STK_UP,
         'stk_dwn' : STK_DWN,
         'stk_rst' : STK_RST,
         'stk_clr' : STK_CLR,
         'stk_set' : STK_SET,
         'stk_setn' : STK_SETN,
         'stk_setc' : STK_SETC,
         'stk_setcn' : STK_SETCN,
         'stk_cpy' : STK_CPY,
         'stk_cpyn' : STK_CPYN,
         'stk_xch' : STK_XCH,
         'stk_xchn' : STK_XCHN,
         'stk_hxch' : STK_HXCH,
         'stk_hxchn' : STK_HXCHN,
         'stk_mov' : STK_MOV,
         'stk_movn' : STK_MOVN,
         'stk_del' : STK_DEL,
         'stk_deln' : STK_DELN,
         'stk_get' : STK_GET,
         'stk_getn' : STK_GETN,
         'stk_ins' : STK_INS,
         'stk_insn' : STK_INSN,
         'stk_2top' : STK_2TOP,
         'stk_tt_dup' : STK_TT_DUP,
         'stk_xt_dup' : STK_XT_DUP,
         'stk_tx_dup' : STK_TX_DUP,
         'stk_top_dup' : STK_TOP_DUP,
         'stk_top_dup2' : STK_TOP_DUP2,
         'stk_dup' : STK_DUP,
         'stk_tapsh' : STK_TAPSH,
         'stk_psh' : STK_PSH,
         'stk_pshc' : STK_PSHC,
         'stk_psh0' : STK_PSH0,
         'stk_psh1' : STK_PSH1,
         'stk_psh2' : STK_PSH2,
         'stk_ovwr' : STK_OVWR,
         'stk_stor' : STK_STOR,
         'stk_pop' : STK_POP,
         'stk_xcht' : STK_XCHT,
         'stk_gcol' : STK_GCOL,
         'openf' : OPENF,
         'rsv_sys2' : RSV_SYS2,
         'rsv_sys3' : RSV_SYS3,
         'rsv_sys4' : RSV_SYS4,
         'rsv_sys5' : RSV_SYS5,
         'rsv_sys6' : RSV_SYS6,
         'rsv_sys7' : RSV_SYS7,
         'rsv_sys8' : RSV_SYS8,
         'rsv_sys9' : RSV_SYS9,
         'rsv_sys10' : RSV_SYS10,
         'rsv_sys11' : RSV_SYS11,
         'rsv_sys12' : RSV_SYS12,
         'rsv_sys13' : RSV_SYS13,
         'rsv_sys14' : RSV_SYS14,
         'rsv_sys15' : RSV_SYS15,
         'show_top_b' : SHOW_TOP_B,
         'show_top_u' : SHOW_TOP_U,
         'show_top_i' : SHOW_TOP_I,
         'show_top_r' : SHOW_TOP_R,
         'show_mem_b' : SHOW_MEM_B,
         'show_mem_u' : SHOW_MEM_U,
         'show_mem_i' : SHOW_MEM_I,
         'show_mem_r' : SHOW_MEM_R,
         'show_mem_s' : SHOW_MEM_S,
         'tdx_b_up' : TDX_B_UP,
         'tdx_b_dwn' : TDX_B_DWN,
         'tdx_w_up' : TDX_W_UP,
         'tdx_w_dwn' : TDX_W_DWN}

no_arg_ops = ( BREAKPOINT,
               DIE,
               NOP,
               RET,
               JMP_C1,
               JMP_C2,
               JMP_C3, 
               JMP_C4, 
               EQ,
               NEQ,
               AND,
               NOT, 
               OR,
               XOR, 
               LSH, 
               RSH,  
               INC_B,
               INC_U, 
               INC_I,
               DEC_B,
               DEC_U,
               DEC_I,
               ADD_B,
               ADD_U,
               ADD_I,
               ADD_R,
               SUB_B,
               SUB_U,
               SUB_I,
               SUB_R,
               MUL_B,
               MUL_U,
               MUL_I,
               MUL_R,
               DIV_B,
               DIV_U,
               DIV_I,
               DIV_R,
               MOD_B,
               MOD_U,
               MOD_I,
               B2U,
               B2I,
               B2R,
               U2B,
               U2I,
               U2R,
               I2B,
               I2U,
               I2R,
               R2B,
               R2U,
               R2I,
               LTEST,
               LCONT,
               LSTOP,
               RSTK_UP,
               RSTK_DWN,
               RSTK_RST,
               SET_TDX_FS,
               T_FD_POPB,
               T_BK_POPB,
               T_FD_POPW,
               T_BK_POPW,
               T_FD_PSHB,
               T_BK_PSHB,
               T_FD_PSHW,
               T_BK_PSHW,
               STK_SPOFFS,
               STK_UP,
               STK_DWN,
               STK_RST,
               STK_CLR,
               STK_TOP_DUP,
               STK_TOP_DUP2,
               STK_TAPSH,
               STK_PSH0,
               STK_PSH1,
               STK_PSH2,
               STK_XCHT,
               STK_GCOL,
               SHOW_TOP_B,
               SHOW_TOP_U,
               SHOW_TOP_I,
               SHOW_TOP_R,
               TDX_B_UP,
               TDX_B_DWN,
               TDX_W_UP,
               TDX_W_DWN )
 
def from_opname(opname):
	return opmap[opname]

def from_opcode(opcode):
	return opmap.keys()[opcode]

def is_tyobj(obj):
	if isinstance(obj, u8):
		return True
	elif isinstance(obj, u64):
		return True
	elif isinstance(obj, s64):
		return True
	elif isinstance(obj, pb):
		return True
	else:
		return False

def make_tyobj(dtype, value):
	if dtype == U8:
		return u8(value)
	elif dtype == U64:
		return u64(value)
	elif dtype == S64:
		return s64(value)
	elif dtype == R64:
		return r64(value)

class u8(ctypes.c_ubyte):
	def __init__(self, value=0):
		self.typecode = U8

		if value > U8_MAX:
			raise OverflowError()
		else:
			ctypes.c_ubyte.__init__(self, value)

	def __bytes__(self):
		return struct.pack('B', self.value)

	def __repr__(self):
		return 'u8({})'.format(self.value)

	def __str__(self):
		return str(self.value)

	def __int__(self):
		return int(self.value)

	def __float__(self):
		return float(self.value)

class u64(ctypes.c_ulong):
	def __init__(self, value=0):
		self.typecode = U64
		if value > U64_MAX:
			raise OverflowError()
		else:
			ctypes.c_ulong.__init__(self, value)

	def __bytes__(self):
		return struct.pack('L', self.value)

	def __repr__(self):
		return 'u64({})'.format(self.value)

	def __str__(self):
		return str(self.value)

	def __int__(self):
		return int(self.value)

	def __float__(self):
		return float(self.value)

class s64(ctypes.c_long):
	def __init__(self, value=0):
		self.typecode = S64

		if value > S64_MAX:
			raise OverflowError()
		else:
			ctypes.c_long.__init__(self, value)

	def __bytes__(self):
		return struct.pack('l', self.value)

	def __repr__(self):
		return 's64({})'.format(self.value)

	def __str__(self):
		return str(self.value)

	def __int__(self):
		return int(self.value)

	def __float__(self):
		return float(self.value)

class r64(ctypes.c_double):
	def __init__(self, value=0):
		self.typecode = R64		
		if value > R64_MAX:
			raise OverflowError()
		else:
			ctypes.c_double.__init__(self, value)

	def __bytes__(self):
		return struct.pack('d', self.value)

	def __repr__(self):
		return 'r64({})'.format(self.value)

	def __str__(self):
		return str(self.value)

	def __float__(self):
		return float(self.value)

	def __int__(self):
		return int(self.value)

class Array:
	def __init__(self):
		self.objs = []

	def __repr__(self):
		string = ['Array(']
		c=0
		for obj in self.objs:
			c += 1
			if c == len(self.objs):
				string.append('{})'.format(repr(obj)))
			else:
				string.append('{}, '.format(repr(obj)))
		return ''.join(string)

	def __str__(self):
		string = ['[']
		c=0
		for obj in self.objs:
			c += 1
			if c == len(self.objs):
				string.append('{}]'.format(str(obj)))
			else:
				string.append('{}, '.format(str(obj)))
		return ''.join(string)

	def byte_len(self):
		return len(bytes(self))

	def __bytes__(self):
		string = bytearray()		
		for obj in self.objs:	
			for byte in bytes(obj):
				string.append(byte)
		return bytes(string)
