// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@SP
AM=M-1
D=M
A=A-1
D=M-D
@EQ_0
D;JEQ
@SP
A=M-1
M=0
@END_EQ_0
0;JMP
(EQ_0)
@SP
A=M-1
M=-1
(END_EQ_0)
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@SP
AM=M-1
D=M
A=A-1
D=M-D
@EQ_1
D;JEQ
@SP
A=M-1
M=0
@END_EQ_1
0;JMP
(EQ_1)
@SP
A=M-1
M=-1
(END_EQ_1)
// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@SP
AM=M-1
D=M
A=A-1
D=M-D
@EQ_2
D;JEQ
@SP
A=M-1
M=0
@END_EQ_2
0;JMP
(EQ_2)
@SP
A=M-1
M=-1
(END_EQ_2)
// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LT_0
D;JLT
@SP
A=M-1
M=0
@END_LT_0
0;JMP
(LT_0)
@SP
A=M-1
M=-1
(END_LT_0)
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LT_1
D;JLT
@SP
A=M-1
M=0
@END_LT_1
0;JMP
(LT_1)
@SP
A=M-1
M=-1
(END_LT_1)
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LT_2
D;JLT
@SP
A=M-1
M=0
@END_LT_2
0;JMP
(LT_2)
@SP
A=M-1
M=-1
(END_LT_2)
// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@GT_0
D;JGT
@SP
A=M-1
M=0
@END_GT_0
0;JMP
(GT_0)
@SP
A=M-1
M=-1
(END_GT_0)
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@GT_1
D;JGT
@SP
A=M-1
M=0
@END_GT_1
0;JMP
(GT_1)
@SP
A=M-1
M=-1
(END_GT_1)
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@GT_2
D;JGT
@SP
A=M-1
M=0
@END_GT_2
0;JMP
(GT_2)
@SP
A=M-1
M=-1
(END_GT_2)
// push constant 57
@57
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 31
@31
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 53
@53
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
// push constant 112
@112
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
// neg
@SP
A=M-1
M=-M
// and
@SP
AM=M-1
D=M
A=A-1
D=M&D
M=D
// push constant 82
@82
D=A
@SP
A=M
M=D
@SP
M=M+1
// or
@SP
AM=M-1
D=M
A=A-1
D=M|D
M=D
// not
@SP
A=M-1
M=!M
(END)
@END
0;JMP
