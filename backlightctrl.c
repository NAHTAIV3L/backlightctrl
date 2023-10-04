#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BRIGHTNESSFILE "/sys/class/backlight/intel_backlight/brightness"
#define MAXBRIGHTNESSFILE "/sys/class/backlight/intel_backlight/max_brightness"
#define BUFSIZE 10

char *readfile(char *filename)
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(filename, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }
    else
    {
        printf("failed to read file %s", filename);
        exit(1);
    }
    return buffer;
}

char *program = NULL;
char *current = NULL;
char *max = NULL;
int icurrent = 0;
int imax = 0;
double onepercent = 0;

void usage()
{
    printf("usage: %s [options]\n", program);
    puts("\t-get\t\tPrints brightness in percent\n"
         "\t-set [percent]\tsets brightness\n"
         "\t-inc [percent]\tincreases brightness by [percent]\n"
         "\t-inc [percent]\tdecreases brightness by [percent]\n");
    exit(1);
}

void setbrightness(int newbrightness)
{
    if (newbrightness > imax)
    {
        newbrightness = imax;
    }
    else if (newbrightness < 0)
    {
        newbrightness = 0;
    }

    char buf[BUFSIZE] = {0};
    snprintf(buf, BUFSIZE, "%d", newbrightness);

    FILE *f = fopen(BRIGHTNESSFILE, "wb");
    if (f)
    {
        fputs(buf, f);
        fclose(f);
    }
    else
    {
        printf("failed to open file");
        exit(1);
    }
}

int main(int argc, char **argv)
{
    program = argv[0];

    if (argc < 2)
        usage();

    current = readfile(BRIGHTNESSFILE);
    max = readfile(MAXBRIGHTNESSFILE);

    imax = atoi(max);
    icurrent = atoi(current);

    onepercent = atof(max) / 100;

    if (!strncmp(argv[1], "-get", 4))
    {
        double num = (atof(current) / atof(max)) * 100;
        printf("%d\n", (int)(num < 0 ? (num - 0.5) : (num + 0.5)));
    }
    else
    {
        if (argc < 3)
            usage();

        int newval = onepercent * atoi(argv[2]);

        if (!strncmp(argv[1], "-set", 4))
        {
        }
        else if (!strncmp(argv[1], "-inc", 4))
            newval += icurrent;

        else if (!strncmp(argv[1], "-dec", 4))
            newval = icurrent - newval;

        else
            usage();

        setbrightness(newval);
    }
    return 0;
}
