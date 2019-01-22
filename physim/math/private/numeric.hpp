#pragma once

// is k close enough to 0? (i.e. below tolerance 'tol')
#define __pm_is_zero(k, tol) (std::abs(k) <= tol)
#define __pm_eq(a,b,tol) (std::abs((a) - (b)) <= tol)
#define __pm_inv(x) (1.0f/(x))
#define __pm_sq(x) ((x)*(x))
