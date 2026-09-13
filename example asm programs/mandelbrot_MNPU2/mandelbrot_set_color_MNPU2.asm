;OUT 7 - color
;OUT 6 - y, draw pixel
;OUT 5 - x
;OUT 4 - screen control - bit 0(buffer), bit 1(clear screen)

; out top
; out bottom
; out tmp
; b tmp
; b top
; b bottom
; a top
; a bottom

; [1] - zr_bottom
; [2] - zr_top
; [3] - zi_bottom
; [4] - zi_top
; [5] - cr_bottom
; [6] - cr_top
; [7] - ci_bottom
; [8] - ci_top

; [9]  - zr*zr_bottom
; [10] - zr*zr_top
; [11] - zr*zi_bottom
; [12] - zr*zi_top
; [13] - zi*zi_bottom
; [14] - zi*zi_top

$ x = 15
$ y = 16
$ n = 17
$ sign_bit = 128

IMM R7
y
POI R7
IMA
64
MST

for_y_loop:
IMM R7
y
POI R7
MLD
BRC NZERO
nend_for_y
SWP
end_for_y_page
end_for_y
nend_for_y:
RST R1
DEC R1
AST R1
MST
    
    IMM R7
    x
    POI R7
    IMA
    64
    MST

    for_x_loop:
    IMM R7
    x
    POI R7
    MLD
    BRC NZERO
    nend_for_x
    SWP
    end_for_x_page
    end_for_x
    nend_for_x:
    RST R1
    DEC R1 ; R1 = x
    AST R1
    MST
    
        ; R1=cr_bottom, R2=cr_top
        LSH R1
        LSH R1
        LSH R1
        AST 0
        ADDC 0
        RST R2
        
        IMA
        0b10000000
        ADD R1
        RST R1
        IMA
        0b11111110
        ADDC R2
        
        IMM R7 ; cr top
        6
        POI R7
        DEC R7
        MST
        POI R7
        AST R1
        MST
        
        SWP
        1
        0

        ]next_page] 1

        ; R1=ci_bottom, R2=ci_top
        IMM R7
        y
        POI R7
        MLD
        RST R1
        LSH R1
        LSH R1
        LSH R1
        AST 0
        ADDC 0
        RST R2
        
        IMA
        0xff
        ADD R2
        
        IMM R7 ; ci top
        8
        POI R7
        DEC R7
        MST
        POI R7
        AST R1
        MST

        ; mandelbrot calculation
        ; n=max interacitons
        IMM R7
        n
        POI R7
        IMA
        20
        MST

        ; zr_top, zr_bottom, zi_top, zi_bottom = 0
        AST 0
        IMM R7
        1
        POI R7 ; zr_bottom = 0
        INC R7
        MST
        POI R7 ; zr_top = 0
        INC R7
        MST
        POI R7 ; zi_bottom = 0
        INC R7
        MST
        POI R7 ; zi_top = 0
        MST

        SWP
        2
        0

        ]next_page] 2
        
        mandelbrot_while:
        ; calcualte zr*zr, zr*zi, zi*zi
        ; zr*zi
        IMM R7
        1
        POI R7 ; zr_bottom
        MLD
        INC R7
        PUSH
        POI R7 ; zr_top
        MLD
        INC R7
        PUSH
        POI R7 ; zi_bottom
        MLD
        INC R7
        PUSH
        POI R7 ; zi_top
        MLD
        PUSH
        CALL
        multiply_page
        multiply
        IMM R7
        12
        POP
        POI R7 ; zr*zi_top
        MST
        DEC R7
        POP
        POI R7 ; zr*zi_bottom
        MST

        ;zr*zr
        IMM R6 ; zr_bottom
        1
        IMM R7 ; zr_top
        2
        POI R6
        MLD
        PUSH
        RST R1 ; zr_bottom
        POI R7
        MLD
        PUSH
        RST R2 ; zr_top
        AST R1
        PUSH
        AST R2
        PUSH
        CALL
        multiply_page
        multiply
        IMM R7
        10
        POP
        POI R7 ; zr*zi_top
        MST
        DEC R7
        POP
        POI R7 ; zr*zi_bottom
        MST

        SWP
        3
        0

        ]next_page] 3

        ;zi*zi
        IMM R6 ; zi_bottom
        3
        IMM R7 ; zi_top
        4
        POI R6
        MLD
        PUSH
        RST R1 ; zi_bottom
        POI R7
        MLD
        PUSH
        RST R2 ; zi_top
        AST R1
        PUSH
        AST R2
        PUSH
        CALL
        multiply_page
        multiply
        IMM R7
        14
        POP
        POI R7 ; zi*zi_top
        MST
        DEC R7
        POP
        POI R7 ; zi*zi_bottom
        MST


        ; zr = ((zr*zr) - (zi*zi)) + cr
        INC R7
        RST R3 ;R3 = zi*zi_bottom
        POI R7
        MLD
        RST R4 ;R4 = zi*zi_top
        IMM R7
        9
        POI R7
        INC R7
        MLD
        RST R1 ;R1 = zr*zr_bottom
        POI R7
        MLD
        RST R2 ;R2 = zr*zr_top

        AST R1
        SUB R3
        RST R5 ;R5 = (zr*zr)-(zi*zi)_bottom

        AST R2
        SUBC R4
        RST R6 ;R6 = (zr*zr)-(zi*zi)_top

        IMM R7
        5
        POI R7
        MLD
        INC R7
        RST R1 ;R1 = cr_bottom
        POI R7
        MLD
        RST R2 ;R2 = cr_top

        AST R5
        ADD R1
        RST R1 ;R1 = ((zr*zr)-(zi*zi))+cr_bottom

        SWP
        4
        0

        ]next_page] 4

        AST R6
        ADDC R2
        RST R2 ;R2 = ((zr*zr)-(zi*zi))+cr_top

        IMM R7
        2
        POI R7
        AST R2
        MST

        DEC R7
        AST R1
        POI R7
        MST


        ; zi = zr*zi*2+ci
        IMM R7
        11
        POI R7
        INC R7
        MLD
        RST R1 ; R1 = zr*zi_bottom
        POI R7
        MLD
        RST R2 ; R2 = zr*zi_top

        LSH R2
        LSH R1 ; R1 = zr*zi*2_bottom
        AST 0
        ADDC R2
        RST R2 ; R2 = zr*zi*2_top

        IMM R7
        7
        POI R7
        INC R7
        MLD
        RST R3 ;R3 = ci_bottom
        POI R7
        MLD
        RST R4 ;R4 = ci_top

        AST R1
        ADD R3
        RST R1

        AST R2
        ADDC R4
        RST R2

        IMM R7
        4
        POI R7
        MST

        DEC R7
        AST R1
        POI R7
        MST

        SWP
        5
        0

        ]next_page] 5

        ; zr*zr+zi*zi
        IMM R7
        13
        POI R7
        MLD
        INC R7
        RST R1 ; R1 = zi*zi bottom
        POI R7
        MLD
        RST R2 ; R2 = zi*zi top
        IMM R7
        9
        POI R7
        MLD
        INC R7
        RST R3 ; R3 = zr*zr bottom
        POI R7
        MLD
        RST R4 ; R4 = zr*zr top

        AST R1
        ADD R3
        RST R5 ; R5 = zr*zr+zi*zi_bottom

        AST R2
        ADDC R4
        RST R6 ; R6 = zr*zr+zi*zi_top

        ;check if 4 > zr*zr+zi*zi
        IMA
        0
        SUB R5
        IMA
        4
        SUBC R6

        SWP
        6
        0

        ]next_page] 6

        BRC LT
        return_iteractions
        IMM R7
        n
        POI R7
        MLD
        BRC ZERO
        return_iteractions

        RST R5
        DEC R5
        AST R5 ; n-=1
        MST

        SWP
        mandelbrot_while_page
        mandelbrot_while

        return_iteractions:
        
        IMM R7
        n
        POI R7
        MLD
        BRC ZERO
        dont_draw

        OUT 7
        IMM R7
        x
        POI R7
        MLD
        OUT 5
        IMM R7
        y
        POI R7
        MLD
        OUT 6
        IMA
        1
        OUT 4

        dont_draw:
        SWP
        for_x_loop_page
        for_x_loop

    end_for_x:
    SWP
    for_y_loop_page
    for_y_loop
end_for_y:

HALT

]next_page] 7

;;;;;;;;;;;;;;;;;;;;;;;;;;;
multiply:
POP
RST R2 ; b top
POP
RST R1 ; b bottom
POP
RST R4 ; a top
POP
RST R3 ; a bottom

; define result sign
AST R4
XOR R2
IMM R7
128
AND R7
POI R7
MST
POI 0

; absolute b
AST R7
AND R2
BRC ZERO
dont_negate_b
    NOT R2
    NEG R1
    AST R2
    ADDC 0
    RST R2
dont_negate_b:

; absolute a
AST R7
AND R4
BRC ZERO
dont_negate_a
    NOT R4
    NEG R3
    AST R4
    ADDC 0
    RST R4
dont_negate_a:

AST R3 ; a bottom
PUSH
AST R4 ; a top
PUSH
AST R1 ; b bottom
PUSH
AST R2 ; b top
PUSH

AST 0
PUSH
PUSH
PUSH
PUSH
POI 0
multiplication_loop:

; load
POP
RST R6 ; out top

POP
RST R5 ; out bottom

POP
RST R4 ; out tmp

POP
RST R3 ; b tmp

POP
RST R2 ; b top

POP
RST R1 ; b bottom

SWP
8
0

]next_page] 8

    ; after loading check if add/end or not
    POP
    RST R7 ; a top
    POP
    PUSH
    BRC LSB
    add_thingies
    OR R7
    BRC NZERO
    dont_add_thingies
        SWP
        multiplication_finish_page
        multiplication_finish
    add_thingies:

; add
AST R1
ADD R4
RST R4

AST R2
ADDC R5
RST R5

AST R3
ADDC R6
RST R6
dont_add_thingies:

;left shift b
LSH R3

LSH R2
AST R3
ADDC 0
RST R3

LSH R1
AST R2
ADDC 0
RST R2

SWP
9
0

]next_page] 9

; right shift a
AST R6
MST

POP
RST R6

RSH R6

RSH R7
BRC NCARRY
rsh_ncarry
    IMA
    128
    ADD R6
    RST R6
rsh_ncarry:

AST R6
PUSH
AST R7
PUSH

;store
AST R1
PUSH

AST R2
PUSH

AST R3
PUSH

AST R4
PUSH

AST R5
PUSH

MLD
PUSH

SWP
multiplication_loop_page
multiplication_loop

multiplication_finish:

POP
IMM R7
128
POI R7
MLD
BRC ZERO
dont_negate_output
    NOT R6
    NEG R5
    AST R6
    ADDC 0
    RST R6
dont_negate_output:
AST R5 ; bottom
PUSH
AST R6 ; top
PUSH

RET