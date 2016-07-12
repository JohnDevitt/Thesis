dnl  mpn_mul_basecase for Pentium 4 and P6 models with SSE2 (i.e., 9,D,E,F).

dnl  Copyright 2001, 2002, 2005, 2007 Free Software Foundation, Inc.
dnl
dnl  This file is part of the GNU MP Library.
dnl
dnl  The GNU MP Library is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU Lesser General Public License as published
dnl  by the Free Software Foundation; either version 3 of the License, or (at
dnl  your option) any later version.
dnl
dnl  The GNU MP Library is distributed in the hope that it will be useful, but
dnl  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl  License for more details.
dnl
dnl  You should have received a copy of the GNU Lesser General Public License
dnl  along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.

include(`../config.m4')

C TODO:
C  * Improve ad-hoc outer loop code and register handling.  Some feed-in
C    scheduling could improve things by several cycles per outer iteration.
C  * In code for un <= 3, try keeping accumulation operands in registers,
C    without storing intermediates to rp.
C  * We might want to keep 32 in a free mm register, since the register form is
C    3 bytes and the immediate form is 4 bytes.  About 70 bytes to save.
C  * Look into different loop alignment, we now expand the code about 50 bytes
C    with possibly needless alignment.
C  * Perhaps rewrap loops 00,01,02 (6 loops) to allow fall-through entry.
C  * Use OSP, should solve feed-in latency problems.
C  * Save a few tens of bytes by doing cross-jumping for Loel0, etc.
C  * Save around 120 bytes by remapping "m 0", "m 1", "m 2" and "m 3" registers
C    so that they can share feed-in code, and changing the branch targets from
C    L<n> to Lm<nn>.

C                           cycles/limb
C P6 model 9   (Banias)         ?
C P6 model 13  (Dothan)         5.24
C P6 model 14  (Yonah)          ?
C P4 model 0-1 (Willamette):    5
C P4 model 2   (Northwood):     4.60 at 32 limbs
C P4 model 3-4 (Prescott):      4.94 at 32 limbs

C INPUT PARAMETERS
C rp		sp + 4
C up		sp + 8
C un		sp + 12
C vp		sp + 16
C vn		sp + 20

	TEXT
	ALIGN(16)
PROLOGUE(mpn_mul_basecase)
	push	%esi
	push	%ebx
	mov	12(%esp), %edx		C rp
	mov	16(%esp), %eax		C up
	mov	20(%esp), %ecx		C un
	mov	24(%esp), %esi		C vp
	mov	28(%esp), %ebx		C vn
	movd	(%esi), %mm7		C
.Lent:	cmp	$3, %ecx
	ja	.Large
	movd	(%eax), %mm6
	pmuludq	%mm7, %mm6
	jz	.Lun3
	cmp	$2, %ecx
	jz	.Lun2

.Lun1:	movd	%mm6, (%edx)		C				un=1
	psrlq	$32, %mm6		C				un=1
	movd	%mm6, 4(%edx)		C				un=1
	jmp	.Lret			C				un=1

.Lun2:	movd	4(%eax), %mm1		C				un=2
	pmuludq	%mm7, %mm1		C				un=2
	movd	%mm6, (%edx)		C				un=2
	psrlq	$32, %mm6		C				un=2
	paddq	%mm1, %mm6		C				un=2
	movd	%mm6, 4(%edx)		C				un=2
	psrlq	$32, %mm6		C				un=2
	movd	%mm6, 8(%edx)		C				un=2
      dec	%ebx			C				un=2
      jz	.Lret			C				un=2
	movd	4(%esi), %mm7		C				un=2
	movd	(%eax), %mm6		C				un=2
	pmuludq	%mm7, %mm6		C				un=2
	movd	4(%eax), %mm1		C				un=2
	movd	4(%edx), %mm4		C				un=2
	pmuludq	%mm7, %mm1		C				un=2
	movd	8(%edx), %mm5		C				un=2
	paddq	%mm4, %mm6		C				un=2
	paddq	%mm1, %mm5		C				un=2
	movd	%mm6, 4(%edx)		C				un=2
	psrlq	$32, %mm6		C				un=2
	paddq	%mm5, %mm6		C				un=2
	movd	%mm6, 8(%edx)		C				un=2
	psrlq	$32, %mm6		C				un=2
	movd	%mm6, 12(%edx)		C				un=2
.Lret:	emms
	pop	%ebx
	pop	%esi
	ret

.Lun3:	movd	4(%eax), %mm1		C				un=3
	pmuludq	%mm7, %mm1		C				un=3
	movd	8(%eax), %mm2		C				un=3
	pmuludq	%mm7, %mm2		C				un=3
	movd	%mm6, (%edx)		C				un=3
	psrlq	$32, %mm6		C				un=3
	paddq	%mm1, %mm6		C				un=3
	movd	%mm6, 4(%edx)		C				un=3
	psrlq	$32, %mm6		C				un=3
	paddq	%mm2, %mm6		C				un=3
	movd	%mm6, 8(%edx)		C				un=3
	psrlq	$32, %mm6		C				un=3
	movd	%mm6, 12(%edx)		C				un=3
      dec	%ebx			C				un=3
      jz	.Lret			C				un=3
	movd	4(%esi), %mm7		C				un=3
	movd	(%eax), %mm6		C				un=3
	pmuludq	%mm7, %mm6		C				un=3
	movd	4(%eax), %mm1		C				un=3
	movd	4(%edx), %mm4		C				un=3
	pmuludq	%mm7, %mm1		C				un=3
	movd	8(%eax), %mm2		C				un=3
	movd	8(%edx), %mm5		C				un=3
	pmuludq	%mm7, %mm2		C				un=3
	paddq	%mm4, %mm6		C				un=3
	paddq	%mm1, %mm5		C				un=3
	movd	12(%edx), %mm4		C				un=3
	movd	%mm6, 4(%edx)		C				un=3
	psrlq	$32, %mm6		C				un=3
	paddq	%mm5, %mm6		C				un=3
	paddq	%mm2, %mm4		C				un=3
	movd	%mm6, 8(%edx)		C				un=3
	psrlq	$32, %mm6		C				un=3
	paddq	%mm4, %mm6		C				un=3
	movd	%mm6, 12(%edx)		C				un=3
	psrlq	$32, %mm6		C				un=3
	movd	%mm6, 16(%edx)		C				un=3
      dec	%ebx			C				un=3
      jz	.Lret			C				un=3
	movd	8(%esi), %mm7		C				un=3
	movd	(%eax), %mm6		C				un=3
	pmuludq	%mm7, %mm6		C				un=3
	movd	4(%eax), %mm1		C				un=3
	movd	8(%edx), %mm4		C				un=3
	pmuludq	%mm7, %mm1		C				un=3
	movd	8(%eax), %mm2		C				un=3
	movd	12(%edx), %mm5		C				un=3
	pmuludq	%mm7, %mm2		C				un=3
	paddq	%mm4, %mm6		C				un=3
	paddq	%mm1, %mm5		C				un=3
	movd	16(%edx), %mm4		C				un=3
	movd	%mm6, 8(%edx)		C				un=3
	psrlq	$32, %mm6		C				un=3
	paddq	%mm5, %mm6		C				un=3
	paddq	%mm2, %mm4		C				un=3
	movd	%mm6, 12(%edx)		C				un=3
	psrlq	$32, %mm6		C				un=3
	paddq	%mm4, %mm6		C				un=3
	movd	%mm6, 16(%edx)		C				un=3
	psrlq	$32, %mm6		C				un=3
	movd	%mm6, 20(%edx)		C				un=3
	jmp	.Lret


.Large:	push	%edi
	pxor	%mm6, %mm6
	lea	4(%esi), %esi
	and	$3, %ecx
	jz	.L0
	cmp	$2, %ecx
	jc	.L1
	jz	.L2
	jmp	.L3			C FIXME: one case should fall through


.L0:	movd	(%eax), %mm3		C				m 0
	sub	24(%esp), %ecx		C inner loop count		m 0
	mov	%ecx, 24(%esp)		C update loop count for later	m 0
	pmuludq	%mm7, %mm3		C				m 0
	movd	4(%eax), %mm0		C				m 0
	pmuludq	%mm7, %mm0		C				m 0
	movd	8(%eax), %mm1		C				m 0
	jmp	.Lm00			C				m 0
	ALIGN(16)			C				m 0
.Llpm0:	pmuludq	%mm7, %mm4		C				m 0
	paddq	%mm0, %mm6		C				m 0
	movd	(%eax), %mm3		C				m 0
	movd	%mm6, -12(%edx)		C				m 0
	psrlq	$32, %mm6		C				m 0
	pmuludq	%mm7, %mm3		C				m 0
	paddq	%mm1, %mm6		C				m 0
	movd	4(%eax), %mm0		C				m 0
	movd	%mm6, -8(%edx)		C				m 0
	psrlq	$32, %mm6		C				m 0
	pmuludq	%mm7, %mm0		C				m 0
	paddq	%mm4, %mm6		C				m 0
	movd	8(%eax), %mm1		C				m 0
	movd	%mm6, -4(%edx)		C				m 0
	psrlq	$32, %mm6		C				m 0
.Lm00:	pmuludq	%mm7, %mm1		C				m 0
	paddq	%mm3, %mm6		C				m 0
	movd	12(%eax), %mm4		C				m 0
	movd	%mm6, (%edx)		C				m 0
	psrlq	$32, %mm6		C				m 0
	lea	16(%eax), %eax		C				m 0
	lea	16(%edx), %edx		C				m 0
	add	$4, %ecx		C				m 0
	ja	.Llpm0			C				m 0
	pmuludq	%mm7, %mm4		C				m 0
	paddq	%mm0, %mm6		C				m 0
	movd	%mm6, -12(%edx)		C				m 0
	psrlq	$32, %mm6		C				m 0
	paddq	%mm1, %mm6		C				m 0
	mov	16(%esp), %edi		C rp				  0
	jmp	.Lx0

.Lolp0:	lea	4(%edi), %edi		C				am 0
	movd	(%esi), %mm7		C				am 0
	lea	4(%esi), %esi		C				am 0
	mov	%edi, %edx		C rp				am 0
	mov	20(%esp), %eax		C up				am 0
	movd	(%eax), %mm3		C				am 0
	mov	24(%esp), %ecx		C inner loop count		am 0
	pxor	%mm6, %mm6		C				am 0
	pmuludq	%mm7, %mm3		C				am 0
	movd	4(%eax), %mm0		C				am 0
	movd	(%edx), %mm5		C				am 0
	pmuludq	%mm7, %mm0		C				am 0
	movd	8(%eax), %mm1		C				am 0
	paddq	%mm3, %mm5		C				am 0
	movd	4(%edx), %mm4		C				am 0
	jmp	.Lam00			C				am 0
	ALIGN(16)			C				mm 0
.Llam0:	pmuludq	%mm7, %mm2		C				am 0
	paddq	%mm4, %mm6		C				am 0
	movd	(%eax), %mm3		C				am 0
	paddq	%mm1, %mm5		C				am 0
	movd	-4(%edx), %mm4		C				am 0
	movd	%mm6, -12(%edx)		C				am 0
	psrlq	$32, %mm6		C				am 0
	pmuludq	%mm7, %mm3		C				am 0
	paddq	%mm5, %mm6		C				am 0
	movd	4(%eax), %mm0		C				am 0
	paddq	%mm2, %mm4		C				am 0
	movd	(%edx), %mm5		C				am 0
	movd	%mm6, -8(%edx)		C				am 0
	psrlq	$32, %mm6		C				am 0
	pmuludq	%mm7, %mm0		C				am 0
	paddq	%mm4, %mm6		C				am 0
	movd	8(%eax), %mm1		C				am 0
	paddq	%mm3, %mm5		C				am 0
	movd	4(%edx), %mm4		C				am 0
	movd	%mm6, -4(%edx)		C				am 0
	psrlq	$32, %mm6		C				am 0
.Lam00:	pmuludq	%mm7, %mm1		C				am 0
	paddq	%mm5, %mm6		C				am 0
	movd	12(%eax), %mm2		C				am 0
	paddq	%mm0, %mm4		C				am 0
	movd	8(%edx), %mm5		C				am 0
	movd	%mm6, (%edx)		C				am 0
	psrlq	$32, %mm6		C				am 0
	lea	16(%eax), %eax		C				am 0
	lea	16(%edx), %edx		C				am 0
	add	$4, %ecx		C				am 0
	jnz	.Llam0			C				am 0
	pmuludq	%mm7, %mm2		C				am 0
	paddq	%mm4, %mm6		C				am 0
	paddq	%mm1, %mm5		C				am 0
	movd	-4(%edx), %mm4		C				am 0
	movd	%mm6, -12(%edx)		C				am 0
	psrlq	$32, %mm6		C				am 0
	paddq	%mm5, %mm6		C				am 0
	paddq	%mm2, %mm4		C				am 0
.Lx0:	movd	%mm6, -8(%edx)		C				am 0
	psrlq	$32, %mm6		C				am 0
	paddq	%mm4, %mm6		C				am 0
	movd	%mm6, -4(%edx)		C				am 0
	psrlq	$32, %mm6		C				am 0
	movd	%mm6, (%edx)		C				am 0
	dec	%ebx			C				am 0
	jnz	.Lolp0			C				am 0
.Loel0:	emms				C				   0
	pop	%edi			C				   0
	pop	%ebx			C				   0
	pop	%esi			C				   0
	ret				C				   0


.L1:	movd	(%eax), %mm4		C				m 1
	sub	24(%esp), %ecx		C				m 1
	mov	%ecx, 24(%esp)		C update loop count for later	m 1
	pmuludq	%mm7, %mm4		C				m 1
	movd	4(%eax), %mm3		C				m 1
	pmuludq	%mm7, %mm3		C				m 1
	movd	8(%eax), %mm0		C				m 1
	jmp	.Lm01			C				m 1
	ALIGN(16)			C				m 1
.Llpm1:	pmuludq	%mm7, %mm4		C				m 1
	paddq	%mm0, %mm6		C				m 1
	movd	4(%eax), %mm3		C				m 1
	movd	%mm6, -8(%edx)		C				m 1
	psrlq	$32, %mm6		C				m 1
	pmuludq	%mm7, %mm3		C				m 1
	paddq	%mm1, %mm6		C				m 1
	movd	8(%eax), %mm0		C				m 1
	movd	%mm6, -4(%edx)		C				m 1
	psrlq	$32, %mm6		C				m 1
.Lm01:	pmuludq	%mm7, %mm0		C				m 1
	paddq	%mm4, %mm6		C				m 1
	movd	12(%eax), %mm1		C				m 1
	movd	%mm6, (%edx)		C				m 1
	psrlq	$32, %mm6		C				m 1
	pmuludq	%mm7, %mm1		C				m 1
	paddq	%mm3, %mm6		C				m 1
	movd	16(%eax), %mm4		C				m 1
	movd	%mm6, 4(%edx)		C				m 1
	psrlq	$32, %mm6		C				m 1
	lea	16(%eax), %eax		C				m 1
	lea	16(%edx), %edx		C				m 1
	add	$4, %ecx		C				m 1
	ja	.Llpm1			C				m 1
	pmuludq	%mm7, %mm4		C				m 1
	paddq	%mm0, %mm6		C				m 1
	movd	%mm6, -8(%edx)		C				m 1
	psrlq	$32, %mm6		C				m 1
	paddq	%mm1, %mm6		C				m 1
	mov	16(%esp), %edi		C rp				  1
	jmp	.Lx1

.Lolp1:	lea	4(%edi), %edi		C				am 1
	movd	(%esi), %mm7		C				am 1
	lea	4(%esi), %esi		C				am 1
	mov	%edi, %edx		C rp				am 1
	mov	20(%esp), %eax		C up				am 1
	movd	(%eax), %mm2		C				am 1
	mov	24(%esp), %ecx		C inner loop count		am 1
	pxor	%mm6, %mm6		C				am 1
	pmuludq	%mm7, %mm2		C				am 1
	movd	4(%eax), %mm3		C				am 1
	movd	(%edx), %mm4		C				am 1
	pmuludq	%mm7, %mm3		C				am 1
	movd	8(%eax), %mm0		C				am 1
	paddq	%mm2, %mm4		C				am 1
	movd	4(%edx), %mm5		C				am 1
	jmp	.Lam01			C				am 1
	ALIGN(16)			C				am 1
.Llam1:	pmuludq	%mm7, %mm2		C				am 1
	paddq	%mm4, %mm6		C				am 1
	movd	4(%eax), %mm3		C				am 1
	paddq	%mm1, %mm5		C				am 1
	movd	(%edx), %mm4		C				am 1
	movd	%mm6, -8(%edx)		C				am 1
	psrlq	$32, %mm6		C				am 1
	pmuludq	%mm7, %mm3		C				am 1
	paddq	%mm5, %mm6		C				am 1
	movd	8(%eax), %mm0		C				am 1
	paddq	%mm2, %mm4		C				am 1
	movd	4(%edx), %mm5		C				am 1
	movd	%mm6, -4(%edx)		C				am 1
	psrlq	$32, %mm6		C				am 1
.Lam01:	pmuludq	%mm7, %mm0		C				am 1
	paddq	%mm4, %mm6		C				am 1
	movd	12(%eax), %mm1		C				am 1
	paddq	%mm3, %mm5		C				am 1
	movd	8(%edx), %mm4		C				am 1
	movd	%mm6, (%edx)		C				am 1
	psrlq	$32, %mm6		C				am 1
	pmuludq	%mm7, %mm1		C				am 1
	paddq	%mm5, %mm6		C				am 1
	movd	16(%eax), %mm2		C				am 1
	paddq	%mm0, %mm4		C				am 1
	movd	12(%edx), %mm5		C				am 1
	movd	%mm6, 4(%edx)		C				am 1
	psrlq	$32, %mm6		C				am 1
	lea	16(%eax), %eax		C				am 1
	lea	16(%edx), %edx		C				am 1
	add	$4, %ecx		C				am 1
	jnz	.Llam1			C				am 1
	pmuludq	%mm7, %mm2		C				am 1
	paddq	%mm4, %mm6		C				am 1
	paddq	%mm1, %mm5		C				am 1
	movd	(%edx), %mm4		C				am 1
	movd	%mm6, -8(%edx)		C				am 1
	psrlq	$32, %mm6		C				am 1
	paddq	%mm5, %mm6		C				am 1
	paddq	%mm2, %mm4		C				am 1
.Lx1:	movd	%mm6, -4(%edx)		C				am 1
	psrlq	$32, %mm6		C				am 1
	paddq	%mm4, %mm6		C				am 1
	movd	%mm6, (%edx)		C				am 1
	psrlq	$32, %mm6		C				am 1
	movd	%mm6, 4(%edx)		C				am 1
	dec	%ebx			C				am 1
	jnz	.Lolp1			C				am 1
.Loel1:	emms				C				   1
	pop	%edi			C				   1
	pop	%ebx			C				   1
	pop	%esi			C				   1
	ret				C				   1


.L2:	movd	(%eax), %mm1		C				m 2
	sub	24(%esp), %ecx		C				m 2
	mov	%ecx, 24(%esp)		C update loop count for later	m 2
	pmuludq	%mm7, %mm1		C				m 2
	movd	4(%eax), %mm4		C				m 2
	pmuludq	%mm7, %mm4		C				m 2
	movd	8(%eax), %mm3		C				m 2
	jmp	.Lm10			C				m 2
	ALIGN(16)			C				m 2
.Llpm2:	pmuludq	%mm7, %mm4		C				m 2
	paddq	%mm0, %mm6		C				m 2
	movd	8(%eax), %mm3		C				m 2
	movd	%mm6, -4(%edx)		C				m 2
	psrlq	$32, %mm6		C				m 2
.Lm10:	pmuludq	%mm7, %mm3		C				m 2
	paddq	%mm1, %mm6		C				m 2
	movd	12(%eax), %mm0		C				m 2
	movd	%mm6, (%edx)		C				m 2
	psrlq	$32, %mm6		C				m 2
	pmuludq	%mm7, %mm0		C				m 2
	paddq	%mm4, %mm6		C				m 2
	movd	16(%eax), %mm1		C				m 2
	movd	%mm6, 4(%edx)		C				m 2
	psrlq	$32, %mm6		C				m 2
	pmuludq	%mm7, %mm1		C				m 2
	paddq	%mm3, %mm6		C				m 2
	movd	20(%eax), %mm4		C				m 2
	movd	%mm6, 8(%edx)		C				m 2
	psrlq	$32, %mm6		C				m 2
	lea	16(%eax), %eax		C				m 2
	lea	16(%edx), %edx		C				m 2
	add	$4, %ecx		C				m 2
	ja	.Llpm2			C				m 2
	pmuludq	%mm7, %mm4		C				m 2
	paddq	%mm0, %mm6		C				m 2
	movd	%mm6, -4(%edx)		C				m 2
	psrlq	$32, %mm6		C				m 2
	paddq	%mm1, %mm6		C				m 2
	mov	16(%esp), %edi		C rp				  2
	jmp	.Lx2

.Lolp2:	lea	4(%edi), %edi		C				am 2
	movd	(%esi), %mm7		C				am 2
	lea	4(%esi), %esi		C				am 2
	mov	%edi, %edx		C rp				am 2
	mov	20(%esp), %eax		C up				am 2
	movd	(%eax), %mm1		C				am 2
	mov	24(%esp), %ecx		C inner loop count		am 2
	pxor	%mm6, %mm6		C				am 2
	pmuludq	%mm7, %mm1		C				am 2
	movd	4(%eax), %mm2		C				am 2
	movd	(%edx), %mm5		C				am 2
	pmuludq	%mm7, %mm2		C				am 2
	movd	8(%eax), %mm3		C				am 2
	paddq	%mm1, %mm5		C				am 2
	movd	4(%edx), %mm4		C				am 2
	jmp	.Lam10			C				am 2
	ALIGN(16)			C				am 2
.Llam2:	pmuludq	%mm7, %mm2		C				am 2
	paddq	%mm4, %mm6		C				am 2
	movd	8(%eax), %mm3		C				am 2
	paddq	%mm1, %mm5		C				am 2
	movd	4(%edx), %mm4		C				am 2
	movd	%mm6, -4(%edx)		C				am 2
	psrlq	$32, %mm6		C				am 2
.Lam10:	pmuludq	%mm7, %mm3		C				am 2
	paddq	%mm5, %mm6		C				am 2
	movd	12(%eax), %mm0		C				am 2
	paddq	%mm2, %mm4		C				am 2
	movd	8(%edx), %mm5		C				am 2
	movd	%mm6, (%edx)		C				am 2
	psrlq	$32, %mm6		C				am 2
	pmuludq	%mm7, %mm0		C				am 2
	paddq	%mm4, %mm6		C				am 2
	movd	16(%eax), %mm1		C				am 2
	paddq	%mm3, %mm5		C				am 2
	movd	12(%edx), %mm4		C				am 2
	movd	%mm6, 4(%edx)		C				am 2
	psrlq	$32, %mm6		C				am 2
	pmuludq	%mm7, %mm1		C				am 2
	paddq	%mm5, %mm6		C				am 2
	movd	20(%eax), %mm2		C				am 2
	paddq	%mm0, %mm4		C				am 2
	movd	16(%edx), %mm5		C				am 2
	movd	%mm6, 8(%edx)		C				am 2
	psrlq	$32, %mm6		C				am 2
	lea	16(%eax), %eax		C				am 2
	lea	16(%edx), %edx		C				am 2
	add	$4, %ecx		C				am 2
	jnz	.Llam2			C				am 2
	pmuludq	%mm7, %mm2		C				am 2
	paddq	%mm4, %mm6		C				am 2
	paddq	%mm1, %mm5		C				am 2
	movd	4(%edx), %mm4		C				am 2
	movd	%mm6, -4(%edx)		C				am 2
	psrlq	$32, %mm6		C				am 2
	paddq	%mm5, %mm6		C				am 2
	paddq	%mm2, %mm4		C				am 2
.Lx2:	movd	%mm6, (%edx)		C				am 2
	psrlq	$32, %mm6		C				am 2
	paddq	%mm4, %mm6		C				am 2
	movd	%mm6, 4(%edx)		C				am 2
	psrlq	$32, %mm6		C				am 2
	movd	%mm6, 8(%edx)		C				am 2
	dec	%ebx			C				am 2
	jnz	.Lolp2			C				am 2
.Loel2:	emms				C				   2
	pop	%edi			C				   2
	pop	%ebx			C				   2
	pop	%esi			C				   2
	ret				C				   2


.L3:	movd	(%eax), %mm0		C				m 3
	sub	24(%esp), %ecx		C				m 3
	mov	%ecx, 24(%esp)		C update loop count for later	m 3
	pmuludq	%mm7, %mm0		C				m 3
	movd	4(%eax), %mm1		C				m 3
	pmuludq	%mm7, %mm1		C				m 3
	movd	8(%eax), %mm4		C				m 3
	jmp	.Llpm3			C				m 3
	ALIGN(16)			C				m 3
.Llpm3:	pmuludq	%mm7, %mm4		C				m 3
	paddq	%mm0, %mm6		C				m 3
	movd	12(%eax), %mm3		C				m 3
	movd	%mm6, (%edx)		C				m 3
	psrlq	$32, %mm6		C				m 3
	pmuludq	%mm7, %mm3		C				m 3
	paddq	%mm1, %mm6		C				m 3
	movd	16(%eax), %mm0		C				m 3
	movd	%mm6, 4(%edx)		C				m 3
	psrlq	$32, %mm6		C				m 3
	pmuludq	%mm7, %mm0		C				m 3
	paddq	%mm4, %mm6		C				m 3
	movd	20(%eax), %mm1		C				m 3
	movd	%mm6, 8(%edx)		C				m 3
	psrlq	$32, %mm6		C				m 3
	pmuludq	%mm7, %mm1		C				m 3
	paddq	%mm3, %mm6		C				m 3
	movd	24(%eax), %mm4		C				m 3
	movd	%mm6, 12(%edx)		C				m 3
	psrlq	$32, %mm6		C				m 3
	lea	16(%eax), %eax		C				m 3
	lea	16(%edx), %edx		C				m 3
	add	$4, %ecx		C				m 3
	ja	.Llpm3			C				m 3
	pmuludq	%mm7, %mm4		C				m 3
	paddq	%mm0, %mm6		C				m 3
	movd	%mm6, (%edx)		C				m 3
	psrlq	$32, %mm6		C				m 3
	paddq	%mm1, %mm6		C				m 3
	mov	16(%esp), %edi		C rp				  3
	jmp	.Lx3

.Lolp3:	lea	4(%edi), %edi		C				am 3
	movd	(%esi), %mm7		C				am 3
	lea	4(%esi), %esi		C				am 3
	mov	%edi, %edx		C rp				am 3
	mov	20(%esp), %eax		C up				am 3
	movd	(%eax), %mm0		C				am 3
	mov	24(%esp), %ecx		C inner loop count		am 3
	pxor	%mm6, %mm6		C				am 3
	pmuludq	%mm7, %mm0		C				am 3
	movd	4(%eax), %mm1		C				am 3
	movd	(%edx), %mm4		C				am 3
	pmuludq	%mm7, %mm1		C				am 3
	movd	8(%eax), %mm2		C				am 3
	paddq	%mm0, %mm4		C				am 3
	movd	4(%edx), %mm5		C				am 3
	jmp	.Llam3			C				am 3
	ALIGN(16)			C				am 3
.Llam3:	pmuludq	%mm7, %mm2		C				am 3
	paddq	%mm4, %mm6		C				am 3
	movd	12(%eax), %mm3		C				am 3
	paddq	%mm1, %mm5		C				am 3
	movd	8(%edx), %mm4		C				am 3
	movd	%mm6, (%edx)		C				am 3
	psrlq	$32, %mm6		C				am 3
	pmuludq	%mm7, %mm3		C				am 3
	paddq	%mm5, %mm6		C				am 3
	movd	16(%eax), %mm0		C				am 3
	paddq	%mm2, %mm4		C				am 3
	movd	12(%edx), %mm5		C				am 3
	movd	%mm6, 4(%edx)		C				am 3
	psrlq	$32, %mm6		C				am 3
	pmuludq	%mm7, %mm0		C				am 3
	paddq	%mm4, %mm6		C				am 3
	movd	20(%eax), %mm1		C				am 3
	paddq	%mm3, %mm5		C				am 3
	movd	16(%edx), %mm4		C				am 3
	movd	%mm6, 8(%edx)		C				am 3
	psrlq	$32, %mm6		C				am 3
	pmuludq	%mm7, %mm1		C				am 3
	paddq	%mm5, %mm6		C				am 3
	movd	24(%eax), %mm2		C				am 3
	paddq	%mm0, %mm4		C				am 3
	movd	20(%edx), %mm5		C				am 3
	movd	%mm6, 12(%edx)		C				am 3
	psrlq	$32, %mm6		C				am 3
	lea	16(%eax), %eax		C				am 3
	lea	16(%edx), %edx		C				am 3
	add	$4, %ecx		C				am 3
	jnz	.Llam3			C				am 3
	pmuludq	%mm7, %mm2		C				am 3
	paddq	%mm4, %mm6		C				am 3
	paddq	%mm1, %mm5		C				am 3
	movd	8(%edx), %mm4		C				am 3
	movd	%mm6, (%edx)		C				am 3
	psrlq	$32, %mm6		C				am 3
	paddq	%mm5, %mm6		C				am 3
	paddq	%mm2, %mm4		C				am 3
.Lx3:	movd	%mm6, 4(%edx)		C				am 3
	psrlq	$32, %mm6		C				am 3
	paddq	%mm4, %mm6		C				am 3
	movd	%mm6, 8(%edx)		C				am 3
	psrlq	$32, %mm6		C				am 3
	movd	%mm6, 12(%edx)		C				am 3
	dec	%ebx			C				am 3
	jnz	.Lolp3			C				am 3
.Loel3:	emms				C				   3
	pop	%edi			C				   3
	pop	%ebx			C				   3
	pop	%esi			C				   3
	ret				C				   3
EPILOGUE()