#pragma once

// is k close enough to 0? (i.e. below tolerance 'tol')
#define is_zero(k, tol) (std::abs(k) <= tol)
