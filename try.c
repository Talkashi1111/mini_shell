#include <stdio.h>
/*unsigned int    lcm(unsigned int a, unsigned int b)
{
	unsigned int res;
	unsigned int x;
	unsigned int j;

	x = a;
	j = b;
	if (!a || !b)
		return (0);
	while (b != 0)
	{
		res = b;
		b = a % b;
		a = res;
	}
	if ((x / a) <= (4294967295 / j))
		return (x / a * j);
	return (0);
}
*/
unsigned int    lcm(unsigned int a, unsigned int b)
{
	unsigned int res;
	unsigned int max_num;
	unsigned int max_int;

	max_int = -1;
	if (!a || !b)
		return (0);
	max_num = (a > b) ? a : b;
	res = max_num;

	while (res % a != 0 || res % b != 0)
	{
		if (res > max_int - max_num)
			return (0);
		res += max_num;
	}
	return (res);
}
int main(void)
{
	printf("resaul %d\n", lcm(-1, 2932));
	return(0);
}