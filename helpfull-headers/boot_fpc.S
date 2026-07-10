/*
 * VeebhaOS
 * Developed by vixxkigoli
 * 
 * Copyright (C) 2025 vixxkigoli
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "regdef.h"
#include "cp0.h"
#include "global_macros.h"
#include "tcu_asm.h"
#include "mem_bridge_asm.h"
#include "sys_ctrl_asm.h"
#include "timer_asm.h"
#include "debug_port_asm.h"
#include "debug_uart_asm.h"
#include "bb_sram_asm.h"
#include "debug_host_asm.h"
#include "tester_chip_asm.h"

#include "chip_id.h"

#include "boot_host.h"
#include "boot_sector_chip.h"
#include "boot_profile_codes.h"
#include "bootp_mode.h"

 .globl    __xcpu_start
 .set nomips16
__xcpu_start:

    nop
    li      a1, KSEG0(REG_INT_SRAM_BASE) // 0x81c0* is SRAM BASE, Load Immediate value into a1
    lw      v1, 0xcc(a1) // Load WORD a1+0xcc into v1, I saw 0x81c000cc = 00000020
    addiu   sp,sp,-0x28 // Make space for locVar, sp-40, downword
    andi    v0,v1,0x40 // v1 AND 0x40 = v0, v1=WORD a1+0xcc
    .set noreorder
    sw      s0,0x1c(sp) // Save s0 to sp+1c, 
    sw      ra,0x24(sp)
    sw		s1,0x20(sp)
    beqz	v0,__unknfun1 // we need to know what v0 is consist of ?
    move      s0,a0 
    lui	      a2,0x81c0
    lw         v0, 0x278(a2) // 81C0024C
    nop
    andi       a0,v0,0x3
    bne        a0,zero,__unknfun1
    lui       t1,0x7e40
    addu       t0,v0,t1
    lui        v1,0x1
    sltu       a3,t0,v1
    bne        a3,zero,__unknfun2
    li        s1,0x1
    lui        t2,0x5e40
    addu       s1,v0,t2
    j          __unknfun2
    sltu      s1,s1,v1

__unknfun1:
    move	s1,zero

__unknfun2:
    lui        t9,0x81c0
    lw         v0,0x274(t9) //6D8F3243
    nop
    andi       t8,v0,0x3 // after andi its maybe 0x03
    bne        t8,zero,__LAB_01c003c4 // Not gonna jump maybe
    lui        v1,0x7e40 // v1 = 0x7e40
    addu       a1,v0,v1 // a1 = 0x7e43
    lui        v1,0x1 // v1 = 0x01
    sltu       ra,a1,v1 // ra = 1 if a1 < v1
    bne        ra,zero,__LAB_01c00324
    lui        t2,0xb007
    lui        a3,0x5e40
    addu       a0,v0,a3
    sltu       a2,a0,v1
    beq        a2,zero,__LAB_01c003c4
    nop

__LAB_01c00324:
    lw         t1,0x0(v0)
    ori        t0,t2,0x5ec6
    bne        t1,t0,__LAB_01c003c4
    lui       ra,0x4ff8
    lw         t9,0x4(v0)
    ori        t8,ra,0xa139
    bne        t9,t8,__LAB_01c003c4
    li         t1,0xc
    sw         zero,0x0(v0)
    lui        v0,0x81c0
    lw         a1,0x274(v0) 
    move       t2,v0
    sw         zero,0x4(a1)
    move	   v0,zero
    lui        a3,0x8000
    li         t0,0x3

__LAB_01c00364:
    lw         t9, 0x274(t2)
    move	   a2,zero
    mtlo       t9
    madd       v0,t1
    mflo       v1
    lw         t8,0xc(v1)
    lw         a0,0x8(v1)
    or         a1,t8,a3
    lw         v1,0x10(v1)
    j          __LAB_01c003a4
    or        a0,a0,a3

__LAB_01c00390:
    lw         ra,0x0(a0)
    nop
    sw         ra,0x0(a1)
    addiu      a0,a0,0x4
    addiu      a1,a1,0x4

__LAB_01c003a4:
    sltu       t8,a2,v1
    bne        t8,zero,__LAB_01c00390
    addiu     a2,a2,0x1
    addiu      v0,v0,0x1
    bne        v0,t0,__LAB_01c00364
    nop
    jal        __SUB_01e002c0
    nop

__LAB_01c003c4:
    beq        s1,zero,__LAB_01c00420
    lui       v0,0x81c0
    lw         t1,0x278(v0)
    lui        t0,0xb007
    lw         a3,0x4(t1)
    ori        a2,t0,0x5ec7
    bne        a3,a2,__LAB_01c00420
    li        a0,0x55
    jalx       __SUB_01e01030
    sw        v0,0x10(sp)
    lw         t1,0x10(sp)
    li         t0,-0xe01
    
    lw         t8, DAT_81c00278(t1)
    lui        v0,0xa1a0
    lw         a3,0x8(t8)
    ori        t2,v0,0x4000
    and        a2,a3,t0
    sw         a2,0x40c(t2)
    lw         a0,0xc(t8)
    lui        t9,0xffcf
    ori        v1,t9,0xffff
    and        a1,a0,v1
    sw         a1,0x418(t2)

__LAB_01c00420:
    jalx       __FUN_01c00688
    nop
    beq        s1,zero,__LAB_01c00474
    lui       ra,0xa1a3
    lui        a0,0x81c0
    lw         v1, 0x278(a0)
    lui        a1,0x4e6a
    lw         t2,0x0(v1)
    ori        ra,a1,0x821c
    bne        t2,ra,__LAB_01c00470
    sw        zero,0x0(v1)
    lui        a2,0xa1a3
    li         t8,0xaa
    ori        t9,a2,0xf000
    li         a0,0x56
    sw         t8, DAT_a1a3f014(t9)
    jalx       __SUB_01e01030
    nop
    jalx       __FUN_01c0068c
    nop

__LAB_01c00470:  
    lui        ra,0xa1a3

__LAB_01c00474:  
    ori        t1,ra,0xf000
    lui        v0,0x0
    lui        t0,0x81c0
    lui        a3,0x0
    sw         zero, 0xa1a3f014(t1)
    addiu      v0,v0,0x0
    addiu      v1,t0,0x280
    j          __LAB_01c004a8
    addiu     a0,a3,0x0

__LAB_01c00498:
    lw         t2,0x0(v0)
    addiu      v0,v0,0x4
    sw         t2,0x0(v1)
    addiu      v1,v1,0x4

__LAB_01c004a8:
    sltu       a1,v0,a0
    bne        a1,zero,__LAB_01c00498
    nop
    beq        s1,zero,__LAB_01c0053c
    lui       a0,0x4a6
    lui        t9,0x81c0
    lw         v0,0x81c00278(t9) 
    lui        v1,0xb007
    lw         a0,0x4(v0)
    ori        s1,v1,0x5ec7
    bne        a0,s1,__LAB_01c00538
    lui       a3,0xebc0
    lw         a2,0x10(v0)
    ori        t8,a3,0xe28
    bne        a2,t8,__LAB_01c0053c
    lui       a0,0x4a6
    lw         a0,0x14(v0)
    lui        v0,0x632
    ori        t0,v0,0xea00
    beq        a0,t0,__LAB_01c00540
    ori       t1,v0,0xea01
    sltu       v0,a0,t1
    beq        v0,zero,__LAB_01c0051c
    lui       t2,0x319
    ori        ra,t2,0x7500
    beq        a0,ra,__LAB_01c00540
    lui       a1,0x4a6
    j          __LAB_01c00530
    ori       v0,a1,0x2f80

__LAB_01c0051c:
    lui        v1,0x770
    ori        s1,v1,0x4c00
    beq        a0,s1,__LAB_01c00540
    lui       t9,0x843
    ori        v0,t9,0xe2ab

__LAB_01c00530:
    beq        a0,v0,__LAB_01c00540
    nop

__LAB_01c00538:
    lui        a0,0x4a6

__LAB_01c0053c:
    ori        a0,a0,0x2f80

__LAB_01c00540:
    jalx       FUN_01c00a90
    sw        a0,0x10(sp)
    lw         a0,0x10(sp)
    jalx       FUN_01c007ec
    lui       s1,0x6666
    jalx       SUB_01e01030
    ori       a0,s1,0x6666
    lui        a1,0xa18c
    ori        t0,a1,0x4
    li         t2,0x1
    sw         t2,0x0(t0)
    lui        t1,0xa208
    lhu        v0,0x0(t1)
    lw         a0,0x0(t0)
    andi       a3,v0,0xffff
    sw         zero,0x0(t0)
    move       a0,a3
    jalx       SUB_01e01030
    sw        a3,0x10(sp)
    lw         a2,0x10(sp)
    li         t8,0xd
    andi       v0,a2,0x1f
    beq        v0,t8,__LAB_01c005d0
    li        ra,0x9
    beq        v0,ra,__LAB_01c005d4
    li        a0,0x2
    li         v1,0x6
    beq        v0,v1,__LAB_01c005d4
    li        a0,0x3
    li         t9,0xa
    bne        v0,t9,__LAB_01c005d0
    nop
    jalx       SUB_01e01030
    ori       a0,s1,0x7777
    j          __LAB_01c005d4
    li        a0,0x4

__LAB_01c005d0:
    li         a0,0x1

    jalx       FUN_01c007d4        
    lui       s1,0x0
    lui        a2,0x4a6
    jalx       FUN_01c007ec         
    ori       a0,a2,0x2f80
    jalx       FUN_01c006e8 
    nop
    jalx       FUN_01c00b28 
    move      a0,v0
    lui        a0,0x8200
    lui        t8,0x0
    addiu      v1,a0,0x0
    addiu      v0,t8,0x0
    j          __LAB_01c00620
    addiu     a0,s1,0x0

__LAB_01c00610:
    lw         a3,0x0(v0)
    addiu      v0,v0,0x4
    sw         a3,0x0(v1)
    addiu      v1,v1,0x4

__LAB_01c00620:
    sltu       t0,v0,a0
    bne        t0,zero,__LAB_01c00610
    lui       t1,0x8200
    lui        v0,0x8200
    addiu      v0,v0,0x0
    j          __LAB_01c00644
    addiu     v1,t1,0x0

__LAB_01c0063c:
    sw         zero,0x0(v0)
    addiu      v0,v0,0x4

__LAB_01c00644:
    sltu       t2,v0,v1
    bne        t2,zero,__LAB_01c0063c
    li        ra,0x210
    lui        v1,0xa1a2
    ori       a1,v1,0x5014 // MOD by Vixxx
    sw         ra,0x0(a1)
    jalx       _my_main 
    nop
    jalx      _my_main // need to replace
    move      a0,v0
    jal        vixxs_main
    move      a0,s0
    lw         ra,0x24(sp)
    lw         s1,0x20(sp)
    lw         s0,0x1c(sp)
    jr         ra
    addiu     sp,sp,0x28
    .set reorder

// mips16
//	nop

/*
__SUB_01e002c0:
__SUB_01e01030:
FUN_01c00688:
FUN_01c0068c:
FUN_01c00a90:
FUN_01c007ec:
FUN_01c007d4:
FUN_01c00b28:
3 SUB
6 FUN
*/
