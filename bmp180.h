#ifndef _BMP180_H_
#define _BMP180_H_

struct bmp180_calib_param {
    short AC1, AC2, AC3, B1, B2, MB, MC, MD;
    unsigned short AC4, AC5, AC6;
};

#endif
