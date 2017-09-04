#ifdef _DEBUG
//Debugモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300d.lib")            // opencv_core
#else
//Releaseモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300.lib") 
#endif

#include <iostream>
#include <vector>
#include <stdlib.h>

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

class Node{
public :
	vector<double> param;
	Node(){
		param = vector<double>(3);
	}
};

class SomLearn{
	int learning_range;
	double learning_rate;
public:
	vector< vector<Node> > map;
	SomLearn(int x, int y){
		map = vector< vector<Node> >(x, vector<Node>(y));
		InitMap();
	}
	int InitMap(){
		srand(10);
		for (int i = 0; i < map.size(); ++i){
			for (int j = 0; j < map[0].size(); ++j){
				for (int k = 0; k < map[0][0].param.size(); ++k){
					map[i][j].param[k] = double(rand() / double(RAND_MAX) * 255.0);
				}
			}
		}
		return 0;
	}
	double GetEuqridDinstance(Node data1, Node data2){
		double n = 0;
		for (int i = 0; i < data1.param.size(); ++i){
			n += (data2.param[i] - data1.param[i]) * (data2.param[i] - data1.param[i]);
		}
		return n;
	}
	int GetMinIndex(Node data, int &i_index, int &j_index){
		double min_val = 0;
		double temp = 0;
		i_index = 0;
		j_index = 0;

		min_val = GetEuqridDinstance(data, map[0][0]);
		for (int i = 0; i < map.size(); ++i){
			for (int j = 0; j < map[0].size(); ++j){
				temp = GetEuqridDinstance(data, map[i][j]);
				if (min_val > temp){
					min_val = temp;
					i_index = i;
					j_index = j;
				}
			}
		}
		return 0;
	}
	int Exposure(Node data){

		int i_index = 0;
		int j_index = 0;
		GetMinIndex(data, i_index, j_index);

		int i_small = i_index - learning_range;
		int i_big = i_index + 1 + learning_range;
		int j_small = j_index - learning_range;
		int j_big = j_index + 1 + learning_range;

		if (i_small < 0){ i_small = 0; }
		if (map.size() < i_big){ i_big = map.size(); }
		if (j_small < 0){ j_small = 0; }
		if (map[0].size()< j_big){ j_big = map[0].size(); }

		for (int i = i_small; i < i_big; ++i){
			for (int j = j_small; j < j_big; ++j){
				for (int k = 0; k < data.param.size(); ++k){
					map[i][j].param[k] += (data.param[k] - map[i][j].param[k]) * learning_rate;
				}
			}
		}
		return 0;
	}
	int Train(vector<Node> &node, int learning_range_in, double learning_rate_in){
		learning_range = learning_range_in;
		learning_rate = learning_rate_in;

		for (int i = 0; i < node.size(); ++i){
			Exposure(node[i]);
		}
		return 0;
	}
};

int main(){
	vector<Node> node(10000);
	srand(1000);
	for (int i = 0; i < node.size(); ++i){
		for (int k = 0; k < node[0].param.size(); ++k){
			node[i].param[k] = double(rand()) / double(RAND_MAX) * 255.0;
		}
	}

	int x = 30;
	int y = 30;
	int lrange = 6;
	double lrate = 0.01;

	SomLearn som(x, y);
	Mat src(Size(som.map.size(), som.map[0].size()), CV_8UC3, Scalar::all(0));
	Mat dst(Size(som.map.size(), som.map[0].size()), CV_8UC3, Scalar::all(0));

	for (int i = 0; i < som.map.size(); ++i){
		for (int j = 0; j < som.map[0].size(); ++j){
			for (int k = 0; k < som.map[0][0].param.size(); ++k){
				src.at<Vec3b>(Point(i, j))[k] = som.map[i][j].param[k];
			}
		}
	}

	som.Train(node, lrange, lrate);
	for (int i = 0; i < som.map.size(); ++i){
		for (int j = 0; j < som.map[0].size(); ++j){
			for (int k = 0; k < som.map[0][0].param.size(); ++k){
				dst.at<Vec3b>(Point(i, j))[k] = som.map[i][j].param[k];
			}
		}
	}



	//resize(src, src, Size(), 25, 25);
	//resize(dst, dst, Size(), 25, 25);
	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);

	return 0;
}