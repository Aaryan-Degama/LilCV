import torch
import os

"""
Export MiDaS Small to ONNX with fixed input size.
This ONNX model is used by LilCV's C++ pipeline via ONNX Runtime.
"""

MODEL_TYPE = "MiDaS_small"
H, W = 256, 256

# Load model
midas = torch.hub.load("intel-isl/MiDaS", MODEL_TYPE)
midas.eval()
midas.cpu()

# Dummy input (fixed size)
dummy_input = torch.randn(1, 3, H, W, device="cpu")

# Export
with torch.no_grad():
    torch.onnx.export(
        midas,
        dummy_input,
        "midas_small.onnx",
        input_names=["input"],
        output_names=["depth"],
        opset_version=18,
        do_constant_folding=True,
        dynamic_axes=None,
        export_params=True,
    )

# Move to project root
os.replace("midas_small.onnx", "../midas_small.onnx")

print("✅ Exported midas_small.onnx to project root")