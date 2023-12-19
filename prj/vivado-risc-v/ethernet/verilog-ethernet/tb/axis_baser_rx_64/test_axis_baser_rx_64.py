#!/usr/bin/env python
"""

Copyright (c) 2021 Alex Forencich

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

"""

import itertools
import logging
import os
import sys

import cocotb_test.simulator

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge
from cocotb.utils import get_time_from_sim_steps
from cocotb.regression import TestFactory

from cocotbext.eth import XgmiiFrame, PtpClockSimTime
from cocotbext.axi import AxiStreamBus, AxiStreamSink

try:
    from baser import BaseRSerdesSource
except ImportError:
    # attempt import from current directory
    sys.path.insert(0, os.path.join(os.path.dirname(__file__)))
    try:
        from baser import BaseRSerdesSource
    finally:
        del sys.path[0]


class TB:
    def __init__(self, dut):
        self.dut = dut

        self.log = logging.getLogger("cocotb.tb")
        self.log.setLevel(logging.DEBUG)

        cocotb.start_soon(Clock(dut.clk, 6.4, units="ns").start())

        self.source = BaseRSerdesSource(dut.encoded_rx_data, dut.encoded_rx_hdr, dut.clk, scramble=False)
        self.sink = AxiStreamSink(AxiStreamBus.from_prefix(dut, "m_axis"), dut.clk, dut.rst)

        self.ptp_clock = PtpClockSimTime(ts_64=dut.ptp_ts, clock=dut.clk)

    async def reset(self):
        self.dut.rst.setimmediatevalue(0)
        await RisingEdge(self.dut.clk)
        await RisingEdge(self.dut.clk)
        self.dut.rst.value = 1
        await RisingEdge(self.dut.clk)
        await RisingEdge(self.dut.clk)
        self.dut.rst.value = 0
        await RisingEdge(self.dut.clk)
        await RisingEdge(self.dut.clk)


async def run_test(dut, payload_lengths=None, payload_data=None, ifg=12):

    tb = TB(dut)

    tb.source.ifg = ifg

    await tb.reset()

    test_frames = [payload_data(x) for x in payload_lengths()]
    tx_frames = []

    for test_data in test_frames:
        test_frame = XgmiiFrame.from_payload(test_data, tx_complete=tx_frames.append)
        await tb.source.send(test_frame)

    for test_data in test_frames:
        rx_frame = await tb.sink.recv()
        tx_frame = tx_frames.pop(0)

        frame_error = rx_frame.tuser & 1
        ptp_ts = rx_frame.tuser >> 1
        ptp_ts_ns = ptp_ts / 2**16

        tx_frame_sfd_ns = get_time_from_sim_steps(tx_frame.sim_time_sfd, "ns")

        if tx_frame.start_lane == 4:
            # start in lane 4 reports 1 full cycle delay, so subtract half clock period
            tx_frame_sfd_ns -= 3.2

        tb.log.info("RX frame PTP TS: %f ns", ptp_ts_ns)
        tb.log.info("TX frame SFD sim time: %f ns", tx_frame_sfd_ns)

        assert rx_frame.tdata == test_data
        assert frame_error == 0
        assert abs(ptp_ts_ns - tx_frame_sfd_ns - 6.4) < 0.01

    assert tb.sink.empty()

    await RisingEdge(dut.clk)
    await RisingEdge(dut.clk)


def size_list():
    return list(range(60, 128)) + [512, 1514, 9214] + [60]*10


def incrementing_payload(length):
    return bytearray(itertools.islice(itertools.cycle(range(256)), length))


def cycle_en():
    return itertools.cycle([0, 0, 0, 1])


if cocotb.SIM_NAME:

    factory = TestFactory(run_test)
    factory.add_option("payload_lengths", [size_list])
    factory.add_option("payload_data", [incrementing_payload])
    factory.add_option("ifg", [12, 0])
    factory.generate_tests()


# cocotb-test

tests_dir = os.path.abspath(os.path.dirname(__file__))
rtl_dir = os.path.abspath(os.path.join(tests_dir, '..', '..', 'rtl'))
lib_dir = os.path.abspath(os.path.join(rtl_dir, '..', 'lib'))
axis_rtl_dir = os.path.abspath(os.path.join(lib_dir, 'axis', 'rtl'))


def test_axis_baser_rx_64(request):
    dut = "axis_baser_rx_64"
    module = os.path.splitext(os.path.basename(__file__))[0]
    toplevel = dut

    verilog_sources = [
        os.path.join(rtl_dir, f"{dut}.v"),
        os.path.join(rtl_dir, "lfsr.v"),
    ]

    parameters = {}

    parameters['DATA_WIDTH'] = 64
    parameters['KEEP_WIDTH'] = parameters['DATA_WIDTH'] // 8
    parameters['HDR_WIDTH'] = 2
    parameters['PTP_TS_ENABLE'] = 1
    parameters['PTP_TS_WIDTH'] = 96
    parameters['USER_WIDTH'] = (parameters['PTP_TS_WIDTH'] if parameters['PTP_TS_ENABLE'] else 0) + 1

    extra_env = {f'PARAM_{k}': str(v) for k, v in parameters.items()}

    sim_build = os.path.join(tests_dir, "sim_build",
        request.node.name.replace('[', '-').replace(']', ''))

    cocotb_test.simulator.run(
        python_search=[tests_dir],
        verilog_sources=verilog_sources,
        toplevel=toplevel,
        module=module,
        parameters=parameters,
        sim_build=sim_build,
        extra_env=extra_env,
    )