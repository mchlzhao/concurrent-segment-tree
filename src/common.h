#ifndef _COMMON_H
#define _COMMON_H

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#define printif(a, b...) if (a) { printf(b); }

#endif
