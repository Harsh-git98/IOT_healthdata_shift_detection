##REFERENCE:

Ray, Partha Pratim, and Dinesh Dash. "IoT-edge anomaly detection for covariate-shifted and point time series health data." Journal of King Saud University - Computer and Information Sciences, vol. 25, November 2021.

**ABSTRACT**:

Abrupt distributional covariate shift and abnormal data points can cause anomalies in time series datasets. Such issues can exacerbate in Internet of Things (IoT)-based scenarios where continuous data is generated from low-cost health sensors. Existing anomaly detection techniques mostly rely on machine learning-aware strategies, resulting in processing and storage overhead in the IoT-based tool chain. In this paper, we perform experiments on anomaly detection on univariate data, anomaly detection of online covariate shift, i.e., IoTSAnom method. we propose the IoTSAnom architecture to detect online covariate shift in the univariate time series pulse rate data stream. We implement the "IRKernel" to investigate the efficiency of exponentially weighted moving average (EWMA)-based lightweight statistical learning by using three variants, e.g., probabilistic (P), shift-detect (SD), and two-stage shift-detect (TS-SD) over the EWMA algorithms. We perform an evaluation of the execution time of the algorithms in classical, incremental, and optimized modes while following the Kolmogorov–Smirnov hypothesis testing at the two-stage anomaly detection.

**IMPROVEMENTS**:

We implemented all the above three mentioned algorithms in C++ language. While Python could have been easier for implementation, considering it's only algorithm-based, C++ can be faster. Additionally, implementing the KS test in the third case was a challenging task but was accomplished from the base level in C++. Furthermore, we implemented the CVM test and Mann-Whitney U test for the second-stage detection and found significant differences in results. Overall, upon studying the output of each algorithm, we found that the KS test focuses on the head section of data, CVM focuses on the tail section of data, whereas the Mann-Whitney U test focuses on the overall data section. These algorithms can be used to check the results from different datasets. Therefore, we need more data to check the overall validity and find out which works best in which cases.Here , proposed all algortihm work efficiently for given dataset.

total 5 algortihms:
1. PEWMA
2. SD-EWMA
3. KS-TS-SD-EWMA
4. CVM-TS-SD-EWMA
5. MWU-TS-SD-EWMA
   

