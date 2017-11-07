//
// Created by A.Zed on 10/30/2017.
//

#include <stdio.h>
#include <unistd.h>

int main (){
    int a1[7], a2[7];
    FILE *fp;
    fp = fopen("/proc/stat", "r"); //opening /proc/stat to read working and total jiffies
    fscanf(fp,"cpu %d %d %d %d %d %d %d", &a1[0], &a1[1], &a1[2], &a1[3], &a1[4], &a1[5], &a1[6]); //reading jiffies from stat 
    sleep(1); //letting the cpu to work for one second
    fclose(fp);//closing the previous stat
    fp = fopen("/proc/stat", "r");//opening new stat file to read new CPU data
    fscanf(fp,"cpu %d %d %d %d %d %d %d", &a2[0], &a2[1], &a2[2], &a2[3], &a2[4], &a2[5], &a2[6]);//reading new jiffies
    float work_jiffies_1, work_jiffies_2, total_jiffies_1, total_jiffies_2, cpu;
    work_jiffies_1 = a1[0] + a1[1] + a1[2];//calculating previous working jiffies
    total_jiffies_1 = work_jiffies_1 + a1[3] + a1[4] + a1[5] + a1[6];//calculating previous total jiffies
    work_jiffies_2 = a2[0] + a2[1] + a2[2];//calculating new working jiffies
    total_jiffies_2 = work_jiffies_2 + a2[3] + a2[4] + a2[5] + a2[6];//calculating new total jiffies
    cpu = (work_jiffies_2 - work_jiffies_1) / (total_jiffies_2 - total_jiffies_1) * 100;//calculating CPU usage based on working and total jiffies in the time span
    printf("CPU usage during last period: %f\n", cpu);//printing ...
    fclose(fp);//closing the new stat file
    return 0;//returning 0
}

