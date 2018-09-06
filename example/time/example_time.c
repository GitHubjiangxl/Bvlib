#include <bv.h>
#include <stdio.h>
#include <string.h>

int main()
{
    BV_TIME_HANDLE timeHandle;
    int i;

    //计算sleep5的时间(ms)
    timeHandle = bv_time_ms_start();
    sleep(5);
    printf("time = %ums\n", bv_time_ms_end(timeHandle));

    //计算一条指令的运行时间(us)
    timeHandle = bv_time_us_start();
    i=5;
    printf("time = %uus\n", bv_time_us_end(timeHandle));
    
    //计算一条指令的运行时间(ns)
    timeHandle = bv_time_ns_start();
    i = 5;
    printf("time = %uns\n", bv_time_ns_end(timeHandle));

    return 0;
}
