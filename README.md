# IOT-heakth-data anamoly detection using EWMA statistics approach

system model:
We placed a pulse sensor on the wrist of one author to collect heart rate in beats per minute (BPM) using Arduino Uno via the analog pin A0. The collected BPM data stream, consisting of 200 samples, is then stored in a CSV file on the Raspberry Pi 4 (4 GB RAM) attached to the assembly. The Raspberry Pi 4 model is equipped with the 'IRKernel' (IRKernle, 2021) from the 'R' distribution (R-project, 2021). Jupyter Notebook (Jupyter Notebook, 2021) is implemented herein on top of Python 3 to allow algorithmic communication with the R scripts.

We intentionally introduce three abrupt shift changes into the collected data stream at points 55, 90, and 170 as covariate ADS anomalies. Regular BPMs ranging from 70 to 72 are observed in the data stream obtained from the pulse sensor. We then execute three key algorithms—P-EWMA, SD-EWMA, and TS-SD-EWMA—in various modes, such as classical processing (CP), incremental processing (IP), optimized CP, and optimized IP, to assess the anomaly detection capabilities. We use the 'ostad' package from R to demonstrate the setup in both stationary and non-stationary scenarios. P-EWMA is sourced from (Carter and Streilein, 2012), and SD-EWMA and TS-SD-EWMA algorithms are from Raza et al. (YYYY), in addition to the deployed system model. The dataset is available at https://github.com/ParthaPRay/IoTpulsehealthdataset.

The sensor data stream used in this study contains three short-lived ADS intentionally incorporated to analyze the accuracy of the implemented system model for the validation of IoT-based health analytics at the edge. The IoTSAnom architecture illustrates how various algorithms and toolsets interact to realize ADS anomaly detection in the given online scenario. We use the 'RProfile' script to intervene R-based package integration within the '/home/pi' domain. Various dependency modules, such as Digest, Uuid, Repr, IRDisplay, Vctrs, Pillar, PbdZMQ, AssertHat, RLang, Evaluate, and Glue, interact with the IRKernel to provide full IoT-edge assisted support for the implied study (Packages et al., 2021).



#P-EWMA

The P-EWMA algorithm supersedes the normal EWMA algorithm by allowing dynamic adaptation of the weighting factor, which depends on the time series observation probability. P-EWMA can detect ADS in a time series data stream and produce dynamic as well as data-driven anomaly cap limits, i.e., thresholds that are robust against ADS-related changes. Thus, P-EWMA enables quick adjustment of the anomaly detector along with adjustment in long-term distributional shifts. The implied P-EWMA algorithm is low-footprint and has less complexity, requiring almost no a priori parameterization beforehand.

The usual design of EWMA uses an update formula shown in Equation (1) to apply an exponentially decreasing weighting factor k to compute the local mean zt. The local mean can be expressed as:

\[ z_t = \frac{\sum_{i=1}^{t} k \cdot (1 - k)^{i-1} \cdot x_i}{1 - k} \]

Here, \( 0 < k < 1 \). EWMA uses the inequality \( |x_t - k \cdot t| > m \cdot r_t \) to generate an alert over the detection of anomaly in the observation. Here, \( m \) and \( r_t \) represent a constant multiplier and standard deviation over the time series observation \( x_t \) at time instant \( t \). From Equation (1), we can infer that if \( k \to 1 \), then \( z_t \) varies with respect to \( x_t \), leading to a poor fit of the model data on the observation, resulting in a lesser chance to detect anomalies.

\[ z_t = k \cdot x_t + (1 - k) \cdot z_{t-1} \quad (1) \]

Thus, the P-EWMA algorithm attempts to adjust \( k \) as shown in Equation (1) depending upon the probability of the present sample data. If we assume \( x_t \) to be the time series sensor data measured over time period \( t \), then we can formulate the local mean of the ongoing process, \( z_t \), as the EWMA of the past samples as shown in Equation (2).

\[ z_t = k(1 - b \cdot P_t) \cdot x_t + (1 - k(1 - b \cdot P_t)) \cdot z_{t-1} \quad (2) \]

Herein, we assume that \( k \) acts as the maximal weighting factor following \( 0 < k < 1 \). We also assume that \( P_t \) denotes the probability measured over \( x_t \), and \( b \) corresponds to the weight placed over \( P_t \). If \( b \to 0 \), then the P-EWMA can converge to the ordinary EWMA. Furthermore, the assumption is considered to adapt \( k \) by \( 1 - b \cdot P_t \) to minimize the possibility of influence over the update estimation in the linear mode.

Algorithm of PEWMA

Algorithm 1 presents the anomaly detection capability of the P-EWMA under the stationary environment. The local stationary assumption of \( x_t \) allows minimizing the mean square error (MSE) for the next sensor data point. The locally standard deviation factor is computed in \( r^{t+1} \).

We assumed the sensor data stream \( x_t \) to be a Gaussian signal normalized as follows: \( 0 < P_t < \frac{1}{\sqrt{2\pi}} \). It is assumed that \( \hat{x}_t = k(1 - bP_t) \) be the predicted data point over the \( r^t \) standard deviation at \( t \). The zero-mean normalization is performed over the random variable \( k_t = x_t - \hat{x}_t \) to find the probability estimate. Such normalization is useful for handling big data chunks where \( \frac{1}{\sqrt{2\pi}r} \) can augment \( P_t \to 0 \) and result in \( r \to 1 \) irrespective of \( x_t \).

The P-EWMA algorithm firstly trains the model based on the initial sensor data points of the whole time series data \( x_t \). It uses \( s_1 = x_1 \) and \( s_2 = \frac{x_2}{2} \) to predict the next sensor data point \( \hat{x}_2 = x_1 \). Normally, an EWMA scheme faces bias towards the beginning data points of the stream, requiring enough time to filter the anomalous data points. However, such issues can be resolved in the P-EWMA by deploying a time period \( T \) for which the model is evaluated to find the local mean and standard deviation over \( x_t \).





#SD-EWMA

Shift detection is a common issue in real-world time series data streams, necessitating appropriate adaptive corrective measures for timely tracking of abnormality. In non-stationary environments, assessing the state of stationarity to check shifts resulting from abnormal data generation processes poses several challenges. Traditional pattern classification concepts assume a stationary data distribution approach during both training and testing phases, known as the "stationary hypothesis." This approach is beneficial for validating stationary systems' fault detection and providing appropriate corrective measures by updating classifier tool parameterization.

However, identifying shifts under non-stationary systems requires an efficient learning model — non-stationary learning — to make the online learning process unaffected by abrupt changes in the dataset. Dealing with such non-stationary learning models requires handling classification tasks in real-time without prior information.



The SD-EWMA algorithm computes parametrized components during the training phase and checks each sample for shifts. If a shift is detected, it reports the situation for immediate corrective measures, resulting in shift-detection points. SD-EWMA can overcome the shortcomings of traditional statistical shift-detection techniques by minimizing processing delay and countering false alarms.

The SD-WEMA algorithm is capable of handling abrupt changes in the dataset shift, where there is a difference between the input and output of training and test phases. For instance, in a pattern classification scenario, a covariate shift (population shift) occurs under the XY problem domain when \( P_{\text{training}}(y, x) \neq P_{\text{testing}}(y, x) \) and \( P_{\text{training}}(x) \neq P_{\text{testing}}(x) \).

Fig. 1 presents a covariate shift where both \( P_{\text{training}}(x) \) and \( P_{\text{testing}}(x) \) are normally distributed, and \( x_{\text{training}} = N(x, 2.5, 3) \) and \( x_{\text{testing}} = N(x, 3.5, 3) \).

The EWMA equation in (1) facilitates 1-step-ahead forecasting over the auto-correlated dataset under the non-stationary system. Equation (3) describes the non-stationary scenario, where \( \epsilon_t \) presents an array of independently distributed random data stream. If we keep \( k = 1 - h \), then it converges to a 1-step-ahead forecasting process, as shown in Fig. 2.

\[ x_t = x_{t-1} + \epsilon_t \cdot h\epsilon_{t-1} \quad (3) \]

We can estimate the normally distributed mean-zero error \( \text{error}_t \) in the 1-step-ahead process as \( \text{error}_t = x_t - z_{t-1} \).

Fig. 2 depicts the control chart of EWMA, where \( L \) represents the control limit multiplier. At a 5% confidence interval of a standard deviation \( r_{\text{error}} \), the control limiting chart is defined by Equations (4)–(6).

\[ P(\text{error}) = -Lr_{\text{error}} \leq \text{error}_t \leq Lr_{\text{error}} \cdot (1 - \alpha) \quad (4) \]

\[ P(z) = t-1 - Lr_{\text{error}} \leq x_t - z_{t-1} \leq Lr_{\text{error}} \cdot (1 - \alpha) \quad (5) \]

\[ P(z) = t-1 - Lr_{\text{error}} \leq x_t \leq z_{t-1} + Lr_{\text{error}} \cdot (1 - \alpha) \quad (6) \]

The control chart is governed by the upper control limit (UCL) and lower control limit (LCL) as depicted in Equations (7) and (8), respectively.

\[ \text{UCL}_t = z_{t-1} + Lr_{\text{error}}^{t-1} \quad (7) \]

\[ \text{LCL}_t = z_{t-1} - Lr_{\text{error}}^{t-1} \quad (8) \]



The SD-WEMA algorithm works in two phases. In phase-I, it performs training where parametrized computations are performed to decide the null hypothesis under the stationary assumption. In phase-II, it finds the covariate shift from the online data stream, lying outside the UCL and LCL range on the control chart.

Covariate shifts trigger rejecting the null hypothesis and indicate the detection of abnormality. As assumed earlier that non-stationarity can only happen in the input data stream, covariate shift detection is performed within the non-stationary data stream.

In phase-I, various parameterized factors are computed within the \( k, z_0, r^2_{\text{error}}(0) \) domain. In phase-II, covariate shift is detected from the time series data. After obtaining the required data stream in the early stage of phase-I, the mean is assigned to \( z_0 \). Estimation of the smoothing factor \( k \) is done in the next stage, crucial for finding the 1-step-ahead forecasting error. Finally, at the end of phase-I, initialization of \( r^2_{\text{error}}(0) \) is done with the sum of squared errors divided by the length of the input data stream \( x \).

In phase-II, testing is done using Equation (1), and computing the \( UCL_t \) and \( LCL_t \) limits using Equations (7) and (8), respectively. It checks whether a data point \( x_t \) falls outside the limit, resulting in the detection of abnormality. An error smoothing constant \( \lambda \) is used to minimize the mean of shifts in the 1-step-ahead error.


