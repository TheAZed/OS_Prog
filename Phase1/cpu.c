//
// Created by A.Zed on 10/30/2017.
//

#include <stdio.h>
#include <unistd.h>

int main (){
    int a1[7], a2[7];
    FILE *fp;
    fp = fopen("/proc/stat", "r");
    fscanf(fp,"cpu %d %d %d %d %d %d %d", &a1[0], &a1[1], &a1[2], &a1[3], &a1[4], &a1[5], &a1[6]);
    usleep(1000);
    fclose(fp);
    fp = fopen("/proc/stat", "r");
    fscanf(fp,"cpu %d %d %d %d %d %d %d", &a2[0], &a2[1], &a2[2], &a2[3], &a2[4], &a2[5], &a2[6]);
    float work_jiffies_1, work_jiffies_2, total_jiffies_1, total_jiffies_2, cpu;
    work_jiffies_1 = a1[0] + a1[1] + a1[2];
    total_jiffies_1 = work_jiffies_1 + a1[3] + a1[4] + a1[5] + a1[6];
    work_jiffies_2 = a2[0] + a2[1] + a2[2];
    total_jiffies_2 = work_jiffies_2 + a2[3] + a2[4] + a2[5] + a2[6];
    cpu = (work_jiffies_2 - work_jiffies_1) / (total_jiffies_2 - total_jiffies_1) * 100;
    printf("CPU usage during last period: %f\n", cpu);
    fclose(fp);
    return 0;
}

