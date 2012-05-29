/**************************************************************************************
* 
*	Project Name : S5PC100 Validation
*
*	Copyright 2006 by Samsung Electronics, Inc.
*	All rights reserved.
*
*	Project Description :
*		This software is only for validating functions of the S5PC100.
*		Anybody can use this software without our permission.
*  
*--------------------------------------------------------------------------------------
* 
*	File Name : def.h
*  
*	File Description : This file defines some types used commonly.
*
*	Author : Haksoo,Kim
*	Dept. : AP Development Team
*	Created Date : 2006/11/08
*	Version : 0.1 
* 
*	History
*	- Created(Haksoo,Kim 2006/11/08)
*  
**************************************************************************************/

#ifndef __DEF_H__
#define __DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

// Type defines 
#define FALSE				(0)
#define TRUE				(1)
#define false				(0)
#define true				(1)

// Type defines 
typedef unsigned int		u32;
typedef unsigned short		u16;
typedef unsigned char		u8;

typedef signed int			s32;
typedef signed short		s16;
typedef signed char			s8;

typedef unsigned char		bool;

#define Outp32(addr, data)	(*(volatile u32 *)(addr) = (data))
#define Outp16(addr, data)	(*(volatile u16 *)(addr) = (data))
#define Outp8(addr, data)	(*(volatile u8 *)(addr) = (data))
#define Inp32(addr)			(*(volatile u32 *)(addr))
#define Inp16(addr)			(*(volatile u16 *)(addr))
#define Inp8(addr)			(*(volatile u8 *)(addr))

#define FEATURE_IROM_DEBUG

#ifdef __cplusplus
}
#endif

#endif

