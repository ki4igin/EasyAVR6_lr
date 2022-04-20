#ifndef LSM_H
#define LSM_H

#include <avr/io.h>
#include <assert.h>

#include "spi.h"

#define LSM_ID 0x6C

#define LSM_WHO_AM_I 0x0FU

#define LSM_CTRL1_XL 0x10U
#define LSM_CTRL2_G  0x11U
#define LSM_CTRL3_C  0x12U
#define LSM_CTRL4_C  0x13U
#define LSM_CTRL5_C  0x14U
#define LSM_CTRL6_C  0x15U
#define LSM_CTRL7_G  0x16U
#define LSM_CTRL8_XL 0x17U
#define LSM_CTRL9_XL 0x18U
#define LSM_CTRL10_C 0x19U

#define LSM2_STATUS_REG 0x1EU

#define LSM_OUT_TEMP_L 0x20U
#define LSM_OUT_TEMP_H 0x21U
#define LSM_OUTX_L_G   0x22U
#define LSM_OUTX_H_G   0x23U
#define LSM_OUTY_L_G   0x24U
#define LSM_OUTY_H_G   0x25U
#define LSM_OUTZ_L_G   0x26U
#define LSM_OUTZ_H_G   0x27U
#define LSM_OUTX_L_A   0x28U
#define LSM_OUTX_H_A   0x29U
#define LSM_OUTY_L_A   0x2AU
#define LSM_OUTY_H_A   0x2BU
#define LSM_OUTZ_L_A   0x2CU
#define LSM_OUTZ_H_A   0x2DU

enum __attribute__((packed)) lsm_xl_fs
{
    LSM_XL_FS_4g = 0x00,
    LSM_XL_FS_8g = 0x02,
    LSM_XL_FS_16g = 0x03,
    LSM_XL_FS_32g = 0x01,
};

enum __attribute__((packed)) lsm_xl_odr
{
    LSM_XL_ODR_OFF = 0x00,
    LSM_XL_ODR_12Hz5 = 0x01,
    LSM_XL_ODR_26Hz = 0x02,
    LSM_XL_ODR_52Hz = 0x03,
    LSM_XL_ODR_104Hz = 0x04,
    LSM_XL_ODR_208Hz = 0x05,
    LSM_XL_ODR_417Hz = 0x06,
    LSM_XL_ODR_833Hz = 0x07,
    LSM_XL_ODR_1667Hz = 0x08,
    LSM_XL_ODR_3333Hz = 0x09,
    LSM_XL_ODR_6667Hz = 0x0A
};

struct lsm_ctrl1_xl
{
    uint8_t not_used_01 : 1;
    uint8_t lpf2_xl_en : 1;
    enum lsm_xl_fs xl_fs : 2;
    enum lsm_xl_odr xl_odr : 4;
};

struct lsm_ctrl2_g
{
    uint8_t not_used_01 : 1;
    uint8_t fs_g : 3; /* fs_125 + fs_g */
    uint8_t odr_g : 4;
};

struct lsm_ctrl3_c
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

struct lsm_ctrl4_c
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

struct lsm_ctrl5_c
{
    uint8_t st_xl : 2;
    uint8_t st_g : 2;
    uint8_t not_used_01 : 1;
    uint8_t rounding : 2;
    uint8_t xl_ulp_en : 1;
};

struct lsm_ctrl6_c
{
    uint8_t ftype : 3;
    uint8_t usr_off_w : 1;
    uint8_t xl_hm_mode : 1;
    uint8_t den_mode : 3; /* trig_en + lvl1_en + lvl2_en */
};

struct lsm_ctrl7_g
{
    uint8_t not_used_01 : 1;
    uint8_t usr_off_on_out : 1;
    uint8_t not_used_02 : 2;
    uint8_t hpm_g : 2;
    uint8_t hp_en_g : 1;
    uint8_t g_hm_mode : 1;
};

struct lsm_ctrl8_xl
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

struct lsm_ctrl9_xl
{
    uint8_t not_used_01 : 1;
    uint8_t i3c_disable : 1;
    uint8_t den_lh : 1;
    uint8_t den_xl_g : 2; /* den_xl_en + den_xl_g */
    uint8_t den_z : 1;
    uint8_t den_y : 1;
    uint8_t den_x : 1;
};

struct lsm_ctrl10_c
{
    uint8_t not_used_01 : 5;
    uint8_t timestamp_en : 1;
    uint8_t not_used_02 : 2;
};

struct lsm_status_reg
{
    uint8_t xlda : 1;
    uint8_t gda : 1;
    uint8_t tda : 1;
    uint8_t not_used_01 : 5;
};

union lsm_reg
{
    uint8_t byte;
    struct lsm_ctrl1_xl ctrl1_xl;
    struct lsm_ctrl2_g ctrl2_g;
    struct lsm_ctrl3_c ctrl3_c;
    struct lsm_ctrl4_c ctrl4_c;
    struct lsm_ctrl5_c ctrl5_c;
    struct lsm_ctrl6_c ctrl6_c;
    struct lsm_ctrl7_g ctrl7_g;
    struct lsm_ctrl8_xl ctrl8_xl;
    struct lsm_ctrl9_xl ctrl9_xl;
    struct lsm_ctrl10_c ctrl10_c;
    struct lsm_status_reg status;
};
static_assert(sizeof(union lsm_reg) == 1, "lsm_reg size error");

struct lsm_reg_pac
{
    uint8_t addr;
    union lsm_reg reg;
};

struct lsm_word
{
    uint8_t l;
    uint8_t h;
};

struct __attribute__((packed)) lsm_word_pac
{
    uint8_t addr;
    struct lsm_word data;
};
static_assert(sizeof(struct lsm_word_pac) == 3, "lsm_word_pac size error");

static inline void lsm_read_reg(struct lsm_reg_pac *pac)
{
    pac->addr |= 0x80;
    spi_txrx(pac, sizeof(struct lsm_reg_pac));
}

static inline void lsm_write_reg(struct lsm_reg_pac *pac)
{
    spi_txrx(pac, sizeof(struct lsm_reg_pac));
}

static inline void lsm_read_word(struct lsm_word_pac *pac)
{
    pac->addr |= 0x80;
    spi_txrx(pac, sizeof(struct lsm_word_pac));
}

#endif

/* End File */
