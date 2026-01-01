# Abyssal Gaze NEW

**Bio-Mechanical Cthulhu Theme VST3 Plugin**

A dark, industrial, eldritch horror audio effect plugin built with JUCE.

## Features
- **Visuals**: 800x600 fixed window with a biomechanical eye background.
- **Controls**: 7 Knobs and 1 Menu integrated into the eye pupils.
- **DSP Chain**: Distortion, Filter, Bitcrush, Tremolo, Delay, Reverb.
- **Presets**: 10 curated presets (Void Walker, Neural Snap, etc.).

## Build Instructions
1. Ensure CMake and a C++ compiler (MSVC on Windows) are installed.
2. Run the following commands:
   ```bash
   cmake -B build
   cmake --build build --config Release
   ```
3. Find the VST3 file in `build/AbyssalGazeNew_artefacts/Release/VST3/`.

## Changelog

### V0.6.0 (Current)
- **Dynamic Color Palette**: The visualizer's color now shifts based on the **Corruption** (Distortion) parameter.
    - **Low Corruption**: Ice Blue / Cyan (Cold/Clean).
    - **High Corruption**: Magma Red / Orange (Hot/Distorted).
- **Version Bump**: Project version updated to 0.6.0.

### V0.5.0
- **Visualizer Enhancement**: 
    - **Dynamics**: Increased dynamic range (up to 110% size) and added non-linear sensitivity for more explosive reactivity.
    - **Colors**: Upgraded to a multi-stop "Eldritch Fire" gradient (Intense White/Yellow Core -> Bright Orange -> Blood Red -> Deep Purple).
- **Version Bump**: Project version updated to 0.5.0.

### V0.4.0
- **Living Visualizer**: Implemented a "Living Pupil" effect in the center abyss.
    - **Audio Reactive**: Pulses based on real-time audio RMS levels.
    - **Visual Style**: Radial gradient (Bright Blood Red core to Dark Red edge).
    - **Layering**: Correctly positioned behind the background image holes.
- **Version Bump**: Project version updated to 0.4.0.

### V0.3.0
- **UI Polish**: Reduced visual knob diameter to 42px (4px margin) for a tighter look.
- **Pointer Refinement**: Thinned the red pointer to 2.0f thickness.
- **Menu Style**: Matched the Revelation menu background size to the new 42px knob size.
- **Version Bump**: Project version updated to 0.3.0.

### V0.2.0
- **UI Refinement**: Updated all control positions to exact pixel coordinates based on manual measurements.
- **Knob Sizing**: Adjusted visual knob size to 38px (0.76 factor) for better fit.
- **Menu Style**: Added a black circle background to the "Revelation" menu to properly "plug" the eye hole.
- **Version Bump**: Project version updated to 0.2.0.

### V0.1.0
- **Visual Update**: Changed knob body color from Dark Grey to **Opaque Black** to blend with background holes.
- **Version Bump**: Project version updated to 0.1.0.

### Initial Release
- **Core Implementation**: 
    - DSP Chain: Corruption, Drown, Obscura, VOID, Erosion, Whispers, Tremor.
    - Preset System: 10 Presets with mapped values.
    - Custom LookAndFeel: "Plug" style knobs.
    - Basic UI Layout.

---

# Abyssal Gaze NEW (中文说明)

**生物机械克苏鲁主题 VST3 插件**

基于 JUCE 构建的黑暗、工业、怪诞恐怖风格音频效果插件。

## 功能特性
- **视觉效果**：800x600 固定窗口，生物机械眼背景。
- **控制**：集成在眼球瞳孔中的 7 个旋钮和 1 个菜单。
- **DSP 链路**：失真 (Distortion)、滤波 (Filter)、位深破碎 (Bitcrush)、颤音 (Tremolo)、延迟 (Delay)、混响 (Reverb)。
- **预设**：10 个精选预设 (Void Walker, Neural Snap 等)。

## 构建说明
1. 确保已安装 CMake 和 C++ 编译器 (Windows 上为 MSVC)。
2. 运行以下命令：
   ```bash
   cmake -B build
   cmake --build build --config Release
   ```
3. 在 `build/AbyssalGazeNew_artefacts/Release/VST3/` 中找到 VST3 文件。

## 更新日志 (Changelog)

### V0.6.0 (当前版本)
- **动态色彩**：Visualizer 的颜色现在会根据 **Corruption** (失真) 参数动态变化。
    - **低失真 (Low Corruption)**：冰蓝 / 青色 (冷色调/纯净)。
    - **高失真 (High Corruption)**：岩浆红 / 橙色 (暖色调/失真)。
- **版本升级**：项目版本更新至 0.6.0。

### V0.5.0 (当前版本)
- **视觉效果增强**：
    - **动态范围**：增加了动态范围（最大可达 110% 尺寸），并添加了非线性灵敏度，使反应更具爆发力。
    - **色彩升级**：升级为多段 "Eldritch Fire" 渐变（高亮白/黄核心 -> 亮橙 -> 血红 -> 深紫）。
- **版本升级**：项目版本更新至 0.5.0。

### V0.4.0 (当前版本)
- **动态视觉效果**：在中心深渊实现了 "Living Pupil" (活体瞳孔) 效果。
    - **音频响应**：根据实时音频 RMS 电平进行脉动。
    - **视觉风格**：径向渐变（中心为明亮的血红色，边缘为暗红色）。
    - **图层叠加**：正确放置在背景图像孔洞的后方。
- **版本升级**：项目版本更新至 0.4.0。

### V0.3.0 (当前版本)
- **UI 润色**：将视觉旋钮直径减小至 42px (4px 边距)，外观更紧凑。
- **指针优化**：将红色指针变细至 2.0f 厚度。
- **菜单样式**：将 Revelation 菜单背景大小调整为与新的 42px 旋钮一致。
- **版本升级**：项目版本更新至 0.3.0。

### V0.2.0 (当前版本)
- **UI 优化**：根据手动测量结果，将所有控件位置更新为精确像素坐标。
- **旋钮尺寸**：调整视觉旋钮尺寸为 38px (0.76 系数) 以更好贴合。
- **菜单样式**：为 "Revelation" 菜单添加黑色圆形背景，以正确“填补”眼球孔洞。
- **版本升级**：项目版本更新至 0.2.0。

### V0.1.0
- **视觉更新**：将旋钮主体颜色从深灰改为 **不透明纯黑 (Opaque Black)**，以融合背景孔洞。
- **版本升级**：项目版本更新至 0.1.0。

### 初始发布
- **核心实现**：
    - DSP 链路：Corruption, Drown, Obscura, VOID, Erosion, Whispers, Tremor。
    - 预设系统：10 个带有映射值的预设。
    - 自定义 LookAndFeel："插头 (Plug)" 风格旋钮。
    - 基础 UI 布局。
