#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "tyson.h"
#include "opcodes.h"

// VM compilation mode is specified by the macro below.
// Set to DEBUG_MODE or REAL_MODE.
#define DEBUG_MODE

#define next_op() \
	goto *optable[*ip]

// VM compilation mode is specified by the macro below.
// Set to DEBUG_MODE or REAL_MODE.
#define DEBUG_MODE

// Define the next_cycle() macro.
#ifdef DEBUG_MODE
	#define next_cycle()      \
    {	if (db_dostep) {      \
			goto debug_start; \
		} else {              \
			next_op();        \
    }
#else
	#define next_cycle() \
		next_op();
#endif


#define stack_byte(offset) \
	(sp - offset)

#define img_byte(offset) \
	((pro->img) + (offset))

#define sp_offset() \
	((u64) (sp - stk))

void
execute_process(Process* pro)
{
	build_optable();

	// Initialise work stack.
	u8  stk[STACK_SIZE]; // array.
	u8* sp = stk; // stack-pointer.

	// Initialise return stack.
	u8* rstk[RECUR_LIMIT]; // pointer-array.
	u8** rp = rstk;        // return-pointer.
	*rstk = img_byte(TEXT_BASE);
	
	// Initialise instruction-pointer.
	u8*  ip = pro->start_byte;

	// Declare loop vars
	u8*  lp_cont;
	u8*  lp_stop;
	u64  lp_count;

	// Declare table pointer.
	u8*  tdx;

	// Declare fast-jump pointers.
	u8 *c1, *c2, *c3, *c4;
	
	// Internal data pointers.
	word *Wp1, *Wp2, *Wp3;
	w64  *wp1, *wp2, *wp3;
	u8   *bp1, *bp2, *bp3;
	u64  *up1, *up2, *up3;
	s64  *ip1, *ip2, *ip3;
	r64  *rp1, *rp2, *rp3;

	// Internal buffers for use by currently executing instr.
	// Each instr must init these itself, cleanup not required.
	u8  dbuf[DATABUF_SIZE];
	u64 c; // general purpose counter.

	#ifdef DEBUG_MODE
	// Set up debug-mode variables.
	build_dbtable();
	u64 cycnum = 0;
    u8  db_dostep = 1;
	u8  db_input;
	clock_t toc;
	clock_t tic = clock();
	#endif

	// runtime entry-point:
	next_cycle();

  	// Instruction Blocks.
	die:
		#ifdef DEBUG_MODE
		toc = clock();
		++cycnum;
		printf("\n\tDIE executed on cycle %u\n", (unsigned) cycnum);
		printf("\n\t   runtime: %f secs", (double) (toc - tic) / CLOCKS_PER_SEC);
		goto db_end;
		#else
		return;
		#endif
	nop:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tNOP executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip; // point ip at next opcode in sequence.
		next_cycle();
	jmp:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tJMP executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip; // point ip at first arg, jump-target address.
		up1 = (u64*) ip; // get u64 pointer to said arg.
		ip = img_byte(*up1); // set ip at jump-target.
		next_cycle();
	call:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tCALL executed on cycle %u", (unsigned) cycnum);
		#endif
		++rp; // inc ret-pointer so ret-stack is ready for push.
		++ip; // point ip at first arg, jump-target address.
		*rp = ip + wordsize; // set rp to the first byte after this instr, the ret address.
		up1 = (u64*) ip; // get u64 pointer to jump-target.
		ip = img_byte(*up1); // set ip to jump-target.
		next_cycle();
	ret:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tRET executed on cycle %u", (unsigned) cycnum);
		#endif
		ip = *rp; // set ip to current return address, top of ret-stack.
		--rp; // dec rp so top of ret-stack is the correct ret adress.
		next_cycle();
	swch:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSWCH executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip; // point ip at jump-tbl base.
		up1 = (u64*) sp; // get pointer to index value.
		sp -= wordsize; // swch auto-pops jump-tbl index value off top.
		up2 = (u64*) (ip + (*up1)); // index jump-tbl with index value to yield target address.
		ip = img_byte(*up2); // set ip to target address then execute next.
		next_cycle();
	jeq_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJEQ_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		bp2 = sp - wordsize;
		if ((*bp1) == (*bp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jneq_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJNEQ_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		bp2 = sp - wordsize;
		if ((*bp1) != (*bp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jeq_w:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJEQ_W executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		wp1 = (w64*) sp;
		wp2 = (w64*) (sp - wordsize);
		if ((*wp1) == (*wp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jneq_w:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJNEQ_W executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		wp1 = (w64*) sp;
		wp2 = (w64*) (sp - wordsize);
		if ((*wp1) != (*wp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jgeq_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJGEQ_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		bp2 = sp - wordsize;
		if ((*bp1) >= (*bp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jleq_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJLEQ_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		bp2 = sp - wordsize;
		if ((*bp1) <= (*bp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jgt_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJGT_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		bp2 = sp - wordsize;
		if ((*bp1) > (*bp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jlt_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJLT_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		bp2 = sp - wordsize;
		if ((*bp1) < (*bp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jgeq_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJGEQ_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		up2 = (u64*) (sp - wordsize);
		if ((*up1) >= (*up2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jleq_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJLEQ_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		up2 = (u64*) (sp - wordsize);
		if ((*up1) <= (*up2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jgt_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJGT_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		up2 = (u64*) (sp - wordsize);
		if ((*up1) > (*up2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jlt_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJLT_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		up2 = (u64*) (sp - wordsize);
		if ((*up1) < (*up2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jgeq_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJGEQ_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		ip2 = (s64*) (sp - wordsize);
		if ((*ip1) >= (*ip2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jleq_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJLEQ_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		ip2 = (s64*) (sp - wordsize);
		if ((*ip1) <= (*ip2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jgt_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJGT_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		ip2 = (s64*) (sp - wordsize);
		if ((*ip1) > (*ip2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jlt_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJGT_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		ip2 = (s64*) (sp - wordsize);
		if ((*ip1) < (*ip2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jgeq_r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJGEQ_R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp1 = (r64*) sp;
		rp2 = (r64*) (sp - wordsize);
		if ((*rp1) >= (*rp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jleq_r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tJLEQ_R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp1 = (r64*) sp;
		rp2 = (r64*) (sp - wordsize);
		if ((*rp1) <= (*rp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jgt_r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tJGT_R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp1 = (r64*) sp;
		rp2 = (r64*) (sp - wordsize);
		if ((*rp1) > (*rp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jlt_r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tJGT_R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp1 = (r64*) sp;
		rp2 = (r64*) (sp - wordsize);
		if ((*rp1) < (*rp2)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += 8;
		}
		next_cycle();
	jmp_c1:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tJMP_C1 executed on cycle %u", (unsigned) cycnum);
		#endif
		ip = c1;
		next_cycle();
	jmp_c2:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tJMP_C2 executed on cycle %u", (unsigned) cycnum);
		#endif
		ip = c2;
		next_cycle();
	jmp_c3:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tJMP_C3 executed on cycle %u", (unsigned) cycnum);
		#endif
		ip = c3;
		next_cycle();
	jmp_c4:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tJMP_C4 executed on cycle %u", (unsigned) cycnum);
		#endif
		ip = c3;
		next_cycle();
	set_c1:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSET_C1 executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		c1 = img_byte(*up1);
		ip += wordsize;
		next_cycle();
	set_c2:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSET_C2 executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		c2 = img_byte(*up1);
		ip += wordsize;
		next_cycle();
	set_c3:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSET_C3 executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		c3 = img_byte(*up1);
		ip += wordsize;
		next_cycle();
	set_c4:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSET_C4 executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		c4 = img_byte(*up1);
		ip += wordsize;
		next_cycle();
	eq:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tEQ executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		wp1 = (w64*) sp;
		wp2 = (w64*) (sp - wordsize);
		sp += wordsize;
		up1 = (u64*) sp;
		if ((*wp1) == (*wp2))
			*up1 = TRUE;
		else
			*up1 = FALSE;
		next_cycle();
	neq:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tNEQ executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		wp1 = (w64*) sp;
		wp2 = (w64*) (sp - wordsize);
		sp += wordsize;
		up1 = (u64*) sp;
		if ((*wp1) != (*wp2))
			*up1 = TRUE;
		else
			*up1 = FALSE;
		next_cycle();
	and:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tAND executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		wp1 = (w64*) sp;
		wp2 = (w64*) (sp - wordsize);
		sp -= dwordsize;
		wp3 = (w64*) sp;
		*wp3 = ((*wp1) & (*wp2));
		next_cycle();
	not:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tOR executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		wp1 = (w64*) sp;
		sp -= wordsize;
		wp2 = (u64*) sp;
		*wp2 = !(*wp1);
		next_cycle();
	or:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tOR executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		wp1 = (w64*) sp;
		wp2 = (w64*) (sp - wordsize);
		sp -= dwordsize;
		wp3 = (w64*) sp;
		*wp3 = ((*wp1) | (*wp2));
		next_cycle();
	xor:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tXOR executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		wp1 = (w64*) sp;
		wp2 = (w64*) (sp - wordsize);
		sp -= dwordsize;
		wp3 = (w64*) sp;
		*wp3 = ((*wp1) ^ (*wp2));
		next_cycle();
	lsh:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tLSH executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		wp1 = (w64*) sp;
		wp2 = (w64*) (sp - wordsize);
		sp -= dwordsize;
		wp3 = (w64*) sp;
		*wp3 = ((*wp1) << (*wp2));
		next_cycle();
	rsh:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tRSH executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		wp1 = (w64*) sp;
		wp2 = (w64*) (sp - wordsize);
		sp -= dwordsize;
		wp3 = (w64*) sp;
		*wp3 = ((*wp1) >> (*wp2));
		next_cycle();
	inc_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tINC_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		++(*sp);
		next_cycle();
	inc_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tINC_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		++(*up1);
		next_cycle();
	inc_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tINC_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		++(*ip1);
		next_cycle();
	dec_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tDEC_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		--(*sp);
		next_cycle();
	dec_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tDEC_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		--(*up1);
		next_cycle();
	dec_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tDEC_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		--(*ip1);
		next_cycle();
	add_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tADD_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		bp2 = (sp - wordsize);
		sp -= dwordsize;
		*sp = ((*bp1) + (*bp2));
		next_cycle();
	add_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tADD_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		up2 = (u64*) (sp - wordsize);
		sp -= dwordsize;
		up3 = (w64*) sp;
		*up3 = ((*up1) + (*up2));
		next_cycle();
	add_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tADD_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		ip2 = (s64*) (sp - wordsize);
		sp -= dwordsize;
		ip3 = (s64*) sp;
		*ip3 = ((*ip1) + (*ip2));
		next_cycle();
	add_r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tADD_R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp1 = (r64*) sp;
		rp2 = (r64*) (sp - wordsize);
		sp -= dwordsize;
		rp3 = (r64*) sp;
		*rp3 = ((*rp1) + (*rp2));
		next_cycle();
	sub_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSUB_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		bp2 = (sp - wordsize);
		sp -= dwordsize;
		*sp = ((*bp1) - (*bp2));
		next_cycle();
	sub_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSUB_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		up2 = (u64*) (sp - wordsize);
		sp -= dwordsize;
		up3 = (w64*) sp;
		*up3 = ((*up1) - (*up2));
		next_cycle();
	sub_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSUB_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		ip2 = (s64*) (sp - wordsize);
		sp -= dwordsize;
		ip3 = (s64*) sp;
		*ip3 = ((*ip1) - (*ip2));
		next_cycle();
	sub_r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSUB_R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp1 = (r64*) sp;
		rp2 = (r64*) (sp - wordsize);
		sp -= dwordsize;
		rp3 = (r64*) sp;
		*rp3 = ((*rp1) - (*rp2));
		next_cycle();
	mul_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tMUL_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		bp2 = (sp - wordsize);
		sp -= dwordsize;
		*sp = ((*bp1) * (*bp2));
		next_cycle();
	mul_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tMUL_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		up2 = (u64*) (sp - wordsize);
		sp -= dwordsize;
		up3 = (w64*) sp;
		*up3 = ((*up1) * (*up2));
		next_cycle();
	mul_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tMUL_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		ip2 = (s64*) (sp - wordsize);
		sp -= dwordsize;
		ip3 = (s64*) sp;
		*ip3 = ((*ip1) * (*ip2));
		next_cycle();
	mul_r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tMUL_R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp1 = (r64*) sp;
		rp2 = (r64*) (sp - wordsize);
		sp -= dwordsize;
		rp3 = (r64*) sp;
		*rp3 = ((*rp1) * (*rp2));
		next_cycle();
	div_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tDIV_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		bp2 = (sp - wordsize);
		sp -= dwordsize;
		*sp = ((*bp1) / (*bp2));
		next_cycle();
	div_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tDIV_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		up2 = (u64*) (sp - wordsize);
		sp -= dwordsize;
		up3 = (w64*) sp;
		*up3 = ((*up1) / (*up2));
		next_cycle();
	div_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tDIV_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		ip2 = (s64*) (sp - wordsize);
		sp -= dwordsize;
		ip3 = (s64*) sp;
		*ip3 = ((*ip1) / (*ip2));
		next_cycle();
	div_r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tDIV_R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp1 = (r64*) sp;
		rp2 = (r64*) (sp - wordsize);
		sp -= dwordsize;
		rp3 = (r64*) sp;
		*rp3 = ((*rp1) / (*rp2));
		next_cycle();
	mod_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tMOD_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		bp2 = (sp - wordsize);
		sp -= dwordsize;
		*sp = ((*bp1) % (*bp2));
		next_cycle();
	mod_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tMOD_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		up2 = (u64*) (sp - wordsize);
		sp -= dwordsize;
		up3 = (w64*) sp;
		*up3 = ((*up1) % (*up2));
		next_cycle();
	mod_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tMOD_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		ip2 = (s64*) (sp - wordsize);
		sp -= dwordsize;
		ip3 = (s64*) sp;
		*ip3 = ((*ip1) % (*ip2));
		next_cycle();
	b2u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tB2U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) dbuf;
		*up1 = (u64) (*sp);
		up2 = (u64*) sp;
		*up2 = *up1;
		next_cycle();
	b2i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tB2I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) dbuf;
		*ip1 = (s64) (*sp);
		ip2 = (s64*) sp;
		*ip2 = *ip1;
		next_cycle();
	b2r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tB2R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;/*
		rp1 = (r64*) dbuf;
		*rp1 = (r64) (*sp);
		rp2 = (r64) sp;
		*rp2 = *rp1;*/ // fix all commented bullshit here.
		next_cycle();
	u2b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tU2B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		*sp = (u8) (*up1);
		next_cycle();
	u2i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tU2I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		ip1 = (s64*) dbuf;
		*ip1 = (s64) (*up1);
		ip2 = (s64*) sp;
		*ip2 = *ip1;
		next_cycle();
	u2r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tU2R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		rp1 = (r64*) dbuf;
		*up1 = (u64) (*up1);
		rp2 = (r64*) sp;
		*rp2 = *rp1;
		next_cycle();
	i2b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tI2B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		*sp = (u8) (*ip1);
		next_cycle();
	i2u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tI2U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		up1 = (u64*) dbuf;
		*up1 = (u64) (*ip1);
		up2 = (u64*) sp;
		*up2 = *up1;
		next_cycle();
	i2r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tI2R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		ip1 = (s64*) sp;
		rp1 = (r64*) dbuf;
		*ip1 = (s64) (*up1);
		rp2 = (r64*) sp;
		*rp2 = *rp1;
		next_cycle();
	r2b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tR2B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp1 = (r64*) sp;
		*sp = (u8) (*rp1);
		next_cycle();
	r2u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tR2U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp1 = (r64*) sp;
		up1 = (u64*) dbuf;
		*up1 = (u64) (*rp1);
		up2 = (u64*) sp;
		*up2 = *up1;
		next_cycle();
	r2i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tR2I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp1 = (r64*) sp;
		ip1 = (s64*) dbuf;
		*ip1 = (s64) (*up1);
		ip2 = (s64*) sp;
		*ip2 = *ip1;
		next_cycle();
	lstart:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tLSTART executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		lp_count = (*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		up2 = (u64*) ip;
		lp_cont = img_byte(*up1);
		lp_stop = img_byte(*up2);
		ip = lp_cont;
		next_cycle();
	ltest:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tLTEST executed on cycle %u", (unsigned) cycnum);
		#endif
		if (lp_count) {
			--lp_count;
			ip = lp_cont;
		} else {
			ip = lp_stop;
		}
		next_cycle();
	lcont:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tLCONT executed on cycle %u", (unsigned) cycnum);
		#endif
		ip = lp_cont;
		next_cycle();
	lstop:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tLSTOP executed on cycle %u", (unsigned) cycnum);
		#endif
		ip = lp_stop;
		next_cycle();
	put_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tPUT_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		*bp1 = *ip;
		++ip;
		next_cycle();
	put_nb:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tPUT_NB executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		memcpy(bp1, ip, (*up1));
		ip += (*up1);
		next_cycle();
	put_hw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tPUT_HW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		memcpy(bp1, ip, hwordsize);
		ip += hwordsize;
		next_cycle();
	put_w:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tPUT_W executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		memcpy(bp1, ip, wordsize);
		ip += wordsize;
		next_cycle();
	put_nw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tPUT_NW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		memcpy(bp1, ip, (wordsize * (*up1)));
		ip += (wordsize * (*up1));
		next_cycle();
	put_dw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tPUT_DW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		memcpy(bp1, ip, dwordsize);
		ip += dwordsize;
		next_cycle();
	put_qw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tPUT_QW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		memcpy(bp1, ip, qwordsize);
		ip += qwordsize;
		next_cycle();
	put_s:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tPUT_S executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		c = strlen(ip) + 1;
		memcpy(bp1, ip, c);
		ip += c;
		next_cycle();
	cpy_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tCPY_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		*bp1 = *bp2;
		next_cycle();
	cpy_nb:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tCPY_NB executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		memcpy(bp1, bp2, (*up1));
		next_cycle();
	cpy_hw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tCPY_HW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		memcpy(bp1, bp2, hwordsize);
		next_cycle();
	cpy_w:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tCPY_W executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		memcpy(bp1, bp2, wordsize);
		next_cycle();
	cpy_nw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tCPY_NW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		memcpy(bp1, ip, (wordsize * (*up1)));
		next_cycle();
	cpy_dw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tCPY_DW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		memcpy(bp1, bp2, dwordsize);
		next_cycle();
	cpy_qw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tCPY_QW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		memcpy(bp1, bp2, qwordsize);
		next_cycle();
	cpy_s:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tCPY_S executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		strcpy(bp1, bp2);
		next_cycle();
	xch_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tXCH_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		bp3 = dbuf;
		*bp3 = *bp1;
		*bp1 = *bp2;
		*bp2 = *bp3;
		next_cycle();
	xch_nb:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tXCH_NB executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		bp3 = dbuf;
		memcpy(bp3, bp1, (*up1));
		memcpy(bp1, bp2, (*up1));
		memcpy(bp2, bp3, (*up1));
		next_cycle();
	xch_hw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tXCH_HW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		bp3 = dbuf;
		memcpy(bp3, bp1, hwordsize);
		memcpy(bp1, bp2, hwordsize);
		memcpy(bp2, bp3, hwordsize);
		next_cycle();
	xch_w:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tXCH_W executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		bp3 = dbuf;
		memcpy(bp3, bp1, wordsize);
		memcpy(bp1, bp2, wordsize);
		memcpy(bp2, bp3, wordsize);
		next_cycle();
	xch_nw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tXCH_NW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		bp3 = dbuf;
		memcpy(bp3, bp1, (wordsize * (*up1)));
		memcpy(bp1, bp2, (wordsize * (*up1)));
		memcpy(bp2, bp3, (wordsize * (*up1)));
		next_cycle();
	xch_qw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tXCH_QW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		bp3 = dbuf;
		memcpy(bp3, bp1, qwordsize);
		memcpy(bp1, bp2, qwordsize);
		memcpy(bp2, bp3, qwordsize);
		next_cycle();
	xch_dw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tXCH_DW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		bp3 = dbuf;
		memcpy(bp3, bp1, dwordsize);
		memcpy(bp1, bp2, dwordsize);
		memcpy(bp2, bp3, dwordsize);
		next_cycle();
	xch_s:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tXCH_S executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		bp3 = dbuf;
		strcpy(bp3, bp1);
		strcpy(bp1, bp2);
		strcpy(bp2, bp3);
		next_cycle();
	rstk_up:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrstk_up executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		++rp;
		next_cycle();		
	rstk_dwn:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrstk_dwn executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		--rp;
		next_cycle();		
	rstk_rst:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrstk_rst executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		rp = rstk;
		next_cycle();
	rsv_sys1:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io1 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys2:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io2 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys3:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io3 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys4:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io4 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys5:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io5 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys6:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io6 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys7:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io7 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys8:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io8 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys9:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io9 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys10:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io10 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys11:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io6 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys12:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io7 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys13:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io8 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys14:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io9 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	rsv_sys15:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nrsv_io10 executed on cycle %u", (unsigned) cycnum);
		#endif
		return;
	put_b_fs:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tPUT_B_FS executed on cycle %u", (unsigned) cycnum);
		#endif
		up1 = (u64*) sp;
		bp1 = img_byte(*up1);
		++ip;
		*bp1 = *ip;
		++ip;
		next_cycle();
	put_w_fs:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tPUT_W_FS executed on cycle %u", (unsigned) cycnum);
		#endif
		up1 = (u64*) sp;
		bp1 = img_byte(*up1);
		++ip;
		memcpy(bp1, ip, wordsize);
		ip += wordsize;
		next_cycle();
	cpy_b_fs:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tCPY_B_FS executed on cycle %u", (unsigned) cycnum);
		#endif
		up1 = (u64*) sp;
		bp1 = img_byte(*up1);
		++ip;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		*bp1 = *bp2;
		next_cycle();
	cpy_w_fs:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tCPY_W_FS executed on cycle %u", (unsigned) cycnum);
		#endif
		up1 = (u64*) sp;
		bp1 = img_byte(*up1);
		++ip;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		memcpy(bp1, bp2, wordsize);
		next_cycle();
	xch_b_fs:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tXCH_B_FS executed on cycle %u", (unsigned) cycnum);
		#endif
		up1 = (u64*) sp;
		bp1 = img_byte(*up1);
		++ip;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		bp3 = dbuf;
		*bp3 = *bp2;
		*bp2 = *bp1;
		*bp1 = *bp3;
		next_cycle();
	xch_w_fs:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tXCH_W_FS executed on cycle %u", (unsigned) cycnum);
		#endif
		up1 = (u64*) sp;
		bp1 = img_byte(*up1);
		++ip;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		bp3 = dbuf;
		memcpy(bp3, bp2, wordsize);
		memcpy(bp2, bp1, wordsize);
		memcpy(bp1, bp3, wordsize);
		next_cycle();
	set_tdx_fc:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSET_TDX_FC executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		tdx = img_byte(*up1);
		ip += wordsize;
		next_cycle();
	set_tdx_fh:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSET_TDX_FH executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		up1 = (u64*) img_byte(*up1);
		ip += wordsize;
		tdx = img_byte(*up1);
		next_cycle();
	set_tdx_fs:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSET_TDX_FH executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		tdx = img_byte(*up1);
		next_cycle();
	tdx_b_up:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tTDX_B_UP executed on cycle %u", (unsigned) cycnum);
		#endif
		++tdx;
		++ip;
		next_cycle();	
	tdx_b_dwn:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tTDX_B_DWN executed on cycle %u", (unsigned) cycnum);
		#endif
		--tdx;
		++ip;
		next_cycle();	
	tdx_w_up:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tTDX_W_UP executed on cycle %u", (unsigned) cycnum);
		#endif
		tdx += wordsize;
		++ip;
		next_cycle();
	tdx_w_dwn:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tTDX_W_DWN executed on cycle %u", (unsigned) cycnum);
		#endif
		tdx -= wordsize;
		++ip;
		next_cycle();
	t_fd_putb:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_FD_PUTB executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		*tdx = *ip;
		++tdx;
		++ip;
		next_cycle();
	t_bk_putb:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_BK_PUTB executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		*tdx = *ip;
		--tdx;
		++ip;
		next_cycle();
	t_fd_putw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_FD_PUTW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		memcpy(tdx, ip, wordsize);
		tdx += wordsize;
		ip += wordsize;
		next_cycle();
	t_bk_putw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_BK_PUTW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		memcpy(tdx, ip, wordsize);
		tdx -= wordsize;
		ip += wordsize;
		next_cycle();
	t_fd_cpyb:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_FD_CPYB executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		*tdx = *bp1;
		++tdx;
		next_cycle();
	t_bk_cpyb:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_BK_CPYB executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		*tdx = *bp1;
		--tdx;
		next_cycle();
	t_fd_cpyw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_FD_CPYW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		memcpy(tdx, bp1, wordsize);
		++tdx;
		next_cycle();
	t_bk_cpyw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_BK_CPYW executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		memcpy(tdx, bp1, wordsize);
		++tdx;
		next_cycle();
	t_fd_popb:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_FD_POPB executed on cycle %u", (unsigned) cycnum);
		#endif
		*sp = *tdx;
		sp -= wordsize;
		++tdx;
		++ip;
		next_cycle();
	t_bk_popb:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_BK_POPB executed on cycle %u", (unsigned) cycnum);
		#endif
		*sp = *tdx;
		sp -= wordsize;
		--tdx;
		++ip;
		next_cycle();
	t_fd_popw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_FD_POPW executed on cycle %u", (unsigned) cycnum);
		#endif
		memcpy(tdx, sp, wordsize);
		sp -= wordsize;
		++tdx;
		++ip;
		next_cycle();
	t_bk_popw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_BK_POPW executed on cycle %u", (unsigned) cycnum);
		#endif
		memcpy(tdx, sp, wordsize);
		sp -= wordsize;
		--tdx;
		++ip;
		next_cycle();
	t_fd_pshb:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_FD_PSHB executed on cycle %u", (unsigned) cycnum);
		#endif
		sp += wordsize;
		*sp = *tdx;
		++tdx;
		++ip;
		next_cycle();
	t_bk_pshb:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_BK_PSHB executed on cycle %u", (unsigned) cycnum);
		#endif
		sp += wordsize;
		*sp = *tdx;
		--tdx;
		++ip;
		next_cycle();
	t_fd_pshw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_FD_PSHW executed on cycle %u", (unsigned) cycnum);
		#endif
		sp += wordsize;
		memcpy(sp, tdx, wordsize);
		tdx += wordsize;
		ip += wordsize;
		next_cycle();
	t_bk_pshw:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nT_BK_PSHW executed on cycle %u", (unsigned) cycnum);
		#endif
		sp += wordsize;
		memcpy(sp, tdx, wordsize);
		tdx -= wordsize;
		ip += wordsize;
		next_cycle();
	stk_spoffs:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_SPOFFS executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		sp += wordsize;
		up1 = (u64*) sp;
		*up1 = sp_offset();
		next_cycle();	
	stk_save:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_SAVE executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		memcpy(bp1, stk, STACK_SIZE);
		ip += wordsize;
		next_cycle();
	stk_load:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_LOAD executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		memcpy(stk, bp1, STACK_SIZE);
		ip += wordsize;
		next_cycle();
	stk_up:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_UP executed on cycle %u", (unsigned) cycnum);
		#endif
		sp += wordsize;
		++ip;
		next_cycle();
	stk_dwn:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_DOWN executed on cycle %u", (unsigned) cycnum);
		#endif
		sp -= wordsize;
		++ip;
		next_cycle();
	stk_rst:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_DOWN executed on cycle %u", (unsigned) cycnum);
		#endif
		sp = stk;
		++ip;
		next_cycle();
	stk_clr:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_DOWN executed on cycle %u", (unsigned) cycnum);
		#endif
		memset(stk, 0, STACK_SIZE);
		sp = stk;
		++ip;
		next_cycle();
	stk_set:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_SET executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = sp - (*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		memcpy(bp1, bp2, wordsize);
		ip += wordsize;
		next_cycle();
	stk_setn:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_SETN executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = sp - (*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		memcpy(bp1, bp2, (*up1));
		ip += wordsize;
		next_cycle();
	stk_setc:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_SETC executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = sp - (*up1);
		ip += wordsize;
		memcpy(bp1, ip, wordsize);
		ip += wordsize;
		next_cycle();
	stk_setcn:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_SETCN executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = sp - (*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = ip;
		ip += wordsize;
		up1 = (u64*) ip;
		memcpy(bp1, bp2, (*up1));
		ip += wordsize;
		next_cycle();
	stk_cpy:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_CPY executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = (sp - (*up1));
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = (sp - (*up1));
		memcpy(bp1, bp2, wordsize);
		ip += wordsize;
		next_cycle();
	stk_cpyn:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_CPYN executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = (sp - (*up1));
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = (sp - (*up1));
		ip += wordsize;
		up1 = (u64*) ip;
		memcpy(bp1, bp2, (*up1));
		ip += wordsize;
		next_cycle();
	stk_xch:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_XCH executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = (sp - (*up1));
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = (sp - (*up1));
		ip += wordsize;
		bp3 = dbuf;
		memcpy(bp3, bp2, wordsize);
		memcpy(bp2, bp1, wordsize);
		memcpy(bp1, bp3, wordsize);
		next_cycle();
	stk_xchn:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_XCHN executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = (sp - (*up1));
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = (sp - (*up1));
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		bp3 = dbuf;
		memcpy(bp3, bp2, (*up1));
		memcpy(bp2, bp1, (*up1));
		memcpy(bp1, bp3, (*up1));
		next_cycle();
	stk_hxch:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_HXCH executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = (sp - (*up1));
		ip += wordsize;
		bp3 = dbuf;
		memcpy(bp3, bp2, wordsize);
		memcpy(bp2, bp1, wordsize);
		memcpy(bp1, bp3, wordsize);
		next_cycle();
	stk_hxchn:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_HXCHN executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = (sp - (*up1));
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		bp3 = dbuf;
		memcpy(bp3, bp2, (*up1));
		memcpy(bp2, bp1, (*up1));
		memcpy(bp1, bp3, (*up1));
		next_cycle();
	stk_mov:
		return;
	stk_movn:
		return;
	stk_del:
		return;
	stk_deln:
		return;
	stk_get:
		return;
	stk_getn:
		return;
	stk_ins:
		return;
	stk_insn:
		return;
	stk_2top:
		return;
	stk_xt_dup:
		return;
	stk_tx_dup:
		return;
	stk_tt_dup:
		return; // REDUNDANT INSTRUCTION REMOVAL PERMENENTLY!
	stk_top_dup:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTK_TOP_DUP executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		sp += wordsize;
		memcpy(sp, bp1, wordsize);
		next_cycle();
	stk_top_dup2:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_TOP_DUP executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		bp1 = sp;
		sp += wordsize;
		memcpy(sp, bp1, wordsize);
		sp += wordsize;
		memcpy(sp, bp1, wordsize);
		next_cycle();
	stk_dup:
		return;
	stk_tapsh:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_TAPSH executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		bp1 = img_byte(*up1);
		memcpy(sp, bp1, wordsize);
		next_cycle();
	stk_psh:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_PSH executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		sp += wordsize;
		memcpy(sp, bp1, wordsize);
		ip += wordsize;
		next_cycle();
	stk_pshc:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_PSHC executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		sp += wordsize;
		memcpy(sp, ip, wordsize);
		ip += wordsize;
		next_cycle();
	stk_psh0:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_PSH0 executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		sp += wordsize;
		up1 = (u64*) sp;
		*up1 = 0;
		next_cycle();
	stk_psh1:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_PSH1 executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		sp += wordsize;
		up1 = (u64*) sp;
		*up1 = 1;
		next_cycle();
	stk_psh2:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_PSH2 executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		sp += wordsize;
		up1 = (u64*) sp;
		*up1 = 2;
		next_cycle();
	stk_ovwr:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_OVWR executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		memcpy(sp, bp1, wordsize);
		ip += wordsize;
		next_cycle();
	stk_ovwrc:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_OVWRC executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		memcpy(sp, ip, wordsize);
		ip += wordsize;
		next_cycle();
	stk_ovwr0:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_OVWR0 executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		*up1 = 0;
		next_cycle();
	stk_ovwr1:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_OVWR1 executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		*up1 = 1;
		next_cycle();
	stk_ovwr2:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTK_OVWR2 executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) sp;
		*up1 = 2;
		next_cycle();
	stk_stor:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTR_STOR executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		memcpy(bp1, sp, wordsize);
		ip += wordsize;
		next_cycle();
	stk_pop:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTR_POP executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		memcpy(bp1, sp, wordsize);
		sp -= wordsize;
		ip += wordsize;
		next_cycle();
	stk_xcht:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTR_XCHT executed on cycle %u", (unsigned) cycnum);
		#endif
		bp1 = dbuf;
		bp2 = sp;
		bp3 = (sp - wordsize);
		memcpy(bp1, bp2, wordsize);
		memcpy(bp2, bp3, wordsize);
		memcpy(bp3, bp1, wordsize);
		++ip;
		next_cycle();
	stk_gcol:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSTR_GCOL executed on cycle %u", (unsigned) cycnum);
		#endif
		c = ((u64) (sp - stk));
		if (c > GCOL_THRESHOLD) {
			;
		}
		++ip;
		next_cycle();
	str_cat:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTR_CAT executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		strcat(bp1, bp2);
		next_cycle();
	str_ncat:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTR_NCAT executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		strncat(bp1, bp2, (*up1));
		next_cycle();
	str_len:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTR_LEN executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		sp += wordsize;
		up1 = (u64*) sp;
		*up1 = strlen(bp1);
		ip += wordsize;
		next_cycle();
	str_cmp:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTR_CMP executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		sp += wordsize;
		up1 = (u64*) sp;
		*up1 = strcmp(bp1, bp2);
		next_cycle();
	str_ncmp:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nSTR_CMP executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		sp += wordsize;
		up1 = (u64*) sp;
		*up1 = strncmp(bp1, bp2, (*up1));
		next_cycle();
	str_str:
		return;
	str_cspn:
		return;
	str_chr:
		return;
	jmp_str_cmp:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\nJMP_STR_CMP executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		if (strcmp(bp1, bp2) == (*up1)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += wordsize;
		}
		next_cycle();
	jmp_str_ncmp:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tJMP_STR_NCMP executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		up1 = (u64*) ip;
		bp2 = img_byte(*up1);
		ip += wordsize;
		up2 = (u64*) ip;
		ip += wordsize;
		up1 = (u64*) ip;
		ip += wordsize;
		if (strncmp(bp1, bp2, (*up2)) == (*up1)) {
			up1 = (u64*) ip;
			ip = img_byte(*up1);
		} else {
			ip += wordsize;
		}
		next_cycle();
	show_top_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSHOW_TOP_B executed on cycle %u", (unsigned) cycnum);
		#endif
		printf("\n\t\tstack-top(u8): %u", (unsigned) *sp);
		++ip;
		next_cycle();
	show_top_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSHOW_TOP_U executed on cycle %u", (unsigned) cycnum);
		#endif
		up1 = (u64*) sp;
		printf("\n\t\tstack-top(u64): %u", (unsigned) *up1);
		++ip;
		next_cycle();
	show_top_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSHOW_TOP_I executed on cycle %u", (unsigned) cycnum);
		#endif
		ip1 = (s64*) sp;
		printf("\n\t\tstack-top(s64): %d", (int) *ip1);
		++ip;
		next_cycle();
	show_top_r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSHOW_TOP_R executed on cycle %u", (unsigned) cycnum);
		#endif
		rp1 = (r64*) sp;
		printf("\n\t\tstack-top(s64): %f", (double) *rp1);
		++ip;
		next_cycle();
	show_mem_b:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSHOW_MEM_B executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = img_byte(*up1);
		ip += wordsize;
		printf("\n\t\theap[%u] = (u8) %u", (unsigned) *up1, (unsigned) *bp1);
		next_cycle();
	show_mem_u:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSHOW_MEM_U executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		up2 = (u64*) img_byte(*up1);
		ip += wordsize;
		printf("\n\t\theap[%u] = (u64) %u", (unsigned) *up1, (unsigned) *up2);
		next_cycle();
	show_mem_i:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSHOW_MEM_I executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		ip1 = (s64*) img_byte(*up1);
		ip += wordsize;
		printf("\n\t\theap[%u] = (s64) %u", (unsigned) *up1, (int) *ip2);
		next_cycle();
	show_mem_r:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSHOW_MEM_R executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		rp1 = (r64*) img_byte(*up1);
		ip += wordsize;
		printf("\n\t\theap[%u] = (r64) %f", (unsigned) *up1, (double) *rp1);
		next_cycle();
	show_mem_s:
		#ifdef DEBUG_MODE
		++cycnum;
		printf("\n\tSHOW_MEM_S executed on cycle %u", (unsigned) cycnum);
		#endif
		++ip;
		up1 = (u64*) ip;
		bp1 = (char*) img_byte(*up1);
		printf("\n\t\theap[%u] = (str) \"%s\"", (unsigned) *up1, (char*) bp1);
		ip += wordsize;
		next_cycle();

// Debugger Control.
	breakpoint:
		#ifdef DEBUG_MODE
		goto db_entry;
		#else
		goto nop;
		#endif

	#ifdef DEBUG_MODE
	db_entry:
		goto *dbtable[dbmenu_input()];

		dbact_end:
			return;
		dbact_stop:
			goto debug_entry;
		dbact_run:
			db_dostep = 0;
		dbact_stepf:
			next_cycle()
		dbact_printm:
			goto db_entry;
		dbact_prints:
			goto db_entry;
		#endif
}

Process*
malloc_process()
{
	void* mc = malloc(sizeof(Process));
	Process* pro;

	if (mc) {
		pro = (Process*) mc;
	} else {
		return 0;
	}

	pro->start_byte = 0;
	pro->img = 0;

	return pro;
}


void
free_process(Process* pro)
{
	free(pro->img);
	free(pro);
}

// Takes path to .tpx file.
// Process object will be created then returned.

/*
	Build Process:
		
*/
Process*
build_process(const char* path)
{
	// Pointer used for setting up process object's start_byte pointer.
	u64* up;

	// Allocate our to-be returned process object.
	Process* pro = malloc_process();

	// Allocate memory to hold the first 8 bytes which will contain the total img size.
	void* mc   = malloc(8);
	u64*  size = (u64*) mc;

	// Attempt to open file.
	FILE* tpx_file = fopen(path, "rb");

	// Read 8 bytes from start of file into the size block then set file-pointer back to start.
	fread(size, 1, 8, tpx_file);
	rewind(tpx_file);

	// Now we know the exact size of the file in question we can allocate an array accurately.
	// This array will go directly into the process object then we'll read the entire file into it.
	mc = malloc(*size);
	pro->img = (u8*) mc;

	// Read file contents into pro->img then close file.
	fread(pro->img, 1, *size, tpx_file);
	fclose(tpx_file);

	// Set up process object's vars.
	up = (u64*) ((pro->img) + wordsize); // pointer to address containing u64 value that is start address.
	pro->start_byte = ((u8*) (((pro->img) + (*up)))); // set start_byte pointer.
	pro->size = *size;

	// Process object is now ready.
	return pro;
}

u64
write_process(Process* pro, const char* path)
{
	FILE* tpx_file = fopen(path, "wb");
	fwrite(pro->img, 1, pro->size, tpx_file);
	fclose(tpx_file);
	return pro->size;
}


void ty_exec(Process* pro) {
	printf("\ttyson startup.\n");
	execute_process(pro);
	printf("\n\n\ttyson shutdown.\n");
}

int main() {
	Process* pro = build_process("first.tx");
	ty_exec(pro);
	return 0;
}



