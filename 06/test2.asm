   @0               // 0000000000000000
   D=M              // 1111110000010000
   @INFINITE_LOOP   // 0000000000010111
   D;JLE            // 1110001100000110
   @counter         // 0000000000010000
   M=D              // 1110001100001000
   @SCREEN          // 0100000000000000
   D=A              // 1110110000010000
   @address         // 0000000000010001
   M=D              // 1110001100001000
(LOOP)
   @address         // 0000000000010001
   A=M              // 1111110000100000
   M=-1             // 1110111010001000
   @address         // 0000000000010001
   D=M              // 1111110000010000
   @32              // 0000000000100000
   D=D+A            // 1110000010010000
   @address         // 0000000000010001
   M=D              // 1110001100001000
   @counter         // 0000000000010000
   MD=M-1           // 1111110010011000
   @LOOP            // 0000000000001010
   D;JGT            // 1110001100000001
(INFINITE_LOOP)
   @INFINITE_LOOP   // 0000000000010111
   0;JMP            // 1110101010000111
   @KBD             // 0110000000000000
