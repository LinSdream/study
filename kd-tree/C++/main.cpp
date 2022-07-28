#include<vector>
#include<algorithm>
#include"include/kd_tree.h"

using namespace std;

int main() 
{
	alg::Coordinate p1(2);
	p1[0] = 1; p1[1] = 2;

	alg::Coordinate p2(2);
	p2[0] = 5; p2[1] = 3;
	
	alg::Coordinate p3(2);
	p3[0] = 10; p3[1] = 2;

	alg::Coordinate p4(2);
	p4[0] = 2; p4[1] = 7;

	alg::Coordinate p5(2);
	p5[0] = 23; p5[1] = 9;
	
	alg::Coordinate p6(2);
	p6[0] = 11; p6[1] = 15;

	alg::Coordinate p7(2);
	p7[0] = 51; p7[1] = 102;

	alg::CoordinateArray arr(7);
	arr[0] = p1;
	arr[1] = p2;
	arr[2] = p3;
	arr[3] = p4;
	arr[4] = p5;
	arr[5] = p6;
	arr[6] = p7;
	alg::KdTree tree(arr);
	alg::CoordinateArray result = tree.inorderTraversal();
	for (int i = 0; i < result.size(); i++) {
		std::cout << "(" << result[i][0] << "," << result[i][1] << ")" << std::endl;
	}
	return 0;
}