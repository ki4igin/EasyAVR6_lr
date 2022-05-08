#ifndef LSM_H
#define LSM_H

#include <avr/io.h>
#include <assert.h>

#include "spi.h"

/* Идентификатор микросхемы */
#define LSM_ID 0x6C

/* Адрес регистра идентификатора */
#define LSM_WHO_AM_I 0x0FU

/* Адреса регистров управления */
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

/* Адрес регистра статуса */
#define LSM2_STATUS_REG 0x1EU

/* Адреса регистров данных */
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

/* Регистр управления акселерометром */
struct lsm_ctrl1_xl
{
    uint8_t not_used_01 : 1;
    uint8_t lpf2_xl_en : 1;      // включение ФНЧ2
    enum lsm_xl_fs xl_fs : 2;    // выбор диапазона
    enum lsm_xl_odr xl_odr : 4;  // выбор частоты выборки
};

enum __attribute__((packed)) lsm_g_fs
{
    LSM_G_125dps = 1,
    LSM_G_250dps = 0,
    LSM_G_500dps = 2,
    LSM_G_1000dps = 4,
    LSM_G_2000dps = 6,
};

enum __attribute__((packed)) lsm_g_odr
{
    LSM_G_ODR_12Hz5_HIGH_PERF = 0x01,
    LSM_G_ODR_26Hz_HIGH_PERF = 0x02,
    LSM_G_ODR_52Hz_HIGH_PERF = 0x03,
    LSM_G_ODR_104Hz_HIGH_PERF = 0x04,
    LSM_G_ODR_208Hz_HIGH_PERF = 0x05,
    LSM_G_ODR_417Hz_HIGH_PERF = 0x06,
    LSM_G_ODR_833Hz_HIGH_PERF = 0x07,
    LSM_G_ODR_1667Hz_HIGH_PERF = 0x08,
    LSM_G_ODR_3333Hz_HIGH_PERF = 0x09,
    LSM_G_ODR_6667Hz_HIGH_PERF = 0x0A
};

/* Регистр управления гироскопом */
struct lsm_ctrl2_g
{
    uint8_t not_used_01 : 1;
    enum lsm_g_fs g_fs : 3;    // выбор диапазона
    enum lsm_g_odr g_odr : 4;  // выбор частоты выборки
};

/* Регистр управления 3 */
struct lsm_ctrl3_c
{
    uint8_t sw_reset : 1;  // сброс устройства
    uint8_t not_used_01 : 1;
    uint8_t if_inc : 1;     // автоматический инкремент адреса (по умолчанию 1)
    uint8_t sim : 1;        // 0: 4-х проводной SPI; 1: 3-х проводной SPI
    uint8_t pp_od : 1;      // 0: INT1/INT2 выводы с 2С; 1: INT1/INT2 выводы с ОК
    uint8_t h_lactive : 1;  // 0: активный уровень прерываний высокий; 1: низкий
    uint8_t bdu : 1;        // 0: данные обновляются непрерывно;
                            // 1: данные не обновляются пока не будут считаны
    uint8_t boot : 1;       // сброс содержимого памяти устройство
};

/* Регистр управления 4 */
struct lsm_ctrl4_c
{
    uint8_t not_used_01 : 1;
    uint8_t lpf1_sel_g : 1;   // включение цифрового ФНЧ1 гироскопа
    uint8_t i2c_disable : 1;  // выключить интерфейс I2C
    uint8_t drdy_mask : 1;    // маскирование сигнала готовности данных пока фильтры настраиваются
    uint8_t not_used_02 : 1;
    uint8_t int2_on_int1 : 1;  // 0: сигналы прерываний разделены между выводами INT1 и INT2
                               // 1: сигналы прерываний через логическое ИЛИ выводятся на INT1
    uint8_t sleep_g : 1;       // включить спящий режим гироскопа
    uint8_t not_used_03 : 1;
};

enum __attribute__((packed)) lsm_st_xl
{
    LSM_ST_XL_DISABLE = 0,
    LSM_ST_XL_POSITIVE = 1,
    LSM_ST_XL_NEGATIVE = 2,
};

enum __attribute__((packed)) lsm_st_g
{
    LSM_ST_GY_DISABLE = 0,
    LSM_ST_GY_POSITIVE = 1,
    LSM_ST_GY_NEGATIVE = 3,
};

enum __attribute__((packed)) lsm_round
{
    LSM_ROUND_DISABLE = 0,
    LSM_ROUND_AXE = 1,
    LSM_ROUND_GY = 2,
    LSM_ROUND_AXE_GY = 3
};

/* Регистр управления 5 */
struct lsm_ctrl5_c
{
    enum lsm_st_xl st_xl : 2;  // режим самотестирования акселерометра
    enum lsm_st_g st_g : 2;    // режим самотестирования гироскопа
    uint8_t not_used_01 : 1;
    enum lsm_round rounding : 2;  // выбор циклического режима чтения данных
    uint8_t xl_ulp_en : 1;        // включить ultra-low-power режим акселерометра
};

enum __attribute__((packed)) lsm_ftype
{
    LSM_ULTRA_LIGHT = 0,
    LSM_VERY_LIGHT = 1,
    LSM_LIGHT = 2,
    LSM_MEDIUM = 3,
    LSM_STRONG = 4,      /* not available for data rate > 1k670Hz */
    LSM_VERY_STRONG = 5, /* not available for data rate > 1k670Hz */
    LSM_AGGRESSIVE = 6,  /* not available for data rate > 1k670Hz */
    LSM_XTREME = 7,      /* not available for data rate > 1k670Hz */
};

enum __attribute__((packed)) lsm_den_mode
{
    LSM_DEN_DISABLE = 0,
    LSM_LEVEL_FIFO = 6,
    LSM_LEVEL_LETCHED = 3,
    LSM_LEVEL_TRIGGER = 2,
    LSM_EDGE_TRIGGER = 4,
};

/* Регистр управления 6 */
struct lsm_ctrl6_c
{
    enum lsm_ftype ftype : 3;        // полоса пропускания ФНЧ1 гироскопа
    uint8_t usr_off_w : 1;           // 0: бит регистра смещения акселерометра равен 2^-10 g; 1: 2^-6 g
    uint8_t xl_hm_mode : 1;          // выключение высокопроизводительного режима акселерометра
    enum lsm_den_mode den_mode : 3;  // выбор режима триггера сигнала DEN
};

enum __attribute__((packed)) lsm_hpm_g
{
    LSM_HPM_G_16mHz = 0,
    LSM_HPM_G_65mHz = 1,
    LSM_HPM_G_260mHz = 2,
    LSM_HPM_G_1Hz04 = 3,
};

/* Регистр управления 7 */
struct lsm_ctrl7_g
{
    uint8_t not_used_01 : 1;
    uint8_t usr_off_on_out : 1;  // включение блока коррекции смещения акселерометра
    uint8_t not_used_02 : 2;
    uint8_t hpm_g : 2;      // полоса подавления цифрового ФВЧ гироскопа
    uint8_t hp_en_g : 1;    // включение цифрового ФВЧ гироскопа
    uint8_t g_hm_mode : 1;  // выключение высокопроизводительного режима гироскопа
};

enum __attribute__((packed)) lsm_hp_slope_xl
{
    LSM_HP_PATH_DISABLE_ON_OUT = 0x00,
    LSM_SLOPE_ODR_DIV_4 = 0x10,
    LSM_HP_ODR_DIV_10 = 0x11,
    LSM_HP_ODR_DIV_20 = 0x12,
    LSM_HP_ODR_DIV_45 = 0x13,
    LSM_HP_ODR_DIV_100 = 0x14,
    LSM_HP_ODR_DIV_200 = 0x15,
    LSM_HP_ODR_DIV_400 = 0x16,
    LSM_HP_ODR_DIV_800 = 0x17,
    LSM_HP_REF_MD_ODR_DIV_10 = 0x31,
    LSM_HP_REF_MD_ODR_DIV_20 = 0x32,
    LSM_HP_REF_MD_ODR_DIV_45 = 0x33,
    LSM_HP_REF_MD_ODR_DIV_100 = 0x34,
    LSM_HP_REF_MD_ODR_DIV_200 = 0x35,
    LSM_HP_REF_MD_ODR_DIV_400 = 0x36,
    LSM_HP_REF_MD_ODR_DIV_800 = 0x37,
    LSM_LP_ODR_DIV_10 = 0x01,
    LSM_LP_ODR_DIV_20 = 0x02,
    LSM_LP_ODR_DIV_45 = 0x03,
    LSM_LP_ODR_DIV_100 = 0x04,
    LSM_LP_ODR_DIV_200 = 0x05,
    LSM_LP_ODR_DIV_400 = 0x06,
    LSM_LP_ODR_DIV_800 = 0x07,
};

/* Регистр управления 8 */
struct lsm_ctrl8_xl
{
    uint8_t low_pass_on_6d : 1;  // включение ФНЧ2 на пути данных в функцию 6D
    uint8_t not_used_01 : 1;
    enum lsm_hp_slope_xl hp_slope_xl : 6;  // выбор полосы пропускания акселерометра
};

enum __attribute__((packed)) lsm_den_xl_g
{
    LSM_DEN_IN_GY_DATA = 0,
    LSM_DEN_IN_XL_DATA = 1,
    LSM_DEN_IN_GY_XL_DATA = 2,
};

/* Регистр управления 9 */
struct lsm_ctrl9_xl
{
    uint8_t not_used_01 : 1;
    uint8_t i3c_disable : 1;         // выключение протокола I3C
    uint8_t den_lh : 1;              // выбор активного уровня сигнала DEN
    enum lsm_den_xl_g den_xl_g : 2;  // выбор места сохранения сигнала DEN
    uint8_t den_z : 1;               // включение сохранения сигнала DEN в данных оси Z
    uint8_t den_y : 1;               // включение сохранения сигнала DEN в данных оси Y
    uint8_t den_x : 1;               // включение сохранения сигнала DEN в данных оси Z
};

/* Регистр управления 10 */
struct lsm_ctrl10_c
{
    uint8_t not_used_01 : 5;
    uint8_t timestamp_en : 1;  // Включить счетчик времени
    uint8_t not_used_02 : 2;
};

/* Регистр статуса */
struct lsm_status_reg
{
    uint8_t xlda : 1;  // флаг готовности новых данных акселерометра
    uint8_t gda : 1;   // флаг готовности новых данных гироскопа
    uint8_t tda : 1;   // флаг готовности новых данных датчика температуры
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

union lsm_word
{
    struct
    {
        uint8_t l;
        uint8_t h;
    };
    int16_t word;
};

struct __attribute__((packed)) lsm_word_pac
{
    uint8_t addr;
    union lsm_word data;
};
static_assert(sizeof(struct lsm_word_pac) == 3, "lsm_word_pac size error");

/**
 * Функция записи регистра
 * 
 * \param pac указатель на структуру lsm_reg_pac с адресом и значением регистра
 */
static inline void lsm_write_reg(struct lsm_reg_pac *pac)
{
    spi_txrx(pac, sizeof(struct lsm_reg_pac));
}

/**
 * Функция чтения регистра
 * 
 * \param pac указатель на структуру lsm_reg_pac с адресом и полем для значения
 *            регистра
 */
static inline void lsm_read_reg(struct lsm_reg_pac *pac)
{
    pac->addr |= 0x80;
    spi_txrx(pac, sizeof(struct lsm_reg_pac));
}

/**
 * Функция чтения регистра данных (16 бит)
 * 
 * \param pac указатель на структуру lsm_word_pac с адресом и полем для данных
 */
static inline void lsm_read_word(struct lsm_word_pac *pac)
{
    pac->addr |= 0x80;
    spi_txrx(pac, sizeof(struct lsm_word_pac));
}

#endif

/* End File */
