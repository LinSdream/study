#include<vector>
#include<list>
#include<algorithm>
#include<math.h>
#include"include/kd_tree.h"

using namespace std;

/// <summary>
/// 两点求距
/// </summary>
/// <returns></returns>
float distance(const alg::Coordinate& cur, const alg::Coordinate& target)
{
	// 两点间距离
	return sqrtf(powf((cur[0] - target[0]), 2) + powf((cur[1] - target[1]), 2));
}

int main() 
{
	alg::Coordinate p1(2);
	p1[0] = 6.27; p1[1] = 5.5;

	alg::Coordinate p2(2);
	p2[0] = 1.24; p2[1] = -2.86;
	
	alg::Coordinate p3(2);
	p3[0] = 17.05; p3[1] = -12.79;

	alg::Coordinate p4(2);
	p4[0] = -6.88; p4[1] = 5.40;

	alg::Coordinate p5(2);
	p5[0] = -2.96; p5[1] = -0.5;
	
	alg::Coordinate p6(2);
	p6[0] = 7.75; p6[1] = -22.68;

	alg::Coordinate p7(2);
	p7[0] = 10.80; p7[1] = -5.03;

	alg::Coordinate p8(2);
	p8[0] = -4.60; p8[1] = -10.55;

	alg::Coordinate p9(2);
	p9[0] = -4.96; p9[1] = 12.61;

	alg::Coordinate p10(2);
	p10[0] = 1.75; p10[1] = 12.26;

	alg::Coordinate p11(2);
	p11[0] = 15.31; p11[1] = -13.16;

	alg::Coordinate p12(2);
	p12[0] = 7.83; p12[1] = 15.70;

	alg::Coordinate p13(2);
	p13[0] = 14.63; p13[1] = -0.35;

	alg::CoordinateArray arr(13);
	arr[0] = p1;
	arr[1] = p2;
	arr[2] = p3;
	arr[3] = p4;
	arr[4] = p5;
	arr[5] = p6;
	arr[6] = p7;
	arr[7] = p8;
	arr[8] = p9;
	arr[9] = p10;
	arr[10] = p11;
	arr[11] = p12;
	arr[12] = p13;
	alg::KdTree tree(arr);
	alg::CoordinateArray result = tree.inOrderTraversal();
	for (int i = 0; i < result.size(); i++) {
		std::cout << "(" << result[i][0] << "," << result[i][1] << ")" << std::endl;
	}

	alg::Coordinate point(2);
	point[0] = -1; point[1] = -5;
	auto r= tree.find(point, 3);
	std::cout << "Result:" << std::endl;
	for (int i = 0; i < r.size(); i++) {
		std::cout << "(" << r[i][0] << "," << r[i][1] << ")" << std::endl;
	}

	vector<vector<float>> tempList;

	for (int i = 0; i < arr.size(); i++) {	
		tempList.push_back(arr[i]);
	}

	std::sort(tempList.begin(), tempList.end(), [&](vector<float> a, vector<float> b) {
		float d1 = distance(point, a);
		float d2 = distance(point, b);
		return d1 < d2;
		});

	std::cout << "Check:" << std::endl;
	for (int i = 0; i < tempList.size(); i++) {
		std::cout << "(" << tempList[i][0] << "," << tempList[i][1] << ")" << std::endl;
	}

	return 0;
}