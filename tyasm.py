import sys
from tyson import *

class Arg:
	def __init__(self, num=None, dtype=None, obj=None):
		self.dtype = dtype
		self.num   = num
		self.obj   = obj

	def __repr__(self):
		return 'Arg(num={}, dtype={}, obj={})'.format(str(self.num), str(self.dtype), repr(self.obj))

	def __str__(self):
		return 'arg #{} {} : {}'.format(str(self.num), ty_types[self.dtype], str(self.obj))

	def __bytes__(self):
		return bytes(self.obj)

class ArgList:
	def __init__(self, initlist=None):
		if initlist is not None:
			if isinstance(initlist, list):
				for item in initlist:
					if not isinstance(item, Arg):
						raise TypeError('Items within ArgList() initlist must be instances of Arg() class.')
				self.args = initlist[:]
			else:
				raise TypeError('ArgList() can only be initialised with lists.')
		else:
			self.args = []

	def new_arg(self, dtype=None, value=0):
		if dtype == U8:
			obj = u8(value)
		elif dtype == U64:
			obj = u64(value)
		elif dtype == S64:
			obj = s64(value)
		elif dtype == R64:
			obj = r64(value)
		elif dtype == ARR:
			obj = value
		else:
			raise TypeError('Datatype must be valid tyson typecode.')
		self.args.append(Arg(len(self.args), dtype, obj))

	def __repr__(self):
		if len(self.args) == 0:
			return 'ArgList()'
		elif len(self.args) == 1:
			return 'ArgList([{}])'.format(repr(self.args[0]))
		else:
			string = ['ArgList([']
			c=1
			for arg in self.args:
				string.append(repr(arg))
				if c != len(self.args):
					string.append(', ')
				c+=1
			string.append('])')
			return ''.join(string)

	def __str__(self):
		string = []
		for arg in self.arg:
			string.append(str(arg))
			string.append('\n')
		return ''.join(string)

	def __bytes__(self):
		string = bytearray()
		for arg in self.args:
			for byte in bytes(arg):
				string.append(byte)
		return bytes(string)

class Instr:
	def __init__(self, num=None, addr=None, opcode=0, args=None):
		self.opcode = u8(opcode)
		self.args   = args
		self.num    = num
		self.addr   = addr

	def __repr__(self):
		return 'Instr(num={}, addr={}, opcode={}, args={})'.format(str(self.num), str(self.addr),
                                                                   repr(self.opcode), repr(self.args))
	def __str__(self):
		string = ['instr #{} {} addr: {}\n'.format(str(self.num), opmap.keys()[self.opcode.value]), str(self.start_addr)]
		for arg in self.args:
			string.append('\t')
			string.append(str(arg))
			string.append('\n')
		return ''.join(string)

	def __bytes__(self):
		string = bytearray()
		for byte in bytes(self.opcode):
			string.append(byte)
		if self.args is not None:
			for byte in bytes(self.args):
				string.append(byte)
		return bytes(string)


class InstrList:
	def __init__(self, initlist=None):
		if initlist is not None:
			if isinstance(initlist, list):
				for item in initlist:
					if not isinstance(item, Instr):
						raise TypeError('Items within InstrList() initlist must be instances of Instr() class.')
				self.instrs = initlist[:]
			else:
				raise TypeError('InstrList() can only be initialised with lists.')
		else:
			self.instrs = []

	def new_instr(self, addr=None, opcode=0, args=None):
		self.instrs.append(Instr(len(self.instrs), addr, opcode, args))

	def __len__(self):
		return len(self.instrs)

	def byte_len(self):
		return len(bytes(self))

	def next_addr(self):
		return TEXT_BASE + (len(bytes(self)))

	def __repr__(self):
		if len(self.instrs) == 0:
			return 'InstrList()'
		elif len(self.instrs) == 1:
			return 'InstrList([{}])'.format(repr(self.instrs[0]))
		else:
			string = ['InstrList([']
			c=1
			for instr in self.instrs:
				string.append(repr(instr))
				if c != len(self.instrs):
					string.append(', ')
				c+=1
			string.append('])')
			return ''.join(string)

	def __str__(self):
		string = []
		for instr in self.instrs:
			string.append(str(instr))
			string.append('\n')
		return ''.join(string)

	def __bytes__(self):
		string = bytearray()
		for instr in self.instrs:
			for byte in bytes(instr):				
				string.append(byte)
		return bytes(string)

class Metadata:
	def __init__(self, text_size=0, start_addr=0, pool_size=0, heap_size=0):
		self.timg_size  = u64(METADATA_SIZE + text_size + pool_size)
		self.start_addr = u64(start_addr)
		self.args_base  = u64(METADATA_SIZE + text_size)
		self.text_base  = u64(METADATA_SIZE)
		self.pool_base  = u64(0)
		self.heap_base  = u64(0)
		self.pimg_size  = u64(0)
		self.text_size  = u64(text_size)
		self.pool_size  = u64(pool_size)
		self.heap_size  = u64(heap_size)
		self.args_count = u64(0)
		self.args_size  = u64(0)
						
	def byte_len(self):
		return METADATA_SIZE

	def __repr__(self):
		return 'Metadata(start_addr={}, text_size={}, pool_size={}, heap_size={})'.format(str(self.start_addr), str(self.text_size), str(self.pool_size), str(self.heap_size))

	def __str__(self):
		return '\tMetadata:\n\tstart_addr={}, text_size={}, pool_size={}, heap_size={}, timg_size={}, pimg_size={}, heap_base={}'.format(str(self.start_addr), str(self.text_size), str(self.pool_size), str(self.timg_size), str(self.pimg_size), str(self.heap_base))

	def __bytes__(self):
		string = bytearray()
		for byte in bytes(self.timg_size):
			string.append(byte)
		for byte in bytes(self.start_addr):
			string.append(byte)
		for byte in bytes(self.args_base):
			string.append(byte)
		for byte in bytes(self.text_base):
			string.append(byte)
		for byte in bytes(self.pool_base):
			string.append(byte)
		for byte in bytes(self.heap_base):
			string.append(byte)
		for byte in bytes(self.pimg_size):
			string.append(byte)
		for byte in bytes(self.text_size):
			string.append(byte)
		for byte in bytes(self.pool_size):
			string.append(byte)
		for byte in bytes(self.heap_size):
			string.append(byte)
		for byte in bytes(self.args_count):
			string.append(byte)
		for byte in bytes(self.args_size):
			string.append(byte)
		return bytes(string)

class TextImage:
	def __init__(self, metadata=None, instrs=None):
		self.metadata = metadata
		self.instrs   = instrs

	def __len__(self):
		return len(bytes(self))
	
	def __repr__(self):
		return 'TextImage(metadata={}, instrs={})'.format(repr(self.metadata), repr(self.instrs))

	def __bytes__(self):
		string = bytearray()
		for byte in bytes(self.metadata):
			string.append(byte)
		for byte in bytes(self.instrs):
			string.append(byte)
		return bytes(string)

	def write(self, path):
		with open(path, 'wb') as f:
			f.write(bytes(self))

class symbol:
	def __init__(self, name=None, dtype=None, value=None):
		self.name = name
		self.dtype = dtype
		self.value = value

	def __str__(self):
		return self.name 

class assembler:
	def __repr__(self):
		return ''
	def __init__(self, in_path=None, out_path=None, report=True):
		if in_path is not None and out_path is not None:
			self.assemble(in_path, out_path, report)

	def init(self, in_path, out_path):
		self.in_path = in_path
		self.out_path = out_path
		self.instrs = InstrList()
		self.symbols = []
		self.start_addr = TEXT_BASE	
		self.lcount = 0
		self.heap_size = 0
		self.pool_size = 0

	def user_report(self):
		print('\t{} instructions({} bytes) - total image size: {} bytes.'.format(str(len(self.instrs)), str(len(self.image)-METADATA_SIZE), str(len(self.image))))
		print('\t{} assembled to {}.'.format(self.in_path, self.out_path))
		
	def assemble(self, in_path, out_path, report):
		self.init(in_path, out_path)
		self.find_labels()
		self.init(in_path, out_path)
		self.build_image()
		self.image.write(self.out_path)
		if report:
			self.user_report()

	def lookup_symbol(self, name):
		for sym in self.symbols:
			if name == sym.name:
				return sym
		return None

	def next_arg_array(self, _default_zero=False):
		self.array = Array()
		while True:
			sym = self.lookup_symbol(self.tok)
			if sym is not None:
				self.array.objs.append(make_tyobj(sym.dtype, sym.value))
				self.i += 1
				try:
					self.tok = self.words[self.i]
				except:
					pass
			elif self.tok in self.labels.keys():
				if self.get_obj(dtype=U64, in_array=True, default_zero=_default_zero) == False:
					return False
			elif self.tok == 'u8':
				if self.get_obj(dtype=U8, in_array=True, default_zero=_default_zero) == False:
					return False
			elif self.tok == 'u64':
				if self.get_obj(dtype=U64, in_array=True, default_zero=_default_zero) == False:
					return False
			elif self.tok == 's64':
				if self.get_obj(dtype=S64, in_array=True, default_zero=_default_zero) == False:
					return False
			elif self.tok == 'r64':
				if self.get_obj(dtype=R64, in_array=True, default_zero=_default_zero) == False:
					return False
			elif self.tok.isdigit():
				if self.get_obj(dtype=U64, in_array=True, default_zero=_default_zero) == False:
					return False
			else:
				if self.get_obj(U64, in_array=True, default_zero=True) == False:
					return False
			if self.i == len(self.words):
				break
		self.args.new_arg(ARR,  self.array)
		return True

	def get_obj(self, dtype, in_array=False, default_zero=False):
		sym = self.lookup_symbol(self.tok)
		if sym:
			if in_array:
				if dtype == R64:
					value = float(self.tok)
				else:
					value = int(self.tok)
				self.array.objs.append(make_tyobj(dtype, value))
				self.i += 1
				try:
					self.tok = self.words[self.i]
				except:
					pass
			else:
				self.args.new_arg(dtype, sym.value)
			return True
		if self.tok in self.labels.keys():
			try:
				value = self.labels[self.tok]
			except:
				value = 0
			if in_array:
				self.array.objs.append(u64(value))
				self.i += 1
				try:
					self.tok = self.words[self.i]
				except:
					pass
			else:
				self.args.new_arg(U64, value)
			return True
		if self.tok in tytypes_map.keys():		
			if self.tok != typecodes[dtype]:
				print('\n\texpected type {} instead was {}, on line {}'.format(typecodes[dtype], self.tok, self.lcount))
				return False
			self.i += 1
			self.tok = self.words[self.i]
		try:
			if in_array:
				if dtype == R64:
					value = float(self.tok)
				else:
					value = int(self.tok)
				self.array.objs.append(make_tyobj(dtype, value))
				self.i += 1
				try:
					self.tok = self.words[self.i]
				except:
					pass
			else:
				if dtype == R64:
					value = float(self.tok)
				else:
					value = int(self.tok)
				self.args.new_arg(dtype, value)
			return True
		except:
			if default_zero:
				if in_array:
					self.array.objs.append(u64(METADATA_SIZE))
					self.i += 1
					try:
						self.tok = self.words[self.i]
					except:
						pass
				else:
					self.args.new_arg(U64, METADATA_SIZE)
				return True
			else:
				print('\n\tfailed to make {} value from {}, on line {}'.format(typecodes[dtype], self.tok, self.lcount))
				return False

	def find_heap_size(self):
		self.i += 1
		self.tok = self.words[self.i]
		try:
			self.heapsize = int(self.tok)
			return True
		except:
			print('\n\tinvalid arg to heapsize directive, on line {}.'.format(self.lcount))
			return False

	def new_label(self):
		if self.tok not in self.labels.keys():
			self.labels[self.tok] = self.instrs.next_addr()
			return True
		else:
			print('\n\talready a label by the name of {}, on line {}.'.format(self.tok, self.lcount))
			return False

	def new_symbol(self):
		self.i += 1
		self.tok = self.words[self.i]
		if self.lookup_symbol(self.tok) is not None:
			print('\n\tfailed to make new symbol, on line {}.'.format(self.lcount))
			return False
		sym = symbol()
		sym.name = self.tok
		self.i += 1
		self.tok = self.words[self.i]
		if self.tok in tytypes_map.keys():
			sym.dtype = tytypes_map[self.tok]
			self.i += 1
			self.tok = self.words[self.i]
		else:
			sym.dtype = U64
		try:
			sym.value = int(self.tok)
		except:
			print('\n\tfailed to make new symbol, on line {}.'.format(self.lcount))
			return False
		self.symbols.append(sym)
		return True

	def build_image(self):
		try:
			self.lines = open(self.in_path, 'r').readlines()
		except:
			print('\n\tfailed to open "{}".'.format(self.in_path))
			raise Exception()
		for self.line in self.lines:
			self.lcount += 1
			self.words = self.line.split()
			self.i = -1
			while True:
				self.i += 1
				try:
					self.tok = self.words[self.i]
				except:
					break
				if self.tok == 'heap':
					if self.find_heap_size() == False:
						return
					continue
				elif self.tok == 'sym':
					if self.new_symbol() == False:
						return
					break
				elif self.tok == 'start:':
					self.start_addr = self.instrs.next_addr()
					self.labels['start'] = self.instrs.next_addr()
					break
				elif self.tok.endswith(':'):
					continue
				elif self.tok in opmap.keys():
					self.opcode = from_opname(self.tok)
					if self.opcode in no_arg_ops:
						self.instrs.new_instr(self.instrs.next_addr(), self.opcode)
						continue
					elif self.opcode == PUT_S:
						self.args = ArgList()
						self.array = Array()
						string = []
						self.i += 1
						self.tok = self.words[self.i]
						if self.get_obj(U64, in_array=True) == False:
							return
						if self.tok != '<':
							print('\n\tout of place token on line {}'.format(self.lcount))
							raise Exception()
						else:
							while True:
								self.i += 1
								self.tok = self.words[self.i]
								if self.tok == '>':
									break
								string.append(self.tok)
							string = ' '.join(string)
							string = string[:-1]
							string = string[1:]
							for ch in string:
								self.array.objs.append(u8(ord(ch)))
							self.array.objs.append(u8(0))
						self.args.new_arg(ARR,  self.array)
						self.instrs.new_instr(self.instrs.next_addr(), self.opcode, self.args)
						continue
					else:
						self.args = ArgList()
						self.i += 1
						self.tok = self.words[self.i]
						if self.next_arg_array() == False:
							return
						self.instrs.new_instr(self.instrs.next_addr(), self.opcode, self.args)
						continue
				else:
					print('\n\tout of place token on line {}'.format(self.lcount))
					raise Exception()
		self.metadata = Metadata(self.instrs.byte_len(), self.start_addr, self.pool_size, self.heap_size)
		self.image = TextImage(self.metadata, self.instrs)

	def find_labels(self):
		self.labels = {}
		try:
			self.lines = open(self.in_path, 'r').readlines()
		except:
			print('\n\tfailed to open "{}".'.format(self.in_path))
			raise Exception()
		for self.line in self.lines:
			self.lcount += 1
			self.words = self.line.split()
			self.i = -1
			while True:
				self.i += 1
				try:
					self.tok = self.words[self.i]
				except:
					break
				if self.tok == 'heap':
					if self.find_heap_size() == False:
						return
					continue
				elif self.tok == 'sym':
					if self.new_symbol() == False:
						return
					break
				elif self.tok == 'start:':
					self.start_addr = self.instrs.next_addr()
					self.labels['start'] = self.instrs.next_addr()
					break
				elif self.tok.endswith(':'):
					self.tok = self.tok[:-1]
					if self.new_label() == False:
						return
					break
				elif self.tok in opmap.keys():
					self.opcode = from_opname(self.tok)
					if self.opcode in no_arg_ops:
						self.instrs.new_instr(self.instrs.next_addr(), self.opcode)
						continue
					elif self.opcode == PUT_S:
						self.args = ArgList()
						self.array = Array()
						string = []
						self.i += 1
						self.tok = self.words[self.i]
						if self.get_obj(dtype=U64, in_array=True, default_zero=True) == False:
							return
						if self.tok != '<':
							print('\n\tout of place token on line {}'.format(self.lcount))
							raise Exception()
						else:
							while True:
								self.i += 1
								self.tok = self.words[self.i]
								if self.tok == '>':
									break
								string.append(self.tok)
							string = ' '.join(string)
							string = string[:-1]
							string = string[1:]
							for ch in string:
								self.array.objs.append(u8(ord(ch)))
							self.array.objs.append(u8(0))
						self.args.new_arg(ARR,  self.array)
						self.instrs.new_instr(self.instrs.next_addr(), self.opcode, self.args)
						continue
					else:
						self.args = ArgList()
						self.i += 1
						self.tok = self.words[self.i]
						if self.next_arg_array(_default_zero=True) == False:
							return
						self.instrs.new_instr(self.instrs.next_addr(), self.opcode, self.args)
						continue
				else:
					print('\n\tout of place token on line {}'.format(self.lcount))
					raise Exception()

if __name__ == '__main__':
	if len(sys.argv) == 4:
		in_path = str(sys.argv[1])
		out_path = str(sys.argv[2])
		report = bool(sys.argv[3])
		assembler(in_path, out_path, report)
		quit()
	else:
		print('\n\tinvalid input to assembler.')
		quit()
