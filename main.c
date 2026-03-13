#include <stdio.h>

int readFile(float data[][4])
{
    FILE *file = fopen("formula.txt", "r");

    if (file == NULL)
    {
        return 1;
    }
    int row = 0;
    while (fscanf(file, "%f %f %f %f", &data[row][0], &data[row][1], &data[row][2], &data[row][3]) == 4)
    {
        row++;
    }
    fclose(file);

    return row;
}

void applyKalman(float col[], int row)
{
    float q = 0.1; // süreç gürültüsü — sisteme güven
    float r = 0.5; // ölçüm gürültüsü — veriye güven
    float p = 1.0; // başlangıç belirsizliği — yüksek başla
    float x = 0.0;
    for (int i = 0; i < row; i++)
    {
        if (col[i] != -999.0)
        {
            x = col[i];
            break;
        }
    }

    for (int i = 0; i < row; i++)
    {
        float x_tahmin = x;
        float p_tahmin = p + q;

        if (col[i] != -999.0)
        {
            float k = p_tahmin / (p_tahmin + r);
            x = x_tahmin + k * (col[i] - x_tahmin);
            p = (1 - k) * p_tahmin;
        }
        else
        {
            x = x_tahmin;
            p = p_tahmin;
            col[i] = x;
        }
    }
}

int main(void)
{

    float data[1000][4];
    int row = readFile(data);

    printf("Okunan satir sayisi: %d\n", row);

    for (int i = 0; i < 4; i++)
    {
        float temp[row];
        for (int j = 0; j < row; j++)
        {
            temp[j] = data[j][i];
        }
        applyKalman(temp, row);
        for (int j = 0; j < row; j++)
        {
            data[j][i] = temp[j];
        }
    }
    for (int i = 0; i < row; i++)
    {
        printf("%f %f %f %f\n", data[i][0], data[i][1], data[i][2], data[i][3]);
    }
    return 0;
}