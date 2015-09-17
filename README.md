# cpplivestat
C++ code for incremental computation of descriptive statistics. This example supports merge operation for parallel computation

	int data2[] = {27,52,92,3,9,2,20};
	LiveStatistics<int,float> q1;

	for(auto & x: data1)
			q1.add(x);
