const canvasElement = document.getElementById("canvas");

if (!(canvasElement instanceof HTMLCanvasElement)) {
  throw new Error("Expected a canvas element with id 'canvas'.");
}

const canvas: HTMLCanvasElement = canvasElement;

const resizeObservers = new Set<() => void>();

async function initWebGPU() {
  if (!navigator.gpu) {
    throw new Error("WebGPU is not supported in this browser.");
  }

  const adapter = await navigator.gpu.requestAdapter();

  if (!adapter) {
    throw new Error("Failed to acquire a WebGPU adapter.");
  }

  const device = await adapter.requestDevice();
  const context = canvas.getContext("webgpu");

  if (!context) {
    throw new Error("Failed to acquire a WebGPU context.");
  }

  const format = navigator.gpu.getPreferredCanvasFormat();

  const configureContext = () => {
    const devicePixelRatio = window.devicePixelRatio || 1;
    const width = Math.max(1, Math.floor(window.innerWidth * devicePixelRatio));
    const height = Math.max(1, Math.floor(window.innerHeight * devicePixelRatio));

    if (canvas.width !== width || canvas.height !== height) {
      canvas.width = width;
      canvas.height = height;
    }

    context.configure({
      device,
      format,
      alphaMode: "opaque",
    });
  };

  configureContext();
  resizeObservers.add(configureContext);

  const shaderModule = device.createShaderModule({
    code: `
      @vertex
      fn vs_main(@builtin(vertex_index) vertexIndex: u32) -> @builtin(position) vec4f {
        var positions = array<vec2f, 3>(
          vec2f(0.0, 0.6),
          vec2f(-0.6, -0.6),
          vec2f(0.6, -0.6)
        );

        let position = positions[vertexIndex];
        return vec4f(position, 0.0, 1.0);
      }

      @fragment
      fn fs_main() -> @location(0) vec4f {
        return vec4f(0.95, 0.55, 0.2, 1.0);
      }
    `,
  });

  const pipeline = device.createRenderPipeline({
    layout: "auto",
    vertex: {
      module: shaderModule,
      entryPoint: "vs_main",
    },
    fragment: {
      module: shaderModule,
      entryPoint: "fs_main",
      targets: [{ format }],
    },
    primitive: {
      topology: "triangle-list",
    },
  });

  const render = () => {
    const commandEncoder = device.createCommandEncoder();
    const renderPass = commandEncoder.beginRenderPass({
      colorAttachments: [
        {
          clearValue: { r: 0.07, g: 0.08, b: 0.11, a: 1 },
          loadOp: "clear",
          storeOp: "store",
          view: context.getCurrentTexture().createView(),
        },
      ],
    });

    renderPass.setPipeline(pipeline);
    renderPass.draw(3);
    renderPass.end();

    device.queue.submit([commandEncoder.finish()]);
  };

  const frame = () => {
    render();
    requestAnimationFrame(frame);
  };

  requestAnimationFrame(frame);
}

function showError(message: string) {
  const error = document.createElement("pre");
  error.textContent = message;
  error.style.padding = "16px";
  error.style.color = "#f8d7da";
  error.style.background = "#2b1116";
  error.style.fontFamily = "monospace";
  document.body.append(error);
}

window.addEventListener("resize", () => {
  for (const observer of resizeObservers) {
    observer();
  }
});

initWebGPU().catch((error: unknown) => {
  const message = error instanceof Error ? error.message : "Unknown WebGPU error.";
  showError(message);
});
