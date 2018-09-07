/********************************************************************************************
@author Bevis
@date 2018-09-03

********************************************************************************************/
#include <bv_time.h>
#include <time.h>
#include <sys/time.h>
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

/*
struct timespec
{
    time_t tv_sec; // 秒
    long tv_nsec;  //纳秒
};
*/

typedef struct time_info
{
    unsigned int uiStartTime;
    unsigned int uiEndtime;
}stTimeInfo, *pstTimeInfo;

char* bv_time_get(void)
{
    static char timeinfo[BV_TIME_INFO_LENGTH + 1];
    struct timespec time;
    struct tm nowTime;
    memset(timeinfo, 0x00, sizeof(timeinfo));
    clock_gettime(CLOCK_REALTIME, &time);  //获取相对于1970到现在的秒数
    localtime_r(&time.tv_sec, &nowTime);
    sprintf(timeinfo, "[%04d-%02d-%02d %02d:%02d:%02d]", nowTime.tm_year + 1900, nowTime.tm_mon + 1, nowTime.tm_mday, 
    nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
    return timeinfo;
}

char* bv_time_get_num(void)
{
    static char timeinfo[BV_TIME_NUM_LENGTH + 1];
    struct timespec time;
    struct tm nowTime;
    memset(timeinfo, 0x00, sizeof(timeinfo));
    clock_gettime(CLOCK_REALTIME, &time);  //获取相对于1970到现在的秒数
    localtime_r(&time.tv_sec, &nowTime);
    sprintf(timeinfo, "%04d%02d%02d%02d%02d%02d", nowTime.tm_year + 1900, nowTime.tm_mon + 1, nowTime.tm_mday, 
    nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
    return timeinfo;
}


BV_TIME_HANDLE bv_time_ms_start(void)
{
    pstTimeInfo pstTime;
    struct timespec time;
    pstTime = (pstTimeInfo)malloc(sizeof(stTimeInfo));
    if (pstTime == NULL)
    {
        return 0;
    }
    clock_gettime(CLOCK_REALTIME, &time);
    pstTime->uiStartTime = time.tv_sec * 1000 + time.tv_nsec / (1000*1000);
    return (BV_TIME_HANDLE)pstTime;
}

unsigned int bv_time_ms_end(BV_TIME_HANDLE timeHandle)
{
    pstTimeInfo pstTime;
    unsigned int timeInterval;
    struct timespec time;
    pstTime = (pstTimeInfo)timeHandle;
    if (pstTime == NULL)
    {
        return 0;
    }
    clock_gettime(CLOCK_REALTIME, &time);
    pstTime->uiEndtime = time.tv_sec * 1000 + time.tv_nsec / (1000*1000);
    timeInterval = (pstTime->uiEndtime - pstTime->uiStartTime);
    free(pstTime);
    return timeInterval;
}

BV_TIME_HANDLE bv_time_us_start(void)
{
    pstTimeInfo pstTime;
    struct timespec time;
    pstTime = (pstTimeInfo)malloc(sizeof(stTimeInfo));
    if (pstTime == NULL)
    {
        return 0;
    }
    clock_gettime(CLOCK_REALTIME, &time);
    pstTime->uiStartTime = time.tv_sec * 1000 * 1000 + time.tv_nsec / 1000;
    return (BV_TIME_HANDLE)pstTime;
}

unsigned int bv_time_us_end(BV_TIME_HANDLE timeHandle)
{
    pstTimeInfo pstTime;
    unsigned int timeInterval;
    struct timespec time;
    pstTime = (pstTimeInfo)timeHandle;
    if (pstTime == NULL)
    {
        return 0;
    }
    clock_gettime(CLOCK_REALTIME, &time);
    pstTime->uiEndtime = time.tv_sec * 1000 * 1000 + time.tv_nsec / 1000;
    timeInterval = (pstTime->uiEndtime - pstTime->uiStartTime);
    free(pstTime);
    return timeInterval;
}

BV_TIME_HANDLE bv_time_ns_start(void)
{
    pstTimeInfo pstTime;
    struct timespec time;
    pstTime = (pstTimeInfo)malloc(sizeof(stTimeInfo));
    if (pstTime == NULL)
    {
        return 0;
    }
    clock_gettime(CLOCK_REALTIME, &time);
    pstTime->uiStartTime = time.tv_sec * 1000 * 1000 * 1000 + time.tv_nsec;
    return (BV_TIME_HANDLE)pstTime;
}

unsigned int bv_time_ns_end(BV_TIME_HANDLE timeHandle)
{
    pstTimeInfo pstTime;
    unsigned int timeInterval;
    struct timespec time;
    pstTime = (pstTimeInfo)timeHandle;
    if (pstTime == NULL)
    {
        return 0;
    }
    clock_gettime(CLOCK_REALTIME, &time);
    pstTime->uiEndtime = time.tv_sec * 1000 * 1000 * 1000 + time.tv_nsec;
    timeInterval = (pstTime->uiEndtime - pstTime->uiStartTime);
    free(pstTime);
    return timeInterval;
}




