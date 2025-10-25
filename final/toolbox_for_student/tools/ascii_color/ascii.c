#include <stdio.h>
#include <stdlib.h>

const char *graycode = " `.~:;I=i+oO8E#M";
typedef struct _ppm {
    int h;
    int w;
    int *pixel;
    int *color;
} PPM;

int getpixel(PPM *img, int x, int y)
{
    return img->pixel[y*img->w + x];
}
int getcolor(PPM *img, int x, int y)
{
    return img->color[y*img->w + x];
}
void destroy_ppm(PPM *img)
{
    free(img->color);
    free(img->pixel);
    free(img);
}

PPM * load_ppm(char *filename)
{
    FILE *fid;
    int w, h, level, i, j;
    PPM *p;
    int r, g, b, c;

    fid = fopen(filename, "r");
    if (fid==NULL) {
        return NULL;
    } else {
        while (fscanf(fid, "%d%d", &w, &h)!=2) {
            while (fgetc(fid)!='\n');
        }
        fscanf(fid, "%d", &level);
        p = (PPM*) malloc(sizeof(PPM));
        p->w = w;
        p->h = h;
        p->pixel = (int*) malloc(sizeof(int)*w*h);
        p->color = (int*) malloc(sizeof(int)*w*h);
        for (i=0; i<h; i++) {
            for (j=0; j<w; j++) {
                fscanf(fid, "%d%d%d", &r, &g, &b);
                p->pixel[i*w+j] = (r+g+b)/48;
                c = ((r/128) << 2) | ((g/128) << 1) | ((b/128));
                if (p->pixel[i*w+j]>4) {
                    p->color[i*w+j] = c | 8;
                } else {
                    p->color[i*w+j] = c;
                }
            }
        }
        fclose(fid);
        return p;
    }
}

int main(int argc, char **argv)
{
    PPM *image;
    FILE *fout1, *fout2;

    int x, y, c;

    if(argc != 4) {
        fprintf(stderr, "usage: ascii foo.pgm foo.pixel foo.color\n");
        return EXIT_FAILURE;
    }

    image = load_ppm(argv[1]);
    if (image == NULL) {
        fprintf(stderr, "ascii Error: could not load pgm `%s'\n", argv[1]);
        return EXIT_FAILURE;
    }
    fout1 = fopen(argv[2], "w");
    fprintf(fout1, "%d %d\n", image->h, image->w);
    fout2 = fopen(argv[3], "w");
    fprintf(fout2, "%d %d\n", image->h, image->w);

    for(y = 0; y < image->h; y++) {
        for(x = 0; x < image->w; x++) {
            c = getpixel(image, x,   y  );
            fprintf(fout1, "%c", graycode[c]);
            c = getcolor(image, x,   y  );
            fprintf(fout2, "%x", c);
        }
        fprintf(fout1, "\n");
        fprintf(fout2, "\n");
    }
    destroy_ppm(image);

    fclose(fout1);
    fclose(fout2);

    return 0;
}
