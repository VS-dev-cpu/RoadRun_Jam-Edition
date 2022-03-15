#pragma once

#include <iostream>
#include <cmath>

using namespace std;

int mapper(long x, long in_min, long in_max, long out_min, long out_max)
{
    return ((x - in_min) * (out_max - out_min) + 0.1) / (in_max - in_min) + out_min;
}

bool isCollide(int x, int y, int xx, int yy, int size_x, int size_y)
{

    if (yy + size_y > y and y > yy)
    {
        if (xx + size_x > x and x > xx)
        {
            return true;
        }
    }
    return false;
}

int roundTo(int roundToNum, int mode, int in_num)
{
    switch (mode)
    {
    case 0:
        double num, dec, numcpy;
        int n;

        num = in_num;

        numcpy = num;
        dec = num - floor(num);
        num = numcpy;
        n = floor(num);

        if (n % roundToNum < roundToNum / 2)
        {
            n = (n / roundToNum) * roundToNum;
        }
        else if (n % roundToNum == roundToNum / 2)
        {
            if (dec > 0)
                n = (((n + roundToNum) / roundToNum) * roundToNum);
            else
                n = (n / roundToNum) * roundToNum;
        }
        else
        {
            n = (((n + roundToNum) / roundToNum) * roundToNum);
        }

        return n;
        break;

    case 1:
        int current = in_num;
        while (!(current < roundToNum))
            current -= roundToNum;
        return in_num - current;
        break;
    }

    return 0;
}

int lock(int low, int high, int value)
{
    if (value < low)
        return low;

    if (value > high)
        return high;

    return value;
}

int stoi(char *text, int len)
{
    int negative = 1;
    int index = 0;
    int state = 1;

    for (int i = len - 1; i >= 0; i--)
    {
        switch (text[i])
        {
        case '0':
            index += 0;
            state *= 10;
            break;
        case '1':
            index += state * 1;
            state *= 10;
            break;
        case '2':
            index += state * 2;
            state *= 10;
            break;
        case '3':
            index += state * 3;
            state *= 10;
            break;
        case '4':
            index += state * 4;
            state *= 10;
            break;
        case '5':
            index += state * 5;
            state *= 10;
            break;
        case '6':
            index += state * 6;
            state *= 10;
            break;
        case '7':
            index += state * 7;
            state *= 10;
            break;
        case '8':
            index += state * 8;
            state *= 10;
            break;
        case '9':
            index += state * 9;
            state *= 10;
            break;
        case '-':
            negative = -1;
            break;
        default:
            i = 0;
            break;
        }
    }

    return index * negative;
}

char itos(int num)
{
    switch (num)
    {
    case 0:
        return '0';
        break;
    case 1:
        return '1';
        break;
    case 2:
        return '2';
        break;
    case 3:
        return '3';
        break;
    case 4:
        return '4';
        break;
    case 5:
        return '5';
        break;
    case 6:
        return '6';
        break;
    case 7:
        return '7';
        break;
    case 8:
        return '8';
        break;
    case 9:
        return '9';
        break;
    default:
        return ' ';
        break;
    }
}