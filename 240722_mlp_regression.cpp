#include <bits/stdc++.h>
#include <graphics.h>
#include <conio.h>

namespace f {
	inline double sigmoid(double x) {
		return 1.0 / (1.0 + exp(-x));
	}
	inline double F(double x) {
//		return 1 * sin(x * 1.8);
		if (x < -4 || x > 4) {
			return x;
		} else {
			return std::max(x, 3.0 / (1.0 + exp(x - 1)) + 0.2 * x * x * x - x + 1.0);
		}
	}
	std::vector<double> dot(std::vector<double> x, std::vector<double> y) {
		assert(x.size() == y.size());
		for (int i = 0; i < x.size(); i ++) {
			x[i] *= y[i];
		}
		return x;
	}
}

namespace l {
	struct base_layer {
		int in_size, out_size;
		std::vector<double> in, out, par;
		base_layer *pin = nullptr, *pout = nullptr;
		virtual void init(int _in_size, int _out_size) {
			in_size = _in_size, out_size = _out_size;
			in.clear(); out.clear(); par.clear();
			in.resize(in_size); out.resize(out_size); par.resize(out_size);
		};
		virtual signed get_in() {
			if (pin == nullptr) return 1;
			if (pin -> out.size() != in.size()) return 2;
			in = pin -> out;
			return 0;
		}
		virtual void forward() {return; };
		virtual void backward() {return; };
		virtual void update(double ita, int batch) {return; };
	};
	
	struct input_layer : base_layer {
		void set_input(double num) {
			out.clear();
			out.push_back(num);
		}
	};
	
	struct sigmoid_layer : base_layer {
		void forward() {
			for (int i = 0; i < in_size; i ++) {
				out[i] = f::sigmoid(in[i]);
			}
		}
		void backward() {
			for (int i = 0; i < in_size; i ++) {
				pin -> par[i] = par[i] * (out[i] * (1.0 - out[i]));
			}
		}
	};
	
	struct net_layer : base_layer {
		std::vector<std::vector<double> > w, gr;
		void init(int _in_size, int _out_size) {
			in_size = _in_size, out_size = _out_size;
			in.clear(); out.clear(); par.clear();
			in.resize(in_size); out.resize(out_size); par.resize(out_size);
			w.clear();
			w.resize(in_size);
			gr.clear();
			gr.resize(in_size);
			for (int i = 0; i < in_size; i ++) {
				w[i].resize(out_size);
				gr[i].resize(out_size);
				for (int j = 0; j < out_size; j ++) {
					w[i][j] = 1.0 * (rand() % 2001 - 1000) / 1000.0;
				}
			}
		}
		void forward() {
			for (int i = 0; i < out_size; i ++) {
				out[i] = 0;
				for (int j = 0; j < in_size; j ++) {
					out[i] += in[j] * w[j][i];
				}
			}
		}
		void backward() {
			for (int i = 0; i < in_size; i ++) {
				pin -> par[i] = 0;
				for (int j = 0; j < out_size; j ++) {
					pin -> par[i] += w[i][j] * par[j];
					gr[i][j] += in[i] * par[j];
				}
			}
		}
		void update(double ita, int batch) {
			for (int i = 0; i < out_size; i ++) {
				for (int j = 0; j < in_size; j ++) {
					w[j][i] -= ita * gr[j][i] / batch;
					gr[j][i] = 0;
				}
			}
		}
	};
	
	struct net {
		int layers = 0, height = 0, batch = 32;
		double ita = 0;
		input_layer *start = nullptr;
		net_layer *end = nullptr;
		void setup(int _layers, int _height, double _ita, double _batch) {
			layers = _layers, height = _height, batch = _batch;
			ita = _ita;
			int preh = 1;
			start = new input_layer;
			start -> init(0, 1);
			base_layer *prev_layer = start;
			for (int i = 0; i < layers; i ++) {
				net_layer *new_net_l = new net_layer;
				new_net_l -> init(preh, height);
				prev_layer -> pout = new_net_l;
				new_net_l -> pin = prev_layer;
				sigmoid_layer *sig_net_l = new sigmoid_layer;
				sig_net_l -> init(height, height);
				new_net_l -> pout = sig_net_l;
				sig_net_l -> pin = new_net_l;
				preh = height;
				prev_layer = sig_net_l;
			}
			end = new net_layer;
			end -> init(preh, 1);
			prev_layer -> pout = end;
			end -> pin = prev_layer;
		}
		double get_res() {
			return *(end -> out.begin());
		}
		double get_loss(double Tc) {
			double r = get_res();
			return 0.5 * (r - Tc) * (r - Tc);
		}
		void set_input(double x) {
			start -> set_input(x);
		}
		void work_forward() {
			for (base_layer *now = (start -> pout); now != nullptr; now = (now -> pout)) {
				now -> get_in();
				now -> forward();
//				std::cout << "\n=====\n";
//				for (double x : now -> out) {
//					std::cout << x << " ";
//				}
//				std::cout << "\n";
			}
		}
		void work_update() {
			for (base_layer *now = (start -> pout); now != nullptr; now = (now -> pout)) {
				now -> update(ita, batch);
			}
		}
		void work_backward(double Tc) {
			end -> par[0] = get_res() - Tc;
			for (base_layer *now = end; now != nullptr; now = (now -> pin)) {
				now -> backward();
			}
		}
	};
}

using l::net;

std::vector<std::pair<double, double> > test;

namespace w {
	void generate_tests(int TestSize) {
		test.clear();
		for (int i = 1; i <= TestSize; i ++) {
			double x = rand() % 2001 - 1000;
			x /= 250.0;
			test.push_back(std::make_pair(x, f::F(x)));
		}
	}
}

int main() {
	srand(time(0));
	int t_size = 102400;
	int batch = 64;
	w::generate_tests(t_size);
	std::cout << "Tests generated." << std::endl;
	
	net *G = new net;
	G -> setup(2, 30, 0.2, batch);
	std::cout << "Network set up." << std::endl;
	
	int tot_e = t_size / batch;
	for (int epoch = 0; epoch < tot_e; epoch ++) {
		double sum_loss = 0;
		for (int i = 0; i < batch; i ++) {
			int idx = epoch * batch + i;
			G -> set_input(test[idx].first);
			G -> work_forward();
			sum_loss += (G -> get_loss(test[idx].second));
			G -> work_backward(test[idx].second);
		}
		G -> work_update();
		if (epoch % 50 == 49 || epoch == 0) std::cout << "epoch[" << epoch + 1 << "/" << tot_e << "] loss: " << sum_loss / batch << std::endl;
	}
	
	return 0;
	initgraph(800, 800);
	setbkcolor(0x00ffffff);
	cleardevice();
	setfillcolor(0x00000000);
	setlinecolor(0x00000000);
	line(400, 0, 400, 800);
	line(0, 400, 800, 400);
	for (double x = -4; x <= 4; x += 0.1) {
		G -> set_input(x);
		G -> work_forward();
		double fx = G -> get_res();
		fillcircle(x * 100 + 400, (-fx) * 50 + 400, 5);
//		std::cout << "f(" << x << ") = " << fx << "\n";
	}
	_getch();
	return 0;
}
