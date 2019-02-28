/*
 * colorful.h
 *
 * Created: 2017/8/29 22:12:28
 *  Author: lee-home
 */ 


#ifndef __SRB_APP_DMOTOR_SHARE_H_
#define __SRB_APP_DMOTOR_SHARE_H_
	           //123456789abcdef
#define NODENAME PS2_handle
	 
	 
namespace NODENAME
{
//----------------datas----------------	
#ifdef __MASTER__
#undef NODENAME
#endif


#ifdef __MAPPING_DECLEAR__
uint8 mapping0[] = {6,3,4,5,6,7,8,9,0,1,2};
uint8 mapping1[] = {6,3,4,5,6,7,8,9,0,1,2};
uint8 mapping2[] = {6,3,4,5,6,7,8,9,0,1,2};
uint8 mapping3[] = {6,3,4,5,6,7,8,9,0,1,2};
#endif
#define KEYDOWN_

struct	sKeyMapping{
	uint8 sel			:1;
	uint8 l3			:1;
	uint8 r3			:1;
	uint8 sta			:1;
	uint8 u				:1;
	uint8 r				:1;
	uint8 d				:1;
	uint8 l				:1;
	
	uint8 l2			:1;
	uint8 r2			:1;
	uint8 l1			:1;
	uint8 r1			:1;
	uint8 trag			:1;
	uint8 cir			:1;
	uint8 cros			:1;
	uint8 squ			:1;
	
};

union sPS2_Handle_report
{
	uint8 u8[6];
	struct	{
		sKeyMapping key;
		struct{				
			struct{
				uint8 x;
				uint8 y;
			}r;
			struct{
				uint8 x;
				uint8 y;
			}l;
		}joy;

	};
};		

struct sDataRs 
{
	uint16 rumble_time;	
	uint8 rumble;
	struct{
		uint8 handle_exist :1;
		uint8 handle_analog :1;
		uint8 dataChanged:1;
	};
	sPS2_Handle_report handle;
};

struct csHandleConfig{
	struct{
		uint8 analog :1;
		uint8 rumble:1;
		uint8 lock_mode:1;
	};
	uint8 period_ms;
};


}
#endif /* __SRB_APP_DMOTOR_SHARE_H_ */
