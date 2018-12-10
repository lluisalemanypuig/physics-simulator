#pragma once

// the first  bit of comp stores: u.x < v.x
// the second bit of comp stores: u.y < v.y
#define __pm2_lt(comp, u,v)					\
	comp = comp | ((u).x < (v).x);			\
	comp = comp | (((u).y < (v).y) << 1)

// the first  bit of comp stores: u.x <= v.x
// the second bit of comp stores: u.y <= v.y
#define __pm2_le(comp, u,v)					\
	comp = comp | ((u).x <= (v).x);			\
	comp = comp | (((u).y <= (v).y) << 1)

// the first  bit of comp stores: u.x > v.x
// the second bit of comp stores: u.y > v.y
#define __pm2_gt(comp, u,v)					\
	comp = comp | ((u).x > (v).x);			\
	comp = comp | (((u).y > (v).y) << 1)

// the first  bit of comp stores: u.x >= v.x
// the second bit of comp stores: u.y >= v.y
#define __pm2_ge(comp, u,v)					\
	comp = comp | ((u).x >= (v).x);			\
	comp = comp | (((u).y >= (v).y) << 1)

// the first  bit of comp stores: u.x == v.x
// the second bit of comp stores: u.y == v.y
#define __pm2_eq(comp, u,v)					\
	comp = comp | ((u).x == (v).x);			\
	comp = comp | (((u).y == (v).y) << 1)

// equality with tolerance comparison
// the first  bit of comp stores: (u.x - v.x) <= tol
// the second bit of comp stores: (u.y - v.y) <= tol
#define __pm2_eq_tol(comp, u,v, tol)				\
	comp = comp | (((u).x - (v).x) <= tol);			\
	comp = comp | ((((u).y - (v).y) <= tol) << 1)
