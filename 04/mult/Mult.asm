// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Setup variables
@R2
M=0

// Initial checks
@R0
D=M
@END
D;JEQ

@R1
D=M
@END
D;JEQ

(ADDLOOP)
	// Check for exit condition
	@R1
	D=M
	@END
	D;JEQ

	@R0
	D=M
	@R2
	M=D+M

	@R1
	M=M-1

	// Iterate
	@ADDLOOP
	0;JMP

(END)
	@END
	0;JMP
