make -C ..

indent() { sed "s/^/    /"; }

progs=( "global-lock" "smart-lock" )



echo -e "\nSTARTING TEST"

echo -e "\nVARIABLE: num_threads"

num_threads=( 1 4 8 12 16 24 32 )

for prog in "${progs[@]}"
do
	echo -e "\n$prog -r 100000 -n 100000"
	for threads in "${num_threads[@]}"
	do
		echo "-t ${threads}" | indent
		../build/$prog -r 100000 -n 100000 -t ${threads} -c | indent
	done
done

echo -e "\nVARIABLE: range"

ranges=( 1000 10000 100000 1000000 )

for prog in "${progs[@]}"
do
	echo -e "\n$prog -n 100000 -t 16"
	for range in "${ranges[@]}"
	do
		echo "-r ${range}" | indent
		../build/$prog -n 100000 -t 16 -r ${range} -c | indent
	done
done
