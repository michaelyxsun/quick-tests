#ifndef MOLSPEC_H
#define MOLSPEC_H

static const double xyz_angstrom[3][3] = {
    { -0.3384, 0.0038, 0.2392 },
    { -0.3351, -0.0019, -0.8328 },
    { 0.6735, -0.0019, 0.5935 },
};

static const double xyz[3][3] = {
    { -0.6395, 0.0072, 0.4520 },
    { -0.6333, -0.0036, -1.5737 },
    { 1.2727, -0.0036, 1.1216 },
};

static const double xyz_H2_angstrom[2][3] = {
    { 0.87345, 0.00000, 0.00000 },
    { 0.87345, 0.00000, 0.00000 },
};

// in bohr
static const double xyz_H2[2][3] = {
    { -1.650565, 0.00000, 0.00000 },
    { 1.650565, 0.00000, 0.00000 },
};

#endif
