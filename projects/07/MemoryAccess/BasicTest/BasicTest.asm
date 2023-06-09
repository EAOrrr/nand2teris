// push constant 10
@10
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
// push constant 21
@21
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 22
@22
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop argument 2
@SP
M=M-1
@ARG
D=M
@2
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// pop argument 1
@SP
M=M-1
@ARG
D=M
@1
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// push constant 36
@36
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop this 6
@SP
M=M-1
@THIS
D=M
@6
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// push constant 42
@42
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 45
@45
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop that 5
@SP
M=M-1
@THAT
D=M
@5
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
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
// push constant 510
@510
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop temp 6
@SP
M=M-1
@5
D=A
@6
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
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
// push that 5
@THAT
D=M
@5
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
// sub
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=D
// push this 6
@THIS
D=M
@6
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push this 6
@THIS
D=M
@6
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
// sub
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=D
// push temp 6
@5
D=A
@6
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
(END)
@END
0;JMP
