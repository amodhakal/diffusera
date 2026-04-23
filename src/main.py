import onnxruntime as ort
import numpy as np

BASE_MODEL_PATH = "terrain-diffusion-30m-onnx/base_model.onnx"
COARSE_MODEL_PATH = "terrain-diffusion-30m-onnx/coarse_model.onnx"
DECODER_MODEL_PATH = "terrain-diffusion-30m-onnx/decoder_model.onnx"


def main():
    available_providers = ort.get_available_providers()

    base_session, coarse_session, decoder_session = [
        ort.InferenceSession(model_path, providers=available_providers)
        for model_path in [BASE_MODEL_PATH, COARSE_MODEL_PATH, DECODER_MODEL_PATH]
    ]


if __name__ == "__main__":
    main()
