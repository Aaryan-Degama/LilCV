# 🖼️ LilCV — Lightweight Computer Vision in C++

LilCV is a minimal, from-scratch computer vision toolkit written in C++, designed to explore how modern ML models (like depth estimation) can be integrated into low-level, dependency-light systems.

What started as an attempt to build portrait blur evolved into a deeper exploration of:

- Depth estimation using neural networks
- ONNX Runtime inference in C++
- Image processing without OpenCV
- Bridging Python ML → C++ systems cleanly

## ✨ Features

### Core Image Operations (C++)

- **Grayscale** (`bw`) — Convert to grayscale
- **Gaussian Blur** (`blur`) — Adjustable blur intensity
- **Depth-Based Blur** (`depthblur`) — Portrait-style blur using neural depth estimation
- **Invert Colors** (`invert`) — RGB color inversion
- **Resize** (`resize`) — User-defined or default 256×256
- **PPM I/O** — Load & save P3 format images

### ML-Powered Depth Estimation

- Uses **MiDaS Small** depth model
- Exported to **ONNX** format
- Inference via **ONNX Runtime** (C++)
- No Python required at runtime ✅

## 🧠 How Depth Blur Works

1. Input image is resized to 256×256
2. Passed through MiDaS ONNX model → depth map
3. Depth map is normalized and resized back to original size
4. Multiple blur levels are precomputed
5. Blur strength is selected per pixel based on depth

**Result:** Near objects remain sharp, far regions blur naturally.

## 📦 Project Structure

```
LilCV/
├── lilcv.cpp
├── image_import.cpp
├── image_import.hpp
├── midas_small.onnx
├── onnx_generation/
│   ├── infer_depth.py
│   └── requirements.txt
├── examples/
│   ├── bird2.ppm
│   └── output.ppm
└── README.md
```

## 🔧 System Requirements

### C++
- GCC ≥ 9
- C++17
- ONNX Runtime (CPU)

### Python (ONNX Export Only)
- Python ≥ 3.10
- PyTorch
- torchvision
- numpy

## 🐍 Python Setup (ONNX Export)

Python is only required **once** to export the MiDaS model to ONNX.

### 1️⃣ Create Virtual Environment

```bash
python3 -m venv .venv
source .venv/bin/activate
```

### 2️⃣ Install Dependencies

```bash
cd onnx_generation
pip install -r requirements.txt
```

### 3️⃣ Export MiDaS to ONNX

```bash
python infer_depth.py
```

This generates `midas_small.onnx` in the project root.

## ⚙️ ONNX Runtime Installation

### Fedora
```bash
sudo dnf install onnxruntime onnxruntime-devel
```

### Ubuntu/Debian
```bash
sudo apt install onnxruntime-dev
```

### Verify Installation
```bash
ls /usr/include/onnxruntime/onnxruntime_cxx_api.h
```

## 🛠️ Build LilCV

```bash
g++ -O3 -std=c++17 \
    lilcv.cpp image_import.cpp \
    -lonnxruntime \
    -o lilcv
```

## ▶️ Usage

### General Format
```bash
./lilcv <input.ppm> <output.ppm> <command> [--intensity X] [--width W] [--height H]
```

### Examples

#### 🖤 Grayscale
```bash
./lilcv input.ppm out.ppm bw
```

#### 🌫️ Gaussian Blur
```bash
./lilcv input.ppm out.ppm blur --intensity 2.5
```

#### 🎯 Depth-Based Blur (Portrait-style)
```bash
./lilcv input.ppm out.ppm depthblur --intensity 3.0
```

#### 🔄 Invert Colors
```bash
./lilcv input.ppm out.ppm invert
```

#### 📐 Resize Image

Default is 256×256 if no size is given:
```bash
./lilcv input.ppm out.ppm resize
```

Custom size:
```bash
./lilcv input.ppm out.ppm resize --width 512 --height 512
```

## 📊 Output Examples

| Input | Depth Map | Depth Blur |
|-------|-----------|-----------|
| Original image | Neural depth estimation | Portrait-style blur result |

*Note: Depth maps are relative depth, not metric depth.*

## 🚀 Why This Project Exists

- Learn how ML models actually run in production
- Avoid black-box OpenCV pipelines
- Explore Python → ONNX → C++ workflows
- Build intuition for performance, memory & inference

### What This Project Intentionally Avoids

- ❌ OpenCV
- ❌ Heavy frameworks
- ❌ Runtime Python dependencies
- ❌ Unnecessary abstractions

## 🔮 Future Ideas

- [ ] Quantized ONNX inference
- [ ] Edge-aware blur using depth gradients
- [ ] Real-time webcam pipeline
- [ ] SIMD acceleration
- [ ] CUDA backend (optional)

## 📝 License

MIT

---

**Made with ❤️ for lightweight, transparent ML systems in C++**