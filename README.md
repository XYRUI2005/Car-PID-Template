# STM32 智能小车 PID 控制模板

基于 **STM32F103RCT6** 的两轮差速智能小车控制工程模板。工程集成编码器测速、电机 PWM 驱动、位置/速度串级 PID、原地转向、八路巡线、MPU6050 姿态解算、OLED 菜单与串口 PID 调参协议，可作为智能小车基础控制程序或竞赛项目的二次开发起点。

> 本仓库是面向特定硬件参数的控制模板。首次上电或修改 PID 前，请将车轮悬空并确认电机方向、编码器方向与 PWM 极性，避免小车失控。

## 功能概览

- 双路直流减速电机 PWM 驱动，支持正反转与输出限幅
- 双路正交编码器采集与脉冲累计
- 20 ms 定时控制周期下的闭环控制框架
- 双轮**位置—速度串级 PID**
- 基于 MPU6050 DMP `yaw` 的原地转向**角度—速度串级 PID**
- 八路红外/灰度传感器巡线，使用差速控制修正方向
- 基于姿态角的直行方向修正控制接口
- OLED 本地菜单，可查看电机、巡线传感器与 MPU6050 数据
- USART1 串口通信及“野火 PID 调试助手”兼容协议
- STM32CubeMX 配置文件与 Keil MDK-ARM 工程

## 硬件与软件环境

| 项目 | 说明 |
| --- | --- |
| MCU | STM32F103RCT6，Cortex-M3 |
| 系统时钟 | HSE 8 MHz 经 PLL 配置为 72 MHz |
| 开发工具 | STM32CubeMX 6.12.0、Keil MDK-ARM 5.32+（工程使用 ARMCC 5） |
| HAL 库 | STM32Cube FW_F1 V1.8.6 |
| 电机 | 两路带 AB 相编码器的直流减速电机 |
| 姿态传感器 | MPU6050，支持 DMP 姿态解算 |
| 巡线模块 | 8 路红外/灰度传感器 |
| 显示与交互 | I²C OLED、3 个按键、RGB LED |
| 调试接口 | SWD、USART1 |

## 引脚与外设分配

| 功能 | 外设/引脚 | 说明 |
| --- | --- | --- |
| 左电机驱动 | TIM2 CH1 / PA15（IN1）、TIM2 CH2 / PB3（IN2） | PWM 正反转控制 |
| 右电机驱动 | TIM3 CH1 / PA6（IN3）、TIM3 CH2 / PA7（IN4） | PWM 正反转控制 |
| 左编码器 | TIM4 CH1 / PB6（A1）、TIM4 CH2 / PB7（B1） | 正交编码器接口 |
| 右编码器 | TIM5 CH1 / PA0（A2）、TIM5 CH2 / PA1（B2） | 正交编码器接口 |
| 控制定时器 | TIM6 | 20 ms 更新中断（50 Hz） |
| MPU6050 | I²C1：PB8（SCL）、PB9（SDA） | 硬件 I²C |
| MPU6050 DMP 总线 | PA11（SCL）、PA12（SDA） | 软件 I²C 引脚，供 DMP 驱动使用 |
| OLED | PC4（SCL）、PC5（SDA） | 软件 I²C |
| 串口调试 | USART1：PA9（TX）、PA10（RX） | PID 调参、数据通信 |
| 八路巡线 | PC6–PC12、PD2 | HW1–HW8 |
| 按键 | PC13、PC14、PC15 | KEY1–KEY3 |
| RGB LED | PC1、PC2、PC3 | R/G/B LED |
| 下载调试 | PA13（SWDIO）、PA14（SWCLK） | SWD |

引脚定义与 CubeMX 外设参数可在 [2024H_SELF_DRIVING_CAR.ioc](2024H_SELF_DRIVING_CAR.ioc) 中查看。

## 项目结构

```text
Car_PID_Template/
├── 2024H_SELF_DRIVING_CAR.ioc     # STM32CubeMX 工程配置
├── Core/
│   ├── Inc/                       # HAL 初始化头文件
│   └── Src/main.c                 # 程序入口与外设启动
├── HardWare/
│   ├── control.c / control.h      # 小车动作与各类串级控制
│   ├── pid.c / pid.h              # PID 数据结构、参数与计算函数
│   ├── motor.c / motor.h          # 电机 PWM、方向和使能控制
│   ├── encoder.c / encoder.h      # 编码器脉冲读取与累计
│   ├── line.c / line.h            # 八路巡线传感器处理
│   ├── mpu6050.c / dmp.c          # MPU6050 与 DMP 姿态解算
│   ├── protocol.c / protocol.h    # 串口 PID 调试协议
│   ├── menu.c / menu.h            # OLED 菜单与状态显示
│   └── oled.c、key.c、led.h 等    # 外设驱动
├── Drivers/                        # STM32 HAL 与 CMSIS
└── MDK-ARM/
    └── 2024H_SELF_DRIVING_CAR.uvprojx  # Keil 工程文件
```

## 控制架构

```text
目标距离 / 目标航向 / 巡线偏差
              │
              ▼
   位置环 / 角度环 / 巡线环（外环）
              │  输出目标转速或左右差速量
              ▼
       左右电机速度 PID（内环）
              │
              ▼
       PWM 输出 ──> 电机驱动 ──> 车轮
              ▲                       │
              └──── 编码器 / MPU6050 / 巡线传感器 ────┘
```

控制计算由 TIM6 的 20 ms 回调触发。编码器计数在该回调中读取，速度环按 50 Hz 运行；位置、转向角度与巡线外环每两个周期运行一次，频率为 25 Hz。

## PID 实现说明

PID 状态使用 `_pid` 结构体保存目标值、实际值、当前/上一时刻误差、积分项与 `Kp`、`Ki`、`Kd` 参数，定义位于 [HardWare/pid.h](HardWare/pid.h)。

| 控制对象 | PID 实例 | 实际反馈 | 输出用途 |
| --- | --- | --- | --- |
| 左/右轮速度 | `pid_speed1` / `pid_speed2` | 单个控制周期内的编码器脉冲换算转速 | PWM 控制量 |
| 左/右轮位置 | `pid_location1` / `pid_location2` | 累计编码器脉冲 | 速度环目标值 |
| 原地转向角度 | `pid_turn_angle` | MPU6050 DMP 的 `yaw` | 左右轮反向目标转速 |
| 巡线 | `pid_line` | `Line_Num` | 左右轮目标转速差 |
| 航向保持 | `pid_angle` | MPU6050 DMP 的 `yaw` | 左右轮目标转速差 |

### 当前默认 PID 参数

下表为 [HardWare/pid.c](HardWare/pid.c) 中的初始值，仅可作为起点；不同电机、负载、电池电压及地面摩擦力均需要重新整定。

| 环路 | Kp | Ki | Kd |
| --- | ---: | ---: | ---: |
| 左/右位置环 | 0.25 | 0 | 0 |
| 左/右速度环 | 1.94 | 0.79 | 1.18 |
| 转向角度环 | 1.10 | 0 | 0 |
| 巡线环 | 2.00 | 0.80 | 1.00 |
| 航向保持环 | 1.10 | 0 | 0 |

速度 PID 带 ±0.5 的误差死区以及 ±1000 的积分限幅；巡线 PID 带 ±20 的积分限幅。参数和限幅值均应结合实际车辆调试结果调整。

## 车辆物理参数

以下参数位于 [HardWare/control.h](HardWare/control.h)，必须依据实际车辆校准：

| 宏 | 默认值 | 含义 |
| --- | ---: | --- |
| `ENCODER_RESOLUTION` | 13 | 编码器线数 |
| `ENCODER_TOTAL_RESOLUTION` | 52 | 四倍频后的编码器分辨率 |
| `REDUCTION_RATIO` | 20 | 减速比 |
| `SPEED_PID_PERIOD` | 20 ms | 速度 PID 周期，应与 TIM6 保持一致 |
| `TARGET_SPEED_MAX` | 120 rpm | 速度环目标上限 |
| `WHEEL_D` | 4.6 cm | 车轮直径 |
| `LUN_JU` | 13 cm | 两轮中心距 |
| `HEAD_TO_WHEEL` | 15 cm | 车轮轴到车头距离 |

## 编译与下载

1. 安装 Keil MDK-ARM，并安装 `Keil.STM32F1xx_DFP` 设备包。
2. 使用 Keil 打开 [MDK-ARM/2024H_SELF_DRIVING_CAR.uvprojx](MDK-ARM/2024H_SELF_DRIVING_CAR.uvprojx)。
3. 根据自己的驱动板、传感器接线和车辆尺寸修改引脚、物理参数与 PID 初始值。
4. 执行 **Rebuild** 编译工程。成功后会生成 HEX 文件：
   [MDK-ARM/2024H_SELF_DRIVING_CAR/2024H_SELF_DRIVING_CAR.hex](MDK-ARM/2024H_SELF_DRIVING_CAR/2024H_SELF_DRIVING_CAR.hex)。
5. 使用 ST-Link 通过 SWD 下载，或在 Keil 中配置对应的下载器后直接 Download。

如需在 STM32CubeMX 中调整外设，请打开 [2024H_SELF_DRIVING_CAR.ioc](2024H_SELF_DRIVING_CAR.ioc) 并重新生成代码。重新生成前请确认用户代码位于 `USER CODE BEGIN/END` 区块或已完成备份。

## 启用控制模式

为避免程序上电后直接驱动车辆，TIM6 回调函数中各 PID 调用示例默认处于注释状态。根据测试目标，在 [HardWare/control.c](HardWare/control.c) 的 `HAL_TIM_PeriodElapsedCallback()` 中**一次仅取消一组控制逻辑的注释**：

| 控制模式 | 调用函数 | 用途 |
| --- | --- | --- |
| 速度闭环 | `Speed1_PID_Control()`、`Speed2_PID_Control()` | 单独整定左右电机速度环 |
| 位置—速度串级 | `Location_Speed_Control()` | 直行距离和原地转向的基础控制 |
| 转向角度—速度串级 | `Turn_Angle_Speed_Control()` | 基于 `yaw` 的原地角度转向调试 |
| 巡线—速度串级 | `Line_Speed_Control()` | 八路巡线差速控制调试 |
| 航向—速度串级 | `Angle_Speed_Control()` | 基于 `yaw` 的直线航向保持调试 |

取消相应代码后，确认回调中的 `Motor_Output()` 被保留，以便将计算得到的 PWM 装载到电机驱动。

## 常用控制接口

### 指定距离直行

`Car_Go(distance_cm)` 会清零两轮累计脉冲，以厘米为单位将目标距离转换为位置环目标，并使能巡线标志位。

```c
Car_Go(100);  // 直行 100 cm
```

### 原地转向

`Car_Spin()` 支持左转 90°、右转 90°和掉头 180°。函数根据轮距和轮径将目标转角转换为两轮相反的位置环脉冲目标。

```c
Car_Spin(left_90);
Car_Spin(right_90);
Car_Spin(back_180);
```

这些接口只负责设定目标与状态标志；需要同时启用对应的串级控制模式，电机才会按目标运行。

## 推荐调参流程

1. **验证硬件方向**：车轮悬空，分别测试正负 PWM 输出，确认电机正方向与编码器正计数方向一致；若不一致，优先修正接线或在软件中统一极性。
2. **确认编码器与速度计算**：检查 OLED 的 Motor 页面，验证静止时脉冲接近 0、转动时转速方向及数值合理。
3. **整定速度内环**：暂时仅启用速度 PID，先增大 `Kp` 获得响应，再加入少量 `Ki` 消除稳态误差，最后按需加入 `Kd` 抑制振荡。
4. **整定位置外环**：启用位置—速度串级，通常从仅 `Kp` 开始；通过 `TARGET_SPEED_MAX` 限制外环输出，防止起步过猛。
5. **整定转向与巡线环**：先保证 MPU6050 的 `yaw` 和 `Line_Num` 方向正确，再从较小的 `Kp` 开始调节差速量。
6. **实车复测**：在不同电池电压、载荷与路面条件下验证，必要时降低目标速度或重新标定车轮直径、轮距和减速比。

> 速度环与位置环的参数不能直接照搬到另一套电机或底盘。每次变更轮胎、减速比、编码器倍频方式或控制周期后，都应重新核对换算公式与 PID 参数。

## 调试与显示

- **OLED 菜单**：可查看陀螺仪、加速度计、欧拉角、电机编码器/转速与八路巡线电平。
- **串口 PID 调参**：默认启用 `PID_ASSISTANT_EN`，协议定义在 [HardWare/protocol.h](HardWare/protocol.h)，支持设置 PID 参数、目标值、控制周期以及启动、停止和复位命令。
- **姿态初始化**：主程序会循环等待 `mpu_dmp_init()` 成功并执行 `MPU6050_calibrate()`；请在车辆静止、水平时上电完成初始化。

## 注意事项

- 电机电源应与 MCU 电源合理隔离或做好滤波，并确保所有模块共地。
- PWM 最大比较值会保留一定余量，避免满占空比导致部分驱动板工作异常。
- 当前工程中的定时器中断优先级较高；不要在中断回调内加入耗时的串口打印、延时或阻塞操作。
- 巡线传感器高低电平、左右轮安装方向和 MPU6050 `yaw` 正方向可能与本模板不同，移植时必须逐项验证。
- 生成的 `MDK-ARM/2024H_SELF_DRIVING_CAR/` 目录包含编译产物；提交二次开发代码时通常应按团队规范决定是否纳入版本控制。

## 许可证

仓库当前未提供独立许可证文件。使用、修改或分发前，请与项目维护者确认授权范围。
