# Project Hail Mary

[English](README.md) | [中文](README_CN.md)

这是一个使用 C++23 和 SDL2 开发的 2D 俯视角动作 roguelike 原型。

Project Hail Mary 当前已经实现可玩的战斗切片：启动后进入菜单，进入程序化生成的房间，控制角色移动、瞄准、发射投射物，并与多种敌人战斗。项目的长期方向是以房间为节点的地牢推进和构筑驱动战斗。

## 当前状态

**阶段：** 早期可玩垂直切片。

**已可用：** 应用主循环、场景切换、配置驱动资源加载、房间生成、玩家控制、投射物战斗、五种敌人、基础 UI、音效和特效。

**仍在开发：** 完整的房间推进、胜负结算、HUD 与结果界面、存档和长期成长、更丰富的内容，以及更广的自动化测试覆盖。

完整的英文制作评估请参阅 [Project Status](introduction/project_status_en.md)。

## 操作方式

| 输入 | 操作 |
| --- | --- |
| `WASD` 或方向键 | 移动 |
| 鼠标左键或 `J` | 攻击 |
| `Enter` | 确认 / 开始 |
| `R` | 重置当前房间 |

## 构建

### Windows

仓库已包含 Windows 构建配置所需的 SDL2 相关运行时库。建议使用 Visual Studio 2022 或更新版本，并使用 64 位生成器。

```bash
cmake -S . -B build -A x64
cmake --build build --config Release
./build/Release/Hail.exe
```

### macOS

先安装 SDL2 依赖：

```bash
brew install sdl2 sdl2_image sdl2_net sdl2_mixer sdl2_ttf
```

然后配置、构建并运行：

```bash
cmake -S . -B build
cmake --build build
./build/Hail
```

## 测试

```bash
ctest --test-dir build --output-on-failure
```

当前测试主要用于基础回归检查，尚未覆盖完整的玩法验证。

## 目录结构

```text
application/   应用生命周期与主循环
engine/        通用系统：输入、资源、音频、场景、物理、UI
gameplay/      角色、战斗、地图生成、敌人生成和游戏场景
assets/        贴图、字体、音频、预加载资源和配置
docs/          项目文档
tests/         自动化测试
thirdparty/    第三方依赖
```

## 文档

- [英文项目现状说明](introduction/project_status_en.md)
- [中文项目现状说明](introduction/project_status_zh_cn.md)
- [English README](README.md)
- [玩法与叙事提案](introduction/Gameplay%20Narrative%20Pitch.pdf)
- [代码规范](docs/coding_standard.md)