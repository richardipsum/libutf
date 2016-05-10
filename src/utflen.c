/* See LICENSE file for copyright and license details. */
#include "utf.h"

size_t
utfnlen(const char *s, size_t len)
{
	unsigned char c, i, n, x;
	const char *p;
	size_t k;
	Rune r;

	for(k = 0; *s != '\0'; len -= s - p, p = s, k++) {
		if(len == 0) /* can't even look at s[0] */
			break;

		c = *s++;

		if((c & 0300) != 0300) /* not a leading byte */
			continue;

		n = utftab[c & 077];

		if(n == 0) /* illegal byte */
			continue;

		if((*s & 0300) != 0200) /* not a continuation byte */
			continue;

		x = 0377 >> n;
		r = c & x;

		r = (r << 6) | (*s++ & 077);

		if(r <= x) /* overlong sequence */
			continue;

		for(i = 2; i < n; i++) {
			if((*s & 0300) != 0200) /* not a continuation byte */
				break;
			s++;
		}
	}

	return k;
}

size_t
utflen(const char *s)
{
	return utfnlen(s, (size_t)-1);
}
