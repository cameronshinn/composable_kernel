#include <stdlib.h>
#include "config.hpp"
#include "device_batched_gemm_reduce_xdl_cshuffle.hpp"
#include "element_wise_operation.hpp"
#include "element_wise_reduce_operation.hpp"
#include "device_operation_instance.hpp"

namespace ck {
namespace tensor_operation {
namespace device {
namespace device_gemm_instance {

using F16 = ck::half_t;
using F32 = float;

using Row = ck::tensor_layout::gemm::RowMajor;
using Col = ck::tensor_layout::gemm::ColumnMajor;

template <ck::index_t... Is>
using S = ck::Sequence<Is...>;

using PassThrough     = ck::tensor_operation::element_wise::PassThrough;
using ReduceSum       = ck::tensor_operation::element_wise::ReduceSum;
using ReduceSquareSum = ck::tensor_operation::element_wise::ReduceSquareSum;

static constexpr auto GemmDefault = ck::tensor_operation::device::GemmSpecialization::Default;

// c[g, m, n] = a[g, m, k] * b[g, n, k]
// d0[g, m] = reduce0(c[g, m, n])
// d1[g, m] = reduce1(c[g, m, n])
using device_batched_gemm_reduce_xdl_cshuffle_f16_f16_f16_f32_f32_gkm_gkn_gmn_instances =
    std::tuple<
        // clang-format off
        //##################################| ALayout| BLayout| CLayout|AData| BData| CData|  GemmAcc| CShuffle| ReduceAcc| DData|           A|           B|           C|        D0|              D1|           GEMM| NumGemmK| Block|  MPer|  NPer|  KPer| AK1| BK1| MPer| NPer| MXdl| NXdl|  ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockLds|  BBlockTransfer| BBlockTransfer| BBlockTransfer| BlockTransfer| BBlockTransfer| BBlockTransfer| BBlockLds|    CShuffle|    CShuffle| CBlockTransferClusterLengths|  CBlockTransfer|              CReduce| CReduceThreadLds2VGprCopy| CReduceThreadVgpr2GlobalCopy|
        //##################################|        |        |        | Type|  Type|  Type| DataType| DataType|  DataType|  Type| Elementwise| Elementwise| Elementwise|    Reduce|          Reduce| Spacialization| Prefetch|  Size| Block| Block| Block|    |    |  XDL|  XDL|  Per|  Per|   ThreadCluster|  ThreadCluster| SrcAccessOrder|   SrcVectorDim|      SrcScalar|      DstScalar| AddExtraM|   ThreadCluster|  ThreadCluster| SrcAccessOrder|  SrcVectorDim|      SrcScalar|      DstScalar| AddExtraN| MXdlPerWave| NXdlPerWave|            _MBlock_MPerBlock| ScalarPerVector| ThreadClusterLengths|     SrcDstScalarPerVector|        SrcDstScalarPerVector|
        //##################################|        |        |        |     |      |      |         |         |          |      |   Operation|   Operation|   Operation| Operation|       Operation|               |    Stage|      |      |      |      |    |    |     |     | Wave| Wave| Lengths_K0_M_K1|   ArrangeOrder|               |               |      PerVector|   PerVector_K1|          | Lengths_K0_N_K1|   ArrangeOrder|               |              |      PerVector|   PerVector_K1|          |  PerShuffle|  PerShuffle|            _NBlock_NPerBlock|      _NPerBlock| _MPerBlock_NPerBlock|                _NPerBlock|                   _MPerBlock|
        //##################################|        |        |        |     |      |      |         |         |          |      |            |            |            |          |                |               |         |      |      |      |      |    |    |     |     |     |     |                |               |               |               |               |               |          |                |               |               |              |               |               |          |            |            |                             |                |                     |                          |                             |
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   256,   256,   128,    32,   2,   2,   32,   32,    4,    2,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              2,     false,     S<8, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              2,     false,           1,           1,               S<1, 32, 1, 8>,               8,             S<64, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   256,   256,   128,    32,   8,   8,   32,   32,    4,    2,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              8,      true,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              2,              8,      true,           1,           1,               S<1, 32, 1, 8>,               8,             S<64, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   256,   128,   256,    32,   4,   4,   32,   32,    2,    4,     S<8, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              2,     false,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              2,     false,           1,           1,               S<1, 32, 1, 8>,               8,             S<64, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   256,   128,   256,    32,   8,   8,   32,   32,    2,    4,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              2,              8,      true,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              8,      true,           1,           1,               S<1, 32, 1, 8>,               8,             S<64, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   128,   128,   128,    32,   2,   2,   32,   32,    4,    2,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              2,     false,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              2,     false,           1,           1,               S<1, 16, 1, 8>,               8,             S<32, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   128,   128,   128,    32,   8,   8,   32,   32,    4,    2,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              8,      true,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              8,      true,           1,           1,               S<1, 16, 1, 8>,               8,             S<32, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   256,   128,   128,    32,   2,   2,   32,   32,    2,    2,     S<8, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              2,     false,     S<8, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              2,     false,           1,           1,               S<1, 32, 1, 8>,               8,             S<64, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   256,   128,   128,    32,   8,   8,   32,   32,    2,    2,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              2,              8,      true,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              2,              8,      true,           1,           1,               S<1, 32, 1, 8>,               8,             S<64, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   128,   128,    64,    32,   2,   2,   32,   32,    2,    2,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              2,     false,     S<4, 16, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              2,     false,           1,           1,               S<1, 32, 1, 4>,               8,             S<64, 2>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   128,   128,    64,    32,   8,   8,   32,   32,    2,    2,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              8,      true,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              2,              8,      true,           1,           1,               S<1, 32, 1, 4>,               8,             S<64, 2>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   128,    64,   128,    32,   2,   2,   32,   32,    2,    2,     S<8, 16, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              2,     false,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              2,     false,           1,           1,               S<1, 16, 1, 8>,               8,             S<32, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   128,    64,   128,    32,   8,   8,   32,   32,    2,    2,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              2,              8,      true,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              8,      true,           1,           1,               S<1, 16, 1, 8>,               8,             S<32, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   256,   128,    64,    32,   2,   2,   32,   32,    2,    1,     S<8, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              2,     false,     S<16,16, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              2,     false,           1,           1,               S<1, 16, 1, 4>,               8,             S<64, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   256,   128,    64,    32,   8,   8,   32,   32,    2,    1,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              2,              8,      true,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              1,              8,      true,           1,           1,               S<1, 16, 1, 4>,               8,             S<64, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   256,    64,   128,    32,   2,   2,   32,   32,    1,    2,     S<16,16, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              2,     false,     S<8, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              2,     false,           1,           1,               S<1, 32, 1, 8>,               8,             S<64, 4>,                         4,                            1>,
        DeviceBatchedGemmReduce_Xdl_CShuffle<     Col,      Row,    Row,  F16,   F16,   F16,      F32,      F32,       F32,   F32, PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum,    GemmDefault,        1,   256,    64,   128,    32,   8,   8,   32,   32,    1,    2,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              1,              8,      true,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              2,              8,      true,           1,           1,               S<1, 32, 1, 8>,               8,             S<64, 4>,                         4,                            1>
        // clang-format on
        >;

void add_device_batched_gemm_reduce_xdl_cshuffle_f16_f16_f16_f32_f32_gkm_gkn_gmn_instances(
    std::vector<
        DeviceGemmReducePtr<PassThrough, PassThrough, PassThrough, ReduceSum, ReduceSquareSum>>&
        instances)
{
    add_device_operation_instances(
        instances,
        device_batched_gemm_reduce_xdl_cshuffle_f16_f16_f16_f32_f32_gkm_gkn_gmn_instances{});
}

} // namespace device_gemm_instance
} // namespace device
} // namespace tensor_operation
} // namespace ck
