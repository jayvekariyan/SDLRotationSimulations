#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#define WIDTH 900
#define HEIGHT 600
#define THICKNESS 3
#define COLOUR_WHITE 0xffffffff
#define COLOUR_BLACK 0x000000
// #define z_screen 10 // only z because it is parallel to xy plane
// #define xs 1100
// #define ys 500
// #define zs -2000
#define l 400 // Length of cube

struct Point3D
{
    double x, y, z;
};
struct Point2D
{
    double x, y;
};

void DrawLineSeg(SDL_Surface *surface, struct Point2D p_start, struct Point2D p_end)
{
    double slop = (p_end.y - p_start.y) / (p_end.x - p_start.x);
    double t = atan(slop);

    double half_lengthsq = (pow(p_start.x - p_end.x, 2) + pow(p_start.y - p_end.y, 2)) / 4;

    double x0 = (p_start.x + p_end.x) / 2;
    double y0 = (p_start.y + p_end.y) / 2;
    double x = x0;
    double y = y0;
    SDL_Rect rect;
    while (pow(x0 - x, 2) + pow(y0 - y, 2) <= half_lengthsq)
    {
        rect = (SDL_Rect){(WIDTH / 2) + x, (HEIGHT / 2) - y, THICKNESS, THICKNESS};
        SDL_FillRect(surface, &rect, 0xffffffff);
        x += cos(t);
        y += sin(t);
    }
    x = x0;
    y = y0;
    while (pow(x0 - x, 2) + pow(y0 - y, 2) <= half_lengthsq)
    {
        rect = (SDL_Rect){(WIDTH / 2) + x, (HEIGHT / 2) - y, THICKNESS, THICKNESS};
        SDL_FillRect(surface, &rect, 0xffffffff);
        x -= cos(t);
        y -= sin(t);
    }
}

// void Project(struct Point3D p3[], struct Point2D p2[])
// {
//     double lmd;
//     for (int i = 0; i < 8; i++)
//     {
//         printf("%f %f %f\n", p3[i].x, p3[i].y, p3[i].z);
//         lmd = (z_screen - zs) / (p3[i].z - zs);
//         printf("%f\n", lmd);
//         p2[i].x = xs + lmd * (p3[i].x-xs);
//         p2[i].y = ys + lmd * (p3[i].y-ys);
//     }
// }

struct Circle
{
    double x;
    double y;
    double r;
};
void FillCircle(SDL_Surface *surface, struct Circle circle, Uint32 color)
{
    double radius_squared = pow(circle.r, 2);
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++)
    {
        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++)
        {
            double distace_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distace_squared < radius_squared)
            {
                SDL_Rect pixel = (SDL_Rect){(WIDTH / 2) + x, (HEIGHT / 2) - y, 1, 1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

void TT(struct Point3D p3[], struct Point2D p2[])
{
    for (int i = 0; i < 4; i++)
    {

        p2[i].x = p3[i].x;
        p2[i].y = p3[i].y;
    }
}

void Rotate(struct Point3D points[], double A, double B, double C)
{
    double x, y, z;
    for (int i = 0; i < 4; i++)
    {
        x = points[i].x;
        y = points[i].y;
        z = points[i].z;
        points[i].x = cos(A) * cos(B) * x + (cos(A) * sin(B) * sin(C) - sin(A) * cos(C)) * y + (cos(A) * sin(B) * cos(C) + sin(A) * sin(C)) * z;
        points[i].y = sin(A) * cos(B) * x + (sin(A) * sin(B) * sin(C) + cos(A) * cos(C)) * y + (sin(A) * sin(B) * cos(C) - cos(A) * sin(C)) * z;
        points[i].z = -sin(B) * x + cos(B) * sin(C) * y + cos(B) * cos(C) * z;

        // printf("%f %f %f\n", points[i].x, points[i].y, points[i].z);
        // printf("%f\n", pow(points[i].x, 2) + pow(points[i].y, 2) + pow(points[i].z, 2));
    }
}

int main(int argc, char *argv[])
{
    printf("WELCOME TO ROTATING CUBE SIMULATOR\n");
    printf("Press w to increase angular velocity about x-axis\nPress s to decrese angular velocity about x-axis\n");
    printf("Press e to increase angular velocity about y-axis\nPress d to decrese angular velocity about y-axis\n");
    printf("Press r to increase angular velocity about z-axis\nPress f to decrese angular velocity about z-axis\n");

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Cube Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    SDL_Rect ERASE_RECT = {0, 0, WIDTH, HEIGHT};

    struct Point3D points3d[4];
    struct Point2D points2d[4];
    double x = l / pow(3, 0.5);
    double h = l * pow(2.0 / 3.0, 0.5);
    points3d[0] = (struct Point3D){x * cos(2 * M_PI / 3), x * sin(2 * M_PI / 3), -h / 2}; // B
    points3d[1] = (struct Point3D){0, 0, h / 2};                                          // A
    points3d[2] = (struct Point3D){-x * cos(M_PI / 3), -x * sin(M_PI / 3), -h / 2};       // C
    points3d[3] = (struct Point3D){x, 0, -h / 2};                                         // D
                                                                                          // points3d[4] = (struct Point3D){l, -l, l};   // G
                                                                                          // points3d[5] = (struct Point3D){l, -l, -l};  // H
                                                                                          // points3d[6] = (struct Point3D){-l, -l, -l}; // E
                                                                                          // points3d[7] = (struct Point3D){-l, -l, l};  // F

    // Project(points3d, points2d);

    // struct Point2D p2 = {WIDTH / 2,  HEIGHT / 2};
    // struct Point2D p1 = {(WIDTH / 2)+100, (HEIGHT / 2) -100};

    // struct Point2D p2 = {0, 0};
    // struct Point2D p1 = {30, 50};

    // for (int i = 0; i < 8; i++)
    // {
    //     printf("%f %f\n", points2d[i].x, points2d[i].y);
    // }

    int running = 1;
    int k1 = 20, k2 = 30, k3 = 10;
    SDL_Event event;
    while (running)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            running = 0;
        }
        SDL_FillRect(surface, &ERASE_RECT, COLOUR_BLACK);
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_W)
        {
            k1++;
            printf("k1:%d\n", k1);
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_E)
        {
            k2++;
            printf("k2:%d\n", k2);
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_R)
        {
            k3++;
            printf("k3:%d\n", k3);
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_S)
        {
            k1--;
            printf("k1:%d\n", k1);
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_D)
        {
            k2--;
            printf("k2:%d\n", k2);
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F)
        {
            k3--;
            printf("k3:%d\n", k3);
        }

        Rotate(points3d, k1 * 0.001, k2 * 0.001, k3 * 0.001);
        TT(points3d, points2d);
        for (int i = 0; i < 4; i++)
        {
            struct Circle circle = {points2d[i].x,points2d[i].y,10};
            FillCircle(surface,circle,COLOUR_WHITE);
            
        }

        // DrawLineSeg(surface, p1, p2);

        DrawLineSeg(surface, points2d[1], points2d[0]);
        DrawLineSeg(surface, points2d[1], points2d[2]);
        DrawLineSeg(surface, points2d[1], points2d[3]);
        DrawLineSeg(surface, points2d[2], points2d[3]);
        DrawLineSeg(surface, points2d[0], points2d[3]);
        DrawLineSeg(surface, points2d[0], points2d[2]);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }

    return 0;
}