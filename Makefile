
all: dpc2sim-stream

run: dpc2sim-stream
	zcat traces/mcf_trace2.dpc.gz | ./dpc2sim-stream

dpc2sim-stream:
	$(CXX) -no-pie -Wall -o dpc2sim-stream example_prefetchers/stream_prefetcher.cc lib/dpc2sim.a

stream-warm-30k: dpc2sim-stream
	zcat traces/gcc_trace2.dpc.gz | ./dpc2sim-stream -warmup_instructions 300000 > results/stream/gcc_trace2.txt
	tail -n 4 results/stream/gcc_trace2.txt
	
	zcat traces/GemsFDTD_trace2.dpc.gz | ./dpc2sim-stream -warmup_instructions 300000 > results/stream/GemsFDTD_trace2.txt
	tail -n 4 results/stream/GemsFDTD_trace2.txt

	zcat traces/lbm_trace2.dpc.gz | ./dpc2sim-stream -warmup_instructions 300000 > results/stream/lbm_trace2.txt
	tail -n 4 results/stream/lbm_trace2.txt

	zcat traces/leslie3d_trace2.dpc.gz | ./dpc2sim-stream -warmup_instructions 300000 > results/stream/leslie3d_trace2.txt
	tail -n 4 results/stream/leslie3d_trace2.txt

	zcat traces/libquantum_trace2.dpc.gz | ./dpc2sim-stream -warmup_instructions 300000 > results/stream/libquantum_trace2.txt
	tail -n 4 results/stream/libquantum_trace2.txt

	zcat traces/mcf_trace2.dpc.gz | ./dpc2sim-stream -warmup_instructions 300000 > results/stream/mcf_trace2.txt
	tail -n 4 results/stream/mcf_trace2.txt

	zcat traces/milc_trace2.dpc.gz | ./dpc2sim-stream -warmup_instructions 300000 > results/stream/milc_trace2.txt
	tail -n 4 results/stream/milc_trace2.txt

	zcat traces/omnetpp_trace2.dpc.gz | ./dpc2sim-stream -warmup_instructions 300000 > results/stream/omnetpp_trace2.txt
	tail -n 4 results/stream/omnetpp_trace2.txt

stream-cold: dpc2sim-stream
	zcat traces/gcc_trace2.dpc.gz | ./dpc2sim-stream > results/stream/gcc_trace2.txt
	tail -n 4 results/stream/gcc_trace2.txt
	
	zcat traces/GemsFDTD_trace2.dpc.gz | ./dpc2sim-stream > results/stream/GemsFDTD_trace2.txt
	tail -n 4 results/stream/GemsFDTD_trace2.txt

	zcat traces/lbm_trace2.dpc.gz | ./dpc2sim-stream > results/stream/lbm_trace2.txt
	tail -n 4 results/stream/lbm_trace2.txt

	zcat traces/leslie3d_trace2.dpc.gz | ./dpc2sim-stream > results/stream/leslie3d_trace2.txt
	tail -n 4 results/stream/leslie3d_trace2.txt

	zcat traces/libquantum_trace2.dpc.gz | ./dpc2sim-stream > results/stream/libquantum_trace2.txt
	tail -n 4 results/stream/libquantum_trace2.txt

	zcat traces/mcf_trace2.dpc.gz | ./dpc2sim-stream > results/stream/mcf_trace2.txt
	tail -n 4 results/stream/mcf_trace2.txt

	zcat traces/milc_trace2.dpc.gz | ./dpc2sim-stream > results/stream/milc_trace2.txt
	tail -n 4 results/stream/milc_trace2.txt

	zcat traces/omnetpp_trace2.dpc.gz | ./dpc2sim-stream > results/stream/omnetpp_trace2.txt
	tail -n 4 results/stream/omnetpp_trace2.txt


ghb:
	$(CXX) -no-pie -Wall -o ghb ./GHB_G_AC/ghb_g_ac.cc lib/dpc2sim.a

ghb-warm-30k: ghb
	zcat traces/gcc_trace2.dpc.gz | ./ghb -warmup_instructions 300000 > results/ghb/gcc_trace2.txt
	tail -n 7 results/ghb/gcc_trace2.txt
	
	zcat traces/GemsFDTD_trace2.dpc.gz | ./ghb -warmup_instructions 300000 > results/ghb/GemsFDTD_trace2.txt
	tail -n 7 results/ghb/GemsFDTD_trace2.txt

	zcat traces/lbm_trace2.dpc.gz | ./ghb -warmup_instructions 300000 > results/ghb/lbm_trace2.txt
	tail -n 7 results/ghb/lbm_trace2.txt

	zcat traces/leslie3d_trace2.dpc.gz | ./ghb -warmup_instructions 300000 > results/ghb/leslie3d_trace2.txt
	tail -n 7 results/ghb/leslie3d_trace2.txt

	zcat traces/libquantum_trace2.dpc.gz | ./ghb -warmup_instructions 300000 > results/ghb/libquantum_trace2.txt
	tail -n 7 results/ghb/libquantum_trace2.txt

	zcat traces/mcf_trace2.dpc.gz | ./ghb -warmup_instructions 300000 > results/ghb/mcf_trace2.txt
	tail -n 7 results/ghb/mcf_trace2.txt

	zcat traces/milc_trace2.dpc.gz | ./ghb -warmup_instructions 300000 > results/ghb/milc_trace2.txt
	tail -n 7 results/ghb/milc_trace2.txt

	zcat traces/omnetpp_trace2.dpc.gz | ./ghb -warmup_instructions 300000 > results/ghb/omnetpp_trace2.txt
	tail -n 7 results/ghb/omnetpp_trace2.txt

ghb-cold: ghb
	zcat traces/gcc_trace2.dpc.gz | ./ghb > results/ghb/gcc_trace2.txt
	tail -n 7 results/ghb/gcc_trace2.txt
	
	zcat traces/GemsFDTD_trace2.dpc.gz | ./ghb > results/ghb/GemsFDTD_trace2.txt
	tail -n 7 results/ghb/GemsFDTD_trace2.txt

	zcat traces/lbm_trace2.dpc.gz | ./ghb > results/ghb/lbm_trace2.txt
	tail -n 7 results/ghb/lbm_trace2.txt

	zcat traces/leslie3d_trace2.dpc.gz | ./ghb > results/ghb/leslie3d_trace2.txt
	tail -n 7 results/ghb/leslie3d_trace2.txt

	zcat traces/libquantum_trace2.dpc.gz | ./ghb > results/ghb/libquantum_trace2.txt
	tail -n 7 results/ghb/libquantum_trace2.txt

	zcat traces/mcf_trace2.dpc.gz | ./ghb > results/ghb/mcf_trace2.txt
	tail -n 7 results/ghb/mcf_trace2.txt

	zcat traces/milc_trace2.dpc.gz | ./ghb > results/ghb/milc_trace2.txt
	tail -n 7 results/ghb/milc_trace2.txt

	zcat traces/omnetpp_trace2.dpc.gz | ./ghb > results/ghb/omnetpp_trace2.txt
	tail -n 7 results/ghb/omnetpp_trace2.txt

ghb-single: ghb
	zcat traces/mcf_trace2.dpc.gz | ./ghb

clean:
	rm -rf dpc2sim-stream

clean-ghb:
	rm -rf ghb

.PHONY: all run clean
