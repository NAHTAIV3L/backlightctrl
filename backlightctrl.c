#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BRIGHTNESSFILE    "/sys/class/backlight/intel_backlight/brightness"
#define MAXBRIGHTNESSFILE "/sys/class/backlight/intel_backlight/max_brightness"
#define BUFSIZE 10

char *readfile(char *filename) {
    char *buffer = 0;
    long length;
    FILE *f = fopen(filename, "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) {
          fread(buffer, 1, length, f);
        }
        fclose(f);
    }
    return buffer;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: %s [options]\n", argv[0]);
        return 1;
    }
    char *current = readfile(BRIGHTNESSFILE);
    char *max = readfile(MAXBRIGHTNESSFILE);
     
    if (!current && !max)
    { printf("files not read"); return 1; }

    if (!strncmp(argv[1], "-get", 4))
    {
            double num = (atof(current) / atof(max)) * 100;
            printf("%d\n", (int)(num < 0 ? (num - 0.5) : (num + 0.5)));
    }
    else if (!strncmp(argv[1], "-set", 4))
    {
        if (argc < 3)
        {
            printf("usage: %s %s [percent]\n", argv[0], argv[1]);
            return 1;
        }
        double oneperc = atof(max) / 100;
        int newval = oneperc * atof(argv[2]);
        if (newval > atoi(max))
        { newval = atoi(max); }

        char buf[BUFSIZE] = { 0 };
        snprintf(buf, BUFSIZE, "%d", newval);

        FILE *f = fopen(BRIGHTNESSFILE, "wb");
        if (f)
        {
            fputs(buf, f);
            fclose(f);
        }
        else
        { printf("failed to open file"); return 1; }
    }
    else if (!strncmp(argv[1], "-inc", 4))
    {
        if (argc < 3)
        {
            printf("usage: %s %s [percent]\n", argv[0], argv[1]);
            return 1;
        }

        double oneperc = atof(max) / 100;
        int newval = atoi(current) + (oneperc * atof(argv[2]));
        if (newval > atoi(max))
        { newval = atoi(max); }

        char buf[BUFSIZE] = { 0 };
        snprintf(buf, BUFSIZE, "%d", newval);

        FILE *f = fopen(BRIGHTNESSFILE, "wb");
        if (f)
        {
            fputs(buf, f);
            fclose(f);
        }
        else
        { printf("failed to open file"); return 1; }
    }
    else if (!strncmp(argv[1], "-dec", 4))
    {
        if (argc < 3)
        {
            printf("usage: %s %s [percent]\n", argv[0], argv[1]);
            return 1;
        }

        double oneperc = atof(max) / 100;
        int newval = atoi(current) - (oneperc * atof(argv[2]));
        if (newval > atoi(max))
        { newval = atoi(max); }

        char buf[BUFSIZE] = { 0 };
        snprintf(buf, BUFSIZE, "%d", newval);

        FILE *f = fopen(BRIGHTNESSFILE, "wb");
        if (f)
        {
            fputs(buf, f);
            fclose(f);
        }
        else
        { printf("failed to open file"); return 1; }
    }
    else
    {
        printf("error: invalid option\n");
    }
    return 0;
}
