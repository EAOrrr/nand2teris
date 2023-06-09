// push constant 0    
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop local 0         
@SP
M=M-1
@LCL
D=M
@0
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// label LOOP_START
(LOOP_START)
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
// push local 0
@LCL
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
// add
@SP
AM=M-1
D=M
A=A-1
D=M+D
M=D
// pop local 0	        
@SP
M=M-1
@LCL
D=M
@0
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
// if-goto LOOP_START  
@SP
AM=M-1
D=M
@LOOP_START
D;JNE
// push local 0
@LCL
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
// return 
(END)
@END
0;JMP
