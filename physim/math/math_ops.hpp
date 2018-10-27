#pragma once

// C++ includes
#include <cmath>

// physim includes
#include <physim/math/math_misc.hpp>
#include <physim/math/math_ops_add.hpp>
#include <physim/math/math_ops_sub.hpp>
#include <physim/math/math_ops_mul.hpp>
#include <physim/math/math_ops_div.hpp>

namespace physim {
namespace math {

/* The readability of the code resulting from using these
 * macros is not too good. However, each of them has a clear
 * 'syntax', so to speak.
 *
 * First, start with '__pm', which makes reference to the
 * namespaces physim::math. Then follows the type of operation:
 * 'add', 'sub', 'mul', 'div'. That is, '__pm_add', '__pm_sub',
 * '__pm_mul', '__pm_div'. Multiplication and division between
 * two vectors are always component-wise.
 *
 * After the operation type comes the parameter specification.
 * There are two basic types of parameters, though three letters
 * are used:
 * - 'v' to indicate vector (see @ref vec3),
 * - 's' to indicate scalar, and
 * - 'c' to indicate 3 scalars.
 *
 * These letters are combined to specify the input parameters
 * and the type of operation between them. Notice that there
 * is always one parameter that is omitted: the output vector.
 * If they are separated by a '_' (e.g. 'v_v') they are related
 * with the operation specified. If they are together (e.g. 'vv')
 * they are both multiplied.
 *
 * For example, the macro:
 * * __pm_mul_v_v: indicates the component-wise multiplication
 * of two vectors.
 * * __pm_add_v_vs: indicates the component-wise addition of a
 * vector ('v') and the result of multiplying a vector by a
 * scalar ('vs').
 *
 * The first vector of these operations always has a leading '+'.
 *
 * Some operations of the form 'u <- u _op_ v' are defined in a
 * number of macros. These were added an 'acc' between the
 * indicator of the operation and the parameters.
 * Therefore, the macros:
 * * __pm_sub_c: implements 'u <- u - (x,y,z)' which is equivalent
 * to 'u -= (x,y,z)'.
 * * __pm_mul_s: implements 'u <- u*s' which is equivalent to
 * 'u *= s', for a scalar value 's'.
 *
 * For longer operations, the operator indicated after 'pm'
 * only refers to the "sign of the accumulation", that is:
 * - add for += X
 * - sub for -= X
 * - mul for *= X
 * - div for /= X
 * The operator between the vectors in the expression X is
 * always the sum (+).
 *
 * Below follow the details on the defintion of each macro.
 */

} // -- namespace math
} // -- namespace physim
