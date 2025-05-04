#include "helpers.h"
#include <math.h>
#include <stdbool.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average =
                round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Convert image to sepia
void swap(int row, int first, int second, int height, int width, RGBTRIPLE image[height][width])
{
    int buffer = 0;
    buffer = image[row][first].rgbtBlue;
    image[row][first].rgbtBlue = image[row][second].rgbtBlue;
    image[row][second].rgbtBlue = buffer;

    buffer = image[row][first].rgbtRed;
    image[row][first].rgbtRed = image[row][second].rgbtRed;
    image[row][second].rgbtRed = buffer;

    buffer = image[row][first].rgbtGreen;
    image[row][first].rgbtGreen = image[row][second].rgbtGreen;
    image[row][second].rgbtGreen = buffer;
}
// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        if (width % 2 == 0)
        {
            for (int j = 0; j < width / 2; j++)
            {
                swap(i, j, width - 1 - j, height, width, image);
            }
        }
        else
        {
            for (int j = 0; j < (width / 2.0 + 0.5); j++)
            {
                swap(i, j, width - 1 - j, height, width, image);
            }
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurimage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumred = 0, sumblue = 0, sumgreen = 0, sumpixels = 0;

            for (int surround_i = i - 1; surround_i <= i + 1; surround_i++)
            {
                for (int surround_j = j - 1; surround_j <= j + 1; surround_j++)
                {

                    if (surround_i >= 0 && surround_i < height && surround_j >= 0 &&
                        surround_j < width)
                    {
                        sumgreen += image[surround_i][surround_j].rgbtGreen;
                        sumblue += image[surround_i][surround_j].rgbtBlue;
                        sumred += image[surround_i][surround_j].rgbtRed;
                        sumpixels++;
                    }
                }
            }

            blurimage[i][j].rgbtGreen = round((float) sumgreen / sumpixels);
            blurimage[i][j].rgbtBlue = round((float) sumblue / sumpixels);
            blurimage[i][j].rgbtRed = round((float) sumred / sumpixels);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtGreen = blurimage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = blurimage[i][j].rgbtBlue;
            image[i][j].rgbtRed = blurimage[i][j].rgbtRed;
        }
    }

    return;
}

void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edgeimage[height][width];
    int surround_i, surround_j;
    int gx = 0, gy = 0, rx = 0, ry = 0, bx = 0, by = 0;
    int green, blue, red;
    int xmatrix[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int ymatrix[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            gx = 0, gy = 0, rx = 0, ry = 0, bx = 0, by = 0;

            surround_i = i;

            for (int o = -1; o <= 1; o++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    surround_i = i + o;
                    surround_j = j + l;

                    if ((surround_i >= 0 && surround_i < height && surround_j >= 0 &&
                         surround_j < width))
                    {
                        gx += image[surround_i][surround_j].rgbtGreen * xmatrix[o + 1][l + 1];
                        gy += image[surround_i][surround_j].rgbtGreen * ymatrix[o + 1][l + 1];
                        rx += image[surround_i][surround_j].rgbtRed * xmatrix[o + 1][l + 1];
                        ry += image[surround_i][surround_j].rgbtRed * ymatrix[o + 1][l + 1];
                        bx += image[surround_i][surround_j].rgbtBlue * xmatrix[o + 1][l + 1];
                        by += image[surround_i][surround_j].rgbtBlue * ymatrix[o + 1][l + 1];
                    }

                    green = round(sqrt((gx * gx) + (gy * gy)));
                    blue = round(sqrt((bx * bx) + (by * by)));
                    red = round(sqrt((rx * rx) + (ry * ry)));
                    if (green > 255)
                    {
                        edgeimage[i][j].rgbtGreen = 255;
                    }
                    else
                    {
                        edgeimage[i][j].rgbtGreen = green;
                    }
                    if (red > 255)
                    {
                        edgeimage[i][j].rgbtRed = 255;
                    }
                    else
                    {
                        edgeimage[i][j].rgbtRed = red;
                    }
                    if (blue > 255)
                    {
                        edgeimage[i][j].rgbtBlue = 255;
                    }
                    else
                    {
                        edgeimage[i][j].rgbtBlue = blue;
                    }
                }
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtGreen = edgeimage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = edgeimage[i][j].rgbtBlue;
            image[i][j].rgbtRed = edgeimage[i][j].rgbtRed;
        }
    }

    return;
}
