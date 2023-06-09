// push constant 3030
@3030
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop pointer 0
@SP
M=M-1
@3
D=A
@0
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// push constant 3040
@3040
D=A
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
// push constant 32
@32
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop this 2
@SP
M=M-1
@THIS
D=M
@2
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// push constant 46
@46
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop that 6
@SP
M=M-1
@THAT
D=M
@6
D=D+A
@SP
A=M
D=M+D
A=D-M
M=D-A
// push pointer 0
@3
D=A
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
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
// add
@SP
AM=M-1
D=M
A=A-1
D=M+D
M=D
// push this 2
@THIS
D=M
@2
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
// push that 6
@THAT
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
(END)
@END
0;JMP
