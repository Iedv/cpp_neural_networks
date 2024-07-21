#include <bits/stdc++.h>

const int TrainSize = 300;
const int TestSize = 100;

std::pair<double, double> train[TrainSize + 5], test[TestSize + 5];
double hat[TrainSize + 5];

struct gen {
    std::pair<double, double> samples[TrainSize + TestSize + 5];

    double func(double x) {
        return 167 * x + (rand() % 50) - 25;
    }

    void generate_samples() {
        for (int i = 1; i <= TrainSize + TestSize; i ++) {
            samples[i] = std::make_pair(i, func(i));
        }
    }

    void split() {
        for (int i = 1; i <= TrainSize; i ++) {
            train[i] = samples[i];
        }
        for (int i =  1; i <= TestSize; i ++) {
            test[i] = samples[TrainSize + i];
        }
    }
};

struct learn {
	double loss, K, alpha = 0.00001;
	
	double calc_loss() {
		double res = 0;
		for (int i = 1; i <= TrainSize; i ++) {
			res += std::pow(hat[i] - train[i].second, 2);
		}
		return sqrt(res / TrainSize);
	}
	
	double main() {
		int epoch = 100;
		K = rand() % 300;
		for (int _ = 1; _ <= epoch; _ ++) {
			double dep = 0;
			for (int i = 1; i <= TrainSize; i ++) {
				double x = train[i].first, y = train[i].second;
				hat[i] = K * train[i].first;
				dep += x * (x * K - y);
			}
			K = K - alpha * dep / TrainSize;
			std::cout << "[" << _ << "]: " << calc_loss() << "\n";
		}
		double ans = 0;
		for (int i = 1; i <= TestSize; i ++) {
			ans += pow(K * test[i].first - test[i].second, 2);
		}
		std::cout << "[ans]: " << K << " " << sqrt(ans / TestSize) << "\n";
	}
};

signed main() {
	srand(time(0));
    gen* G = new gen;
    G -> generate_samples();
    G -> split();
    learn* L = new learn;
    L -> main();
    return 0;
}
