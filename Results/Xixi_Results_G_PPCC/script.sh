#Xixi results

grep "file_name" Xixi_G_results.txt | awk '{print $9}'
grep "Query size" Xixi_G_results.txt | awk '{print $9}'
grep ": Query:" Xixi_G_results.txt
grep "Query found" Xixi_G_results.txt | awk '{print $9}'
grep "time" Xixi_G_results.txt | awk '{print $9}'
