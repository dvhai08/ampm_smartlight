
			EXPORT FRACLIB_FFMUL32
            AREA  |.text|, CODE, READONLY
            THUMB

FRACLIB_FFMUL32
        push {R4-R5,LR} ; store R4 register and LR onto stack   
        asrs R4,R0,#16  ; R4, A = (lsrc1>>16)&0xffff  
        asrs R2,R1,#16  ; R2, C = (lsrc2>>16)&0xffff
        uxth R0,R0      ; R0, B = lsrc1&0xffff 
        uxth R1,R1      ; R1, D = lsrc2&0xffff
        movs R3,R0      ; R3 = B
        muls R0,R1,R0   ; R0 = BD
        muls R1,R4,R1   ; R1 = AD
        muls R3,R2,R3   ; R3 = CB
        muls R2,R4,R2   ; R2 = AC
        lsls R4,R1,#16  ; R4 = (AD<<16)&0xffff0000
        lsls R5,R3,#16  ; R5 = (CB<<16)&0xffff0000
        asrs R1,R1,#16  ; R1 = (AD>>16)&0xffff
        asrs R3,R3,#16  ; R3 = (CB>>16)&0xffff     
        adds R0,R4,R0   ; R0 = BD+(AD<<16)&0xffff0000
        adcs R1,R3,R1   ; R1 = (CB>>16)&0xffff+(AD>>16)&0xffff+"C"
        adds R0,R5,R0   ; R0 = BD+(AD<<16)&0xffff0000+(CB<<16)&0xffff0000
        adcs R1,R2,R1   ; R1 = (CB>>16)&0xffff+(AD>>16)&0xffff+AC+"C"
        adds R0,R0,R0   ; convert integer to fractional (64bit shift-left) 
        adcs R1,R1,R1   ;
        pop {R4-R5,PC}  ; restore R4, R5, LR registers and return        
		
;/****************************************************************************//*!
 ;*
 ;* @brief   Multiply two 32-bit signed values generating 64-bit signed result.
 ;*
 ;* @param   lsrc1   - Input 32-bit signed value.
 ;* @param   lsrc2   - Input 32-bit signed value.         
 ;*
 ;* @return  Function returns 64-bit value in signed integer format.
 ;*
 ;* @remarks Implementation in ASM-language. 
 ;*
 ;*******************************************************************************/
			EXPORT FRACLIB_SSMUL32
            AREA  |.text|, CODE, READONLY
            THUMB

FRACLIB_SSMUL32
        push {R4-R5,LR} ; store R4 register and LR onto stack   
        asrs R4,R0,#16  ; R4, A = (lsrc1>>16)&0xffff  
        asrs R2,R1,#16  ; R2, C = (lsrc2>>16)&0xffff
        uxth R0,R0      ; R0, B = lsrc1&0xffff 
        uxth R1,R1      ; R1, D = lsrc2&0xffff
        movs R3,R0      ; R3 = B
        muls R0,R1,R0   ; R0 = BD
        muls R1,R4,R1   ; R1 = AD
        muls R3,R2,R3   ; R3 = CB
        muls R2,R4,R2   ; R2 = AC
        lsls R4,R1,#16  ; R4 = (AD<<16)&0xffff0000
        lsls R5,R3,#16  ; R5 = (CB<<16)&0xffff0000
        asrs R1,R1,#16  ; R1 = (AD>>16)&0xffff
        asrs R3,R3,#16  ; R3 = (CB>>16)&0xffff     
        adds R0,R4,R0   ; R0 = BD+(AD<<16)&0xffff0000
        adcs R1,R3,R1   ; R1 = (CB>>16)&0xffff+(AD>>16)&0xffff+"C"
        adds R0,R5,R0   ; R0 = BD+(AD<<16)&0xffff0000+(CB<<16)&0xffff0000
        adcs R1,R2,R1   ; R1 = (CB>>16)&0xffff+(AD>>16)&0xffff+AC+"C"
        pop {R4-R5,PC}  ; restore R4, R5, LR registers and return        
        
;/***************************************************************************//*!
 ;*
 ;* @brief   Multiply 32-bit unsigned value with 32-bit signed value generating 
 ;*          64-bit signed result.
 ;*
 ;* @param   lsrc1   - Input 32-bit unsigned value.
 ;* @param   lsrc2   - Input 32-bit signed value.         
 ;*
 ;* @return  Function returns 64-bit value in signed integer format.
 ;*
 ;* @remarks Implementation in ASM-language. 
 ;*
 ;*******************************************************************************/
			EXPORT FRACLIB_USMUL32
            AREA  |.text|, CODE, READONLY
            THUMB

FRACLIB_USMUL32
        push {R4-R5,LR} ; store R4 register and LR onto stack   
        lsrs R4,R0,#16  ; R4, A = (lsrc1>>16)&0xffff  
        asrs R2,R1,#16  ; R2, C = (lsrc2>>16)&0xffff
        uxth R0,R0      ; R0, B = lsrc1&0xffff 
        uxth R1,R1      ; R1, D = lsrc2&0xffff
        movs R3,R0      ; R3 = B
        muls R0,R1,R0   ; R0 = BD
        muls R1,R4,R1   ; R1 = AD
        muls R3,R2,R3   ; R3 = CB
        muls R2,R4,R2   ; R2 = AC
        lsls R4,R1,#16  ; R4 = (AD<<16)&0xffff0000
        lsls R5,R3,#16  ; R5 = (CB<<16)&0xffff0000
        lsrs R1,R1,#16  ; R1 = (AD>>16)&0xffff
        asrs R3,R3,#16  ; R3 = (CB>>16)&0xffff     
        adds R0,R4,R0   ; R0 = BD+(AD<<16)&0xffff0000
        adcs R1,R3,R1   ; R1 = (CB>>16)&0xffff+(AD>>16)&0xffff+"C"
        adds R0,R5,R0   ; R0 = BD+(AD<<16)&0xffff0000+(CB<<16)&0xffff0000
        adcs R1,R2,R1   ; R1 = (CB>>16)&0xffff+(AD>>16)&0xffff+AC+"C"
        pop {R4-R5,PC}  ; restore R4, R5, LR registers and return
        
;/****************************************************************************//*!
 ;*
 ;* @brief   Multiply 32-bit signed value with 32-bit unsigned value generating 
 ;*          64-bit signed result.
 ;*
 ;* @param   lsrc1   - Input 32-bit signed value.
 ;* @param   lsrc2   - Input 32-bit unsigned value.         
 ;*
 ;* @return  Function returns 64-bit value in signed integer format.
 ;*
 ;* @remarks Implementation in ASM-language. 
 ;*
 ;*******************************************************************************/
			EXPORT FRACLIB_SUMUL32
            AREA  |.text|, CODE, READONLY
            THUMB

FRACLIB_SUMUL32
        push {R4-R5,LR} ; store R4 register and LR onto stack   
        asrs R4,R0,#16  ; R4, A = (lsrc1>>16)&0xffff  
        lsrs R2,R1,#16  ; R2, C = (lsrc2>>16)&0xffff
        uxth R0,R0      ; R0, B = lsrc1&0xffff 
        uxth R1,R1      ; R1, D = lsrc2&0xffff
        movs R3,R0      ; R3 = B
        muls R0,R1,R0   ; R0 = BD
        muls R1,R4,R1   ; R1 = AD
        muls R3,R2,R3   ; R3 = CB
        muls R2,R4,R2   ; R2 = AC
        lsls R4,R1,#16  ; R4 = (AD<<16)&0xffff0000
        lsls R5,R3,#16  ; R5 = (CB<<16)&0xffff0000
        asrs R1,R1,#16  ; R1 = (AD>>16)&0xffff
        lsrs R3,R3,#16  ; R3 = (CB>>16)&0xffff     
        adds R0,R4,R0   ; R0 = BD+(AD<<16)&0xffff0000
        adcs R1,R3,R1   ; R1 = (CB>>16)&0xffff+(AD>>16)&0xffff+"C"
        adds R0,R5,R0   ; R0 = BD+(AD<<16)&0xffff0000+(CB<<16)&0xffff0000
        adcs R1,R2,R1   ; R1 = (CB>>16)&0xffff+(AD>>16)&0xffff+AC+"C"
        pop {R4-R5,PC}  ; restore R4, R5, LR registers and return        
		
;/****************************************************************************//*!
 ;*
 ;* @brief   Multiply two 32-bit unsigned values generating 64-bit unsigned result.
 ;*
 ;* @param   lsrc1   - Input 32-bit unsigned value.
 ;* @param   lsrc2   - Input 32-bit unsigned value.         
 ;*
 ;* @return  Function returns 64-bit value in unsigned integer format.
 ;*
 ;* @remarks Implementation in ASM-language. 
 ;*
 ;*******************************************************************************/
			EXPORT FRACLIB_UUMUL32
            AREA  |.text|, CODE, READONLY
            THUMB

FRACLIB_UUMUL32
        push {R4-R5,LR} ; store R4 register and LR onto stack   
        lsrs R4,R0,#16  ; R4, A = (lsrc1>>16)&0xffff  
        lsrs R2,R1,#16  ; R2, C = (lsrc2>>16)&0xffff
        uxth R0,R0      ; R0, B = lsrc1&0xffff 
        uxth R1,R1      ; R1, D = lsrc2&0xffff
        movs R3,R0      ; R3 = B
        muls R0,R1,R0   ; R0 = BD
        muls R1,R4,R1   ; R1 = AD
        muls R3,R2,R3   ; R3 = CB
        muls R2,R4,R2   ; R2 = AC
        lsls R4,R1,#16  ; R4 = (AD<<16)&0xffff0000
        lsls R5,R3,#16  ; R5 = (CB<<16)&0xffff0000
        lsrs R1,R1,#16  ; R1 = (AD>>16)&0xffff
        lsrs R3,R3,#16  ; R3 = (CB>>16)&0xffff     
        adds R0,R4,R0   ; R0 = BD+(AD<<16)&0xffff0000
        adcs R1,R3,R1   ; R1 = (CB>>16)&0xffff+(AD>>16)&0xffff+"C"
        adds R0,R5,R0   ; R0 = BD+(AD<<16)&0xffff0000+(CB<<16)&0xffff0000
        adcs R1,R2,R1   ; R1 = (CB>>16)&0xffff+(AD>>16)&0xffff+AC+"C"
        pop {R4-R5,PC}  ; restore R4, R5, LR registers and return         
        END
;/******************************************************************************
 ;* End of module                                                              *
 ;******************************************************************************/   

