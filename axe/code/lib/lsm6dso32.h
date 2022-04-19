#ifndef LSM6DSO32_H
#define LSM6DSO32_H

#include "avr/io.h"

#define LSM6DSO32_ID 0x6C

#define LSM6DSO32_WHO_AM_I 0x0FU

#define LSM6DSO32_CTRL1_XL 0x10U
#define LSM6DSO32_CTRL2_G  0x11U
#define LSM6DSO32_CTRL3_C  0x12U
#define LSM6DSO32_CTRL4_C  0x13U
#define LSM6DSO32_CTRL5_C  0x14U
#define LSM6DSO32_CTRL6_C  0x15U
#define LSM6DSO32_CTRL7_G  0x16U
#define LSM6DSO32_CTRL8_XL 0x17U
#define LSM6DSO32_CTRL9_XL 0x18U
#define LSM6DSO32_CTRL10_C 0x19U

#define LSM6DSO32_OUT_TEMP_L 0x20U
#define LSM6DSO32_OUT_TEMP_H 0x21U
#define LSM6DSO32_OUTX_L_G   0x22U
#define LSM6DSO32_OUTX_H_G   0x23U
#define LSM6DSO32_OUTY_L_G   0x24U
#define LSM6DSO32_OUTY_H_G   0x25U
#define LSM6DSO32_OUTZ_L_G   0x26U
#define LSM6DSO32_OUTZ_H_G   0x27U
#define LSM6DSO32_OUTX_L_A   0x28U
#define LSM6DSO32_OUTX_H_A   0x29U
#define LSM6DSO32_OUTY_L_A   0x2AU
#define LSM6DSO32_OUTY_H_A   0x2BU
#define LSM6DSO32_OUTZ_L_A   0x2CU
#define LSM6DSO32_OUTZ_H_A   0x2DU

enum lsm6dso32_fs_xl
{
    LSM6DSO32_FS_XL_4g = 0x00,
    LSM6DSO32_FS_XL_8g = 0x02,
    LSM6DSO32_FS_XL_16g = 0x03,
    LSM6DSO32_FS_XL_32g = 0x01,
};

enum lsm6dso32_odr_xl
{
    LSM6DSO32_XL_ODR_OFF = 0x00,
    LSM6DSO32_XL_ODR_12Hz5 = 0x01,
    LSM6DSO32_XL_ODR_26Hz = 0x02,
    LSM6DSO32_XL_ODR_52Hz = 0x03,
    LSM6DSO32_XL_ODR_104Hz = 0x04,
    LSM6DSO32_XL_ODR_208Hz = 0x05,
    LSM6DSO32_XL_ODR_417Hz = 0x06,
    LSM6DSO32_XL_ODR_833Hz = 0x07,
    LSM6DSO32_XL_ODR_1667Hz = 0x08,
    LSM6DSO32_XL_ODR_3333Hz = 0x09,
    LSM6DSO32_XL_ODR_6667Hz = 0x0A
};

struct lsm6dso32_ctrl1_xl
{
    uint8_t not_used_01 : 1;
    uint8_t lpf2_xl_en : 1;
    enum lsm6dso32_fs_xl fs_xl : 2;
    enum lsm6dso32_odr_xl odr_xl : 4;
};

struct lsm6dso32_ctrl2_g
{
    uint8_t not_used_01 : 1;
    uint8_t fs_g : 3; /* fs_125 + fs_g */
    uint8_t odr_g : 4;
};

struct lsm6dso32_ctrl3_c
{
    uint8_t sw_reset : 1;
    uint8_t not_used_01 : 1;
    uint8_t if_inc : 1;
    uint8_t sim : 1;
    uint8_t pp_od : 1;
    uint8_t h_lactive : 1;
    uint8_t bdu : 1;
    uint8_t boot : 1;
};

struct lsm6dso32_ctrl4_c
{
    uint8_t not_used_01 : 1;
    uint8_t lpf1_sel_g : 1;
    uint8_t i2c_disable : 1;
    uint8_t drdy_mask : 1;
    uint8_t not_used_02 : 1;
    uint8_t int2_on_int1 : 1;
    uint8_t sleep_g : 1;
    uint8_t not_used_03 : 1;
};

struct lsm6dso32_ctrl5_c
{
    uint8_t st_xl : 2;
    uint8_t st_g : 2;
    uint8_t not_used_01 : 1;
    uint8_t rounding : 2;
    uint8_t xl_ulp_en : 1;
};

struct lsm6dso32_ctrl6_c
{
    uint8_t ftype : 3;
    uint8_t usr_off_w : 1;
    uint8_t xl_hm_mode : 1;
    uint8_t den_mode : 3; /* trig_en + lvl1_en + lvl2_en */
};

struct lsm6dso32_ctrl7_g
{
    uint8_t not_used_01 : 1;
    uint8_t usr_off_on_out : 1;
    uint8_t not_used_02 : 2;
    uint8_t hpm_g : 2;
    uint8_t hp_en_g : 1;
    uint8_t g_hm_mode : 1;
};

struct lsm6dso32_ctrl8_xl
{
    uint8_t low_pass_on_6d : 1;
    uint8_t not_used_01 : 1;
    // union
    // {
    uint8_t hp_slope_xl : 6;
    //   struct
    //   {
    //       uint8_t hp_slope_xl_en : 1;
    //       uint8_t fastsettl_mode_xl : 1;
    //       uint8_t hp_ref_mode_xl : 1;
    //       uint8_t hpcf_xl : 3;
    //   };
    // };
};

struct lsm6dso32_ctrl9_xl
{
    uint8_t not_used_01 : 1;
    uint8_t i3c_disable : 1;
    uint8_t den_lh : 1;
    uint8_t den_xl_g : 2; /* den_xl_en + den_xl_g */
    uint8_t den_z : 1;
    uint8_t den_y : 1;
    uint8_t den_x : 1;
};

struct lsm6dso32_ctrl10_c
{
    uint8_t not_used_01 : 5;
    uint8_t timestamp_en : 1;
    uint8_t not_used_02 : 2;
};

union lsm6dso32_reg_ctrl
{
    struct lsm6dso32_ctrl1_xl ctrl1_xl;
    struct lsm6dso32_ctrl2_g ctrl2_g;
    struct lsm6dso32_ctrl3_c ctrl3_c;
    struct lsm6dso32_ctrl4_c ctrl4_c;
    struct lsm6dso32_ctrl5_c ctrl5_c;
    struct lsm6dso32_ctrl6_c ctrl6_c;
    struct lsm6dso32_ctrl7_g ctrl7_g;
    struct lsm6dso32_ctrl8_xl ctrl8_xl;
    struct lsm6dso32_ctrl9_xl ctrl9_xl;
    struct lsm6dso32_ctrl10_c ctrl10_c;
    uint8_t byte;
};

struct ctrl_line
{
    uint8_t address;
    union lsm6dso32_reg_ctrl reg;
};

struct out_line
{
    uint8_t address;
    uint16_t data;
};

#endif
