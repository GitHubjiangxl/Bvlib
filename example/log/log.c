#include <bv.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char buff[1024];
    BV_RETURN ret;
    BV_LOG_HANDLE logHandle;
    BV_LOG_HANDLE logHandle1;
    memset(buff, 'a', sizeof(buff));
    //buff[1023] = '\0';
    logHandle = bv_log_create("./log.txt");
    logHandle1 = bv_log_create("./tmp.txt");
    if (logHandle != 0)
    {
        bv_log(logHandle, BV_LOG_LEVEL_FATAL, buff);
        bv_log_fatal(logHandle, "%s", buff);
        #if 0
        bv_log_set_level(logHandle, BV_LOG_LEVEL_ALL);
        ret = bv_log(logHandle, BV_LOG_LEVEL_FATAL, buff);
        printf("ret=%d\n", ret);
        ret = bv_log(logHandle1, BV_LOG_LEVEL_ERROR, buff);
        printf("ret=%d\n", ret);
        bv_log(logHandle, BV_LOG_LEVEL_WARN, "test");
        bv_log(logHandle, BV_LOG_LEVEL_INFO, "file");
        bv_log(logHandle, BV_LOG_LEVEL_DEBUG, "know");
        bv_log(logHandle, BV_LOG_LEVEL_TRACE, "?");
        #endif
    }
    else
    {
        printf("logHandle=%d\n", logHandle);
    }


    sleep(3);
    ret = bv_log_close(logHandle);
    printf("close handle ret=%d\n", ret);
    ret = bv_log_close(logHandle1);
    printf("close handle1 ret=%d\n", ret);
    return 0;
}
