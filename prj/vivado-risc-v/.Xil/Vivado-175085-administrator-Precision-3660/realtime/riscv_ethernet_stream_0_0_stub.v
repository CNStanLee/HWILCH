// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "ethernet_nexys_video,Vivado 2023.2" *)
module riscv_ethernet_stream_0_0(reset, clock125, clock125_90, clock200, 
  tx_axis_tdata, tx_axis_tkeep, tx_axis_tvalid, tx_axis_tready, tx_axis_tlast, tx_axis_tuser, 
  rx_axis_tdata, rx_axis_tkeep, rx_axis_tvalid, rx_axis_tready, rx_axis_tlast, rx_axis_tuser, 
  status_vector, rgmii_txd, rgmii_tx_ctl, rgmii_tx_clk, rgmii_rxd, rgmii_rx_ctl, rgmii_rx_clk);
  input reset;
  input clock125 /* synthesis syn_isclock = 1 */;
  input clock125_90 /* synthesis syn_isclock = 1 */;
  input clock200 /* synthesis syn_isclock = 1 */;
  input [7:0]tx_axis_tdata;
  input [0:0]tx_axis_tkeep;
  input tx_axis_tvalid;
  output tx_axis_tready;
  input tx_axis_tlast;
  input tx_axis_tuser;
  output [7:0]rx_axis_tdata;
  output [0:0]rx_axis_tkeep;
  output rx_axis_tvalid;
  input rx_axis_tready;
  output rx_axis_tlast;
  output rx_axis_tuser;
  output [15:0]status_vector;
  output [3:0]rgmii_txd;
  output rgmii_tx_ctl;
  output rgmii_tx_clk;
  input [3:0]rgmii_rxd;
  input rgmii_rx_ctl;
  input rgmii_rx_clk;
endmodule
