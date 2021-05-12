#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

void assign(int** arr, int m, int n)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            arr[i][j] = i + j;
        }
    }
}

int minitsor(file)
{
    int count = 1;
    char c = '0';
    while (!feof(file))
    {
        c = fgetc(file);
        if (c == '\n')
        {
            count++;
        }
    }
    rewind(file);
    return count;
}

int minitoszlop(file)
{
    int count = 1;
    char c = '0';
    while (!feof(file))
    {
        c = fgetc(file);
        if (c == ' ')
        {
            count++;
        }
        else if(c == '\n')
        {
            break;
        }
    }
    rewind(file);
    return count;
}


int main()
{
    char utvonal[100];
    int i = 0, j = 0;
    double egyuttes = 0, perem1 = 0, perem2 = 0, felt1 = 0, felt2 = 0, inf = 0, count = 0;
    FILE* f;

    printf("Adja meg a forrasfajl eleresi utvonalat (fajlnevvel es kiterjesztessel egyutt):\n");
    gets(utvonal);
    f = fopen(utvonal, "r");
    if (f == NULL) 
    {
        perror("Hiba a fajl megnyitasa soran!\nA program billentyu lenyomas utan kilep!\n");
        getchar();
        abort();
        return(-1);
    }
    else 
    {
            printf("File megnyitas sikeres!\n"); 
    }
    
    int sor = minitsor(f);
    int oszlop = minitoszlop(f);
    // printf("A sorok szama: %d\n", sor);
    // printf("Az oszlopok szama: %d\n", oszlop);
    double **matrix = (double**)malloc(sor * oszlop * sizeof(double*));

    for (int r = 0; r < sor; r++)
    {
        matrix[r] = (double*)malloc(sor * oszlop * sizeof(int));
    }

    assign(matrix, sor, oszlop);

    if (matrix == NULL)
    {
        printf("Nem tudtam memoriat allokalni!\nA program billentyu lenyomas utan kilep!\n");
        getchar();
        abort();
        return -1;
    }

        for (i = 0; i < sor; i++)
        {
            for (j = 0; j < oszlop; j++)
            {
                fscanf(f, "%lf", &matrix[i][j]);
            }
        }
        fclose(f);
        /*for (i = 0; i < sor; i++)
        {
            for (j = 0; j < oszlop; j++)
            {
                printf("%#.4Lf\t", matrix[i][j]);
            }
            printf("\n");
        }*/
        // Együttes entrópia
        for (i = 0; i <sor; i++)
        {
            for (j = 0; j < oszlop; j++)
            {
                if (matrix[i][j] != 0)
                {
                    egyuttes += -(matrix[i][j] * (log2(matrix[i][j])));
                }
            }
        }
        // printf("H (X,Y): %#.4Lf\n", egyuttes);
        // Peremeloszlások entrópiája
        // Y
        for (i = 0; i < sor; i++)
        {
            for (j = 0; j < oszlop; j++)
            {
                count += matrix[i][j];
            }
            perem2 += -(count * log2(count));
            count = 0;
        }
        // X
        for (j = 0; j < oszlop; j++)
        {
            for (i = 0; i < sor; i++)
            {
                count += matrix[i][j];
            }
            perem1 += -(count * log2(count));
            count = 0;
        }
        // printf("H (X): %#.4Lf\nH (Y): %#.4Lf\n", perem1, perem2);
        // Feltételes eloszlások
        // H (X|Y)
        felt2 = egyuttes - perem2;
        // H (Y|X)
        felt1 = egyuttes - perem1;
        // printf("H (X|Y): %#.4Lf\n", felt1);
        // printf("H (Y|X): %#.4Lf\n", felt2);
        // Kölcsönös információmennyiség
        // I (X,Y)
        inf = perem2 - felt1;
        // printf("I (X,Y): %#.4Lf\n", inf);

        printf("Adja meg a mentes utvonalat (fajlnevvel egyutt):\n");
        gets(utvonal);

        f = fopen(utvonal, "w");
        if (f == NULL)
        {
            perror("Hiba a fajl megnyitasa soran!\nA program most kilep!\n");
            abort();
            return(-1);
        }
        else
        {
            printf("File megnyitas sikeres!\n");
        }
        fprintf(f, "H (X,Y): %#.4Lf\nH (X): %#.4Lf\nH (Y): %#.4Lf\nH (X|Y): %#.4Lf\nH (Y|X): %#.4Lf\nI (X,Y): %#.4Lf\n", egyuttes, perem1, perem2, felt2, felt1, inf);
        fclose(f);
        printf("Kilepeshez nyomjon le egy billentyut!");
        getchar();

    free (matrix);
    return 0;
}