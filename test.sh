for i in {1..8};
do
	echo "Test case $i";
	echo;
	echo "Prog Time";
	time ./bignumcalc testcases/testcase$i/in.txt testcases/testcase$i/out.txt;
	echo;
	echo "BC Time";
	time bc testcases/testcase$i/in_bc.txt > testcases/testcase$i/out_bc.txt 2>&1;
	echo "---------------";
done;