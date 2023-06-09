// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
(LOOP)

@SCREEN
D = A
@start
M = D // start = SCREEN

@KBD
D = A
@end
M = D  // end = KBD

// if *KBD(now D is KBD) = 0, set print 0(white)
A = D
D = M
@WHITE
D;JEQ
// else set print -1(black)
@BLACK
0;JMP

(WHITE)
@print
M = 0
@WHILE
0;JMP // skip the black part
(BLACK)
@print
M = -1

(WHILE) // while start < end

@end
D = M
@start
D = M - D
@LOOP
D;JGE  // if start - end >= 0, jump out of the while loop, which means go back to the outside loop

@print
D = M 
@start 
A = M 
M = D  // *start = print
@start
M = M + 1; // start++

@WHILE
0;JMP
