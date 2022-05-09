# Compare the trace with the actual output for some length
head -n $1 trace.txt > trace_tmp
head -n $1 out.txt > out_tmp
diff -n trace_tmp out_tmp
rm trace_tmp out_tmp
