add_custom_target(example_contraction)
add_custom_target(example_contraction_scale)
add_custom_target(example_contraction_bilinear)

# FP32
add_example_executable(example_contraction_bilinear_xdl_fp32 contraction_bilinear_xdl_fp32.cpp)
add_example_dependencies(example_contraction_bilinear example_contraction_bilinear_xdl_fp32)

add_example_executable(example_contraction_scale_xdl_fp32 contraction_scale_xdl_fp32.cpp)
add_example_dependencies(example_contraction_scale example_contraction_scale_xdl_fp32)

add_example_executable(example_contraction_bilinear_xdl_fp32_compute_bf16 contraction_bilinear_xdl_fp32_compute_bf16.cpp)
add_example_dependencies(example_contraction_bilinear example_contraction_bilinear_xdl_fp32_compute_bf16)

add_example_executable(example_contraction_scale_xdl_fp32_compute_bf16 contraction_scale_xdl_fp32_compute_bf16.cpp)
add_example_dependencies(example_contraction_scale example_contraction_scale_xdl_fp32_compute_bf16)

add_example_executable(example_contraction_bilinear_xdl_fp32_compute_fp16 contraction_bilinear_xdl_fp32_compute_fp16.cpp)
add_example_dependencies(example_contraction_bilinear example_contraction_bilinear_xdl_fp32_compute_fp16)

add_example_executable(example_contraction_scale_xdl_fp32_compute_fp16 contraction_scale_xdl_fp32_compute_fp16.cpp)
add_example_dependencies(example_contraction_scale example_contraction_scale_xdl_fp32_compute_fp16)

# FP64
add_example_executable(example_contraction_bilinear_xdl_fp64 contraction_bilinear_xdl_fp64.cpp)
add_example_dependencies(example_contraction_bilinear example_contraction_bilinear_xdl_fp64)

add_example_executable(example_contraction_scale_xdl_fp64 contraction_scale_xdl_fp64.cpp)
add_example_dependencies(example_contraction_scale example_contraction_scale_xdl_fp64)

add_example_executable(example_contraction_bilinear_xdl_fp64_compute_fp32 contraction_bilinear_xdl_fp64_compute_fp32.cpp)
add_example_dependencies(example_contraction_bilinear example_contraction_bilinear_xdl_fp64_compute_fp32)

add_example_executable(example_contraction_scale_xdl_fp64_compute_fp32 contraction_scale_xdl_fp64_compute_fp32.cpp)
add_example_dependencies(example_contraction_scale example_contraction_scale_xdl_fp64_compute_fp32)

# FP16
add_example_executable(example_contraction_bilinear_xdl_fp16_compute_fp32 contraction_bilinear_xdl_fp16_compute_fp32.cpp)
add_example_dependencies(example_contraction_bilinear example_contraction_bilinear_xdl_fp16_compute_fp32)

add_example_executable(example_contraction_scale_xdl_fp16_compute_fp32 contraction_scale_xdl_fp16_compute_fp32.cpp)
add_example_dependencies(example_contraction_scale example_contraction_scale_xdl_fp16_compute_fp32)

# BF16
add_example_executable(example_contraction_bilinear_xdl_bf16_compute_fp32 contraction_bilinear_xdl_bf16_compute_fp32.cpp)
add_example_dependencies(example_contraction_bilinear example_contraction_bilinear_xdl_bf16_compute_fp32)

add_example_executable(example_contraction_scale_xdl_bf16_compute_fp32 contraction_scale_xdl_bf16_compute_fp32.cpp)
add_example_dependencies(example_contraction_scale example_contraction_scale_xdl_bf16_compute_fp32)

add_example_dependencies(example_contraction example_contraction_scale)
add_example_dependencies(example_contraction example_contraction_bilinear)
