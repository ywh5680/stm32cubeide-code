# 设计文档

## 概述

本文档描述了智能小车高速寻迹系统的详细设计。该系统采用传感器融合技术，结合8路灰度传感器阵列和MPU6050惯性测量单元，实现高速稳定的轨迹跟踪。系统使用状态机管理不同的跟踪场景（直线、弯道、十字路口、丢线），并通过蓝牙串口提供实时参数调整功能。

核心设计理念：
- **传感器融合**：灰度传感器提供位置信息，陀螺仪提供姿态稳定性
- **自适应控制**：根据赛道状况动态调整PID参数和速度
- **状态驱动**：使用状态机清晰管理不同场景的控制策略
- **实时可调**：通过蓝牙串口实现运行时参数调整

## 架构

### 系统架构图

```
┌─────────────────────────────────────────────────────────────┐
│                      主控制循环 (100Hz+)                      │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      传感器数据采集层                         │
├─────────────────────────────────────────────────────────────┤
│  ┌──────────────────┐         ┌──────────────────┐         │
│  │ 灰度传感器阵列    │         │   MPU6050 IMU    │         │
│  │  - 8路ADC读取    │         │  - 陀螺仪Z轴     │         │
│  │  - 加权位置计算  │         │  - 加速度计Y轴   │         │
│  └──────────────────┘         └──────────────────┘         │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      状态识别与管理层                         │
├─────────────────────────────────────────────────────────────┤
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐   │
│  │  直线段  │  │  弯道段  │  │ 十字路口 │  │  丢线    │   │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘   │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      控制算法层                              │
├─────────────────────────────────────────────────────────────┤
│  ┌──────────────────┐         ┌──────────────────┐         │
│  │   PID控制器      │         │  陀螺仪阻尼器    │         │
│  │  - 位置误差输入  │         │  - 角速度输入    │         │
│  │  - 转向输出      │         │  - 阻尼输出      │         │
│  └──────────────────┘         └──────────────────┘         │
│                    │                   │                     │
│                    └─────────┬─────────┘                     │
│                              ▼                               │
│                    ┌──────────────────┐                      │
│                    │  速度控制器      │                      │
│                    │  - 动态速度计算  │                      │
│                    └──────────────────┘                      │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      电机驱动层                              │
├─────────────────────────────────────────────────────────────┤
│  ┌──────────────────┐         ┌──────────────────┐         │
│  │   左电机PWM      │         │   右电机PWM      │         │
│  └──────────────────┘         └──────────────────┘         │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                      蓝牙串口调参模块                         │
├─────────────────────────────────────────────────────────────┤
│  - 命令解析                                                  │
│  - 参数更新                                                  │
│  - 状态查询                                                  │
└─────────────────────────────────────────────────────────────┘
```

### 模块职责

1. **传感器数据采集层**
   - 读取8路灰度传感器ADC值
   - 计算加权位置误差
   - 读取MPU6050陀螺仪和加速度计数据
   - 提供滤波后的传感器数据

2. **状态识别与管理层**
   - 根据传感器数据识别当前赛道状况
   - 管理状态转换
   - 为不同状态选择合适的控制参数

3. **控制算法层**
   - PID控制器：基于位置误差计算转向修正
   - 陀螺仪阻尼器：基于角速度抑制震荡
   - 速度控制器：动态调整前进速度

4. **电机驱动层**
   - 将控制信号转换为PWM输出
   - 差速控制实现转向

5. **蓝牙串口调参模块**
   - 解析调参命令
   - 实时更新系统参数
   - 返回参数查询结果

## 组件和接口

### 1. 灰度传感器模块 (GraySensor)

**现有驱动接口（Core/BSP/GRAY/gray.h）：**
```c
void GRAY_Init(void);                    // 初始化灰度传感器GPIO
uint8_t GRAY_Read(GRAY_TypeDef sensor);  // 读取单个传感器值 (0或1)
void GRAY_ReadAll(uint8_t *values);      // 读取所有8个传感器值到数组
uint8_t GRAY_ReadByte(void);             // 读取所有传感器值并组合成一个字节
```

**需要在hrun模块中实现的高层接口：**
```c
// 计算加权位置误差 (返回值范围: -3.5 到 +3.5)
float HRun_GetPosition(uint8_t gray_values[8]);

// 检测是否丢线
bool HRun_IsLineLost(uint8_t gray_values[8]);

// 检测是否为十字路口
bool HRun_IsCrossroad(uint8_t gray_values[8]);

// 获取检测到黑线的传感器数量
uint8_t HRun_GetActiveCount(uint8_t gray_values[8]);
```

**实现细节：**
- 灰度传感器返回数字值：0表示检测到黑线，1表示白色背景
- 加权位置算法：`position = Σ(i * weight[i]) / Σ(weight[i])`，其中i为传感器索引(-3.5到+3.5)
- 权重计算：检测到黑线的传感器权重为1，否则为0
- 十字路口判断：至少5个连续传感器检测到黑线（值为0）

### 2. MPU6050模块

**现有驱动接口（Core/BSP/MPU6050/MPU6050.h）：**
```c
void MPU6050_Init(void);  // 初始化MPU6050
void IMU_GetData(float *AccX, float *AccY, float *AccZ, 
                 float *GyroX, float *GyroY, float *GyroZ);  // 获取所有轴数据
float MPU6050_GetYawRate(void);  // 获取Z轴角速度
```

**使用方式：**
- 直接调用`MPU6050_GetYawRate()`获取Z轴角速度用于陀螺仪阻尼
- 调用`IMU_GetData()`获取加速度计Y轴值用于侧向加速度监测

**实现细节：**
- 使用软件I2C通信读取MPU6050数据
- 陀螺仪Z轴数据用于阻尼控制（单位：deg/s）
- 加速度计Y轴用于侧向加速度监测（单位：g）

### 3. PID控制器模块

**接口：**
```c
typedef struct {
    float Kp;           // 比例系数
    float Ki;           // 积分系数
    float Kd;           // 微分系数
    float integral;     // 积分累积
    float last_error;   // 上次误差
    float integral_max; // 积分限幅
} PID_Controller;

// 初始化PID控制器
void PID_Init(PID_Controller* pid, float kp, float ki, float kd);

// 计算PID输出
float PID_Calculate(PID_Controller* pid, float error, float dt);

// 重置PID控制器
void PID_Reset(PID_Controller* pid);
```

**实现细节：**
- 标准PID算法：`output = Kp*error + Ki*integral + Kd*derivative`
- 积分限幅防止积分饱和
- 微分项使用误差变化率

### 4. 陀螺仪阻尼模块

**接口：**
```c
// 计算陀螺仪阻尼输出
float GyroDamping_Calculate(float gyro_z, float damping_coeff);
```

**实现细节：**
- 简单的比例控制：`damping = -Kg * gyro_z`
- 负反馈抑制角速度
- 不同状态使用不同的Kg系数

### 5. 状态机模块

**接口：**
```c
typedef enum {
    STATE_STRAIGHT,    // 直线段
    STATE_TURN,        // 弯道段
    STATE_CROSSROAD,   // 十字路口
    STATE_LINE_LOST    // 丢线
} TrackingState;

typedef struct {
    TrackingState current_state;
    TrackingState last_state;
    uint32_t state_enter_time;
    float last_position;
} StateMachine;

// 初始化状态机
void StateMachine_Init(StateMachine* sm);

// 更新状态
void StateMachine_Update(StateMachine* sm, float position, bool is_line_lost, bool is_crossroad);

// 获取当前状态
TrackingState StateMachine_GetState(StateMachine* sm);
```

**状态转换图：**
```
                    ┌──────────────┐
                    │   STRAIGHT   │
                    │   (直线段)    │
                    └──────────────┘
                      │    ▲    │
         十字路口检测  │    │    │ 大误差持续
                      ▼    │    ▼
    ┌──────────────┐      │      ┌──────────────┐
    │  CROSSROAD   │      │      │     TURN     │
    │  (十字路口)   │      │      │   (弯道段)    │
    └──────────────┘      │      └──────────────┘
                      │    │    │
                      │    │    │ 误差恢复
                      │    └────┘
                      │
                      │ 丢线检测
                      ▼
                ┌──────────────┐
                │  LINE_LOST   │
                │   (丢线)      │
                └──────────────┘
                      │
                      │ 找回线
                      ▼
                  (返回上一状态)
```

### 6. 速度控制模块

**接口：**
```c
typedef struct {
    float straight_speed;  // 直线速度
    float turn_speed;      // 弯道速度
    float search_speed;    // 搜索速度
    float min_speed;       // 最小速度
    float max_speed;       // 最大速度
    float speed_k;         // 速度调整系数
} SpeedController;

// 初始化速度控制器
void SpeedController_Init(SpeedController* sc);

// 计算目标速度
float SpeedController_Calculate(SpeedController* sc, TrackingState state, float error);
```

**实现细节：**
- 基础速度根据状态选择
- 动态调整：`speed = base_speed - k * |error|`
- 速度限幅在min_speed和max_speed之间

### 7. 电机控制模块

**现有驱动接口（Core/BSP/M/motor.h）：**
```c
void Motor_Init(void);  // 电机驱动初始化
void Motor_A_SetSpeed(int16_t speed);  // 设置A电机速度 (0-1000)
void Motor_B_SetSpeed(int16_t speed);  // 设置B电机速度 (0-1000)
void Car_Move(int16_t left_speed, int16_t right_speed);  // 直接控制左右轮速度
```

**需要在hrun模块中实现的高层接口：**
```c
// 设置小车速度和转向 (speed: 0-100, steering: -100到+100)
void HRun_SetMotor(float speed, float steering);
```

**实现细节：**
- 差速控制算法：
  - `left_speed = speed - steering`
  - `right_speed = speed + steering`
- 将0-100的速度映射到0-1000的PWM范围
- 调用`Car_Move()`控制左右电机

### 8. 蓝牙串口调参模块

**现有驱动接口（Core/BSP/UART/usart.h）：**
```c
void usart_init(UART_HandleTypeDef *huart);  // 初始化串口
void usart_send_string(const char *str);     // 发送字符串
uint8_t usart_rx_complete(void);             // 检查是否接收完成
void usart_clear_rx_buffer(void);            // 清空接收缓冲区
extern uint8_t USART_RX_BUF[USART_REC_LEN];  // 接收缓冲区
```

**需要在hrun模块中实现的高层接口：**
```c
// 处理接收到的命令
void HRun_ProcessCommand(char* cmd);

// 发送响应消息
void HRun_SendResponse(const char* msg);
```

**命令协议：**
```
SET <参数名> <值>    - 设置参数
GET <参数名>         - 查询单个参数
GET ALL             - 查询所有参数
```

**支持的参数：**
- PID_KP, PID_KI, PID_KD
- GYRO_KG_STRAIGHT, GYRO_KG_TURN
- SPEED_STRAIGHT, SPEED_TURN, SPEED_MIN, SPEED_MAX, SPEED_K
- THRESHOLD_TURN, THRESHOLD_LOST, THRESHOLD_CROSS
- ACCEL_LIMIT

## 数据模型

### 系统配置参数结构

```c
typedef struct {
    // PID参数
    float pid_kp;
    float pid_ki;
    float pid_kd;
    
    // 陀螺仪阻尼系数
    float gyro_kg_straight;  // 直线段阻尼系数
    float gyro_kg_turn;      // 弯道段阻尼系数
    
    // 速度参数
    float speed_straight;    // 直线速度 (0-100)
    float speed_turn;        // 弯道速度 (0-100)
    float speed_search;      // 搜索速度 (0-100)
    float speed_min;         // 最小速度
    float speed_max;         // 最大速度
    float speed_k;           // 速度调整系数
    
    // 阈值参数
    float threshold_turn;    // 弯道检测阈值
    float threshold_lost;    // 丢线检测阈值
    uint8_t threshold_cross; // 十字路口检测阈值(传感器数量)
    float accel_limit;       // 侧向加速度限制
    
    // 灰度传感器阈值
    uint16_t gray_threshold; // 黑线检测阈值
    
    // 超时参数
    uint32_t lost_timeout_ms; // 丢线超时时间(ms)
    
} SystemConfig;
```

### 运行时状态数据

```c
typedef struct {
    // 传感器数据
    uint16_t gray_values[8];
    float position;
    float gyro_z;
    float accel_y;
    
    // 控制数据
    float pid_output;
    float damping_output;
    float steering_output;
    float target_speed;
    
    // 状态数据
    TrackingState current_state;
    bool is_line_lost;
    bool is_crossroad;
    uint32_t lost_start_time;
    float last_valid_position;
    
    // PID控制器
    PID_Controller pid;
    
    // 统计数据
    uint32_t loop_count;
    uint32_t loop_time_us;
    
} RuntimeData;
```

## 正确性属性

*属性是一个特征或行为，应该在系统的所有有效执行中保持为真——本质上是关于系统应该做什么的形式化陈述。属性作为人类可读规范和机器可验证正确性保证之间的桥梁。*

