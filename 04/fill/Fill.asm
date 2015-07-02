// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

// Setup variables
	@desired
	M=0       // Using 0 for white
	@current
	M=0

(START)
	// Check keyboard
	@KBD
	D=M
	@BLACKDESIRED
	D;JNE

	// Otherwise set white
	@desired
	M=0
	@COLORPICKED
	0;JMP

(BLACKDESIRED)
	@desired
	M=1
	@COLORPICKED
	0;JMP

(COLORPICKED)
	// The color has been set check if it's different from current color
	@desired
	D=M
	@current
	D=D-M
	@CHANGESCREEN
	D;JNE

	// Otherwise start over
	@START
	0;JMP

(CHANGESCREEN)
	// Set current to desired
	@desired
	D=M
	@current
	M=D

	// Determine what color to set
	@desired
	D=M
	@BLACKSCREEN
	D;JNE

	// Set white if no jump
	@color
	M=0

(SETSCREEN)
	@KBD
	D=A
	@i
	M=D-1
(SCREENLOOP)
	@color
	D=M
	@i
	A=M
	M=D

	// Iterate
	@SCREEN
	D=M
	@i
	M=M-D
	M=M-1
	D=M
	@SCREENLOOP
	D;JNE

	@START
	0;JMP

(BLACKSCREEN)
	//
	@color
	M=-1

	@SETSCREEN
	0;JMP
