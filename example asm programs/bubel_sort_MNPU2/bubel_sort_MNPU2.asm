;put sth in ram to sort it
IMM R7
64
initialize:
DEC R7
AST R7
POI R7
MST
BRC NZERO
initialize

;bubble sort
IMM R5
64
back:
IMM R6
0
DEC R5
AST R5
CMP R6
BRC EQ
end
IMM R7
1

POI R6
MLD
RST R1


start:
POI R7
MLD
RST R2

CMP R1
BRC GEQ
swap

AST R2
RST R1
BRC TRUE
no_swap

swap:
AST R1 ; r7 is pointer
MST

AST R2
POI R6
MST

no_swap:
INC R6
AST R6
CMP R5
BRC EQ
back

INC R7
BRC TRUE
start
end:
HALT