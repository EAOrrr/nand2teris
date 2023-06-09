// push argument 1
@ARG
D=M
@1
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// pop pointer 1           
@SP
M=M-1
@3
D=A
@1
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop that 0              
@SP
M=M-1
@THAT
D=M
@0
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop that 1              
@SP
M=M-1
@THAT
D=M
@1
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// push argument 0
@ARG
D=M
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=D
// pop argument 0          
@SP
M=M-1
@ARG
D=M
@0
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// label MAIN_LOOP_START
(MAIN_LOOP_START)
// push argument 0
@ARG
D=M
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// if-goto COMPUTE_ELEMENT 
@SP
AM=M-1
D=M
@COMPUTE_ELEMENT
D;JNE
// goto END_PROGRAM        
@END_PROGRAM
0;JMP
// label COMPUTE_ELEMENT
(COMPUTE_ELEMENT)
// push that 0
@THAT
D=M
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push that 1
@THAT
D=M
@1
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// add
@SP
AM=M-1
D=M
A=A-1
D=M+D
M=D
// pop that 2              
@SP
M=M-1
@THAT
D=M
@2
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// push pointer 1
@3
D=A
@1
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// add
@SP
AM=M-1
D=M
A=A-1
D=M+D
M=D
// pop pointer 1           
@SP
M=M-1
@3
D=A
@1
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// push argument 0
@ARG
D=M
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=D
// pop argument 0          
@SP
M=M-1
@ARG
D=M
@0
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// goto MAIN_LOOP_START
@MAIN_LOOP_START
0;JMP
// label END_PROGRAM
(END_PROGRAM)
// return 
(END)
@END
0;JMP
