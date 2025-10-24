# 基于51单片机的智能台灯系统

<div align="center">

![51单片机](https://img.shields.io/badge/MCU-STC89C52-blue)
![语言](https://img.shields.io/badge/语言-C51-green)
![许可证](https://img.shields.io/badge/许可证-MIT-yellow)
![状态](https://img.shields.io/badge/状态-稳定运行-brightgreen)

**一个智能、节能、易用的51单片机台灯系统**

[项目介绍](#-项目介绍) • [功能特性](#-功能特性) • [快速开始](#-快速开始) • [项目结构](#-项目结构) • [学习价值](#-学习价值)

</div>

## 项目介绍

本项目基于STC89C52单片机，实现了一个功能完整的智能台灯系统。通过环境光感知、人体检测模拟和智能调光算法，让传统台灯具备"智能化"特性。项目采用模块化设计，代码规范清晰，非常适合单片机学习者参考和实践。

> 📖 **完整开发历程**：查看 [设计过程.md](Docs/设计过程.md) 了解项目开发故事

## 功能特性

### 核心功能

- **智能调光** - 根据环境光线自动调节亮度（0-100% PWM输出）
- **人体检测** - 模拟"人来灯亮，人走灯灭"的智能控制
- **双模式切换** - 手动/自动模式自由选择，满足不同使用场景
- **实时显示** - 4位数码管实时显示环境光强度

### 手动模式

- 5档精确调光（关闭/25%/50%/75%/100%）
- 独立按键控制，操作简单直观
- 双LED模式状态指示

### 自动模式

- 实时环境光监测（0-4095 ADC值）
- 智能亮度匹配算法
- 无人状态自动延时关闭（默认30秒）

## 硬件组成

| 模块       | 型号/规格          | 说明                      |
| ---------- | ------------------ | ------------------------- |
| 主控芯片   | STC89C52RC         | 8位单片机，11.0592MHz晶振 |
| 光线传感器 | 光敏电阻 + XPT2046 | 12位ADC，精确光强检测     |
| 显示模块   | 4位共阳极数码管    | 动态扫描显示              |
| 控制按键   | 4路独立按键        | 模式切换、亮度调节        |
| 指示灯     | 双LED              | 两个LED灯用于指示         |
| 主照明     | 高亮LED            | PWM调光输出               |

## 快速开始

### 环境要求
- **开发环境**：Keil μVision 5
- **烧录工具**：STC-ISP
- **单片机**：STC89C52RC
- **晶振频率**：11.0592MHz

### 编译与烧录
1. **打开工程**
   ```bash
   使用Keil打开 Project/project.uvproj
   ```

2. **编译项目**
   - 点击 `Build Target` 或按 `F7`
   - 在 `Objects/` 文件夹生成 HEX 文件

3. **烧录程序**
   - 使用 STC-ISP 选择生成的 HEX 文件
   - 设置正确的串口号和波特率
   - 点击"下载/编程"完成烧录

### 引脚连接
主要引脚定义（详见 [config.h](Common/config.h)）：
```c
sbit LED_MAIN = P2^0;  // 主灯控制
sbit LED1 = P2^6;      // 自动模式指示灯  
sbit LED2 = P2^7;      // 手动模式指示灯
// 更多引脚定义请查看配置文件
```

## 项目结构

```
51MCU-SmartLamp/
├── Core/                 # 核心业务逻辑
│   ├── main.c           # 主程序入口 & 状态机
│   └── main.h           # 系统数据类型定义
├── Driver/               # 硬件驱动层
│   ├── key/             # 按键状态机驱动（消抖算法）
│   ├── nixie/           # 数码管动态扫描驱动
│   ├── timer/           # 定时器配置（100μs中断）
│   └── xpt2046/         # ADC数据采集驱动
├── Common/               # 公共组件
│   ├── delay.c          # 精确延时函数
│   ├── delay.h          # 
│   └── config.h         # 系统参数 & 引脚定义
├── Docs/                 # 项目文档
│   ├── 技术说明.md       # PWM原理 & 实现细节
│   ├── 设计过程.md       # 开发历程 & 决策思考
│   └── 设计问题.md       # 问题解决记录
├── Project/              # Keil工程文件
└── README.md            # 项目说明文档
```

## 核心算法亮点

### PWM调光技术
```c
// 定时器中断中实现PWM输出
void timer0_ISR(void) interrupt 1 {
    systemData.pwmCount++;
    systemData.pwmCount %= 100;  // 100级精度
    
    if(systemData.pwmCount < systemData.lightGrade[systemData.lightState]) {
        LED_MAIN = 0;  // 点亮
    } else {
        LED_MAIN = 1;  // 熄灭
    }
}
```

### 环境光自适应算法
```c
// 根据光线强度智能匹配亮度
if(systemData.lightIntensity < LIGHT_LOWEST) {
    systemData.lightState = LAMP_HIGHEST;  // 光线暗 → 亮度高
} else if(systemData.lightIntensity < LIGHT_LOW) {
    systemData.lightState = LAMP_HIGH;
}
// 更多分级条件...
```

### 按键状态机消抖
采用四状态机确保按键响应可靠：
- `KEY_STATE_IDLE` → `KEY_STATE_DEBOUNCE_PRESS` → `KEY_STATE_PRESSED` → `KEY_STATE_DEBOUNCE_RELEASE`

## 学习价值

这个项目是学习51单片机的**绝佳实践案例**，涵盖：

### 硬件层面

- ✅ 定时器中断编程
- ✅ PWM调光技术实现
- ✅ ADC数据采集与处理
- ✅ 数码管动态扫描显示

### 软件层面

- ✅ 模块化编程思想
- ✅ 状态机设计模式
- ✅ 按键消抖算法
- ✅ 系统架构设计

### 适合人群

- 电子信息工程专业学生
- 单片机入门学习者
- 嵌入式开发爱好者
- 课程设计/毕业设计参考

## 性能参数

| 参数     | 数值  | 说明                 |
| -------- | ----- | -------------------- |
| PWM频率  | 100Hz | 无闪烁平滑调光       |
| 采样周期 | 100μs | 定时器中断间隔       |
| 消抖时间 | 20ms  | 机械按键去抖动       |
| 关闭延时 | 30s   | 可配置的人走灯灭延迟 |
| ADC精度  | 12位  | 0-4095光强分辨率     |

## 扩展建议

想要进一步提升？可以考虑：
- 添加DS1302时钟模块实现定时功能
- 集成HC-SR501人体感应传感器
- 增加蓝牙模块支持手机控制
- 实现光强数据存储和统计分析

这也是我改进的方向，改进之后我也会同步上传到`gitHub`

## 贡献指南

欢迎提交 Issue 和 Pull Request！如果您有改进建议：

1. Fork 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 致谢

感谢以下资源提供的帮助：
- [兆龙电子单片机设计](https://blog.csdn.net/m0_70357684/article/details/152048979)
- STC89C52数据手册
- 所有测试和反馈的朋友们

---

<div align="center">

**如果这个项目对您有帮助，请给个 ⭐ Star 支持一下！**

📧 **联系作者**：YourbaCrymove  
🐛 **问题反馈**：[提交 Issue](https://github.com/YourbaCrymove/51MCU-SmartLamp/issues)

</div>
