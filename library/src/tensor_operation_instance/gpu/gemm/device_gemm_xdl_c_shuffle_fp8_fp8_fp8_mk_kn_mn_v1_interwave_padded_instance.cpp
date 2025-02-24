// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2023, Advanced Micro Devices, Inc. All rights reserved.

#include "ck/library/tensor_operation_instance/gpu/device_gemm_xdl_c_shuffle_fp8_fp8_fp8_mk_kn_mn_v1_interwave_instance.hpp"

#ifdef CK_ENABLE_FP8
namespace ck {
namespace tensor_operation {
namespace device {
namespace instance {

static constexpr auto MNKPadding = ck::tensor_operation::device::GemmSpecialization::MNKPadding;

void add_device_gemm_xdl_c_shuffle_f8_f8_f8_mk_kn_mn_v1_interwave_padded_instances(
    std::vector<std::unique_ptr<
        DeviceGemm<Row, Row, Row, F8, F8, F8, PassThrough, PassThrough, PassThrough>>>& instances)
{
    add_device_operation_instances(
        instances,
        device_gemm_xdl_c_shuffle_f8_f8_f8_mk_kn_mn_v1_interwave_instances<MNKPadding>{});
}

} // namespace instance
} // namespace device
} // namespace tensor_operation
} // namespace ck
#endif
