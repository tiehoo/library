#include <stdio.h>
#include <string.h>

int main(int arc, char** argv)
{
	char sz1[5], sz2[5];
	strcpy(sz1, "AB");
	strcpy(sz2, "ab");
	printf("%d\n", strncasecmp(sz1, sz2, 0));
	//printf("Hello\n");
}

