#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "computer.h"
#undef mips			/* gcc already has a def for mips */

unsigned int endianSwap(unsigned int);

void PrintInfo (int changedReg, int changedMem);
unsigned int Fetch (int);
void Decode (unsigned int, DecodedInstr*, RegVals*);
int Execute (DecodedInstr*, RegVals*);
int Mem(DecodedInstr*, int, int *);
void RegWrite(DecodedInstr*, int, int *);
void UpdatePC(DecodedInstr*, int);
void PrintInstruction (DecodedInstr*);

/*Globally accessible Computer variable*/
Computer mips;
RegVals rVals;

/*
 *  Return an initialized computer with the stack pointer set to the
 *  address of the end of data memory, the remaining registers initialized
 *  to zero, and the instructions read from the given file.
 *  The other arguments govern how the program interacts with the user.
 */
void InitComputer (FILE* filein, int printingRegisters, int printingMemory,
  int debugging, int interactive) {
    int k;
    unsigned int instr;

    /* Initialize registers and memory */

    for (k=0; k<32; k++) {
        mips.registers[k] = 0;
    }
    
    /* stack pointer - Initialize to highest address of data segment */
    mips.registers[29] = 0x00400000 + (MAXNUMINSTRS+MAXNUMDATA)*4;

    for (k=0; k<MAXNUMINSTRS+MAXNUMDATA; k++) {
        mips.memory[k] = 0;
    }

    k = 0;
    while (fread(&instr, 4, 1, filein)) {
	/*swap to big endian, convert to host byte order. Ignore this.*/
        mips.memory[k] = ntohl(endianSwap(instr));
        k++;
        if (k>MAXNUMINSTRS) {
            fprintf (stderr, "Program too big.\n");
            exit (1);
        }
    }

    mips.printingRegisters = printingRegisters;
    mips.printingMemory = printingMemory;
    mips.interactive = interactive;
    mips.debugging = debugging;
}

unsigned int endianSwap(unsigned int i) {
    return (i>>24)|(i>>8&0x0000ff00)|(i<<8&0x00ff0000)|(i<<24);
}

/*
 *  Run the simulation.
 */
void Simulate () {
    char s[40];  /* used for handling interactive input */
    unsigned int instr;
    int changedReg=-1, changedMem=-1, val;
    DecodedInstr d;
    
    /* Initialize the PC to the start of the code section */
    mips.pc = 0x00400000;
    while (1) {
        if (mips.interactive) {
            printf ("> ");
            fgets (s,sizeof(s),stdin);
            if (s[0] == 'q') {
                return;
            }
        }

        /* Fetch instr at mips.pc, returning it in instr */
        instr = Fetch (mips.pc);

        printf ("Executing instruction at %8.8x: %8.8x\n", mips.pc, instr);

        /* 
	 * Decode instr, putting decoded instr in d
	 * Note that we reuse the d struct for each instruction.
	 */
        Decode (instr, &d, &rVals);

        /*Print decoded instruction*/
        PrintInstruction(&d);

        /* 
	 * Perform computation needed to execute d, returning computed value 
	 * in val 
	 */
        val = Execute(&d, &rVals);

	UpdatePC(&d,val);

        /* 
	 * Perform memory load or store. Place the
	 * address of any updated memory in *changedMem, 
	 * otherwise put -1 in *changedMem. 
	 * Return any memory value that is read, otherwise return -1.
         */
        val = Mem(&d, val, &changedMem);

        /* 
	 * Write back to register. If the instruction modified a register--
	 * (including jal, which modifies $ra) --
         * put the index of the modified register in *changedReg,
         * otherwise put -1 in *changedReg.
         */
        RegWrite(&d, val, &changedReg);

        PrintInfo (changedReg, changedMem);
    }
}

/*
 *  Print relevant information about the state of the computer.
 *  changedReg is the index of the register changed by the instruction
 *  being simulated, otherwise -1.
 *  changedMem is the address of the memory location changed by the
 *  simulated instruction, otherwise -1.
 *  Previously initialized flags indicate whether to print all the
 *  registers or just the one that changed, and whether to print
 *  all the nonzero memory or just the memory location that changed.
 */
void PrintInfo ( int changedReg, int changedMem) {
    int k, addr;
    printf ("New pc = %8.8x\n", mips.pc);
    if (!mips.printingRegisters && changedReg == -1) {
        printf ("No register was updated.\n");
    } else if (!mips.printingRegisters) {
        printf ("Updated r%2.2d to %8.8x\n",
        changedReg, mips.registers[changedReg]);
    } else {
        for (k=0; k<32; k++) {
            printf ("r%2.2d: %8.8x  ", k, mips.registers[k]);
            if ((k+1)%4 == 0) {
                printf ("\n");
            }
        }
    }
    if (!mips.printingMemory && changedMem == -1) {
        printf ("No memory location was updated.\n");
    } else if (!mips.printingMemory) {
        printf ("Updated memory at address %8.8x to %8.8x\n",
        changedMem, Fetch (changedMem));
    } else {
        printf ("Nonzero memory\n");
        printf ("ADDR	  CONTENTS\n");
        for (addr = 0x00400000+4*MAXNUMINSTRS;
             addr < 0x00400000+4*(MAXNUMINSTRS+MAXNUMDATA);
             addr = addr+4) {
            if (Fetch (addr) != 0) {
                printf ("%8.8x  %8.8x\n", addr, Fetch (addr));
            }
        }
    }
}

/*
 *  Return the contents of memory at the given address. Simulates
 *  instruction fetch. 
 */
unsigned int Fetch ( int addr) {
    return mips.memory[(addr-0x00400000)/4];
}

/* Decode instr, returning decoded instruction. */
void Decode ( unsigned int instr, DecodedInstr* d, RegVals* rVals) {
    /* Your code goes here */
	
	d->op = instr >> 26; //all types' opcode are right most 6bits, so shift left 26, left with 6bits which is the opcode field
	
	switch (d->op)
	{
		/* All of R type, opcode=0 */
		case 0x00: 
			d->regs.r.funct = instr & 0x0000003F;	//and get last 6bits
			switch (d->regs.r.funct)// by fuction field
			{
				case 0x00: // sll
				case 0x02: // srl
				case 0x08: // jr
				case 0x21: // addu
				case 0x23: // subu
				case 0x24: // and
				case 0x25: 	// or
				case 0x2A: // slt
					d->type = R;
					//seperate registers rs,rt,rd
					d->regs.r.rs = (instr & 0x03E00000)>>21; 	// (11111) get from 25-21 store it to rs register
					rVals->R_rs = mips.registers[d->regs.r.rs]; //
					d->regs.r.rt = (instr & 0x001F0000)>>16; 	// 20-16  store it to rt register
					rVals->R_rt = mips.registers[d->regs.r.rt]; 
					d->regs.r.rd = (instr & 0x0000F800)>>11; 	//15-11
					rVals->R_rd = mips.registers[d->regs.r.rd];
					d->regs.r.shamt = (instr & 0x000007C0)>>6;	//10-6
					break;
				default:
					// Instructions say exit on others without printing an error, so...
					exit(0);
			}
			break;
		
		/* J type */
		case 0x02: // j  opcode=2
		case 0x03: // jal  op=3
			d->type = J;
			d->regs.j.target = instr & 0x03FFFFFF; //get left 26 bits to target address
			break;
		
		/* All of I type */
		case 0x04: // beq (need extend the 16bit immediate field to 32)
		case 0x05: // bne (sign extend)
		case 0x09: // addiu (sign extend)
		case 0x0C: // andi 
		case 0x0D: // ori 
		case 0x23: // lw (sign extend)
		case 0x2B: // sw (sign extend)
			d->type = I;
			d->regs.i.rs = (instr & 0x03E00000)>>21; //25-21 for rs register
			rVals->R_rs = mips.registers[d->regs.r.rs];
			d->regs.i.rt = (instr & 0x001F0000)>>16; //20-16 rt register
			rVals->R_rt = mips.registers[d->regs.r.rt];
			d->regs.i.addr_or_immed = instr & 0xFFFF;// 0-15 for immediate field
			
			// sign extend?
			if ((d->op == 04) || (d->op == 05) || (d->op == 0x09) || (d->op == 0x23) || (d->op == 0x2B))
				if (d->regs.i.addr_or_immed & 0x8000) //if is negative immediate field
					d->regs.i.addr_or_immed = d->regs.i.addr_or_immed | 0xFFFF0000; //extend 16immediate field by extend the left most bit'1'
			
			break;
		default:
			// Instructions say exit on others without printing an error, so...
			exit(0);
	}
}

/*
 *  Print the disassembled version of the given instruction
 *  followed by a newline.
 */
void PrintInstruction ( DecodedInstr* d) {
    /* Your code goes here */
	
	switch (d->op)
	{
		/* R type */
		case 0x00: // R type
			switch (d->regs.r.funct)		//print out mips instructions: function, registers,...
			{
				case 0x00: // sll
					printf("sll\t$%d, $%d, %d\n",
							d->regs.r.rd, d->regs.r.rt, d->regs.r.shamt);
					break;
				case 0x02: // srl
					printf("srl\t$%d, $%d, %d\n",
							d->regs.r.rd, d->regs.r.rt, d->regs.r.shamt);
					break;
				case 0x08: // jr
					printf("jr\t$%d\n", d->regs.r.rs);
					break;
				case 0x21: // addu
					printf("addu\t$%d, $%d, $%d\n",
							d->regs.r.rd, d->regs.r.rs, d->regs.r.rt);
					break;
				case 0x23: // subu
					printf("subu\t$%d, $%d, $%d\n",
							d->regs.r.rd, d->regs.r.rs, d->regs.r.rt);
					break;
				case 0x24: // and
					printf("and\t$%d, $%d, $%d\n",
							d->regs.r.rd, d->regs.r.rs, d->regs.r.rt);
					break;
				case 0x25: 	// or
					printf("or\t$%d, $%d, $%d\n",
							d->regs.r.rd, d->regs.r.rs, d->regs.r.rt);
					break;
				case 0x2A: // slt
					printf("slt\t$%d, $%d, $%d\n",
							d->regs.r.rd, d->regs.r.rs, d->regs.r.rt);
					break;
			}
			break;
		
		/* J type */

		case 0x02: // j
			printf("j\t0x%8.8X\n", (d->regs.j.target << 2));
			break;
		case 0x03: // jal
			printf("jal\t0x%8.8X\n", (d->regs.j.target << 2));
			break;
		
		/* I type */
		case 0x04: // beq  PC+4+immediateField
			printf("beq\t$%d, $%d, 0x%8.8X\n",
					d->regs.i.rs, d->regs.i.rt, (d->regs.i.addr_or_immed<<2)+mips.pc+4 );
			break;
		case 0x05: // bne
			printf("bne\t$%d, $%d, 0x%8.8X\n",
					d->regs.i.rs, d->regs.i.rt, (d->regs.i.addr_or_immed<<2)+mips.pc+4 );
			break;
		case 0x09: // addiu
			printf("addiu\t$%d, $%d, %d\n",
					d->regs.i.rt, d->regs.i.rs, d->regs.i.addr_or_immed);
			break;
		case 0x0C: // andi
			printf("andi\t$%d, $%d, %d\n",
					d->regs.i.rt, d->regs.i.rs, d->regs.i.addr_or_immed);
			break;
		case 0x0D: // ori
			printf("ori\t$%d, $%d, 0x%X\n",
					d->regs.i.rt, d->regs.i.rs, d->regs.i.addr_or_immed);
			break;
		case 0x23: // lw
			printf("lw\t$%d, %d($%d)\n",
					d->regs.i.rt, d->regs.i.addr_or_immed, d->regs.i.rs);
			break;
		case 0x2B: // sw
			printf("sw\t$%d, %d($%d)\n",
					d->regs.i.rt, d->regs.i.addr_or_immed, d->regs.i.rs);
			break;
		
		default:
			// Instructions say exit on others without printing an error, so...
			exit(0);
	}
}

/* Perform computation needed to execute d, returning computed value */
int Execute ( DecodedInstr* d, RegVals* rVals) {
    /* Your code goes here */
	
	switch(d->op)
	{
		/* R type */
		case 0x00: // opcode = 0   R type
			switch (d->regs.r.funct)
			{
				case 0x00: // sll
					rVals->R_rd = (rVals->R_rt << d->regs.r.shamt); //rt shiftLeft by shamt store result to rd
					return rVals->R_rd;
				case 0x02: // srl
					rVals->R_rd = (rVals->R_rt >> d->regs.r.shamt);
					return rVals->R_rd;
				case 0x08: // jr
					return rVals->R_rs;
				case 0x21: // addu
					rVals->R_rd = ((uint)rVals->R_rs + (uint)rVals->R_rt); // convert rs& rt unsigned by (unit)
					return rVals->R_rd;
				case 0x23: // subu
					rVals->R_rd = ((uint)rVals->R_rs - (uint)rVals->R_rt);
					return rVals->R_rd;
				case 0x24: // and
					rVals->R_rd = (rVals->R_rs & rVals->R_rt);
					return rVals->R_rd;
				case 0x25: 	// or
					rVals->R_rd = (rVals->R_rs | rVals->R_rt);
					return rVals->R_rd;
				case 0x2A: // slt
					rVals->R_rd = (rVals->R_rs < rVals->R_rt) ? 1 : 0;
					return rVals->R_rd;
			}
			break;
		
		/* J type */
		case 0x02: // j
		case 0x03: // jal
			return d->regs.j.target << 2;
		
		/* I type */
		case 0x04: // beq immediate field shiftLeft by 2
			return (rVals->R_rs == rVals->R_rt) ?
					(d->regs.i.addr_or_immed<<2) : 0;
		case 0x05: // bne
			return (rVals->R_rs != rVals->R_rt) ?
					(d->regs.i.addr_or_immed<<2) : 0;
		case 0x09: // addiu
			rVals->R_rt = ((uint)rVals->R_rs + (uint)d->regs.i.addr_or_immed);
			return rVals->R_rt;
		case 0x0C: // andi
			rVals->R_rt = (rVals->R_rs & d->regs.i.addr_or_immed);
			return rVals->R_rt;
		case 0x0D: // ori
			rVals->R_rt = (rVals->R_rs | d->regs.i.addr_or_immed);
			return rVals->R_rt;
		case 0x23: // lw
			return (rVals->R_rs + d->regs.i.addr_or_immed);
		case 0x2B: // sw
			return (d->regs.i.addr_or_immed + rVals->R_rs);
		
		default:
			// Instructions say exit on others without printing an error, so...
			exit(0);
	}
	
	return 0;
}

/* 
 * Update the program counter based on the current instruction. For
 * instructions other than branches and jumps, for example, the PC
 * increments by 4 (which we have provided).
 */
void UpdatePC ( DecodedInstr* d, int val) {
    mips.pc+=4;
    /* Your code goes here */
	
	switch(d->op)
	{
		/* R type */
		case 0x00: // R type
			if (d->regs.r.funct == 0x08) // jr
				mips.pc = val;
			break;
		
		/* J type */
		case 0x02: // j
			mips.pc = val;
			break;
		case 0x03: // jal
			mips.registers[31] = mips.pc;	/*/(PC+4) already added; current address 
											must be stored here before pc changed, cuz it will be back to the current PC+4 */						 
			mips.pc = val;
			break;
		
		/* I type */
		case 0x04: // beq
		case 0x05: // bne
			mips.pc += val;
	}
}

/*
 * Perform memory load or store. Place the address of any updated memory 
 * in *changedMem, otherwise put -1 in *changedMem. Return any memory value 
 * that is read, otherwise return -1. 
 *
 * Remember that we're mapping MIPS addresses to indices in the mips.memory 
 * array. mips.memory[0] corresponds with address 0x00400000, mips.memory[1] 
 * with address 0x00400004, and so forth.
 *
 */
int Mem( DecodedInstr* d, int val, int *changedMem) {//here only consider LOAD and STORE
    /* Your code goes here */
	
	if (d->op == 0x23 || d->op == 0x2B)
	{
		if (val < 0x00401000 || val > 0x00403FFF )
		{
			printf("Memory Access Exception at [0x%X]: address [0x%X]", mips.pc, val);
			exit(0);
		}
		
		val -= 0x00400000; //relavent addresss
		val /= 4; //shiftRight by 2
		
		switch(d->op)
		{
			case 0x23: // lw
				*changedMem = -1;
				return mips.memory[val];
			case 0x2B: // sw
				mips.memory[val] = mips.registers[d->regs.i.rt];
				*changedMem = ((val*4)+0x00400000);//put updated memory into changedMen
				return -1;
		}
	}
	
	*changedMem = -1;
	return -1;
}

/* 
 * Write back to register. If the instruction modified a register--
 * (including jal, which modifies $ra) --
 * put the index of the modified register in *changedReg,
 * otherwise put -1 in *changedReg.
 */
void RegWrite( DecodedInstr* d, int val, int *changedReg) {
    /* Your code goes here */
	
	/* Since neither the result of the Execute function
	 * (Mem() overwrites val with a mem load or -1)
	 * nor the RegVals ever get passed here the
	 * calculations and reads must be redone
	 */
	
	*changedReg = -1;
	
	switch(d->op)
	{
		case 0x00: // R type
			*changedReg = d->regs.r.rd;
			switch (d->regs.r.funct)
			{
				case 0x00: // sll
					mips.registers[d->regs.r.rd] = mips.registers[d->regs.r.rt] << d->regs.r.shamt;
					return;
				case 0x02: // srl
					mips.registers[d->regs.r.rd] = mips.registers[d->regs.r.rt] >> d->regs.r.shamt;
					return;
				case 0x21: // addu
					mips.registers[d->regs.r.rd] = mips.registers[d->regs.r.rs] + mips.registers[d->regs.r.rt];
					return;
				case 0x23: // subu
					mips.registers[d->regs.r.rd] = mips.registers[d->regs.r.rs] - mips.registers[d->regs.r.rt];
					return;
				case 0x24: // and
					mips.registers[d->regs.r.rd] = mips.registers[d->regs.r.rs] & mips.registers[d->regs.r.rt];
					return;
				case 0x25: 	// or
					mips.registers[d->regs.r.rd] = mips.registers[d->regs.r.rs] | mips.registers[d->regs.r.rt];
					return;
				case 0x2A: // slt
					mips.registers[d->regs.r.rd] = (mips.registers[d->regs.r.rs] < mips.registers[d->regs.r.rt]) ? 1 : 0;
					return;
				default:
					*changedReg = -1;
					return;
			}
			return;
		
		/* J type */
		case 0x03: // jal
			// mips.registers[31] already set in UpdatePC() or original pc lost
			*changedReg = 31;
			return;
		
		/* I type */
		case 0x09: // addiu: rt=rs+immediate
			mips.registers[d->regs.i.rt] = mips.registers[d->regs.i.rs] + d->regs.i.addr_or_immed;
			*changedReg = d->regs.i.rt;
			return;
		case 0x0C: // andi : rt=rs&immediate
			mips.registers[d->regs.i.rt] = mips.registers[d->regs.i.rs] & d->regs.i.addr_or_immed;
			*changedReg = d->regs.i.rt;
			return;
		case 0x0D: // ori
			mips.registers[d->regs.i.rt] = mips.registers[d->regs.i.rs] | d->regs.i.addr_or_immed;
			*changedReg = d->regs.i.rt;
			return;
		case 0x23: // lw
			mips.registers[d->regs.i.rt] = val;
			*changedReg = d->regs.i.rt;
			return;
		
		default:
			*changedReg = -1;
			return;
	}
}
