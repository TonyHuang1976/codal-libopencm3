//  Rewrite standard C math functions with qfplib to reduce ROM size.
#include <stdint.h>
#include <math.h>
#include <qfplib.h>
#define M_LN10		2.30258509299404568402  //  Natural log of 10
#define M_PI_2		1.57079632679489661923  //  Pi divided by 2

//  We count the number of times each function was called. So we can check whether our unit tests cover all functions.
enum float_usage_index {
    FIRST_USAGE_INDEX = 0,
    USAGE_AEABI_DDIV,
    USAGE_AEABI_DMUL,
    USAGE_AEABI_DCMPEQ,
    USAGE_AEABI_DCMPLT,
    USAGE_AEABI_DCMPLE,
    USAGE_AEABI_DCMPGE,
    USAGE_AEABI_DCMPGT,
    USAGE_AEABI_DCMPUN,
    USAGE_AEABI_FDIV,
    USAGE_AEABI_D2IZ,
    USAGE_AEABI_D2UIZ,
    USAGE_SQRT,
    USAGE_LOG,
    USAGE_EXP,
    USAGE_LOG10,
    USAGE_POW,
    USAGE_LDEXP,
    USAGE_SIN,
    USAGE_COS,
    USAGE_TAN,
    USAGE_ATAN2,
    USAGE_ATAN,
    USAGE_ASIN,
    USAGE_ACOS,
    USAGE_TRUNC,
    USAGE_FLOOR,
    USAGE_CEIL,
    USAGE_FMOD,
    USAGE_FABS,
    LAST_FLOAT_USAGE_INDEX
};
static uint8_t float_usage[LAST_FLOAT_USAGE_INDEX];
uint8_t *get_float_usage(uint16_t *size) { *size = (uint16_t) LAST_FLOAT_USAGE_INDEX - 1; return float_usage; }

//  Run-time ABI for the ARM Architecture.  The function names are wrapped via "-Wl,-wrap,__aeabi..."
//  in newlib/CMakeLists.txt.  See http://infocenter.arm.com/help/topic/com.arm.doc.ihi0043d/IHI0043D_rtabi.pdf

///////////////////////////////////////////////////////////////////////////////
//  Table 2, Standard double precision floating-point arithmetic helper functions

//  double-precision division, n / d
double __wrap___aeabi_ddiv(double n, double d) { 
    float_usage[USAGE_AEABI_DDIV]++;
    return qfp_fdiv_fast(n, d); 
}
//  Unit Tests:
//  aeabi_ddiv(2205.1969, 270.8886) = 8.140604292687105
//  aeabi_ddiv(-2205.1969, 270.8886) = -8.140604292687105
//  aeabi_ddiv(2205.1969, -270.8886) = -8.140604292687105
//  aeabi_ddiv(-2205.1969, -270.8886) = 8.140604292687105

//  double-precision multiplication
double __wrap___aeabi_dmul(double x, double y) { 
    float_usage[USAGE_AEABI_DMUL]++;
    return qfp_fmul(x, y); 
}
//  Unit Tests:
//  aeabi_dmul(2205.1969, 270.8886) = 597362.70096534
//  aeabi_dmul(-2205.1969, 270.8886) = -597362.70096534
//  aeabi_dmul(2205.1969, -270.8886) = -597362.70096534
//  aeabi_dmul(-2205.1969, -270.8886) = 597362.70096534

///////////////////////////////////////////////////////////////////////////////
//  Table 3, double precision floating-point comparison helper functions

//  qfp_fcmp(r0, r1):
//  equal? return 0
//  r0 > r1? return +1
//  r0 < r1: return -1

//  result (1, 0) denotes (=, ?<>) [2], use for C == and !=
int __wrap___aeabi_dcmpeq(double x, double y) {
    float_usage[USAGE_AEABI_DCMPEQ]++;
    return (qfp_fcmp(x, y) == 0)  //  x == y
        ? 1 : 0;
}
//  Unit Tests:
//  aeabi_dcmpeq(2205.1969, 2205.1969) = 1
//  aeabi_dcmpeq(2205.1969, 2205.1968) = 0
//  aeabi_dcmpeq(2205.1969, 2205.1970) = 0
//  aeabi_dcmpeq(2205.1969, 0) = 0
//  aeabi_dcmpeq(-2205.1969, -2205.1969) = 1
//  aeabi_dcmpeq(-2205.1969, -2205.1968) = 0
//  aeabi_dcmpeq(-2205.1969, -2205.1970) = 0
//  aeabi_dcmpeq(-2205.1969, 0) = 0

//  result (1, 0) denotes (<, ?>=) [2], use for C <
int __wrap___aeabi_dcmplt(double x, double y) {
    float_usage[USAGE_AEABI_DCMPLT]++;
    return (qfp_fcmp(x, y) < 0)  //  x < y
        ? 1 : 0;
}
//  Unit Tests:
//  aeabi_dcmplt(2205.1969, 2205.1969) = 0
//  aeabi_dcmplt(2205.1969, 2205.1968) = 0
//  aeabi_dcmplt(2205.1969, 2205.1970) = 1
//  aeabi_dcmplt(2205.1969, 0) = 0
//  aeabi_dcmplt(-2205.1969, -2205.1969) = 0
//  aeabi_dcmplt(-2205.1969, -2205.1968) = 1
//  aeabi_dcmplt(-2205.1969, -2205.1970) = 0
//  aeabi_dcmplt(-2205.1969, 0) = 1

//  result (1, 0) denotes (<=, ?>) [2], use for C <=
int __wrap___aeabi_dcmple(double x, double y) { 
    float_usage[USAGE_AEABI_DCMPLE]++;
    return (qfp_fcmp(x, y) > 0)  //  x > y
        ? 0 : 1; 
}
//  Unit Tests:
//  aeabi_dcmple(2205.1969, 2205.1969) = 1
//  aeabi_dcmple(2205.1969, 2205.1968) = 0
//  aeabi_dcmple(2205.1969, 2205.1970) = 1
//  aeabi_dcmple(2205.1969, 0) = 0
//  aeabi_dcmple(-2205.1969, -2205.1969) = 1
//  aeabi_dcmple(-2205.1969, -2205.1968) = 1
//  aeabi_dcmple(-2205.1969, -2205.1970) = 0
//  aeabi_dcmple(-2205.1969, 0) = 1

//  result (1, 0) denotes (>=, ?<) [2], use for C >=
int __wrap___aeabi_dcmpge(double x, double y) { 
    float_usage[USAGE_AEABI_DCMPGE]++;
    return (qfp_fcmp(x, y) < 0)  //  x < y
        ? 0 : 1; 
}
//  Unit Tests:
//  aeabi_dcmpge(2205.1969, 2205.1969) = 1
//  aeabi_dcmpge(2205.1969, 2205.1968) = 1
//  aeabi_dcmpge(2205.1969, 2205.1970) = 0
//  aeabi_dcmpge(2205.1969, 0) = 1
//  aeabi_dcmpge(-2205.1969, -2205.1969) = 1
//  aeabi_dcmpge(-2205.1969, -2205.1968) = 0
//  aeabi_dcmpge(-2205.1969, -2205.1970) = 1
//  aeabi_dcmpge(-2205.1969, 0) = 0

//  result (1, 0) denotes (>, ?<=) [2], use for C >
int __wrap___aeabi_dcmpgt(double x, double y) { 
    float_usage[USAGE_AEABI_DCMPGT]++;
    return (qfp_fcmp(x, y) > 0)  //  x > y
        ? 1 : 0; 
}
//  Unit Tests:
//  aeabi_dcmpgt(2205.1969, 2205.1969) = 0
//  aeabi_dcmpgt(2205.1969, 2205.1968) = 1
//  aeabi_dcmpgt(2205.1969, 2205.1970) = 0
//  aeabi_dcmpgt(2205.1969, 0) = 1
//  aeabi_dcmpgt(-2205.1969, -2205.1969) = 0
//  aeabi_dcmpgt(-2205.1969, -2205.1968) = 0
//  aeabi_dcmpgt(-2205.1969, -2205.1970) = 1
//  aeabi_dcmpgt(-2205.1969, 0) = 0

//  result (1, 0) denotes (?, <=>) [2], use for C99 isunordered()
int __wrap___aeabi_dcmpun(double x, double y) { 
    float_usage[USAGE_AEABI_DCMPUN]++;
    return (qfp_fcmp(x, y) == 0)  //  x == y
        ? 0 : 1;
}
//  Unit Tests:
//  aeabi_dcmpun(2205.1969, 2205.1969) = 0
//  aeabi_dcmpun(2205.1969, 2205.1968) = 1
//  aeabi_dcmpun(2205.1969, 2205.1970) = 1
//  aeabi_dcmpun(2205.1969, 0) = 1
//  aeabi_dcmpun(-2205.1969, -2205.1969) = 0
//  aeabi_dcmpun(-2205.1969, -2205.1968) = 1
//  aeabi_dcmpun(-2205.1969, -2205.1970) = 1
//  aeabi_dcmpun(-2205.1969, 0) = 1

///////////////////////////////////////////////////////////////////////////////
//  Table 4, Standard single precision floating-point arithmetic helper functions

//  single-precision division, n / d
float  __wrap___aeabi_fdiv(float  n, float d)  { 
    float_usage[USAGE_AEABI_FDIV]++;
    return qfp_fdiv_fast(n, d); 
}
//  Unit Tests:
//  aeabi_fdiv(2205.1969, 270.8886) = 8.140604292687105
//  aeabi_fdiv(-2205.1969, 270.8886) = -8.140604292687105
//  aeabi_fdiv(2205.1969, -270.8886) = -8.140604292687105
//  aeabi_fdiv(-2205.1969, -270.8886) = 8.140604292687105

///////////////////////////////////////////////////////////////////////////////
//  Table 6, Standard floating-point to integer conversions

//  double to integer C-style conversion. "z" means round towards 0.
int __wrap___aeabi_d2iz(double x) { 
    float_usage[USAGE_AEABI_D2IZ]++;
    if (qfp_fcmp(x, 0) == 0) { return 0; }
    //  qfp_float2int() works like floor().  If x is negative, we add 1 to the result.
    int xfloored = qfp_float2int(x);
    if (xfloored < 0) { return xfloored + 1; }
    return xfloored; 
}
//  Unit Tests:
//  aeabi_d2iz(0) = 0
//  aeabi_d2iz(2205.1969) = 2205
//  aeabi_d2iz(-2205.1969) = -2205

//  double to unsigned C-style conversion. "z" means round towards 0.
unsigned __wrap___aeabi_d2uiz(double x) { 
    float_usage[USAGE_AEABI_D2UIZ]++;
    if (qfp_fcmp(x, 0) == 0) { return 0; }
    if (qfp_fcmp(x, 0) < 0) { return 0; }
    return qfp_float2uint(x); 
}
//  Unit Tests:
//  aeabi_d2iz(0) = 0
//  aeabi_d2uiz(2205.1969) = 2205
//  aeabi_d2uiz(-2205.1969) = 0

///////////////////////////////////////////////////////////////////////////////
//  <math.h> Functions

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::sqrt(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:925: undefined reference to `sqrt'

double sqrt(double x) { 
    float_usage[USAGE_SQRT]++;
    return qfp_fsqrt_fast(x); 
}
// Unit Tests:
// sqrt(100) = 10.000000
// sqrt(2) = 1.414214
// sqrt(-0) = -0.000000
//// sqrt(-1.0) = -nan

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::log(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:901: undefined reference to `log'

double log(double x) { 
    float_usage[USAGE_LOG]++;
    return qfp_fln(x); 
}
// Unit Tests:
// log(1) = 0.000000
// log(2) = _M_LN2
// log(10) = M_LN10
//// log(+Inf) = inf
//// log(0) = -inf

double exp(double x) { 
    float_usage[USAGE_EXP]++;
    return qfp_fexp(x); 
}
// Unit Tests:
// exp(1) = 2.718282
// exp(_M_LN2) = 2
// exp(M_LN10) = 10
// exp(-0) = 1.000000
//// exp(-Inf) = 0.000000

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::log10(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:904: undefined reference to `log10'

//  log10(x) = ln(x) / ln(10)
//  e.g. log10(1000) = ln(1000) / ln(10) = 3
double log10(double x) { 
    float_usage[USAGE_LOG10]++;
    return qfp_fmul(
        qfp_fln(x),
        1.0 / M_LN10  //  Constant
    ); 
}
// Unit Tests:
// log10(1000) = 3.000000
// log10(0.001) = -3.000000
// base-5 logarithm of 125 = 3.000000
// log10(1) = 0.000000
//// log10(+Inf) = inf
//// log10(0) = -inf

//  pow(b, x) = pow(e, log(b) * x) = exp(log(b) * x)
//  e.g. pow(10, 3) = exp(log(10) * 3) = 1000
double pow(double b, double x) { 
    float_usage[USAGE_POW]++;
    //  If b and x are both negative and x is odd, then return -pow(-b, x).
    //  e.g. pow(-2, -3) = -pow(2, -3)
    if (qfp_fcmp(b,  0) < 0 && qfp_fcmp(x,  0) < 0) {
        int xfloored = qfp_float2int(-x);
        if (xfloored % 2 == 1) {
            return -qfp_fexp(
                qfp_fmul(
                    qfp_fln(-b),
                    x
                )        
            );
        }
    }
    return qfp_fexp(
        qfp_fmul(
            qfp_fln(b),
            x
        )        
    );
}
// Unit Tests:
// pow(2, 10) = 1024.000000
// pow(2, 0.5) = 1.414214
// pow(-2, -3) = -0.125000
// pow(-1, NAN) = nan
// pow(+1, NAN) = 1.000000
//// pow(INFINITY, 2) = inf
//// pow(INFINITY, -1) = 0.000000
//// pow(-1, 1/3) = -nan

//  ldexp(x, ex) = x * pow(2, ex) 
//               = x * exp(log(2) * ex)
double ldexp(double x, int ex) {
    float_usage[USAGE_LDEXP]++;
    return qfp_fmul(
        x, 
        qfp_fexp(
            qfp_fmul( _M_LN2 , ex )
        )
    );
}
// Unit Tests:
// ldexp(7, -4) = 0.437500
// ldexp(-0, 10) = -0.000000
//// ldexp(-Inf, -1) = -inf
//// ldexp(1, 1024) = inf

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::sin(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:910: undefined reference to `sin'

double sin(double x) { 
    float_usage[USAGE_SIN]++;
    return qfp_fsin(x); 
}
// Unit Tests:
// sin(pi/6) = 0.500000
// sin(pi/2) = 1.000000
// sin(-3*pi/4) = -0.707107
// sin(+0) = 0.000000
// sin(-0) = -0.000000
//// sin(INFINITY) = -nan

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::cos(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:913: undefined reference to `cos'

double cos(double x) { 
    float_usage[USAGE_COS]++;
    //  If x is Pi/2, return 0.
    if (qfp_fcmp(x, M_PI_2) == 0) { return 0; }
    return qfp_fcos(x); 
}
// Unit Tests:
// cos(pi/3) = 0.500000
// cos(pi/2) = 0.000000
// cos(-3*pi/4) = -0.707107
// cos(+0) = 1.000000
// cos(-0) = 1.000000
//// cos(INFINITY) = -nan

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::tan(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:907: undefined reference to `tan'

double tan(double x) { 
    float_usage[USAGE_TAN]++;
    return qfp_ftan(x); 
}
// Unit Tests:
// tan  (pi/4) = +1.000000
// tan(3*pi/4) = -1.000000
// tan(5*pi/4) = +1.000000
// tan(7*pi/4) = -1.000000
// tan(+0) = 0.000000
// tan(-0) = -0.000000
//// tan(INFINITY) = -nan

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::atan(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:916: undefined reference to `atan'

double atan2(double y, double x) { 
    float_usage[USAGE_ATAN2]++;
    //  If x and y are both 0, return 0.
    if (qfp_fcmp(y,  0) == 0 && qfp_fcmp(x,  0) == 0) { return 0; }
    return qfp_fatan2( y, x ); 
}
// Unit Tests:
// atan2(+1,+1) = 0.785398
// atan2(+1,-1) = 2.356194
// atan2(-1,-1) = -2.356194
// atan2(-1,+1) = -0.785398
// atan2(0, 0) = 0.000000 
//// atan2(0, -0)=3.141593
// atan2(7, 0) = 1.570796 
// atan2(7, -0)=1.570796

////  TODO: Confirm
double atan(double y_over_x) {
    float_usage[USAGE_ATAN]++;
    //  If the argument is NaN, NaN is returned
    if (isnan(y_over_x)) { return NAN; }

    //  If the argument is ±0, it is returned unmodified
    if (qfp_fcmp(y_over_x, 0) == 0) { return y_over_x; }

    //  If the argument is +∞, +π/2 is returned
    if (isinf(y_over_x) && qfp_fcmp(y_over_x, 0) > 0) { return M_PI_2; }

    //  If the argument is -∞, -π/2 is returned
    if (isinf(y_over_x) && qfp_fcmp(y_over_x, 0) < 0) { return M_PI_2; }

    //  Must call atan2 instead of qfp_fatan2 in case the values are 0.
    return atan2( y_over_x, 1 ); 
}
// Unit Tests:
// atan(1) = 0.785398
//// atan(Inf) = 1.570796
// atan(-0.0) = -0.000000
// atan(+0.0) = +0.000000

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::asin(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:919: undefined reference to `asin'

//  arcsin(x) = arctan( x / sqrt( 1 - x^2 ) )
//            = arctan2( x , sqrt( 1 - (x*x) ) )
double asin(double x) { 
    float_usage[USAGE_ASIN]++;
    //  If the argument is NaN, NaN is returned
    if (isnan(x)) { return NAN; }

    //  If the argument is ±0, it is returned unmodified
    if (qfp_fcmp(x, 0) == 0) { return x; }

    //  If |arg| > 1, a domain error occurs and NaN is returned.
    if (qfp_fcmp(x,  1) > 0) { return NAN; }
    if (qfp_fcmp(x, -1) < 0) { return NAN; }

    //  Must call atan2 instead of qfp_fatan2 in case the values are 0.
    return atan2(
        x,
        qfp_fsqrt_fast(
            qfp_fsub(
                1,
                qfp_fmul( x , x )
            ) 
        )
    );
}
// Unit Tests:
// asin( 1.0) = +1.570796
// asin(-0.5) = -0.523599
// asin(0.0) = 0.000000
// asin(-0.0)=-0.000000
//// asin(1.1) = nan

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::acos(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:922: undefined reference to `acos'

//  arccos(x) = 2 * arctan(
//                      sqrt( 1 - x^2 ) /
//                      ( 1 + x )
//                  ) where -1 < x <= 1
double acos(double x) {
    float_usage[USAGE_ACOS]++;
    //  if the argument is NaN, NaN is returned
    if (isnan(x)) { return NAN; }

    //  If the argument is +1, the value +0 is returned.
    if (qfp_fcmp(x, 1) == 0) { return 0; }

    //  If |arg| > 1, a domain error occurs and NaN is returned.
    if (qfp_fcmp(x,  1) > 0) { return NAN; }
    if (qfp_fcmp(x, -1) < 0) { return NAN; }

    //  If x is -1, return Pi.
    if (qfp_fcmp(x, -1) == 0) { return M_PI_2 * 2.0; }

    //  Must call atan2 instead of qfp_fatan2 in case the values are 0.
    return 2 * atan2(
        qfp_fsqrt_fast(
            qfp_fsub( 
                1,
                qfp_fmul( x , x ) 
            )
        ),
        qfp_fadd( 1 , x )
    );
}
// Unit Tests:
// acos(-1) = 3.141593
// acos(-0.9) = 2.690565
// acos(0.0) = 1.570796 
// acos(0.5) = 1.047198 
// acos(1) = 0.000000
//// acos(1.1) = nan

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::trunc(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:934: undefined reference to `trunc'

//  Computes the nearest integer not greater in magnitude than x.
//  TODO: Warn if number is out of 32-bit int range.
double trunc(double x) { 
    float_usage[USAGE_TRUNC]++;
    //  If arg is NaN, NaN is returned
    if (isnan(x)) { return NAN; }

    //  If arg is ±∞, it is returned, unmodified
    if (isinf(x)) { return x; }

    //  If arg is ±0, it is returned, unmodified
    if (qfp_fcmp(x, 0) == 0) { return x; }

    //  qfp_float2int() behaves like floor().  If negative, add one.
    int floored = qfp_float2int(x);
    if (floored < 0) { return floored + 1; }
    return floored;
}
// Unit Tests:
// trunc(+2.7) = +2.0
// trunc(-2.7) = -2.0
// trunc(-0.0) = -0.0
// trunc(2205.1969) = 2205.000000
// trunc(-270.8886) = -270.000000

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::floor(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:928: undefined reference to `floor'

//  Computes the largest integer value not greater than arg.
//  TODO: Warn if number is out of 32-bit int range.
double floor(double x) { 
    float_usage[USAGE_FLOOR]++;
    //  If arg is NaN, NaN is returned
    if (isnan(x)) { return NAN; }

    //  If arg is ±∞, it is returned, unmodified
    if (isinf(x)) { return x; }

    //  If arg is ±0, it is returned, unmodified
    if (qfp_fcmp(x, 0) == 0) { return x; }

    //  qfp_float2int() behaves like floor().
    int floored = qfp_float2int(x);
    return floored;
}
// Unit Tests:
// floor(+2.7) = +2.0
// floor(-2.7) = -3.0
// floor(-0.0) = -0.0
// floor(2205.1969) = 2205.000000
// floor(-270.8886) = -271.000000

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::ceil(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:931: undefined reference to `ceil'

//  Computes the smallest integer value not less than arg.
//  TODO: Warn if number is out of 32-bit int range.
double ceil(double x) { 
    float_usage[USAGE_CEIL]++;
    //  If arg is NaN, NaN is returned
    if (isnan(x)) { return NAN; }

    //  If arg is ±∞, it is returned, unmodified
    if (isinf(x)) { return x; }

    //  If arg is ±0, it is returned, unmodified
    if (qfp_fcmp(x, 0) == 0) { return x; }

    //  qfp_float2int() behaves like floor().  Always add one.
    int floored = qfp_float2int(x);
    return floored + 1;
}
// Unit Tests:
// ceil(+2.4) = +3.0
// ceil(-2.4) = -2.0
// ceil(-0.0) = -0.0
// ceil(2205.1969) = 2206.000000
// ceil(-270.8886) = -270.000000

//  Computes the floating-point remainder of the division operation x/y
//  i.e. x - n*y, where n is x/y with its fractional part truncated.
double fmod(double x, double y) { 
    float_usage[USAGE_FMOD]++;
    // If either argument is NaN, NaN is returned
    if (isnan(x) || isnan(y)) { return NAN; }

    // If x is ±0 and y is not zero, ±0 is returned
    if (qfp_fcmp(x, 0) == 0 && qfp_fcmp(y, 0) != 0) { return 0; }

    // If x is ±∞ and y is not NaN, NaN is returned and FE_INVALID is raised
    if (isinf(x) && !isnan(y)) { return NAN; }

    // If y is ±0 and x is not NaN, NaN is returned and FE_INVALID is raised
    if (qfp_fcmp(y, 0) == 0 && !isnan(x)) { return NAN; }

    // If y is ±∞ and x is finite, x is returned.
    if (isinf(y) && !isinf(x)) { return x; }

    // From https://en.cppreference.com/w/c/numeric/math/fmod
    double xabs = fabs(x);
    double yabs = fabs(y);
    // Was: double result = remainder(fabs(x), (y = fabs(y)));
    double n = trunc(qfp_fdiv_fast(xabs, yabs));
    double result = qfp_fsub(xabs, qfp_fmul(n, yabs));  //  x - n*y, always positive

    // Was: if (signbit(result)) result += y;
    if (qfp_fcmp(result, 0) < 0) { result += yabs; }

    // Composes a floating point value with the magnitude of result and the sign of x.
    // Was: return copysign(result, x);
    return (qfp_fcmp(x, 0) < 0) ? -result : result;
}
// Unit Tests:
// fmod(+5.1, +3.0) = 2.1
// fmod(-5.1, +3.0) = -2.1
// fmod(+5.1, -3.0) = 2.1
// fmod(-5.1, -3.0) = -2.1
// fmod(+0.0, 1.0) = 0.0
// fmod(-0.0, 1.0) = -0.0
// fmod(+5.1, Inf) = 5.1
//// fmod(+5.1, 0) = nan

//  Computes the absolute value of a floating point value arg.
double fabs(double x) {
    float_usage[USAGE_FABS]++;
    if (isnan(x) || isinf(x) || qfp_fcmp(x, 0) == 0) { return x; }
    if (qfp_fcmp(x, 0) < 0) { return -x; }
    return x;
}
//  Unit Tests:
//  fabs(+3) = 3.000000
//  fabs(-3) = 3.000000
//  fabs(-0) = 0.000000
//  fabs(2205.1969) = 2205.1969
//  fabs(-270.8886) = 270.8886
////  fabs(-Inf) = inf

//  TODO: Support other functions
//  Computes the floating-point remainder of the division operation x/y
//  i.e. x - n*y, where the value n is the integral value nearest the exact value x/y
//  double remainder(double x, double y)
//  double cosh(double x)
//  double sinh(double x)
//  double tanh(double x)
