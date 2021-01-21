/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

/*******************************************************************************
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_math.h

  Summary:
    Contains some general purpose math functions

  Description:
    Math support functions.
*******************************************************************************/

/** \file legato_math.h
 * @brief Defines common math functions for general use.
 *
 * @details This is an array implementation that is used internally by the Legato user
 * interface library.
 */

#ifndef LE_MATH_H
#define LE_MATH_H

#include "gfx/legato/common/legato_common.h"

/**
 * @brief Used to define the types of trig functions
 * @details Trigonomtry is used in Legato.
 */
typedef enum
{
    LE_TRIG_SINE_TYPE,
    LE_TRIG_COSINE_TYPE,
} LE_TRIG_FUNCTION_TYPE;

/**
 * @brief Used to define the basic four quandrants of a coordinate plane
 * @details .
 */
typedef enum 
{
    LE_Q1,
    LE_Q2,
    LE_Q3,
    LE_Q4
} LE_QUADRANT;

/**
 * @brief Used to define arc direction
 * @details The parameters CW and CCW enable you to define the direction of the arc.
 */
typedef enum 
{
    LE_CCW,     /**< counter clock wise. */
    LE_CW,      /**< clock wise. */
} leArcDir;

// *****************************************************************************
/* Function:
    int32_t leMini(int32_t l, int32_t r);

  Summary:
    Returns the smaller of two integers.

  Parameters:
    l - the first number to test
    r - the second number to test

  Returns:
    int32_t - the smaller of the two numbers
*/
/**
 * @brief Calculate minimum of two integers.
 * @details Calculates the lessor of integers <span style="color: #820a32"><em>l</em></span>
 * and <span style="color: #820a32"><em>r</em></span>.
 * @code
 * uint32_t smaller = leMini(l, r);
 * @endcode
 * @param param1 l is an integer.
 * @param param2 r is an integer.
 * @return the lessor of l and r.
 */
int32_t leMini(int32_t l, int32_t r);

// *****************************************************************************
/* Function:
    int32_t leMaxi(int32_t l, int32_t r);

  Summary:
    Returns the larger of two integers.

  Parameters:
    l - the first number to test
    r - the second number to test

  Returns:
    int32_t - the larger of the two numbers
*/
/**
 * @brief Calculate maximum of two integers.
 * @details Calculates the maximum of integers <span style="color: #820a32"><em>l</em></span>
 * and <span style="color: #820a32"><em>r</em></span>.
 * @code
 * uint32_t larger = leMaxi(l, r);
 * @endcode
 * @param param1 l is an integer.
 * @param param2 r is an integer.
 * @return the maximum of l and r.
 */
int32_t leMaxi(int32_t l, int32_t r);

// *****************************************************************************
/* Function:
    float leMinf(float l, float r);

  Summary:
    Returns the smaller of two floats.

  Parameters:
    l - the first float to test
    r - the second float to test

  Returns:
    float - the smaller of the two floats
*/
/**
 * @brief Calculate minimum of two floats.
 * @details Calculates the minimum of two floats <span style="color: #820a32"><em>l</em></span>
 * and <span style="color: #820a32"><em>r</em></span>.
 * @code
 * float smaller = leMinf(l, r);
 * @endcode
 * @param param1 l is an float.
 * @param param2 r is an float.
 * @return the minimum of l and r.
 */
float leMinf(float l, float r);

// *****************************************************************************
/* Function:
    float leMaxf(float l, float r);

  Summary:
    Returns the larger of two floats.

  Parameters:
    l - the first float to test
    r - the second float to test

  Returns:
    float - the larger of the two floats
*/
/**
 * @brief Calculate maximum of two floats.
 * @details Calculates the maximum of two floats <span style="color: #820a32"><em>l</em></span>
 * and <span style="color: #820a32"><em>r</em></span>.
 * @code
 * float larger = leMaxf(l, r);
 * @endcode
 * @param param1 l is an float.
 * @param param2 r is an float.
 * @return the maximum of l and r.
 */
float leMaxf(float l, float r);

// *****************************************************************************
/* Function:
    int32_t leClampi(int32_t min, int32_t max, int32_t i);

  Summary:
    Clamps an integer between a min and max

  Parameters:
    min - the minimum value
    max - the maximum value
    i - the number to clamp

  Returns:
    int32_t - the clamped value
*/

/**
 * @brief Calculates clamp of an integer.
 * @details Calculates the clamp value of <span style="color: #820a32"><em>i</em></span>
 * constrained to the range <span style="color: #820a32"><em>min</em></span>
 * to <span style="color: #820a32"><em>max</em></span>.
 * @code
 * int32_t val = leClampi(min, max, i);
 * @endcode
 * @param param1 min is an integer.
 * @param param2 max is an integer.
 * @param param3 i is an integer.
 * @return a clamp integer
 */
int32_t leClampi(int32_t min, int32_t max, int32_t i);

// *****************************************************************************
/* Function:
    float leClampf(float min, float max, float i);

  Summary:
    Clamps a float between a min and max

  Parameters:
    min - the minimum value
    max - the maximum value
    i - the float to clamp

  Returns:
    float - the clamped value
*/
/**
 * @brief Calculate clamp of a float.
 * @details Calculates the clamp value of <span style="color: #820a32"><em>i</em></span>
 * constrained to the range <span style="color: #820a32"><em>min</em></span>
 * to <span style="color: #820a32"><em>max</em></span>.
 * @code
 * float val = leClampf(min, max, i);
 * @endcode
 * @param param1 min is an float.
 * @param param2 max is an float.
 * @param param3 f is an float.
 * @return a clamp integer
 */
float leClampf(float min, float max, float f);

// *****************************************************************************
/* Function:
    uint32_t lePercent(uint32_t l, uint32_t r);

  Summary:
    Calculates the percentage of one number when applied to another.  Integer
    based.  Accuracy for higher numbers is not guaranteed.

    The result is the decimal percentage multiplied by 100.

  Parameters:
    l - the first number of the equation
    r - the second number of the equation

  Returns:
    uint32_t - the percentage represented as a whole number
*/
/**
 * @brief Calculate percent of number.
 * @details Calculates the decimal percent of <span style="color: #820a32"><em>l</em></span>
 * and <span style="color: #820a32"><em>r</em></span>. Integer based.  Accuracy for
 * higher numbers is not guaranteed.The result is the decimal percentage
 * multiplied by 100.
 * @code
 * uint32_t pc = lePercent(l, r);
 * @endcode
 * @param param1 l is an integer.
 * @param param2 r is an integer.
 * @return percentage represented as a whole number
 */
uint32_t lePercent(uint32_t l, uint32_t r);

// *****************************************************************************
/* Function:
    uint32_t lePercentWholeRounded(uint32_t l, uint32_t r);

  Summary:
    Calculates the percentage of one number when applied to another.  Integer
    based.  Accuracy for higher numbers is not guaranteed.

    The difference between this and lePercent is that the decimal portion
    of the whole number is rounded off.

  Parameters:
    l - the first number of the equation
    r - the second number of the equation

  Returns:
    uint32_t - the percentage represented as a whole number
*/
/**
 * @brief Calculate percent whole rounded.
 * @details Calculates the whole number integer based percent of
 * <span style="color: #820a32"><em>l</em></span>
 * and <span style="color: #820a32"><em>r</em></span>.
 * The difference between this and lePercent is that the decimal portion
 * of the whole number is rounded off.
 * @see lePercent
 * @note Accuracy for higher numbers is not guaranteed.
 * @code
 * uint32_t pc = lePercentWholeRounded(l, r);
 * @endcode
 * @param param1 l is an integer.
 * @param param2 r is an integer.
 * @return percentage represented as a whole number.
 */
uint32_t lePercentWholeRounded(uint32_t l, uint32_t r);

// *****************************************************************************
/* Function:
    uint32_t lePercentOf(uint32_t l, uint32_t percent);

  Summary:
    Calculates the percentage of a number.  Returns a whole number with no
    decimal component.

  Parameters:
    num - the number to consider
    percent - the percentage to apply

  Returns:
    uint32_t - the resultant percentage of the number
*/
/**
 * @brief Calculate percent of a number.
 * @details Calculate the whole number percentage of <span style="color: #820a32"><em>num</em></span>
 * and <span style="color: #820a32"><em>percent</em></span>.
 * @code
 * uint32_t whole = lePercentOf(l, percent);
 * @endcode
 * @param param1 num is an integer.
 * @param param2 percent is the percent to apply.
 * @return the percentage of the number.
 */
uint32_t lePercentOf(uint32_t num, uint32_t percent);

// *****************************************************************************
/* Function:
    void lePercentOfDec(uint32_t num, uint32_t percent, uint32_t* whl, uint32_t* dec);

  Summary:
    Calculates the percentage of a number.  Returns a whole number and the tenths digit
    decimal component.

  Parameters:
    num - the number to consider
    percent - the percentage to apply
    whl - pointer to whole number value
    dec - pointer to tenths digit as an interger

  Returns:
    none
*/
/**
 * @brief Calculate percent of a decimal.
 * @details Calculates the whole number percent of <span style="color: #820a32"><em>num</em></span>
 * and <span style="color: #820a32"><em>percent</em></span> and stores the result in
 * <span style="color: #820a32"><em>whl</em></span> and
 * <span style="color: #820a32"><em>dec</em></span>
 * @code
 * uint32_t whole = lePercentOf(num, percent, whl, dec);
 * @endcode
 * @param param1 l is an integer.
 * @param param2 percent is the percent to apply.
 * @return resultant percentage of the number.
 */
void lePercentOfDec(uint32_t num, uint32_t percent, uint32_t* whl, uint32_t* dec);

// *****************************************************************************
/* Function:
    uint32_t leScaleInteger(uint32_t num, uint32_t oldMax, uint32_t newMax);

  Summary:
    Scales an integer from one number range of 0 -> n0 to another range 0 -> n1
    based on percentages.

  Parameters:
    num - the number to consider
    oldMax - the old range maximum
    newMax - the new range maximum

  Returns:
    uint32_t - the number as defined in the new number range
*/
/**
 * @brief Calculate the scale of an integer.
 * @details Calculates the scale of <span style="color: #820a32"><em>num</em></span>
 * from <span style="color: #820a32"><em>oldMax</em></span>
 * to <span style="color: #820a32"><em>newMax</em></span>
 * @note one number range of 0 -> n0 to another
 * range 0 -> n1 based on percentages.
 * @code
 * uint32_t scale = leScaleInteger(num, oldMax, newMax);
 * @endcode
 * @param param1 num is an integer.
 * @param param2 oldMax is the old range maximum.
 * @param param3 newMax is the new range maximum.
 * @return resultant percentage of the number.
 */
uint32_t leScaleInteger(uint32_t num, uint32_t oldMax, uint32_t newMax);

// *****************************************************************************
/* Function:
    int32_t leScaleIntegerSigned(int32_t num, int32_t oldMax, int32_t newMax);

  Summary:
    Scales a signed integer from one number range of 0 -> n0 to another range 0 -> n1
    based on percentages.

  Parameters:
    num - the number to consider
    oldMax - the old range maximum
    newMax - the new range maximum

  Returns:
    int32_t - the number as defined in the new number range
*/
/**
 * @brief Calculate the scale of signed integer
 * @details Calculates the scale of <span style="color: #820a32"><em>num</em></span>
 * in range <span style="color: #820a32"><em>oldMax</em></span> to range
 * <span style="color: #820a32"><em>newMax</em></span>.
 * @code
 * uint32_t scale = leScaleIntegerSigned(l, percent);
 * @endcode
 * @param param1 num is an integer.
 * @param param2 oldMax is an integer.
 * @param param3 newMax is the percent to apply.
 * @return resultant percentage of the number.
 */
int32_t leScaleIntegerSigned(int32_t num, int32_t oldMax, int32_t newMax);

// *****************************************************************************
/* Function:
    uint32_t leAbsoluteValue(int32_t val);

  Summary:
    Calculates the absolute value of a signed integer.

  Parameters:
    val - the number to consider

  Returns:
    uint32_t - the absolute value
*/
/**
 * @brief Calculates the absolute value of a signed integer.
 * @details Calculates the absolute value of
 * <span style="color: #820a32"><em>val</em></span>.
 * @code
 * uint32_t absl = leAbsoluteValue(val);
 * @endcode
 * @param param1 val is an integer.
 * @return the absolute value
 */
uint32_t leAbsoluteValue(int32_t val);

// *****************************************************************************
/* Function:
    int32_t leLerp(int32_t x, int32_t y, uint32_t per);

  Summary:
    Performs a linear interpolation of an integer based on a percentage between
    two signed points.

  Parameters:
    x - the first point to consider
    y - the second point to consider
    per - the percentage of interpolation

  Returns:
    int32_t - the interpolated value
*/
/**
 * @brief Calculates a linear interpolation of an integer based on a percentage
 * between two signed points.
 * @details Calculates a linear interpolation between <span style="color: #820a32"><em>x</em></span>
 * and <span style="color: #820a32"><em>y</em></span> based on
 * <span style="color: #820a32"><em>per</em></span>.
 * @code
 * uint32_t scale = leLerp(l, percent);
 * @endcode
 * @param param1 x is point.
 * @param param2 y is point.
 * @param param2 per is the percent to apply.
 * @return the interpolated value
 */
int32_t leLerp(int32_t x, int32_t y, uint32_t per);

// *****************************************************************************
/* Function:
    int32_t leDivideRounding(int32_t num, int32_t denom);

  Summary:
    Calculates integer division of num/denom with rounding based on LSB/2.

  Parameters:
    num - numerator
    denom - denominator

  Returns:
    int32_t - result, equivalent to int32_t( ((float)num)/((float)denom) + 0.5 ) without using floating point
*/
/**
 * @brief Performs a linear interpolation of an integer based on a percentage
 * between two signed points.
 * @details Calculates integer division of <span style="color: #820a32"><em>num</em></span>
 * divided by <span style="color: #820a32"><em>denom</em></span>.
 * @code
 * int32_t val = leDivideRounding(num, denom);
 * @endcode
 * @param param1 num is an integer.
 * @param param2 denom is the denom.
 * @param param3 per is the percent to apply.
 * @return equivalent to int32_t( ((float)num)/((float)denom) + 0.5 ) without using floating point
 */
int32_t leDivideRounding(int32_t num, int32_t denom);

/**************************************************************************
  Function:
       leResult lePolarToXY(int32_t r, int32_t a, lePoint* p);
    
  Summary:
    Performs a cosine lookup to determine the points in an arc at specified
    radius and angle (polar to Cartesian conversion)
  Parameters:
    r -  radius
    a -  angle (in degrees)
  Returns:
    p - the output point in Cartesian plane                                
  **************************************************************************/
/**
 * @brief Generate points in an arc.
 * @details Generates <span style="color: #820a32"><em>points</em></span> in an arc using
 * <span style="color: #820a32"><em>radius</em></span>
 * and <span style="color: #820a32"><em>angle</em></span>.
 * @code
 * leResult res = lePolarToXY(l, percent);
 * @endcode
 * @param param1 radius is the radius of arc.
 * @param param2 angle is the angle of arc.
 * @param param3 points is the position to query.
 * @return resultant percentage of the number.
 */
leResult lePolarToXY(int32_t r, int32_t a, lePoint* p);

// *****************************************************************************
/* Function:
    int32_t leNormalizeAngle(int32_t t)

  Summary:
     normalize an angle between 0 - 360

  Parameters:
    int32_t t - angle (in degrees)

  Returns:
    int32_t - normalize an angle in degrees.
              Example: t = -5, return value is 355
                       t = 450, return value is 90
*/
/**
 * @brief Normalize an angle between 0 - 360.
 * @details Normalizes <span style="color: #820a32"><em>angle</em></span>.
 * Example: t = -5, return value is 355
 *          t = 450, return value is 90
 * @code
 * int32_t val = leNormalizeAngle(angle);
 * @endcode
 * @param param1 x is an integer.
 * @param param2 y is the percent to apply.
 * @param param2 per is the percent to apply.
 * @return normalize an angle in degrees.
 */
int32_t leNormalizeAngle(int32_t t);


// *****************************************************************************
/* Function:
    int32_t leSin(int32_t v)

  Summary:
    Returns the sine of the value v * 256

  Parameters:
    v - angle (in degrees)

  Returns:
    int32_t - result of sine fixed point value (times 256), calling function needs
              to divide by 256 to get good result

              ex. "a * sin(v)" would be "a * leSin(v) / 256";
*/
/**
 * @brief Calculate sin of a number.
 * @details Calculates sin of <span style="color: #820a32"><em>angle</em></span>.
 * @remark - result of sine fixed point value (times 256), calling function
 * needs to divide by 256 to get good result.
 *      ex. "a * sin(v)" would be "a * leSin(v) / 256";
 * @code
 * int32_t val = leSin(v);
 * @endcode
 * @param param1 v is the value in degrees.
 * @return sine fixed point value
 */
int32_t leSin(int32_t v);

// *****************************************************************************
/* Function:
    int32_t leCos(int32_t v)

  Summary:
    Returns the cosine of the value v

  Parameters:
    v - angle (in degrees)

  Returns:
    int32_t - result of cosine fixed point value (times 256), calling function
              needs to divide by 256 to get good result

              ex. "a * cos(v)" would be "a * leCos(v) / 256";
*/
/**
 * @brief Calculate cosine of a number.
 * @details Calculates cosine of <span style="color: #820a32"><em>angle</em></span>.
 * @remark - result of cosine fixed point value (times 256), calling function
 * needs to divide by 256 to get good result.
 *      ex. "a * cos(v)" would be "a * leCos(v) / 256";
 * @code
 * int32_t val = leCos(v);
 * @endcode
 * @param param1 v is the value in degrees.
 * @return cosine fixed point value
 */
int32_t leCos(int32_t v);

/************************************************************************************************
  Function:
       leResult leEllipsePoint(int32_t t, int32_t a, int32_t b, int32_t theta, lePoint* p);
    
  Summary:
    Performs a cosine lookup to determine the points in an arc at specified
    radius and angle (polar \> cartesian conversion)
  Parameters:
    t -      angle of the point on the ellipse (in degrees)
    a -      the half\-length of 0\-180 axis of the ellipse
    b -      the half\-length of 90\-270 axis of the ellipse
    theta -  angle of the ellipse (in degrees)
  Returns:
    p - the output point in cartesian plane                                                      
  ************************************************************************************************/
/**
 * @bold  Help
 * @brief Calculates points in an arc.
 * @details Generates <span style="color: #820a32"><em>points</em></span> in an arc at
 * <span style="color: #820a32"><em>radius</em></span>
 * and <span style="color: #820a32"><em>angle</em></span>.
 * @code
 * leEllipsePoint(angle, endAngleArc0, dir, points);
 * @endcode
 * @param param1 startAngleArc0 is point to rotate.
 * @param param2 endAngleArc0 of the rotation.
 * @param param2 dir of the rotation.
 * @param param2 quadrant of the rotation.
 * @return void.
 */
leResult leEllipsePoint(int32_t t, int32_t a, int32_t b, int32_t theta, lePoint* p);

// *****************************************************************************
/* Function:
    double leAtan(int32_t x, int32_t y);

  Summary:
    Performs the inverse tangent operation

  Parameters:
    val - floating point value

  Returns:
    double - the angle in radians
*/
/**
 * @brief Calculate atan of points.
 * @details Calculate atan of <span style="color: #820a32"><em>x</em></span> and
 * <span style="color: #820a32"><em>y</em></span>.
 * @code
 * double val;
 * double angle = leAtan(val);
 * @endcode
 * @param param1 val is value.
 * @return the angle in radians
 */
double leAtan(double val);

typedef struct
{
    leBool q1;
    leBool q2;
    leBool q3;
    leBool q4;
} leArcQuadrantQuery;

leBool lePointOnLineSide(lePoint* pt,
                         lePoint* linePt,
                         lePoint* sign);

// *****************************************************************************
/* Function:
    void leSortPointsX(lePoint* p1, lePoint* p2)

  Summary:
    Sorts two points on the X axis.

  Description:
    Sorts two points on the X axis.

  Parameters:
    lePoint* p1 - the first point
    lePoint* p2 - the second point

  Returns:
    The pointers are modified with the results of the sort

  Remarks:

*/
/**
 * @brief Sorts two points on the X axis.
 * @details Sort points <span style="color: #820a32"><em>p1</em></span> and
 * <span style="color: #820a32"><em>p2</em></span> on the X axis.
 * @code
 * leSortPointsX(p1, p2);
 * @endcode
 * @param param1 p1 is point.
 * @param param2 p2 is point.
 * @return void.
 */
void leSortPointsX(lePoint* p1, lePoint* p2);

// *****************************************************************************
/* Function:
    void leSortPointsX(lePoint* p1, lePoint* p2)

  Summary:
    Sorts two points on the Y axis.

  Description:
    Sorts two points on the Y axis.

  Parameters:
    lePoint* p1 - the first point
    lePoint* p2 - the second point

  Returns:
    The pointers are modified with the results of the sort

  Remarks:

*/
/**
 * @brief Sorts two points on the Y axis.
 * @details Sort points <span style="color: #820a32"><em>p1</em></span> and
 * <span style="color: #820a32"><em>p2</em></span> on the Y axis.
 * @code
 * leSortPointsY(p1, p2);
 * @endcode
 * @param param1 p1 is a point.
 * @param param2 p2 is a point.
 * @return void.
 */
void leSortPointsY(lePoint* p1, lePoint* p2);

// *****************************************************************************
/* Function:
    int32_t leGetXGivenYOnLine(lePoint p1, lePoint p2, int32_t y)

  Summary:
    Projects a point onto another point given a y coordinate.

  Description:
    Projects a point onto another point given a y coordinate.

  Parameters:
    lePoint p1 - the first point
    lePoint p2 - the second point
    int32_t y - the y coordinate

  Returns:
    int32_t - the resultant x coordinate

  Remarks:

*/
/**
 * @brief Project point.
 * @details Project point <span style="color: #820a32"><em>p1</em></span> onto
 * <span style="color: #820a32"><em>p2</em></span> on
 * <span style="color: #820a32"><em>y</em></span> coordinate.
 * @code
 * lePoint p1;
 * lePoint p2;
 * int32_t y;
 * int32_t x = leGetYGivenXOnLine(p1, p2, y);
 * @endcode
 * @param param1 p1 is point to rotate.
 * @param param2 p2 of the rotation.
 * @param param3 y is the angle in degrees.
 * @return the x coordinate.
 */
int32_t leGetXGivenYOnLine(lePoint p1, lePoint p2, int32_t y);

// *****************************************************************************
/* Function:
    int32_t leGetYGivenXOnLine(lePoint p1, lePoint p2, int32_t x)

  Summary:
    Projects a point onto another point given a x coordinate.

  Description:
    Projects a point onto another point given a x coordinate.

  Parameters:
    lePoint p1 - the first point
    lePoint p2 - the second point
    int32_t x - the x coordinate

  Returns:
    int32_t - the resultant y coordinate

  Remarks:

*/
/**
 * @brief Project Y give X.
 * @details Project point <span style="color: #820a32"><em>p1</em></span> onto
 * <span style="color: #820a32"><em>p2</em></span> on
 * <span style="color: #820a32"><em>x</em></span> coordinate.
 * @code
 * lePoint p1;
 * lePoint p2;
 * int32_t x;
 * int32_t y = leGetYGivenXOnLine(p1, p2, x);
 * @endcode
 * @param param1 p1 is point to rotate.
 * @param param2 p2 of the rotation.
 * @param param3 x is the angle in degrees.
 * @return the y coordinate.
 */
int32_t leGetYGivenXOnLine(lePoint p1, lePoint p2, int32_t x);

// *****************************************************************************
/* Function:
    void leRotatePoint(lePoint pos,
                       lePoint org,
                       int32_t ang,
                       lePoint* res)

  Summary:
    Rotates a point given an origin and an angle in degrees.

  Description:
    Rotates a point given an origin and an angle in degrees.

  Parameters:
    lePoint pos - the point to rotate
    lePoint org - the origin of the rotation
    int32_t ang - the angle in degrees

  Returns:
    lePoint - the rotated point

  Remarks:

*/
/**
 * @brief Rotates point.
 * @details Rotates <span style="color: #820a32"><em>point</em></span> around
 * <span style="color: #820a32"><em>origin</em></span>
 * at <span style="color: #820a32"><em>angle</em></span> degrees.
 * @code
 * lePoint point;
 * lePoint origin;
 * int32_t angle;
 * lePoint res;
 * leRotatePoint(point, origin, angle, &res);
 * @endcode
 * @param param1 point is point to rotate.
 * @param param2 origin of the rotation.
 * @param param3 angle is the angle in degrees.
 * @param param4 the resulting point
 * @return void
 */
void leRotatePoint(lePoint pos,
                   lePoint org,
                   int32_t ang,
                   lePoint* res);

// *****************************************************************************
/* Function:
    void leRotatedRectBounds(leRect rect,
                             lePoint org,
                             int32_t ang,
                             leRect* res)

  Summary:
    Calculates the bounding rectangle of an area rotated around
    an anchor point.

  Description:
    Calculates the bounding rectangle of an area rotated around
    an anchor point.

  Parameters:
    leRect rect - the area to rotate
    lePoint org - the origin of the rotation
    int32_t ang - the angle in degrees

  Returns:
    leRect - the rectangle that completely contains the rotated area

  Remarks:

*/
/**
 * @brief Calculate bounding rectangle.
 * @details Calculates the bounding rectangle for the area
 * <span style="color: #820a32"><em>rect</em></span>
 * rotated about <span style="color: #820a32"><em>origin</em></span>
 * at <span style="color: #820a32"><em>angle</em></span>.
 * @code
 * uint32_t whole = lePercentOf(l, percent);
 * @endcode
 * @param rect area to rotate.
 * @param ang the angle in degrees.
 * @param res the resultant rectangle
 * @return void
 */

void leRotatedRectBounds(leRect rect,
                         int32_t ang,
                         leRect* res);


// *****************************************************************************
/* Function:
    float leSqrt(const float x)

  Summary:
    Fast square root approximation function.

  Description:
    Approximates a square root using the magic number method.

  Parameters:
    const float x - the number to calculate the root for

  Returns:
    float - square root of the input number

  Remarks:

*/
float leSqrt(const float x);

void lePointOnCircle(uint32_t radius,
                     int32_t angle,
                     lePoint* res);

uint32_t leDegreesFromPercent(uint32_t percent,
                              int32_t centerAngle,
                              int32_t startAngle);

uint32_t lePercentFromDegrees(uint32_t degrees,
                              int32_t centerAngle,
                              int32_t startAngle);

void leNormalizeAngles(int32_t startAngle,
                       int32_t spanAngle,
                       int32_t* normalizedStartAngle,
                       int32_t* normalizedEndAngle);

typedef struct leResolvedAngleRanges
{
    uint32_t angleCount;
    struct
    {
        int32_t startAngle;
        int32_t endAngle;
        leArcQuadrantQuery quadrants;
    } angle0;
    struct
    {
        int32_t startAngle;
        int32_t endAngle;
        leArcQuadrantQuery quadrants;
    } angle1;
} leResolvedAngleRanges;

void leResolveAngles(int32_t startAngle,
                     int32_t spanAngle,
                     leResolvedAngleRanges* res);

float leRound(float flt);

#endif /* LE_MATH_H */
