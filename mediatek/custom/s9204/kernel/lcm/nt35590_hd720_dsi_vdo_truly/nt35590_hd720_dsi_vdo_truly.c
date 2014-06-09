/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)

#define LCM_ID_NT35590 (0x90)

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

static unsigned int lcm_esd_test = FALSE;      ///only for ESD test

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)   

#if 0	// defined(MTK_WFD_SUPPORT)
#define   LCM_DSI_CMD_MODE							1
#else
#define   LCM_DSI_CMD_MODE							0
#endif


static void init_lcm_registers(void)
{
	unsigned int data_array[16];

#if 1
	//NT35590B+4.99AUO HD
//SSD2825_Gen_write_1A_1P(0xFF,0x01);//Command2 page0
	data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001FF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xFB,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001FB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x00,0x4A);//Set select panel application
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00004A00;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x01,0x33);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00003301;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x02,0x53);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005302;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x03,0x55);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005503;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x04,0x55);//0x01->0x04 Set Pump Freq
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005504;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x05,0x33);//Set AVDD=6V/AVEE=-6V 
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00003305;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x06,0x22);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002206;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x08,0x56);//Set VGH level 2AVDD/VGL level 2VCL-AVDD
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005608;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x09,0x8F);//Set VGL -8V
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008F09;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0B,0xCF);//Set GVDDP=-4.7V
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000CF0B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0C,0xCF);//Set GVDDP=+4.7V
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000CF0C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0D,0x2F);//Set VCI1=3.05V
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002F0D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0E,0x29);//Set VGHO=9V
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000290E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x11,0x83);//0x7D
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008311;                 
       dsi_set_cmdq(data_array, 2, 1); 

//SSD2825_Gen_write_1A_1P(0x12,0x03);//Set VCOMDC3
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000312;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x36,0x73);//Enable gate EQ
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00007336;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0F,0x0A);//NVT ENGINEER COMMAND
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000A0F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xFF,0xEE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000EEFF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xFB,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001FB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x12,0x50);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005012;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x13,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000213;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x6A,0x60);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000606A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xFF,0x05);//LTPS Timing Start
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000005FF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xFB,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001FB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//D2825_Gen_write_1A_1P(0x01,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000001;                
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x02,0x82);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008202;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x03,0x82);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008203;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x04,0x82);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008204;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x06,0x33);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00003306;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x07,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000107;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x08,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000008;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x09,0x46);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00004609;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0A,0x46);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000460A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0D,0x0B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000B0D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0E,0x1D);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00001D0E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0F,0x08);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000080F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x10,0x53);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005310;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x11,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000011;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x12,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000012;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x14,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000114;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x15,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000015;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x16,0x05);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000516;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x17,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000017;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x19,0x7F);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00007F19;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1A,0xFF);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FF1A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1B,0x0F);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000F1B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1C,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000001C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1D,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000001D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1E,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000001E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1F,0x07);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000071F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x20,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001FF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x21,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000021;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x22,0x55);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005522;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x23,0x4D);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00004D23;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x2D,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000022D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x83,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000183;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9E,0x58);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000589E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9F,0x6A);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00006A9F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA0,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001A0;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA2,0x10);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000010A2;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBB,0x0A);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000ABB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBC,0x0A);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000ABC;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x28,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000028;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x2F,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000002F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x32,0x08);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000832;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x33,0xB8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B833;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x36,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000136;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x37,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000037;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x43,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000143;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x4B,0x21);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000214B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x4C,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000034C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x50,0x21);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002150;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x51,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000351;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x58,0x21);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002158;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x59,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000359;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x5D,0x21);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000215D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x5E,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000035E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x6C,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000006C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x6D,0x00);//LTPS Timing End
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000006D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   

//SSD2825_Read_Register(0xF4,2);
#if 1//Gamma Setting
//SSD2825_Gen_write_1A_1P(0xFF,0x01);//R+
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001FF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xFB,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001FB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x75,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000075;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x76,0x98);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00009876;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x77,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000077;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x78,0xAF);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000AF78;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x79,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000079;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7A,0xD1);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D17A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7B,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000007B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7C,0xE9);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E97C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7D,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000007D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7E,0xFE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FE7E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7F,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000017F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x80,0x10);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00001080;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x81,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000181;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x82,0x20);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002082;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x83,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000183;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x84,0x2E);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002E84;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x85,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000185;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x86,0x3B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00003B86;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x87,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000187;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x88,0x65);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00006588;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x89,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000189;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8A,0x88);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000888A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8B,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000018B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8C,0xBD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BD8C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8D,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000018D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8E,0xE7);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E78E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8F,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000028F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x90,0x27);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002790;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x91,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000291;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x92,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005992;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x93,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000293;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x94,0x5B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005B94;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x95,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000295;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x96,0x87);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008796;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x97,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000297;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x98,0xB6);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B698;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x99,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000299;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9A,0xD5);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D59A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9B,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000029B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9C,0xFD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FD9C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9D,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000039D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9E,0x19);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000199E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9F,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000039F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA0,0x40);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000040A0;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA2,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003A2;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA3,0x4C);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00004CA3;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA4,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003A4;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA5,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000059A5;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA6,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003A6;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA7,0x67);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000067A7;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA9,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003A9;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xAA,0x78);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000078AA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xAB,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003AB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xAC,0x8A);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008AAC;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xAD,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003AD;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xAE,0xA8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000A8AE;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xAF,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003AF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB0,0xB8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B8B0;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB1,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003B1;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB2,0xBE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BEB2;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   

//SSD2825_Gen_write_1A_1P(0xB3,0x00);//R-
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000B3;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB4,0x98);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000098B4;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB5,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000B5;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB6,0xAF);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000AFB6;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB7,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000B7;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB8,0xD1);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D1B8;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB9,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000B9;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBA,0xE9);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E9BA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBB,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000BB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBC,0xFE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FEBC;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBD,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001BD;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBE,0x10);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001BE;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBF,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001BF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC0,0x20);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000020C0;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC1,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001C1;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC2,0x2E);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002EC2;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC3,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001C3;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC4,0x3B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00003BC4;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC5,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001C5;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC6,0x65);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000065C6;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC7,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001C7;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC8,0x88);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000088C8;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC9,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001C9;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xCA,0xBD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BDCA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xCB,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001CB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xCC,0xE7);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E7CC;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xCD,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002CD;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xCE,0x27);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000027CE;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xCF,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002CF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD0,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000059D0;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD1,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002D1;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD2,0x5B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005BD2;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD3,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002D3;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD4,0x87);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000087D4;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD5,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002D5;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD6,0xB6);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B6D6;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD7,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002D7;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD8,0xD5);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D5D8;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD9,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002D9;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xDA,0xFD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FDDA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xDB,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003DB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xDC,0x19);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000019DC;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xDD,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003DD;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xDE,0x40);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000040DE;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xDF,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003DF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE0,0x4C);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00004CE0;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE1,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003E1;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE2,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000059E2;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE3,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003E3;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE4,0x67);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000067E4;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE5,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003E5;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE6,0x78);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000078E6;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE7,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003E7;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE8,0x8A);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008AE8;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE9,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003E9;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xEA,0xA8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000A8EA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xEB,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003EB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xEC,0xB8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B8EC;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xED,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003ED;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xEE,0xBE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BEEE;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   

//SSD2825_Gen_write_1A_1P(0xEF,0x00);//G+
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000EF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xF0,0x98);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000098F0;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xF1,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000F1;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xF2,0xAF);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000AFF2;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xF3,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000F3;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xF4,0xD1);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D1F4;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xF5,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000F5;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xF6,0xE9);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E9F6;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xF7,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000F7;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xF8,0xFE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FEF8;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xF9,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001F9;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xFA,0x10);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000010FA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xFF,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002FF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xFB,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001FB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x00,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000100;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x01,0x20);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002001;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x02,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000102;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x03,0x2E);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002E03;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x04,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000104;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x05,0x3B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00003B05;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x06,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000106;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x07,0x65);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00006507;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x08,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000108;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x09,0x88);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008809;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0A,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000010A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0B,0xBD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BD0B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0C,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000010C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0D,0xE7);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E70D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0E,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000020E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x0F,0x27);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000270F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x10,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000210;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x11,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005911;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x12,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000212;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x13,0x5B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005B13;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x14,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000214;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x15,0x87);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008715;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x16,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000216;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x17,0xB6);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B617;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x18,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000218;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x19,0xD5);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D519;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1A,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000021A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1B,0xFD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FD1B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1C,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000031C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1D,0x19);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000191D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1E,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000031E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x1F,0x40);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000401F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x20,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000320;                 
       dsi_set_cmdq(data_array, 2, 1); 

	   
//SSD2825_Gen_write_1A_1P(0x21,0x4C);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00004C21;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x22,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000322;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x23,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005923;                 
       dsi_set_cmdq(data_array, 2, 1); 

//SSD2825_Gen_write_1A_1P(0x24,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000324;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x25,0x67);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00006725;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x26,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000326;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x27,0x78);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00007827;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x28,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000328;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x29,0x8A);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008A29;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x2A,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000032A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x2B,0xA8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000A82B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x2D,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000032D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x2F,0xB8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002FB8;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x30,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000330;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x31,0xBE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BE31;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   

//SSD2825_Gen_write_1A_1P(0x32,0x00);//G-
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000032;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x33,0x98);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00009833;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x34,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000034;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x35,0xAF);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000AF35;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x36,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000036;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x37,0xD1);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D137;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x38,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000038;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x39,0xE9);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E939;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x3A,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000003A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x3B,0xFE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FE3B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x3D,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000013D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x3F,0x10);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000103F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x40,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000140;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x41,0x20);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002041;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x42,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000142;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x43,0x2E);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002E43;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x44,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000144;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x45,0x3B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00003B45;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x46,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000146;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x47,0x65);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00006547;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x48,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000148;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x49,0x88);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008849;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x4A,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000014A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x4B,0xBD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BD4B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x4C,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000014C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x4D,0xE7);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E74D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x4E,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000024E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x4F,0x27);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000274F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x50,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000250;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x51,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005951;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x52,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000252;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x53,0x5B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005B53;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x54,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000254;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x55,0x87);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008755;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0x56,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000256;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x58,0xB6);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B658;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x59,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000259;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x5A,0xD5);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D55A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x5B,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000025B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x5C,0xFD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FD5C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x5D,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000035D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x5E,0x19);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000195E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x5F,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000035F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x60,0x40);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00004060;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x61,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000361;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x62,0x4C);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00004C62;                 
       dsi_set_cmdq(data_array, 2, 1); 

//SSD2825_Gen_write_1A_1P(0x63,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000363;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0x64,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005964;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x65,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000365;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x66,0x67);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00006766;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x67,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000367;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x68,0x78);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00007868;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x69,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000369;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x6A,0x8A);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008A6A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x6B,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000036B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x6C,0xA8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000A86C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x6D,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000036D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x6E,0xB8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B86E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x6F,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000036F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x70,0xBE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BE70;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   

//SSD2825_Gen_write_1A_1P(0x71,0x00);//B+
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000071;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0x72,0x98);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00009872;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x73,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000073;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x74,0xAF);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000AF74;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x75,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000075;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x76,0xD1);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D176;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0x77,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000077;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x78,0xE9);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E978;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0x79,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000079;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7A,0xFE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FE7A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7B,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000017B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7C,0x10);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000107C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7D,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000017D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7E,0x20);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000207E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x7F,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000017E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x80,0x2E);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002E80;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x81,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000181;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x82,0x3B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00003B82;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x83,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000183;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x84,0x65);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00006584;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x85,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000185;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x86,0x88);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008886;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x87,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000187;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x88,0xBD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BD88;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x89,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000189;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8A,0xE7);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E78A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8B,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000028B;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8C,0x27);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000278C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8D,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000028D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8E,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000598E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x8F,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000028F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x90,0x5B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005B90;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x91,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000291;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0x92,0x87);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008792;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0x93,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000293;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x94,0xB6);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B694;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x95,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000295;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0x96,0xD5);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D596;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0x97,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000297;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x98,0xFD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FD98;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x99,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00000399;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9A,0x19);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000199A;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9B,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000039B;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0x9C,0x40);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000409C;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9D,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000039D;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9E,0x4C);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00004C9E;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0x9F,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000039F;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA0,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000059A0;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA2,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003A2;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xA3,0x67);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000067A3;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA4,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003A4;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA5,0x78);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000078A5;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA6,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003A6;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xA7,0x8A);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008AA7;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0XA9,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003A9;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xAA,0xA8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000A8AA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xAB,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003AB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xAC,0xB8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B8AC;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xAD,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003AD;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xAE,0xBE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BEAE;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   

//SSD2825_Gen_write_1A_1P(0xAF,0x00);//B-
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000AF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB0,0x98);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000098B0;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB1,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000B1;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB2,0xAF);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000AFB2;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB3,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000B3;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB4,0xD1);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D1B4;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB5,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000B5;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB6,0xE9);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E9B6;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB7,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000B7;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB8,0xFE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FEB8;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xB9,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001B9;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBA,0x10);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001BA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBB,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001BB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBC,0x20);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000020BC;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBD,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001BD;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBE,0x2E);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00002EBE;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBF,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001BF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC0,0x3B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00003BC0;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xC1,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001C1;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC2,0x65);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000065C2;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC3,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001C3;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC4,0x88);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000088C4;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xC5,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001C5;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC6,0xBD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BDC6;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC7,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001C7;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC8,0xE7);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000E7C8;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC9,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002C9;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xCA,0x27);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000027CA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xCB,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002CB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xCC,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000059CC;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xCD,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002CD;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xCE,0x5B);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00005BCE;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xCF,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002CF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD0,0x87);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000087D0;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xD1,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002D1;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD2,0xB6);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B6D2;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD3,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002D3;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xD4,0xD5);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000D5D4;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD5,0x02);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000002D5;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD6,0xFD);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000FDD6;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD7,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003D7;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD8,0x19);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000019D8;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xD9,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003D9;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xDA,0x40);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000040DA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xDB,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003DB;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xDC,0x4C);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00004CDC;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xDD,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003DD;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xDE,0x59);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000059DE;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xDF,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003DF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE0,0x67);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000067E0;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE1,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003E1;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE2,0x78);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000078E2;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE3,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003E3;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xE4,0x8A);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x00008AE4;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE5,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003E5;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xE6,0xA8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000A8E6;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xE7,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003E7;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xE8,0xB8);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000B8E8;                 
       dsi_set_cmdq(data_array, 2, 1); 
       
//SSD2825_Gen_write_1A_1P(0xE9,0x03);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003E9;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xEA,0xBE);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x0000BEEA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xFF,0x00);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000000FF;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
#endif

//SSD2825_Gen_write_1A_1P(0xFB,0x01);
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000001FB;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xBA,0x03);//4-lanes
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003BA;                 
       dsi_set_cmdq(data_array, 2, 1); 
	   
//SSD2825_Gen_write_1A_1P(0xC2,0x03);//0x0B:Video mode with Ram     0x08:Command Mode With RAM;0x03:Video mode bypass RAM
       data_array[0] = 0x00023902;                          
       data_array[1] = 0x000003C2;                 
       dsi_set_cmdq(data_array, 2, 1); 
       

//SSD2825_DCS_write_1A_0P(0x11);
//WaitTime(120);
//SSD2825_DCS_write_1A_0P(0x29);

       data_array[0] = 0x00110500; // Sleep Out
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(120);
	  
	data_array[0] = 0x00290500; // Display On
	dsi_set_cmdq(data_array, 1, 1);

#endif


#if 0
	data_array[0] = 0x00023902;                          
    data_array[1] = 0x00009036;                 
    dsi_set_cmdq(data_array, 2, 1); 

	data_array[0] = 0x00023902;//CMD1                           
    data_array[1] = 0x000000FF;                 
    dsi_set_cmdq(data_array, 2, 1);     
    	
    data_array[0] = 0x00023902;//03 4lane  02 3lanes               
    data_array[1] = 0x000003BA;				//0x000002BA;                 //by lishengli 20130701
    dsi_set_cmdq(data_array, 2, 1);    
    	
    data_array[0] = 0x00023902;//03 Video 08 command
    #if (LCM_DSI_CMD_MODE)
		data_array[1] = 0x000008C2; 
    #else
		data_array[1] = 0x000003C2; 
    #endif                
    dsi_set_cmdq(data_array, 2, 1);   
    	
    data_array[0] = 0x00023902;//CMD2,Page0  
    data_array[1] = 0x000001FF;                 
    dsi_set_cmdq(data_array, 2, 1);   
    	
    data_array[0] = 0x00023902;//720*1280 
    data_array[1] = 0x00003A00;                 
    dsi_set_cmdq(data_array, 2, 1);  
    	
    data_array[0] = 0x00023902;
    data_array[1] = 0x00003301; //4401                
    dsi_set_cmdq(data_array, 2, 1);  
    	
    data_array[0] = 0x00023902;
    data_array[1] = 0x00005302; //5402               
    dsi_set_cmdq(data_array, 2, 1); 
	
	data_array[0] = 0x00023902;//VGL=-6V 
    data_array[1] = 0x00008509; //0309                
    dsi_set_cmdq(data_array, 2, 1);  
    	
    data_array[0] = 0x00023902;//VGH=+8.6V 
    data_array[1] = 0x0000250E;                 
    dsi_set_cmdq(data_array, 2, 1);  
    	
    data_array[0] = 0x00023902;//turn off VGLO regulator   
    data_array[1] = 0x00000A0F;                 
    dsi_set_cmdq(data_array, 2, 1);  
    	
    data_array[0] = 0x00023902;//GVDDP=4V     
    data_array[1] = 0x0000970B;                 
    dsi_set_cmdq(data_array, 2, 1);  
    	
    data_array[0] = 0x00023902;
    data_array[1] = 0x0000970C;                 
    dsi_set_cmdq(data_array, 2, 1);  

    data_array[0] = 0x00023902; 
    data_array[1] = 0x00008611; //8611                
    dsi_set_cmdq(data_array, 2, 1); 

	data_array[0] = 0x00023902;//VCOMDC 
    data_array[1] = 0x00000312;                 
    dsi_set_cmdq(data_array, 2, 1); 
    	
    data_array[0] = 0x00023902;  
    data_array[1] = 0x00007B36;                 
    dsi_set_cmdq(data_array, 2, 1);
	
#if 1
	data_array[0] = 0x00023902;  
    data_array[1] = 0x000080B0;                 
    dsi_set_cmdq(data_array, 2, 1); 

	data_array[0] = 0x00023902;  
    data_array[1] = 0x000002B1;                 
    dsi_set_cmdq(data_array, 2, 1); 
#endif 

    data_array[0] = 0x00023902;//GVDDP=4V     
    data_array[1] = 0x00002C71;                 
    dsi_set_cmdq(data_array, 2, 1);  
#if 1
    data_array[0] = 0x00023902;
    data_array[1] = 0x000005FF;         
    dsi_set_cmdq(data_array, 2, 1);   

	data_array[0] = 0x00023902; /////////////LTPS 
    data_array[1] = 0x00000001;                   
    dsi_set_cmdq(data_array, 2, 1);              
    data_array[0] = 0x00023902;                   
    data_array[1] = 0x00008D02;                   
    dsi_set_cmdq(data_array, 2, 1);              
    data_array[0] = 0x00023902;                   
    data_array[1] = 0x00008D03;                   
    dsi_set_cmdq(data_array, 2, 1);              
    data_array[0] = 0x00023902;                   
    data_array[1] = 0x00008D04;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x00003005;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;//06         
    data_array[1] = 0x00003306;             
    dsi_set_cmdq(data_array, 2, 1); 
	
    data_array[0] = 0x00023902;             
    data_array[1] = 0x00007707;             
    dsi_set_cmdq(data_array, 2, 1);        
    data_array[0] = 0x00023902;             
    data_array[1] = 0x00000008;        
    dsi_set_cmdq(data_array, 2, 1);   
    data_array[0] = 0x00023902;        
    data_array[1] = 0x00000009;        
    dsi_set_cmdq(data_array, 2, 1);   
    data_array[0] = 0x00023902;        
    data_array[1] = 0x0000000A;        
    dsi_set_cmdq(data_array, 2, 1);   
    data_array[0] = 0x00023902;        
    data_array[1] = 0x0000800B;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;//0C 
    data_array[1] = 0x0000C80C;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902; //0D
    data_array[1] = 0x0000000D;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x00001B0E; 
	
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x0000070F;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x00005710;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x00000011;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;//12 
    data_array[1] = 0x00000012;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;            
    data_array[1] = 0x00001E13;            
    dsi_set_cmdq(data_array, 2, 1);       
    data_array[0] = 0x00023902;            
    data_array[1] = 0x00000014;            
    dsi_set_cmdq(data_array, 2, 1);       
    data_array[0] = 0x00023902;            
    data_array[1] = 0x00001A15;            
    dsi_set_cmdq(data_array, 2, 1);       
    data_array[0] = 0x00023902;            
    data_array[1] = 0x00000516;            
    dsi_set_cmdq(data_array, 2, 1); 
	
    data_array[0] = 0x00023902;            
    data_array[1] = 0x00000017;             
    dsi_set_cmdq(data_array, 2, 1);     
    data_array[0] = 0x00023902;//12 
    data_array[1] = 0x00001E18;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;            
    data_array[1] = 0x0000FF19;            
    dsi_set_cmdq(data_array, 2, 1);       
    data_array[0] = 0x00023902;            
    data_array[1] = 0x0000001A;            
    dsi_set_cmdq(data_array, 2, 1);       
    data_array[0] = 0x00023902;            
    data_array[1] = 0x0000FC1B;            
    dsi_set_cmdq(data_array, 2, 1);       
    data_array[0] = 0x00023902;            
    data_array[1] = 0x0000801C;            
    dsi_set_cmdq(data_array, 2, 1);       
    data_array[0] = 0x00023902;            
    data_array[1] = 0x0000001D; //101D            
    dsi_set_cmdq(data_array, 2, 1);     
    data_array[0] = 0x00023902;
	data_array[1] = 0x0000101E; //011E            
	dsi_set_cmdq(data_array, 2, 1);     
			                                     
	data_array[0] = 0x00023902;          
    data_array[1] = 0x0000771F;          
    dsi_set_cmdq(data_array, 2, 1);  
	data_array[0] = 0x00023902;                                   
    data_array[1] = 0x00000020;          
    dsi_set_cmdq(data_array, 2, 1);     
    data_array[0] = 0x00023902;          
    data_array[1] = 0x00000221;         
    dsi_set_cmdq(data_array, 2, 1);     
    data_array[0] = 0x00023902;          
    data_array[1] = 0x00000022; //5522          
    dsi_set_cmdq(data_array, 2, 1);      
    data_array[0] = 0x00023902;            
    data_array[1] = 0x00000D23;            
    dsi_set_cmdq(data_array, 2, 1);        
    data_array[0] = 0x00023902;//06 
    data_array[1] = 0x0000A031;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x00000032;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x0000B833;         
    dsi_set_cmdq(data_array, 2, 1);
	
    data_array[0] = 0x00023902;            
    data_array[1] = 0x0000BB34;            
    dsi_set_cmdq(data_array, 2, 1);        
    data_array[0] = 0x00023902;             
    data_array[1] = 0x00001135;             
    dsi_set_cmdq(data_array, 2, 1);        
    data_array[0] = 0x00023902;             
    data_array[1] = 0x00000136;             
    dsi_set_cmdq(data_array, 2, 1);        
    data_array[0] = 0x00023902;//0C         
    data_array[1] = 0x00000B37;             
    dsi_set_cmdq(data_array, 2, 1);        
    data_array[0] = 0x00023902; //0D        
    data_array[1] = 0x00000138;             
    dsi_set_cmdq(data_array, 2, 1);        
    data_array[0] = 0x00023902;             
    data_array[1] = 0x00000B39;             
    dsi_set_cmdq(data_array, 2, 1); 	
    data_array[0] = 0x00023902;             
    data_array[1] = 0x00000844;             
    dsi_set_cmdq(data_array, 2, 1);        
    data_array[0] = 0x00023902;             
    data_array[1] = 0x00008045;             
    dsi_set_cmdq(data_array, 2, 1); 
	
    data_array[0] = 0x00023902;                
    data_array[1] = 0x0000CC46;                
    dsi_set_cmdq(data_array, 2, 1);           
    data_array[0] = 0x00023902;//12            
    data_array[1] = 0x00000447;                
    dsi_set_cmdq(data_array, 2, 1);           
    data_array[0] = 0x00023902;                          
    data_array[1] = 0x00000048;                          
    dsi_set_cmdq(data_array, 2, 1);                     
    data_array[0] = 0x00023902;                          
    data_array[1] = 0x00000049;                                 
    dsi_set_cmdq(data_array, 2, 1);                            
    data_array[0] = 0x00023902;                                 
    data_array[1] = 0x0000014A;                                 
    dsi_set_cmdq(data_array, 2, 1);  
	data_array[0] = 0x00023902;                                 
    data_array[1] = 0x0000036C;                                 
    dsi_set_cmdq(data_array, 2, 1);                            
    data_array[0] = 0x00023902;                                 
    data_array[1] = 0x0000036D;                                 
    dsi_set_cmdq(data_array, 2, 1);                            
    data_array[0] = 0x00023902;//18                             
    data_array[1] = 0x00002F6E;                                 
	dsi_set_cmdq(data_array, 2, 1); 		
			
    data_array[0] = 0x00023902; ////
    data_array[1] = 0x00000043;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x0000234B;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x0000014C;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;      
    data_array[1] = 0x00002350;      
    dsi_set_cmdq(data_array, 2, 1); 
    data_array[0] = 0x00023902;      
    data_array[1] = 0x00000151;      
    dsi_set_cmdq(data_array, 2, 1); 
    data_array[0] = 0x00023902;//06  
    data_array[1] = 0x00002358;      
    dsi_set_cmdq(data_array, 2, 1); 
    data_array[0] = 0x00023902;      
    data_array[1] = 0x00000159;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x0000235D;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x0000015E;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x00002362;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x00000163;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;//0C 
    data_array[1] = 0x00002367;       
    dsi_set_cmdq(data_array, 2, 1); 
    data_array[0] = 0x00023902; //0D
    data_array[1] = 0x00000168;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x00000089;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x0000018D;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x0000648E;
    dsi_set_cmdq(data_array, 2, 1);
	
    data_array[0] = 0x00023902;                       
    data_array[1] = 0x0000208F;                       
    dsi_set_cmdq(data_array, 2, 1); 	
	data_array[0] = 0x00023902;//12                   
    data_array[1] = 0x00008E97;                       
    dsi_set_cmdq(data_array, 2, 1);                  
    data_array[0] = 0x00023902;                                 
    data_array[1] = 0x00008C82;                                 
    dsi_set_cmdq(data_array, 2, 1);                            
    data_array[0] = 0x00023902;                                 
    data_array[1] = 0x00000283;                                 
    dsi_set_cmdq(data_array, 2, 1);                            
    data_array[0] = 0x00023902;                                 
    data_array[1] = 0x00000ABB;                                 
    dsi_set_cmdq(data_array, 2, 1);                            
    data_array[0] = 0x00023902;                                 
    data_array[1] = 0x00000ABC; // 02BC                                
    dsi_set_cmdq(data_array, 2, 1);                            
    data_array[0] = 0x00023902;                                 
    data_array[1] = 0x00002524;                                 
    dsi_set_cmdq(data_array, 2, 1);                            
    data_array[0] = 0x00023902;//18                             
    data_array[1] = 0x00005525;                                 
	dsi_set_cmdq(data_array, 2, 1); 	
			
	data_array[0] = 0x00023902;      
    data_array[1] = 0x00000526;      
    dsi_set_cmdq(data_array, 2, 1); 
    data_array[0] = 0x00023902;      
    data_array[1] = 0x00002327;      
    dsi_set_cmdq(data_array, 2, 1); 
    data_array[0] = 0x00023902;      
    data_array[1] = 0x00000128;      
    dsi_set_cmdq(data_array, 2, 1); 
    data_array[0] = 0x00023902;      
    data_array[1] = 0x00003129; // 0029     
    dsi_set_cmdq(data_array, 2, 1); 
    data_array[0] = 0x00023902;      
    data_array[1] = 0x00005D2A;      
    dsi_set_cmdq(data_array, 2, 1); 
    data_array[0] = 0x00023902;//06 
    data_array[1] = 0x0000012B;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x0000002F;     
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0] = 0x00023902;     
    data_array[1] = 0x00001030; 
    dsi_set_cmdq(data_array, 2, 1); 
	
    data_array[0] = 0x00023902;             
    data_array[1] = 0x000012A7;             
    dsi_set_cmdq(data_array, 2, 1);        
    data_array[0] = 0x00023902;             
    data_array[1] = 0x0000032D;             
    dsi_set_cmdq(data_array, 2, 1);
#endif

    data_array[0] = 0x00023902;////CMD1 
    data_array[1] = 0x000000FF;                 
    dsi_set_cmdq(data_array, 2, 1);    
    data_array[0] = 0x00023902;
    data_array[1] = 0x000001FB;                 
    dsi_set_cmdq(data_array, 2, 1);    
    data_array[0] = 0x00023902;//CMD2,Page0 
    data_array[1] = 0x000001FF;                 
    dsi_set_cmdq(data_array, 2, 1);       
    data_array[0] = 0x00023902;
    data_array[1] = 0x000001FB;                 
    dsi_set_cmdq(data_array, 2, 1);   
    data_array[0] = 0x00023902;//CMD2,Page1 
    data_array[1] = 0x000002FF;                 
    dsi_set_cmdq(data_array, 2, 1);       	
    data_array[0] = 0x00023902;
    data_array[1] = 0x000001FB;                 
    dsi_set_cmdq(data_array, 2, 1);       
    	
    data_array[0] = 0x00023902;//CMD2,Page2 
    data_array[1] = 0x000003FF;                 
    dsi_set_cmdq(data_array, 2, 1);       	
    data_array[0] = 0x00023902;
    data_array[1] = 0x000001FB;                 
    dsi_set_cmdq(data_array, 2, 1);     
    data_array[0] = 0x00023902;//CMD2,Page3
    data_array[1] = 0x000004FF;         
    dsi_set_cmdq(data_array, 2, 1);                                        
    data_array[0] = 0x00023902;         
    data_array[1] = 0x000001FB;         
    dsi_set_cmdq(data_array, 2, 1);    
    data_array[0] = 0x00023902;//CMD2,Page4
    data_array[1] = 0x000005FF;         
    dsi_set_cmdq(data_array, 2, 1);    
    data_array[0] = 0x00023902;         
    data_array[1] = 0x000001FB;         
    dsi_set_cmdq(data_array, 2, 1);
	data_array[0] = 0x00023902;     ////CMD1     
    data_array[1] = 0x000000FF;         
    dsi_set_cmdq(data_array, 2, 1); 

	/*******debug-----start********/
	data_array[0] = 0x00110500;                
    dsi_set_cmdq(data_array, 1, 1); 
    MDELAY(120); 
    	
    data_array[0] = 0x00023902;//not open CABC    
    data_array[1] = 0x0000FF51;         
    dsi_set_cmdq(data_array, 2, 1);    
    	                                    
    data_array[0] = 0x00023902;         
    data_array[1] = 0x00002C53;         
    dsi_set_cmdq(data_array, 2, 1); 
    	
    data_array[0] = 0x00023902;     
    data_array[1] = 0x00000055;         
    dsi_set_cmdq(data_array, 2, 1);  
    	
    data_array[0] = 0x00290500;                
    dsi_set_cmdq(data_array, 1, 1); 
    	
    data_array[0] = 0x00023902;         
    data_array[1] = 0x000000FF;         
    dsi_set_cmdq(data_array, 2, 1); 
    	
    data_array[0] = 0x00023902;     
    data_array[1] = 0x00000035;         
    dsi_set_cmdq(data_array, 2, 1); 
	
	data_array[0] = 0x00033902;
	data_array[1] = (((FRAME_HEIGHT/2)&0xFF) << 16) | (((FRAME_HEIGHT/2)>>8) << 8) | 0x44;
	dsi_set_cmdq(data_array, 2, 1);
	/*******debug-----end********/
#endif
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

        #if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
        #else
		params->dsi.mode   = BURST_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE; 
        #endif
	
		// DSI
		/* Command mode setting */
		//1 Three lane or Four lane
		params->dsi.LANE_NUM				= LCM_FOUR_LANE;		//LCM_THREE_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Video mode setting		
		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		
		params->dsi.vertical_sync_active				= 1;// 3    2
		params->dsi.vertical_backporch					= 1;// 20   1
		params->dsi.vertical_frontporch					= 2; // 1  12
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 2;// 50  2
		params->dsi.horizontal_backporch				= 12;
		params->dsi.horizontal_frontporch				= 80;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

	    //params->dsi.LPX=8; 

		// Bit rate calculation
		//1 Every lane speed
		params->dsi.pll_select=1;
		params->dsi.pll_div1=0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		params->dsi.pll_div2=1;		// div2=0,1,2,3;div1_real=1,2,4,4	
#if (LCM_DSI_CMD_MODE)
		params->dsi.fbk_div =19;
#else
		params->dsi.fbk_div =16;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	
#endif

}

static void lcm_init(void)
{
       unsigned int data_array[16];

		
       SET_RESET_PIN(0);
       MDELAY(20); 
       SET_RESET_PIN(1);
       MDELAY(20); 
       
       data_array[0] = 0x00023902;
       data_array[1] = 0x0000EEFF; 				
       dsi_set_cmdq(data_array, 2, 1);
       MDELAY(2); 
       data_array[0] = 0x00023902;
       data_array[1] = 0x00000826; 				
       dsi_set_cmdq(data_array, 2, 1);
       MDELAY(2); 
       data_array[0] = 0x00023902;
       data_array[1] = 0x00000026; 				
       dsi_set_cmdq(data_array, 2, 1);
       MDELAY(2); 
       data_array[0] = 0x00023902;
       data_array[1] = 0x000000FF; 				
       dsi_set_cmdq(data_array, 2, 1);
       
       SET_RESET_PIN(0);
       MDELAY(20); 
       SET_RESET_PIN(1);
       MDELAY(20); 
       
	init_lcm_registers();
#if 0	
	    unsigned int data_array[16];
		
		SET_RESET_PIN(0);
		MDELAY(20); 
		SET_RESET_PIN(1);
		MDELAY(20); 
	
		data_array[0] = 0x00023902;
		data_array[1] = 0x0000EEFF; 				
		dsi_set_cmdq(data_array, 2, 1);
		MDELAY(2); 
		data_array[0] = 0x00023902;
		data_array[1] = 0x00000826; 				
		dsi_set_cmdq(data_array, 2, 1);
		MDELAY(2); 
		data_array[0] = 0x00023902;
		data_array[1] = 0x00000026; 				
		dsi_set_cmdq(data_array, 2, 1);
		MDELAY(2); 
		data_array[0] = 0x00023902;
		data_array[1] = 0x000000FF; 				
		dsi_set_cmdq(data_array, 2, 1);
		
		SET_RESET_PIN(0);
		MDELAY(20); 
		SET_RESET_PIN(1);
		MDELAY(20); 
	
		data_array[0] = 0x00023902; 						 
		data_array[1] = 0x00009036; 				
		dsi_set_cmdq(data_array, 2, 1); 
	
		data_array[0] = 0x00023902;//CMD1							
		data_array[1] = 0x000000FF; 				
		dsi_set_cmdq(data_array, 2, 1); 	
			
		data_array[0] = 0x00023902;//03 4lane  02 3lanes			   
		data_array[1] = 0x000002BA; 				
		dsi_set_cmdq(data_array, 2, 1);    
			
		data_array[0] = 0x00023902;//03 Video 08 command
    #if (LCM_DSI_CMD_MODE)
			data_array[1] = 0x000008C2; 
    #else
			data_array[1] = 0x000003C2; 
    #endif                
		dsi_set_cmdq(data_array, 2, 1);   
			
		data_array[0] = 0x00023902;//CMD2,Page0  
		data_array[1] = 0x000001FF; 				
		dsi_set_cmdq(data_array, 2, 1);   
			
		data_array[0] = 0x00023902;//720*1280 
		data_array[1] = 0x00003A00; 				
		dsi_set_cmdq(data_array, 2, 1);  
			
		data_array[0] = 0x00023902;
		data_array[1] = 0x00003301; //4401				  
		dsi_set_cmdq(data_array, 2, 1);  
			
		data_array[0] = 0x00023902;
		data_array[1] = 0x00005302; //5402				 
		dsi_set_cmdq(data_array, 2, 1); 
		
		data_array[0] = 0x00023902;//VGL=-6V 
		data_array[1] = 0x00008509; //0309				  
		dsi_set_cmdq(data_array, 2, 1);  
			
		data_array[0] = 0x00023902;//VGH=+8.6V 
		data_array[1] = 0x0000250E; 				
		dsi_set_cmdq(data_array, 2, 1);  
			
		data_array[0] = 0x00023902;//turn off VGLO regulator   
		data_array[1] = 0x00000A0F; 				
		dsi_set_cmdq(data_array, 2, 1);  
			
		data_array[0] = 0x00023902;//GVDDP=4V	  
		data_array[1] = 0x0000970B; 				
		dsi_set_cmdq(data_array, 2, 1);  
			
		data_array[0] = 0x00023902;
		data_array[1] = 0x0000970C; 				
		dsi_set_cmdq(data_array, 2, 1);  
	
		data_array[0] = 0x00023902; 
		data_array[1] = 0x00008611; //8611				  
		dsi_set_cmdq(data_array, 2, 1); 
	
		data_array[0] = 0x00023902;//VCOMDC 
		data_array[1] = 0x00000312; 				
		dsi_set_cmdq(data_array, 2, 1); 
			
		data_array[0] = 0x00023902;  
		data_array[1] = 0x00007B36; 				
		dsi_set_cmdq(data_array, 2, 1);
		
#if 1
		data_array[0] = 0x00023902;  
		data_array[1] = 0x000080B0; 				
		dsi_set_cmdq(data_array, 2, 1); 
	
		data_array[0] = 0x00023902;  
		data_array[1] = 0x000002B1; 				
		dsi_set_cmdq(data_array, 2, 1); 
#endif 
	
		data_array[0] = 0x00023902;//GVDDP=4V	  
		data_array[1] = 0x00002C71; 				
		dsi_set_cmdq(data_array, 2, 1);  
#if 1
		data_array[0] = 0x00023902;
		data_array[1] = 0x000005FF; 		
		dsi_set_cmdq(data_array, 2, 1);   
	
		data_array[0] = 0x00023902; /////////////LTPS 
		data_array[1] = 0x00000001; 				  
		dsi_set_cmdq(data_array, 2, 1); 			 
		data_array[0] = 0x00023902; 				  
		data_array[1] = 0x00008D02; 				  
		dsi_set_cmdq(data_array, 2, 1); 			 
		data_array[0] = 0x00023902; 				  
		data_array[1] = 0x00008D03; 				  
		dsi_set_cmdq(data_array, 2, 1); 			 
		data_array[0] = 0x00023902; 				  
		data_array[1] = 0x00008D04; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x00003005; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902;//06 		
		data_array[1] = 0x00003306; 			
		dsi_set_cmdq(data_array, 2, 1); 
		
		data_array[0] = 0x00023902; 			
		data_array[1] = 0x00007707; 			
		dsi_set_cmdq(data_array, 2, 1); 	   
		data_array[0] = 0x00023902; 			
		data_array[1] = 0x00000008; 	   
		dsi_set_cmdq(data_array, 2, 1);   
		data_array[0] = 0x00023902; 	   
		data_array[1] = 0x00000009; 	   
		dsi_set_cmdq(data_array, 2, 1);   
		data_array[0] = 0x00023902; 	   
		data_array[1] = 0x0000000A; 	   
		dsi_set_cmdq(data_array, 2, 1);   
		data_array[0] = 0x00023902; 	   
		data_array[1] = 0x0000800B; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902;//0C 
		data_array[1] = 0x0000C80C; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; //0D
		data_array[1] = 0x0000000D; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x00001B0E; 
		
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x0000070F; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x00005710; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x00000011; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902;//12 
		data_array[1] = 0x00000012; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x00001E13; 		   
		dsi_set_cmdq(data_array, 2, 1); 	  
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x00000014; 		   
		dsi_set_cmdq(data_array, 2, 1); 	  
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x00001A15; 		   
		dsi_set_cmdq(data_array, 2, 1); 	  
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x00000516; 		   
		dsi_set_cmdq(data_array, 2, 1); 
		
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x00000017; 			
		dsi_set_cmdq(data_array, 2, 1); 	
		data_array[0] = 0x00023902;//12 
		data_array[1] = 0x00001E18; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x0000FF19; 		   
		dsi_set_cmdq(data_array, 2, 1); 	  
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x0000001A; 		   
		dsi_set_cmdq(data_array, 2, 1); 	  
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x0000FC1B; 		   
		dsi_set_cmdq(data_array, 2, 1); 	  
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x0000801C; 		   
		dsi_set_cmdq(data_array, 2, 1); 	  
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x0000001D; //101D			  
		dsi_set_cmdq(data_array, 2, 1); 	
		data_array[0] = 0x00023902;
		data_array[1] = 0x0000101E; //011E			  
		dsi_set_cmdq(data_array, 2, 1); 	
													 
		data_array[0] = 0x00023902; 		 
		data_array[1] = 0x0000771F; 		 
		dsi_set_cmdq(data_array, 2, 1);  
		data_array[0] = 0x00023902; 								  
		data_array[1] = 0x00000020; 		 
		dsi_set_cmdq(data_array, 2, 1); 	
		data_array[0] = 0x00023902; 		 
		data_array[1] = 0x00000221; 		
		dsi_set_cmdq(data_array, 2, 1); 	
		data_array[0] = 0x00023902; 		 
		data_array[1] = 0x00000022; //5522			
		dsi_set_cmdq(data_array, 2, 1); 	 
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x00000D23; 		   
		dsi_set_cmdq(data_array, 2, 1); 	   
		data_array[0] = 0x00023902;//06 
		data_array[1] = 0x0000A031; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x00000032; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x0000B833; 		
		dsi_set_cmdq(data_array, 2, 1);
		
		data_array[0] = 0x00023902; 		   
		data_array[1] = 0x0000BB34; 		   
		dsi_set_cmdq(data_array, 2, 1); 	   
		data_array[0] = 0x00023902; 			
		data_array[1] = 0x00001135; 			
		dsi_set_cmdq(data_array, 2, 1); 	   
		data_array[0] = 0x00023902; 			
		data_array[1] = 0x00000136; 			
		dsi_set_cmdq(data_array, 2, 1); 	   
		data_array[0] = 0x00023902;//0C 		
		data_array[1] = 0x00000B37; 			
		dsi_set_cmdq(data_array, 2, 1); 	   
		data_array[0] = 0x00023902; //0D		
		data_array[1] = 0x00000138; 			
		dsi_set_cmdq(data_array, 2, 1); 	   
		data_array[0] = 0x00023902; 			
		data_array[1] = 0x00000B39; 			
		dsi_set_cmdq(data_array, 2, 1); 	
		data_array[0] = 0x00023902; 			
		data_array[1] = 0x00000844; 			
		dsi_set_cmdq(data_array, 2, 1); 	   
		data_array[0] = 0x00023902; 			
		data_array[1] = 0x00008045; 			
		dsi_set_cmdq(data_array, 2, 1); 
		
		data_array[0] = 0x00023902; 			   
		data_array[1] = 0x0000CC46; 			   
		dsi_set_cmdq(data_array, 2, 1); 		  
		data_array[0] = 0x00023902;//12 		   
		data_array[1] = 0x00000447; 			   
		dsi_set_cmdq(data_array, 2, 1); 		  
		data_array[0] = 0x00023902; 						 
		data_array[1] = 0x00000048; 						 
		dsi_set_cmdq(data_array, 2, 1); 					
		data_array[0] = 0x00023902; 						 
		data_array[1] = 0x00000049; 								
		dsi_set_cmdq(data_array, 2, 1); 						   
		data_array[0] = 0x00023902; 								
		data_array[1] = 0x0000014A; 								
		dsi_set_cmdq(data_array, 2, 1);  
		data_array[0] = 0x00023902; 								
		data_array[1] = 0x0000036C; 								
		dsi_set_cmdq(data_array, 2, 1); 						   
		data_array[0] = 0x00023902; 								
		data_array[1] = 0x0000036D; 								
		dsi_set_cmdq(data_array, 2, 1); 						   
		data_array[0] = 0x00023902;//18 							
		data_array[1] = 0x00002F6E; 								
		dsi_set_cmdq(data_array, 2, 1); 		
				
		data_array[0] = 0x00023902; ////
		data_array[1] = 0x00000043; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x0000234B; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x0000014C; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	 
		data_array[1] = 0x00002350; 	 
		dsi_set_cmdq(data_array, 2, 1); 
		data_array[0] = 0x00023902; 	 
		data_array[1] = 0x00000151; 	 
		dsi_set_cmdq(data_array, 2, 1); 
		data_array[0] = 0x00023902;//06  
		data_array[1] = 0x00002358; 	 
		dsi_set_cmdq(data_array, 2, 1); 
		data_array[0] = 0x00023902; 	 
		data_array[1] = 0x00000159; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x0000235D; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x0000015E; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x00002362; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x00000163; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902;//0C 
		data_array[1] = 0x00002367; 	  
		dsi_set_cmdq(data_array, 2, 1); 
		data_array[0] = 0x00023902; //0D
		data_array[1] = 0x00000168; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x00000089; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x0000018D; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x0000648E;
		dsi_set_cmdq(data_array, 2, 1);
		
		data_array[0] = 0x00023902; 					  
		data_array[1] = 0x0000208F; 					  
		dsi_set_cmdq(data_array, 2, 1); 	
		data_array[0] = 0x00023902;//12 				  
		data_array[1] = 0x00008E97; 					  
		dsi_set_cmdq(data_array, 2, 1); 				 
		data_array[0] = 0x00023902; 								
		data_array[1] = 0x00008C82; 								
		dsi_set_cmdq(data_array, 2, 1); 						   
		data_array[0] = 0x00023902; 								
		data_array[1] = 0x00000283; 								
		dsi_set_cmdq(data_array, 2, 1); 						   
		data_array[0] = 0x00023902; 								
		data_array[1] = 0x00000ABB; 								
		dsi_set_cmdq(data_array, 2, 1); 						   
		data_array[0] = 0x00023902; 								
		data_array[1] = 0x00000ABC; // 02BC 							   
		dsi_set_cmdq(data_array, 2, 1); 						   
		data_array[0] = 0x00023902; 								
		data_array[1] = 0x00002524; 								
		dsi_set_cmdq(data_array, 2, 1); 						   
		data_array[0] = 0x00023902;//18 							
		data_array[1] = 0x00005525; 								
		dsi_set_cmdq(data_array, 2, 1); 	
				
		data_array[0] = 0x00023902; 	 
		data_array[1] = 0x00000526; 	 
		dsi_set_cmdq(data_array, 2, 1); 
		data_array[0] = 0x00023902; 	 
		data_array[1] = 0x00002327; 	 
		dsi_set_cmdq(data_array, 2, 1); 
		data_array[0] = 0x00023902; 	 
		data_array[1] = 0x00000128; 	 
		dsi_set_cmdq(data_array, 2, 1); 
		data_array[0] = 0x00023902; 	 
		data_array[1] = 0x00003129; // 0029 	
		dsi_set_cmdq(data_array, 2, 1); 
		data_array[0] = 0x00023902; 	 
		data_array[1] = 0x00005D2A; 	 
		dsi_set_cmdq(data_array, 2, 1); 
		data_array[0] = 0x00023902;//06 
		data_array[1] = 0x0000012B; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x0000002F; 	
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x00001030; 
		dsi_set_cmdq(data_array, 2, 1); 
		
		data_array[0] = 0x00023902; 			
		data_array[1] = 0x000012A7; 			
		dsi_set_cmdq(data_array, 2, 1); 	   
		data_array[0] = 0x00023902; 			
		data_array[1] = 0x0000032D; 			
		dsi_set_cmdq(data_array, 2, 1);
#endif
	
		data_array[0] = 0x00023902;////CMD1 
		data_array[1] = 0x000000FF; 				
		dsi_set_cmdq(data_array, 2, 1);    
		data_array[0] = 0x00023902;
		data_array[1] = 0x000001FB; 				
		dsi_set_cmdq(data_array, 2, 1);    
		data_array[0] = 0x00023902;//CMD2,Page0 
		data_array[1] = 0x000001FF; 				
		dsi_set_cmdq(data_array, 2, 1); 	  
		data_array[0] = 0x00023902;
		data_array[1] = 0x000001FB; 				
		dsi_set_cmdq(data_array, 2, 1);   
		data_array[0] = 0x00023902;//CMD2,Page1 
		data_array[1] = 0x000002FF; 				
		dsi_set_cmdq(data_array, 2, 1); 		
		data_array[0] = 0x00023902;
		data_array[1] = 0x000001FB; 				
		dsi_set_cmdq(data_array, 2, 1); 	  
			
		data_array[0] = 0x00023902;//CMD2,Page2 
		data_array[1] = 0x000003FF; 				
		dsi_set_cmdq(data_array, 2, 1); 		
		data_array[0] = 0x00023902;
		data_array[1] = 0x000001FB; 				
		dsi_set_cmdq(data_array, 2, 1); 	
		data_array[0] = 0x00023902;//CMD2,Page3
		data_array[1] = 0x000004FF; 		
		dsi_set_cmdq(data_array, 2, 1); 									   
		data_array[0] = 0x00023902; 		
		data_array[1] = 0x000001FB; 		
		dsi_set_cmdq(data_array, 2, 1);    
		data_array[0] = 0x00023902;//CMD2,Page4
		data_array[1] = 0x000005FF; 		
		dsi_set_cmdq(data_array, 2, 1);    
		data_array[0] = 0x00023902; 		
		data_array[1] = 0x000001FB; 		
		dsi_set_cmdq(data_array, 2, 1);
		data_array[0] = 0x00023902; 	////CMD1	 
		data_array[1] = 0x000000FF; 		
		dsi_set_cmdq(data_array, 2, 1); 
	
		/*******debug-----start********/
		data_array[0] = 0x00110500; 			   
		dsi_set_cmdq(data_array, 1, 1); 
		MDELAY(120); 
	
		data_array[0] = 0x00023902;
		data_array[1] = 0x0000EEFF; 				
		dsi_set_cmdq(data_array, 2, 1);
		MDELAY(1); 
		data_array[0] = 0x00023902;
		data_array[1] = 0x00005012; 				
		dsi_set_cmdq(data_array, 2, 1);
		MDELAY(1); 
		data_array[0] = 0x00023902;
		data_array[1] = 0x00000213; 				
		dsi_set_cmdq(data_array, 2, 1);
		MDELAY(1); 
		data_array[0] = 0x00023902;
		data_array[1] = 0x0000606A; 				
		dsi_set_cmdq(data_array, 2, 1);
		MDELAY(1); 
		data_array[0] = 0x00023902;
		data_array[1] = 0x000000FF; 				
		dsi_set_cmdq(data_array, 2, 1);
		MDELAY(1); 
		data_array[0] = 0x00023902; 	
		data_array[1] = 0x00000035; 		
		dsi_set_cmdq(data_array, 2, 1); 
		
		data_array[0] = 0x00033902;
		data_array[1] = (((FRAME_HEIGHT/2)&0xFF) << 16) | (((FRAME_HEIGHT/2)>>8) << 8) | 0x44;
		dsi_set_cmdq(data_array, 2, 1);
			
		data_array[0] = 0x00290500; 			   
		dsi_set_cmdq(data_array, 1, 1); 
		MDELAY(50);
#endif		
}



static void lcm_suspend(void)
{
	unsigned int data_array[16];

	data_array[0]=0x00280500; // Display Off
	dsi_set_cmdq(data_array, 1, 1);
	
	data_array[0] = 0x00100500; // Sleep In
	dsi_set_cmdq(data_array, 1, 1);

	
	SET_RESET_PIN(1);	
	SET_RESET_PIN(0);
	MDELAY(1); // 1ms
	
	SET_RESET_PIN(1);
	MDELAY(120);      
}


static void lcm_resume(void)
{
	lcm_init();

    #ifdef BUILD_LK
	  printf("[LK]---cmd---nt35590----%s------\n",__func__);
    #else
	  printk("[KERNEL]---cmd---nt35590----%s------\n",__func__);
    #endif	
}
         
#if (LCM_DSI_CMD_MODE)
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);
	
	data_array[0]= 0x00053902;
	data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[2]= (y1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0]= 0x002c3909;
	dsi_set_cmdq(data_array, 1, 0);

}
#endif

static unsigned int lcm_compare_id(void)
{
	unsigned int id=0;
	unsigned char buffer[2];
	unsigned int array[16];  

	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(1);
	
	SET_RESET_PIN(1);
	MDELAY(20); 

	array[0] = 0x00023700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);
	
	read_reg_v2(0xF4, buffer, 2);
	id = buffer[0]; //we only need ID
    #ifdef BUILD_LK
		printf("%s, LK nt35590 debug: nt35590 id = 0x%08x\n", __func__, id);
    #else
		printk("%s, kernel nt35590 horse debug: nt35590 id = 0x%08x\n", __func__, id);
    #endif

    if(id == LCM_ID_NT35590)
    	return 1;
    else
        return 0;


}


static unsigned int lcm_esd_check(void)
{
  #ifndef BUILD_LK
	char  buffer[3];
	int   array[4];

	if(lcm_esd_test)
	{
		lcm_esd_test = FALSE;
		return TRUE;
	}

	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0x36, buffer, 1);
	if(buffer[0]==0x90)
	{
		return FALSE;
	}
	else
	{			 
		return TRUE;
	}
 #endif

}

static unsigned int lcm_esd_recover(void)
{
	//lcm_init();
	lcm_resume();

	return TRUE;
}



LCM_DRIVER nt35590_hd720_dsi_vdo_truly_lcm_drv = 
{
    .name			= "nt35590_hd720_dsi_vdo_truly",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,
	//.esd_check = lcm_esd_check,
	//.esd_recover = lcm_esd_recover,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
    };
