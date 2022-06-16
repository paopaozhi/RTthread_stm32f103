#include "main.h"
#include "rtthread.h"
#include <stdio.h>

static struct rt_thread thread1;
rt_thread_t thread2_ptr;
static rt_uint8_t thread1_stack[512];

void thread1_entry(void *parameter);

void thread2_entry(void *parameter);

int main(void) {
    rt_err_t result;

    /* 初始化线程 1 */
    /* 线程的入口是 thread1_entry，参数是 RT_NULL
     * 线程栈是 thread1_stack
     * 优先级是 200，时间片是 10 个 OS Tick
     */
    result = rt_thread_init(&thread1,
                            "thread1",
                            thread1_entry, RT_NULL,
                            &thread1_stack[0], sizeof(thread1_stack),
                            30, 10);

    /* 启动线程 */
    if (result == RT_EOK) rt_thread_startup(&thread1);

    /* 创建线程 2 */
    /* 线程的入口是 thread2_entry, 参数是 RT_NULL
     * 栈空间是 512，优先级是 250，时间片是 25 个 OS Tick
     */
    thread2_ptr = rt_thread_create("thread2",
                                   thread2_entry, RT_NULL,
                                   128, 29, 25);

    /* 启动线程 */
    if (thread2_ptr != RT_NULL) rt_thread_startup(thread2_ptr);

    while (1) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
        rt_thread_delay(500);
    }
}

void thread1_entry(void *parameter) {
    int i;

    while (1) {
        for (i = 0; i < 10; i++) {
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
            rt_kprintf("thread1_entry_线程\rii=%d\r", i);
            /* 延时 100ms */
            rt_thread_mdelay(1000);
        }
        return;
    }
}

/* 线程 2 入口 */
void thread2_entry(void *parameter) {
    int count = 0;
    while (1) {
        rt_kprintf("count=%d\r", count);
        /* 延时 50ms */
        rt_thread_mdelay(500);
    }
}
/* USER CODE END 0 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
