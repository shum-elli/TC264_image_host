/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            main
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ3184284598)
 * @version         查看doc内version文件 版本说明
 * @Software        ADS v1.2.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-3-23
 ********************************************************************************************************************/


#include "headfile.h"
#pragma section all "cpu0_dsram"
//将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height;
    uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
        }
    }

    //计算每个像素在整幅图像中的比例
    float maxPro = 0.0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        if (pixelPro[i] > maxPro)
        {
            maxPro = pixelPro[i];
        }
    }

    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    for (i = 0; i < GrayScale; i++)     // i作为阈值
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
            if (j <= i)   //背景部分
            {
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }
            else   //前景部分
            {
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }
        u0 = u0tmp / w0;
        u1 = u1tmp / w1;
        u = u0tmp + u1tmp;
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = (uint8)i;
        }
    }

    return threshold;
}


uint8 image_threshold;  //图像阈值
uint32 use_time;

//工程导入到软件之后，应该选中工程然后点击refresh刷新一下之后再编译
//工程默认设置为关闭优化，可以自己右击工程选择properties->C/C++ Build->Setting
//然后在右侧的窗口中找到C/C++ Compiler->Optimization->Optimization level处设置优化等级
//一般默认新建立的工程都会默认开2级优化，因此大家也可以设置为2级优化

//对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用enableInterrupts();来开启中断嵌套
//简单点说实际上进入中断后TC系列的硬件自动调用了disableInterrupts();来拒绝响应任何的中断，因此需要我们自己手动调用enableInterrupts();来开启中断的响应。
int core0_main(void)
{
    int i,j,k;
    int temp;
    uint8 data;
    uint8 *p;

    get_clk();//获取时钟频率  务必保留

    //目前的库采集总钻风 图像最后一列为固定的黑色
    //这是由于单片机造成的，不是摄像头的问题

    ips200_init();  //初始化IPS屏幕
    ips200_showstr(0, 0, "SEEKFREE MT9V03x");
    ips200_showstr(0, 1, "Initializing...");

    uart_init(UART_0, 115200, UART0_TX_P14_0, UART0_RX_P14_1);
    mt9v03x_init(); //初始化摄像头
    //如果屏幕一直显示初始化信息，请检查摄像头接线
    //如果使用主板，一直卡在while(!uart_receive_flag)，请检查是否电池连接OK?或者摄像头的配置串口与单片机连接是否正确
    //如果图像只采集一次，请检查场信号(VSY)是否连接OK?
	IfxCpu_emitEvent(&g_cpuSyncEvent);
	IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
    enableInterrupts();

    //注意 从V1.1.6版本之后  printf打印的信息从串口输出具体可以学习库例程6-Printf_Demo
    //注意 从V1.1.6版本之后  printf打印的信息从串口输出具体可以学习库例程6-Printf_Demo
    //注意 从V1.1.6版本之后  printf打印的信息从串口输出具体可以学习库例程6-Printf_Demo
    while (TRUE)
    {
        if(mt9v03x_finish_flag)
        {
            systick_start(STM1);
            //image_threshold = otsuThreshold(mt9v03x_image[0],MT9V03X_W,MT9V03X_H);  //大津法计算阈值
            //use_time = systick_getval_us(STM1);                        //计算大津法程序消耗时间，单位微秒。
            //printf("use_time: %ld\n", use_time);
            image_threshold = 140;

            //systick_delay_ms(STM0, 30);

            //发送二值化图像至上位机
            p = mt9v03x_image[0];
            //uart_putchar(UART_0,0x0f);
//            for(i=0; i<MT9V03X_W*MT9V03X_H; i++)
//            {
//                 if(p[i]>image_threshold)    uart_putchar(UART_0,0xff);
//                 else                        uart_putchar(UART_0,0x00);
//            }
            temp=0;
            //p[0]=255;
            uart_putchar(UART_0,'m');
            for(i=0; i<MT9V03X_H*MT9V03X_W/8; i++)
            {

                data=0x00;
                for(k=0;k<8;k++){
               if(p[temp++]>image_threshold){
                   data |= 1 << (7-k);}
               }
                uart_putchar(UART_0,data);




//                 if(p[i]>image_threshold)    uart_putchar(UART_0,0x00);
//
//                 else                        uart_putchar(UART_0,0x00);
            }


            //uart_putchar(UART_0,0x02);
            ips200_displayimage032(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
            //seekfree_sendimg_03x(UART_0, mt9v03x_image[0], sMT9V03X_W, MT9V03X_H);

            mt9v03x_finish_flag = 0;//在图像使用完毕后  务必清除标志位，否则不会开始采集下一幅图像
            //注意：一定要在图像使用完毕后在清除此标志位

        }

    }
}

#pragma section all restore


