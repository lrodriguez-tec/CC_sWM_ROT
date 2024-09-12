OUTPUT:=output


.PONHY:clean

${OUTPUT}/traces_AO_sort_uniq_shuf_115_sort.txt: ${OUTPUT}/traces_AO_sort_uniq_shuf_115.txt
	awk '{ print length, $$0 }' ${OUTPUT}/traces_AO_sort_uniq_shuf_115.txt | sort -n | cut -d" " -f2- > ${OUTPUT}/traces_AO_sort_uniq_shuf_115_sort.txt

${OUTPUT}/traces_AO_sort_uniq_shuf_115.txt: ${OUTPUT}/traces_AO_sort_uniq_shuf.txt
	head -n 115 ${OUTPUT}/traces_AO_sort_uniq_shuf.txt > ${OUTPUT}/traces_AO_sort_uniq_shuf_115.txt 

${OUTPUT}/traces_AO_sort_uniq_shuf.txt: ${OUTPUT}/traces_AO_sort_uniq.txt
	shuf ${OUTPUT}/traces_AO_sort_uniq.txt > ${OUTPUT}/traces_AO_sort_uniq_shuf.txt 


#${OUTPUT}/traces_AO_sort_uniq.txt: ${OUTPUT}/traces_AO_sort.txt
	#uniq ${OUTPUT}/traces_AO_sort.txt > ${OUTPUT}/traces_AO_sort_uniq.txt 

${OUTPUT}/traces_AO_sort_uniq.txt: traces_AO.txt
	mkdir -p ${OUTPUT}
	cp traces_AO.txt ${OUTPUT}
	sort -u traces_AO.txt > ${OUTPUT}/traces_AO_sort_uniq.txt

clean:
	rm -rf output
