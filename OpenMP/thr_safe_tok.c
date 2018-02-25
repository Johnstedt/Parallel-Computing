#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* safe_strtok(char* str, char* s);

int main(int argc, char* argv[]) 
{

	char str[80] = "kjhfds-kjfds";
	char s[2] = "-";

	char* first = safe_strtok(str, s);

	printf("%s\n", first);
}


char* safe_strtok(char* str, char* s){
	size_t length = strlen(str) + 1;

	char *dest = calloc(length, sizeof(*dest));

	memcpy(dest, str, length);
	
	char *lasts;
    register int ch;

    if (dest == 0)
	dest = lasts;
    do {
	if ((ch = *dest++) == '\0')
	    return 0;
    } while (strchr(delim, ch));
    --dest;
    lasts = s + strcspn(dest, delim);
    if (*lasts != 0)
	*lasts++ = 0;
    return dest;
}