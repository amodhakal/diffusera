# Perf Testing

## (4 chunks with DO_TRIANGLE_LINE)

- **Basic chunk creation & deletion:** 3 fps
- **Includes not rendering faces inside blocks (inside a chunk only):** 25 fps
- **With basic noise FBM setup**: 6 fps
- **Broken Frustum culling/circular rendering:** 878 fps

## (8 chunks with DO_TRIANGLE_LINE to false)

- **Broken Frustum culling/circular rendering:** 240 fps
- **Fixed Frustum culling/Reduced normal sent:** 294 fps

## (16 chunks with DO_TRIANGLE_LINE to false)

- **Current:** 95 fps and 2.71 GB
- **Clean up vbo data once used**: 95 fps and 868 MB
