/**********************************************************************************************************
 * @�ļ�     judgement.h
 * @˵��     judgement.c��ͷ�ļ�����������ϵͳ�����ݶ���
 * @�汾  	 V1.1
 * @����     2019.04
**********************************************************************************************************/
#ifndef __JUDGEMENT_H__
#define __JUDGEMENT_H__
#include "stm32f10x.h"
/***********************************
 0x0001 --- ����״̬����
 0x0002 --- �����������
 0x0003 --- ���������˴������
 0x0101 --- �����¼�
 0x0102 --- ����վ����
 0x0103 --- ����վ����
 0x0201 --- ������״̬
 0x0202 --- ����������
 0x0203 --- ������λ��
 0x0204 --- ��������������
 0x0205 --- ����������״̬����
 0x0206 --- �˺�����
 0x0207 --- �������
 0x0301 --- �����˽�������
************************************/
typedef __packed struct
{
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
} ext_game_state_t;

typedef __packed struct
{
	uint8_t winner;
} ext_game_result_t;

typedef __packed struct
{
	uint16_t robot_legion;
} ext_game_robot_survivors_t;

typedef __packed struct
{
	uint32_t event_type;
} ext_event_data_t;

typedef __packed struct
{
	uint8_t supply_projectile_id;
	uint8_t supply_robot_id;
	uint8_t supply_projectile_step;
	uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

typedef __packed struct
{
	uint8_t supply_projectile_id;
	uint8_t supply_robot_id;
	uint8_t supply_num;
} ext_supply_projectile_booking_t;

typedef __packed struct
{
	uint8_t robot_id;
	uint8_t robot_level;
	uint16_t remain_HP;
	uint16_t max_HP;
	uint16_t shooter_heat0_cooling_rate;
	uint16_t shooter_heat0_cooling_limit;
	uint16_t shooter_heat1_cooling_rate;
	uint16_t shooter_heat1_cooling_limit;
	uint8_t mains_power_gimbal_output : 1;
	uint8_t mains_power_chassis_output : 1;
	uint8_t mains_power_shooter_output : 1;
} ext_game_robot_state_t;

typedef __packed struct
{
	uint16_t chassis_volt;
	uint16_t chassis_current;
	float chassis_power;
	uint16_t chassis_power_buffer;
	uint16_t shooter_heat0;
	uint16_t shooter_heat1;
} ext_power_heat_data_t;

typedef __packed struct
{
	float x;
	float y;
	float z;
	float yaw;
} ext_game_robot_pos_t;

typedef __packed struct
{
	uint8_t power_rune_buff;
}ext_buff_musk_t;

typedef __packed struct
{
	uint8_t energy_point;
	uint8_t attack_time;
} aerial_robot_energy_t;

typedef __packed struct
{
	uint8_t armor_id : 4;
	uint8_t hurt_type : 4;
}ext_robot_hurt_t;

typedef __packed struct
{
	uint8_t bullet_type;
	uint8_t bullet_freq;
	float bullet_speed;
} ext_shoot_data_t;

typedef  struct Judgement
{
	ext_game_state_t                  game_state_t;
	ext_game_result_t                 game_result_t;
	ext_game_robot_survivors_t        game_robot_survivors_t;
	ext_event_data_t                  event_data_t;
	ext_supply_projectile_action_t    supply_projectile_action_t;
	ext_supply_projectile_booking_t   supply_projectile_booking_t;
	ext_game_robot_state_t            game_robot_state_t;
	ext_power_heat_data_t             power_heat_data_t;
	ext_game_robot_pos_t              game_robot_pos_t;
	ext_buff_musk_t                   buff_musk_t;
	aerial_robot_energy_t             robot_energy_t;
	ext_robot_hurt_t                  robot_hurt_t;
	ext_shoot_data_t                  shoot_data_t;
}Judgement;
#endif // __JUDGEMENT_H__
