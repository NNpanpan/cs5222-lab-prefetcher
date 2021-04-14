
all: dpc2sim-stream

run: dpc2sim-stream
	zcat traces/mcf_trace2.dpc.gz | ./dpc2sim-stream

dpc2sim-stream:
	$(CXX) -no-pie -Wall -o dpc2sim-stream example_prefetchers/stream_prefetcher.cc lib/dpc2sim.a

ghb:
	$(CXX) -no-pie -Wall -o ghb example_prefetchers/global_history_buffer.cc lib/dpc2sim.a

ghb-all: ghb
	zcat traces/gcc_trace2.dpc.gz | ./ghb >> results/ghb/gcc_trace2.txt
	zcat traces/GemsFDTD_trace2.dpc.gz | ./ghb >> results/ghb/GemsFDTD_trace2.txt
	zcat traces/lbm_trace2.dpc.gz | ./ghb >> results/ghb/lbm_trace2.txt
	zcat traces/leslie3d_trace2.dpc.gz | ./ghb >> results/ghb/leslie3d_trace2.txt
	zcat traces/libquantum_trace2.dpc.gz | ./ghb >> results/ghb/libquantum_trace2.txt
	zcat traces/mcf_trace2.dpc.gz | ./ghb >> results/ghb/mcf_trace2.txt
	zcat traces/milc_trace2.dpc.gz | ./ghb >> results/ghb/milc_trace2.txt
	zcat traces/omnetpp_trace2.dpc.gz | ./ghb >> results/omnetpp_trace2.txt

ghb-single: ghb
	zcat traces/mcf_trace2.dpc.gz | ./ghb

clean:
	rm -rf dpc2sim-stream

clean-ghb:
	rm -rf ghb

.PHONY: all run clean
