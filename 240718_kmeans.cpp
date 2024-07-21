#include <bits/stdc++.h>
#include <graphics.h>
#include <conio.h>

const int N = 1e5 + 2;

int n, K;
std::pair<int, int> point[N];
std::pair<double, double> mean[N];
int rlist[N], clr[N];
std::vector<int> cl[N];

inline long long i_dist(std::pair<int, int> x, std::pair<int, int> y) {
	return (x.first - y.first) * (x.first - y.first) + (x.second - y.second) * (x.second - y.second);
}

inline double f_dist(std::pair<int, int> x, std::pair<double, double> y) {
	return (1.0 * x.first - y.first) * (1.0 * x.first - y.first) + (1.0 * x.second - y.second) * (1.0 * x.second - y.second);
}

void draw() {
	for (int i = 0; i < K; i ++) {
//		std::cout << std::hex << clr[i] << " " << std::dec << cl[i].size() << "\n";
		setfillcolor(clr[i]);
		setlinecolor(clr[i]);
		for (int x : cl[i]) {
			fillcircle(point[x].first, point[x].second, 4);
		}
	}
}

signed main() {
	std::cout << "Total amount of points n:" << std::endl;
	std::cin >> n;
	std::cout << "Counts of class K:" << std::endl;
	std::cin >> K;
	srand(time(0));
	std::cout << "Input points manually (a) or Generate Randomly (b):" << std::endl;
	char _in;
	std::cin >> _in;
	if (_in == 'a') {
		for (int i = 0; i < n; i ++) {
			std::cout << "Point (x_i, y_i) " << i + 1 << "/" << n << " :" << std::endl;
			std::cin >> point[i].first >> point[i].second;
		}
	} else if (_in == 'b') {
		for (int i = 0; i < n; i ++) {
			point[i].first = rand() % 1000;
			point[i].second = rand() % 1000;
		}
	} else {
		std::cout << "Input Error";
		return 0;
	}
	std::sort(point, point + n);
	n = std::unique(point, point + n) - point;
	if (n < K) {
		std::cout << "Data Error\nTry again or Try a smaller K!";
		return 0;
	}
	
	initgraph(1000, 1000);
	setbkcolor(0x00ffffff);
	long long rgb_l = 1 << 24;
	for (int i = 0; i < K; i ++) {
		clr[i] = 1ll * rand() * rand() * rand() % (rgb_l - i);
	}
	std::sort(clr, clr + K);
	for (int i = 1; i < K; i ++) {
		if (clr[i] == clr[i - 1]) clr[i] ++;
	}
	
	for (int i = 0; i < K; i ++) {
		rlist[i] = rand() % (n - i);
	}
	std::sort(rlist, rlist + K);
	for (int i = 1; i < K; i ++) {
		if (rlist[i] == rlist[i - 1]) rlist[i] ++;
	}
	for (int i = 0; i < n; i ++) {
		int res = 0;long long now = 1e9;
		for (int j = 0; j < K; j ++) {
			long long ndist = i_dist(point[i], point[rlist[j]]);
			if (ndist < now) {
				res = j;
				now = ndist;
			}
		}
		cl[res].push_back(i);
	}
	cleardevice();
	draw();
	std::cout << "Start\n";
	_getch();
	int rep = 100000000 / n / K;
	while (rep --) {
		for (int i = 0; i < K; i ++) {
			double mx = 0, my = 0;
			for (int x : cl[i]) {
				mx += point[x].first;
				my += point[x].second;
			}
			mean[i].first = mx / cl[i].size();
			mean[i].second = my / cl[i].size();
			cl[i].clear();
		}
		for (int i = 0; i < n; i ++) {
			int res = 0;double now = 1e9;
			for (int j = 0; j < K; j ++) {
				long long ndist = f_dist(point[i], mean[j]);
				if (ndist < now) {
					res = j;
					now = ndist;
				}
			}
			cl[res].push_back(i);
		}
//		cleardevice();
//		draw();
//		_getch();
	}
	
	cleardevice();
	draw();
	std::cout << "End\n";
	_getch();
	closegraph();
	return 0;
}

