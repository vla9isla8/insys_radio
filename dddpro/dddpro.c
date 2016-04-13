#include <prf\sys101.h>

void main()
{
	int i;
	while (1) {
		Blink();
		for (i = 0; i<8000000; i++);
	}
}
