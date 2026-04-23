const canvasElement = document.getElementById("canvas");

if (!(canvasElement instanceof HTMLCanvasElement)) {
  throw new Error("Expected a canvas element with id 'canvas'.");
}

const canvas: HTMLCanvasElement = canvasElement;

const resizeObservers = new Set<() => void>();
const DEG_TO_RAD = Math.PI / 180;
const FLOATS_PER_VERTEX = 6;
const VERTEX_STRIDE = FLOATS_PER_VERTEX * Float32Array.BYTES_PER_ELEMENT;
const CHUNK_SIZE_X = 16;
const CHUNK_SIZE_Z = 16;
const CHUNK_HEIGHT = 1024;
const BLOCK_AIR = 0;
const BLOCK_SOLID = 1;

import { getTerrainHeight } from "./noise";

type Vec3 = [number, number, number];
type Mat4 = Float32Array;

type Chunk = {
  chunkX: number;
  chunkZ: number;
  blocks: Uint8Array;
};

type FaceDefinition = {
  normal: [number, number, number];
  corners: [
    [number, number, number],
    [number, number, number],
    [number, number, number],
    [number, number, number],
  ];
  neighborOffset: [number, number, number];
};

const FACE_DEFINITIONS: FaceDefinition[] = [
  {
    normal: [0, 0, 1],
    corners: [
      [0, 0, 1],
      [1, 0, 1],
      [1, 1, 1],
      [0, 1, 1],
    ],
    neighborOffset: [0, 0, 1],
  },
  {
    normal: [0, 0, -1],
    corners: [
      [1, 0, 0],
      [0, 0, 0],
      [0, 1, 0],
      [1, 1, 0],
    ],
    neighborOffset: [0, 0, -1],
  },
  {
    normal: [1, 0, 0],
    corners: [
      [1, 0, 1],
      [1, 0, 0],
      [1, 1, 0],
      [1, 1, 1],
    ],
    neighborOffset: [1, 0, 0],
  },
  {
    normal: [-1, 0, 0],
    corners: [
      [0, 0, 0],
      [0, 0, 1],
      [0, 1, 1],
      [0, 1, 0],
    ],
    neighborOffset: [-1, 0, 0],
  },
  {
    normal: [0, 1, 0],
    corners: [
      [0, 1, 1],
      [1, 1, 1],
      [1, 1, 0],
      [0, 1, 0],
    ],
    neighborOffset: [0, 1, 0],
  },
  {
    normal: [0, -1, 0],
    corners: [
      [0, 0, 0],
      [1, 0, 0],
      [1, 0, 1],
      [0, 0, 1],
    ],
    neighborOffset: [0, -1, 0],
  },
];

function clamp(value: number, min: number, max: number) {
  return Math.min(max, Math.max(min, value));
}

function vec3Normalize([x, y, z]: Vec3): Vec3 {
  const length = Math.hypot(x, y, z) || 1;
  return [x / length, y / length, z / length];
}

function vec3Cross([ax, ay, az]: Vec3, [bx, by, bz]: Vec3): Vec3 {
  return [ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx];
}

function vec3Add([ax, ay, az]: Vec3, [bx, by, bz]: Vec3): Vec3 {
  return [ax + bx, ay + by, az + bz];
}

function vec3Scale([x, y, z]: Vec3, scalar: number): Vec3 {
  return [x * scalar, y * scalar, z * scalar];
}

function mat4Perspective(
  fovY: number,
  aspect: number,
  near: number,
  far: number,
): Mat4 {
  const f = 1 / Math.tan(fovY / 2);

  return new Float32Array([
    f / aspect,
    0,
    0,
    0,
    0,
    f,
    0,
    0,
    0,
    0,
    far / (near - far),
    -1,
    0,
    0,
    (near * far) / (near - far),
    0,
  ]);
}

function mat4LookAt(eye: Vec3, target: Vec3, up: Vec3): Mat4 {
  const zAxis = vec3Normalize([
    eye[0] - target[0],
    eye[1] - target[1],
    eye[2] - target[2],
  ]);
  const xAxis = vec3Normalize(vec3Cross(up, zAxis));
  const yAxis = vec3Cross(zAxis, xAxis);

  return new Float32Array([
    xAxis[0],
    yAxis[0],
    zAxis[0],
    0,
    xAxis[1],
    yAxis[1],
    zAxis[1],
    0,
    xAxis[2],
    yAxis[2],
    zAxis[2],
    0,
    -(xAxis[0] * eye[0] + xAxis[1] * eye[1] + xAxis[2] * eye[2]),
    -(yAxis[0] * eye[0] + yAxis[1] * eye[1] + yAxis[2] * eye[2]),
    -(zAxis[0] * eye[0] + zAxis[1] * eye[1] + zAxis[2] * eye[2]),
    1,
  ]);
}

function mat4Multiply(a: Mat4, b: Mat4): Mat4 {
  const out = new Float32Array(16);

  for (let column = 0; column < 4; column += 1) {
    for (let row = 0; row < 4; row += 1) {
      out[column * 4 + row] =
        a[row] * b[column * 4] +
        a[4 + row] * b[column * 4 + 1] +
        a[8 + row] * b[column * 4 + 2] +
        a[12 + row] * b[column * 4 + 3];
    }
  }

  return out;
}

function createSolidColor(
  r: number,
  g: number,
  b: number,
): [number, number, number] {
  return [r, g, b];
}

function createChunk(chunkX: number, chunkZ: number): Chunk {
  const blocks = new Uint8Array(CHUNK_SIZE_X * CHUNK_HEIGHT * CHUNK_SIZE_Z);

  for (let localZ = 0; localZ < CHUNK_SIZE_Z; localZ += 1) {
    for (let localX = 0; localX < CHUNK_SIZE_X; localX += 1) {
      const worldX = chunkX * CHUNK_SIZE_X + localX;
      const worldZ = chunkZ * CHUNK_SIZE_Z + localZ;
      const height = getTerrainHeight(worldX, worldZ);

      for (let y = 0; y < height; y += 1) {
        blocks[getBlockIndex(localX, y, localZ)] = BLOCK_SOLID;
      }
    }
  }

  return { chunkX, chunkZ, blocks };
}

function getBlockIndex(x: number, y: number, z: number) {
  return x + z * CHUNK_SIZE_X + y * CHUNK_SIZE_X * CHUNK_SIZE_Z;
}

function getChunkBlock(chunk: Chunk, x: number, y: number, z: number) {
  if (
    x < 0 ||
    x >= CHUNK_SIZE_X ||
    y < 0 ||
    y >= CHUNK_HEIGHT ||
    z < 0 ||
    z >= CHUNK_SIZE_Z
  ) {
    return BLOCK_AIR;
  }

  return chunk.blocks[getBlockIndex(x, y, z)];
}

function getBlockColor(y: number, normalY: number): [number, number, number] {
  if (normalY > 0) {
    return createSolidColor(0.42, 0.8, 0.28);
  }
  if (normalY < 0) {
    return createSolidColor(0.32, 0.24, 0.18);
  }
  if (y < 4) {
    return createSolidColor(0.45, 0.34, 0.25);
  }

  return createSolidColor(0.55, 0.43, 0.3);
}

function buildChunkMesh(chunk: Chunk) {
  const vertices: number[] = [];
  const indices: number[] = [];
  let vertexCount = 0;

  for (let y = 0; y < CHUNK_HEIGHT; y += 1) {
    for (let z = 0; z < CHUNK_SIZE_Z; z += 1) {
      for (let x = 0; x < CHUNK_SIZE_X; x += 1) {
        if (getChunkBlock(chunk, x, y, z) === BLOCK_AIR) {
          continue;
        }

        for (const face of FACE_DEFINITIONS) {
          const [offsetX, offsetY, offsetZ] = face.neighborOffset;

          if (
            getChunkBlock(chunk, x + offsetX, y + offsetY, z + offsetZ) !==
            BLOCK_AIR
          ) {
            continue;
          }

          const [r, g, b] = getBlockColor(y, face.normal[1]);

          for (const corner of face.corners) {
            vertices.push(
              chunk.chunkX * CHUNK_SIZE_X + x + corner[0],
              y + corner[1],
              chunk.chunkZ * CHUNK_SIZE_Z + z + corner[2],
              r,
              g,
              b,
            );
          }

          indices.push(
            vertexCount,
            vertexCount + 1,
            vertexCount + 2,
            vertexCount,
            vertexCount + 2,
            vertexCount + 3,
          );
          vertexCount += 4;
        }
      }
    }
  }

  return {
    vertices: new Float32Array(vertices),
    indices: new Uint32Array(indices),
  };
}

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
  let depthTexture: GPUTexture | null = null;

  const configureContext = () => {
    const devicePixelRatio = window.devicePixelRatio || 1;
    const width = Math.max(1, Math.floor(window.innerWidth * devicePixelRatio));
    const height = Math.max(
      1,
      Math.floor(window.innerHeight * devicePixelRatio),
    );

    if (canvas.width !== width || canvas.height !== height) {
      canvas.width = width;
      canvas.height = height;
    }

    context.configure({
      device,
      format,
      alphaMode: "opaque",
    });

    depthTexture?.destroy();
    depthTexture = device.createTexture({
      size: { width: canvas.width, height: canvas.height },
      format: "depth24plus",
      usage: GPUTextureUsage.RENDER_ATTACHMENT,
    });
  };

  configureContext();
  resizeObservers.add(configureContext);

  const shaderModule = device.createShaderModule({
    code: `
      struct Uniforms {
        viewProjection: mat4x4f,
      };

      @group(0) @binding(0) var<uniform> uniforms: Uniforms;

      struct VertexInput {
        @location(0) position: vec3f,
        @location(1) color: vec3f,
      };

      struct VertexOutput {
        @builtin(position) position: vec4f,
        @location(0) color: vec3f,
      };

      @vertex
      fn vs_main(input: VertexInput) -> VertexOutput {
        var output: VertexOutput;
        output.position = uniforms.viewProjection * vec4f(input.position, 1.0);
        output.color = input.color;
        return output;
      }

      @fragment
      fn fs_main(input: VertexOutput) -> @location(0) vec4f {
        return vec4f(input.color, 1.0);
      }
    `,
  });

  const pipeline = device.createRenderPipeline({
    layout: "auto",
    vertex: {
      module: shaderModule,
      entryPoint: "vs_main",
      buffers: [
        {
          arrayStride: VERTEX_STRIDE,
          attributes: [
            { shaderLocation: 0, offset: 0, format: "float32x3" },
            {
              shaderLocation: 1,
              offset: 3 * Float32Array.BYTES_PER_ELEMENT,
              format: "float32x3",
            },
          ],
        },
      ],
    },
    fragment: {
      module: shaderModule,
      entryPoint: "fs_main",
      targets: [{ format }],
    },
    primitive: {
      topology: "triangle-list",
      cullMode: "back",
    },
    depthStencil: {
      format: "depth24plus",
      depthWriteEnabled: true,
      depthCompare: "less",
    },
  });

  const chunk = createChunk(0, 0);
  const mesh = buildChunkMesh(chunk);

  const vertexBuffer = device.createBuffer({
    size: mesh.vertices.byteLength,
    usage: GPUBufferUsage.VERTEX | GPUBufferUsage.COPY_DST,
    mappedAtCreation: true,
  });
  new Float32Array(vertexBuffer.getMappedRange()).set(mesh.vertices);
  vertexBuffer.unmap();

  const indexBuffer = device.createBuffer({
    size: mesh.indices.byteLength,
    usage: GPUBufferUsage.INDEX | GPUBufferUsage.COPY_DST,
    mappedAtCreation: true,
  });
  new Uint32Array(indexBuffer.getMappedRange()).set(mesh.indices);
  indexBuffer.unmap();

  const uniformBuffer = device.createBuffer({
    size: 16 * Float32Array.BYTES_PER_ELEMENT,
    usage: GPUBufferUsage.UNIFORM | GPUBufferUsage.COPY_DST,
  });

  const bindGroup = device.createBindGroup({
    layout: pipeline.getBindGroupLayout(0),
    entries: [{ binding: 0, resource: { buffer: uniformBuffer } }],
  });

  const pressedKeys = new Set<string>();
  const camera = {
    position: [8, CHUNK_HEIGHT + 20, 28] as Vec3,
    yaw: -90 * DEG_TO_RAD,
    pitch: -0.45,
  };

  const controls = document.createElement("div");
  controls.className = "controls-hint";
  controls.textContent = `Click canvas to look. Move with WASD or arrows, Space up, Shift down. Chunk: ${CHUNK_SIZE_X}x${CHUNK_SIZE_Z}x${CHUNK_HEIGHT}.`;
  document.body.append(controls);

  const movementAliases = new Map<string, string>([
    ["w", "forward"],
    ["arrowup", "forward"],
    ["s", "backward"],
    ["arrowdown", "backward"],
    ["a", "left"],
    ["arrowleft", "left"],
    ["d", "right"],
    ["arrowright", "right"],
    [" ", "up"],
    ["spacebar", "up"],
    ["shift", "down"],
  ]);

  const setKeyState = (event: KeyboardEvent, isPressed: boolean) => {
    const action = movementAliases.get(event.key.toLowerCase());
    if (!action) {
      return;
    }

    event.preventDefault();

    if (isPressed) {
      pressedKeys.add(action);
    } else {
      pressedKeys.delete(action);
    }
  };

  window.addEventListener("keydown", (event) => setKeyState(event, true));
  window.addEventListener("keyup", (event) => setKeyState(event, false));
  window.addEventListener("blur", () => pressedKeys.clear());

  canvas.addEventListener("click", async () => {
    if (document.pointerLockElement !== canvas) {
      await canvas.requestPointerLock();
    }
  });

  document.addEventListener("pointerlockchange", () => {
    controls.textContent =
      document.pointerLockElement === canvas
        ? `Looking enabled. WASD/arrows move, Space up, Shift down. Faces: ${mesh.indices.length / 6}.`
        : `Click canvas to look. Move with WASD or arrows, Space up, Shift down. Chunk: ${CHUNK_SIZE_X}x${CHUNK_SIZE_Z}x${CHUNK_HEIGHT}.`;
  });

  document.addEventListener("mousemove", (event) => {
    if (document.pointerLockElement !== canvas) {
      return;
    }

    const sensitivity = 0.0025;
    camera.yaw += event.movementX * sensitivity;
    camera.pitch = clamp(
      camera.pitch - event.movementY * sensitivity,
      -89 * DEG_TO_RAD,
      89 * DEG_TO_RAD,
    );
  });

  const up: Vec3 = [0, 1, 0];
  const viewProjection = new Float32Array(16);

  const getForwardVector = (): Vec3 =>
    vec3Normalize([
      Math.cos(camera.pitch) * Math.cos(camera.yaw),
      Math.sin(camera.pitch),
      Math.cos(camera.pitch) * Math.sin(camera.yaw),
    ]);

  const updateCamera = (deltaTime: number) => {
    const forward = getForwardVector();
    const flatForward = vec3Normalize([forward[0], 0, forward[2]]);
    const right = vec3Normalize(vec3Cross(flatForward, up));
    const speed = 10 * deltaTime;

    if (pressedKeys.has("forward")) {
      camera.position = vec3Add(camera.position, vec3Scale(flatForward, speed));
    }
    if (pressedKeys.has("backward")) {
      camera.position = vec3Add(
        camera.position,
        vec3Scale(flatForward, -speed),
      );
    }
    if (pressedKeys.has("left")) {
      camera.position = vec3Add(camera.position, vec3Scale(right, -speed));
    }
    if (pressedKeys.has("right")) {
      camera.position = vec3Add(camera.position, vec3Scale(right, speed));
    }
    if (pressedKeys.has("up")) {
      camera.position = vec3Add(camera.position, vec3Scale(up, speed));
    }
    if (pressedKeys.has("down")) {
      camera.position = vec3Add(camera.position, vec3Scale(up, -speed));
    }

    return getForwardVector();
  };

  let previousTime = performance.now();

  const render = (time: number) => {
    const deltaTime = Math.min(0.05, (time - previousTime) / 1000);
    previousTime = time;

    const forward = updateCamera(deltaTime);
    const target = vec3Add(camera.position, forward);
    const aspect = canvas.width / canvas.height;
    const projection = mat4Perspective(60 * DEG_TO_RAD, aspect, 0.1, 50_000);
    const view = mat4LookAt(camera.position, target, up);
    viewProjection.set(mat4Multiply(projection, view));
    device.queue.writeBuffer(uniformBuffer, 0, viewProjection);

    const commandEncoder = device.createCommandEncoder();
    const renderPass = commandEncoder.beginRenderPass({
      colorAttachments: [
        {
          clearValue: { r: 0.53, g: 0.74, b: 0.95, a: 1 },
          loadOp: "clear",
          storeOp: "store",
          view: context.getCurrentTexture().createView(),
        },
      ],
      depthStencilAttachment: {
        view: depthTexture!.createView(),
        depthClearValue: 1,
        depthLoadOp: "clear",
        depthStoreOp: "store",
      },
    });

    renderPass.setPipeline(pipeline);
    renderPass.setBindGroup(0, bindGroup);
    renderPass.setVertexBuffer(0, vertexBuffer);
    renderPass.setIndexBuffer(indexBuffer, "uint32");
    renderPass.drawIndexed(mesh.indices.length);
    renderPass.end();

    device.queue.submit([commandEncoder.finish()]);
    requestAnimationFrame(render);
  };

  requestAnimationFrame(render);
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
  const message =
    error instanceof Error ? error.message : "Unknown WebGPU error.";
  showError(message);
});
