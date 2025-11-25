# Perf Testing

## (4 chunks with DO_TRIANGLE_LINE)

- **Basic chunk creation & deletion:** 3.20 fps
- **Includes not rendering faces inside blocks (inside a chunk only):** 24.75 fps
- **With basic noise FBM setup**: 6.0 fps
- **Broken Frustum culling/circular rendering:** 878.38 fps

## (8 chunks with DO_TRIANGLE_LINE to false)

- **Broken Frustum culling/circular rendering:** 240.11 fps
- **Fixed Frustum culling/Reduced normal sent:** 293.92 fps

## (16 chunks with DO_TRIANGLE_LINE to false)
