# TinyEngine 2D游戏引擎开发计划 (30天)

本项目旨在实现一个轻量级的 2D 游戏引擎框架，用于展示对游戏循环、渲染管线、资源管理和ECS架构的理解。

## 第一阶段：基础框架与数学库 (Day 1 - Day 7)
*   **Day 1**: 项目环境配置。完善 `CMakeLists.txt`，引入第三方库管理 (以 Git Submodule 或 CMake FetchContent 引入 SDL2)。
*   **Day 2**: 核心层 (Core) - 实现 `Window` 类，封装底层窗口创建和事件分发。
*   **Day 3**: 核心层 (Core) - 实现基础游戏循环 `GameLoop` 和时间管理 `Timer` (计算 Delta Time)。
*   **Day 4**: 数学库 (Math) - 实现 2D 向量 `Vector2` (加减乘除、点乘、叉乘、归一化)。
*   **Day 5**: 数学库 (Math) - 实现 3D 向量 `Vector3` / `Vector4` 及基础工具函数。
*   **Day 6**: 数学库 (Math) - 实现 4x4 矩阵 `Matrix4x4`，重点完成平移、缩放、正交投影 (Orthographic Projection) 矩阵计算。
*   **Day 7**: 阶段复盘与测试。编写简单的日志系统 `Log`，并为数学库编写单元测试。

## 第二阶段：渲染管线封装 (Day 8 - Day 14)
*   **Day 8**: 渲染层 (Graphics) - 初始化 OpenGL 上下文 或 SDL_Renderer，清屏与缓冲区交换。
*   **Day 9**: 渲染层 (Graphics) - 封装 `Shader` 类，实现顶点着色器和片段着色器的加载、编译与链接。
*   **Day 10**: 渲染层 (Graphics) - 封装 `VertexArray`, `VertexBuffer`, `IndexBuffer`。
*   **Day 11**: 渲染层 (Graphics) - 实现基本的图元绘制能力 (Draw Quad)。
*   **Day 12**: 渲染层 (Graphics) - 引入 stb_image，实现 `Texture2D` 纹理数据的解析和 OpenGL 纹理对象的封装。
*   **Day 13**: 渲染层 (Graphics) - 结合数学库的矩阵运算，实现正交摄像机 `OrthographicCamera`。
*   **Day 14**: 渲染系统重构，实现 `Renderer2D`，提供简单的 API 如 `DrawQuad(position, size, texture)`.

## 第三阶段：资源管理与组件系统 (ECS) (Day 15 - Day 22)
*   **Day 15**: 资源管理 (Resource) - 设计 `ResourceManager` 基础接口，定义资源句柄或引用计数智能指针机制。
*   **Day 16**: 资源管理 (Resource) - 实现全局缓存池，支持 `Texture` 的按路径加载和防止重复实例化。
*   **Day 17**: 资源管理 (Resource) - 扩展音频库 (如 SDL_mixer)，加入 `Audio` 资源的加载与缓存机制。
*   **Day 18**: 组件系统 (ECS) - 调研并设计数据结构：实现基础的 `Entity` (ID 系统) 和 `Component` 基类/模板。
*   **Day 19**: 组件系统 (ECS) - 实现 `Registry` (注册表) 用于管理所有 Entity 的组件数组分配，保证内存连续性 (Sparse Set 或简易数组结构)。
*   **Day 20**: 组件系统 (ECS) - 定义核心组件：`TransformComponent`, `SpriteRendererComponent`。
*   **Day 21**: 组件系统 (ECS) - 实现核心系统：`RenderSystem` (遍历含 Sprite 的实体并提交渲染)。
*   **Day 22**: 阶段复盘。联调 ECS + Renderer + Resource Manager 测试简单场景。

## 第四阶段：业务逻辑与 Demo 制作 (Day 23 - Day 30)
*   **Day 23**: 输入处理 (Input) - 封装键盘与鼠标状态单例，支持按键检测和事件回调。
*   **Day 24**: 系统补充 (ECS) - 添加 `PlayerControllerSystem` 等脚本逻辑支持功能。
*   **Day 25**: 场景管理 (Scene) - 封装 `Scene` 对象，管理内部的 ECS Registry 及其生命周期。
*   **Day 26**: 游戏Demo研发 - 设计一个小游戏 (如 Flappy Bird 或 简易打砖块) 的关卡与实体。
*   **Day 27**: 游戏Demo研发 - 利用自己编写的引擎完成玩家输入、碰撞检测 (AABB) 等功能。
*   **Day 28**: 音频与特效 - 在 Demo 中加入背景音乐与音效，使用引擎的 Audio 模块。
*   **Day 29**: 性能分析与优化 - 整理内存泄露警告，完善析构函数的清理；如果有余力可尝试加入 Batch Rendering (批处理渲染)。
*   **Day 30**: 最终总结 - 整理代码注释，丰富 `README.md` 的构建和运行指引，复盘项目技术难点和面试可讲点。
