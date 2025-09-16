#include "gtest/gtest.h"
#include "Matrix.h"
#include "Matrix.cpp"
#include "chrono"

using namespace std::chrono;

TEST(correctness, operator_equal) {
	Matrix<2, 2, int> m1({ {1,2}, {1,2} });
	Matrix<2, 2, int> m2({ {1,2}, {1,2} });
	Matrix<2, 2, int> m3({ {1,3}, {1,2} });
	Matrix<2, 2, double> m4({ {1.0, 2.0}, {1.0, 2.0} });
	Matrix<1, 2, int>m5({ {1,2} });
	EXPECT_TRUE(m1 == m2);
	EXPECT_TRUE(m1 == m4);
	EXPECT_FALSE(m1 == m3);
	//EXPECT_FALSE(m1 == m5); // Should not compile
}

TEST(correctness, operator_not_equal) {
	Matrix<2, 2, int> m1({ {1,2}, {1,2} });
	Matrix<2, 2, int> m2({ {1,3}, {1,2} });
	Matrix<2, 2, int> m3({ {1,2}, {1,2} });
	Matrix<1, 2, int>m5({ {1,2} });

	EXPECT_TRUE(m1 != m2);
	EXPECT_FALSE(m1 != m3);
	//	EXPECT_FALSE(m1 != m5); // Should not compile

}

TEST(correctness, operator_plus_equal) {
	Matrix<2, 2, int> m1({ {1,2}, {1,2} });
	Matrix<2, 2, int> m2({ {1,2}, {1,2} });
	Matrix<2, 2, double> m3({ {1.0,2.0}, {1.0,2.0} });

	Matrix<2, 2, int> test({ {2,4}, {2,4} });
	m1 += m2;
	EXPECT_EQ(m1, test);

	m2 += m3;
	EXPECT_EQ(m2, test);
}

TEST(correctness, operator_plus_external) {
	Matrix<2, 2, int> m1({ {1,2}, {1,2} });
	Matrix<2, 2, double> m2({ {1.0,2.0}, {1.0, 2.0} });
	Matrix<1, 2, int>m3({ {1,2} });

	Matrix<2, 2, double> test({ {2.0, 4.0}, {2.0, 4.0} });
	EXPECT_EQ(m1 + m2, test);
	EXPECT_TRUE(typeid(m1 + m2) == typeid(test));
}

TEST(correctness, operator_minus_equal) {
	Square_matrix<2, int> m({ {2,4}, {2,4} });
	Square_matrix<2, double>m2({ {1,2}, {1,2} });
	Matrix<1, 2, double> m3({ {1,2} });

	Square_matrix<2, int> test({ {1,2}, {1,2} });
	EXPECT_EQ(m -= m2, test);
	EXPECT_TRUE(typeid(m) == typeid(test));
	//m -= m3; // Should not compile
}

TEST(correctness, operator_multy_equal) {
	Matrix<2, 2, int> m1({ {1,2}, {1,2} });
	Matrix<2, 2, double> m2({ {1,2}, {1,2} });
	Matrix<1, 2, int> m3({ {1,2} });

	Square_matrix<2, int> test({ {3,6}, {3,6} });

	EXPECT_EQ(m1 *= m2, test);
	EXPECT_TRUE(typeid(m1 *= m2) == typeid(test));
	//m1 *= m3; // Should not compile
}

TEST(correctness, operator_multy_int) {
	Square_matrix<2, int> m({ {1,2}, {1,2} });

	Square_matrix<2, int> test({ {3,6}, {3,6} });
	EXPECT_EQ(m *= 3, test);
	EXPECT_TRUE(typeid(m *= 3) == typeid(m));
}

TEST(correctness, matrix_sum) {
	const Matrix<2, 2, int> m({ {1,2}, {3,4} });
	const Matrix_expression<Matrix<2, 2, int>>& matr1 = m;

	Square_matrix<2, int> m2({ {1,2}, {3,4} });
	Matrix_expression< Square_matrix<2, int>>& matr2 = m2;

	Square_matrix<2, int> m3({ {1,2}, {3,4} });
	Matrix_expression<Square_matrix<2, int>>& matr3 = m3;

	Square_matrix<2, double> m4({ {1,2}, {3,4} });
	Matrix_expression<Square_matrix<2, double>>& matr4 = m4;

	Matrix<1, 2, int> m5({ 1,2 });
	Matrix_expression<Matrix<1, 2, int>>& matr5 = m5;

	auto res = matr1 + matr2 + matr3 + matr4;
	EXPECT_TRUE(res[1][1] == 16);
	EXPECT_TRUE(res[0][0] == 4);

	//auto res2 = matr1 + matr2 + matr3 + matr4 + matr5;
	//EXPECT_TRUE(res2[1][1] == 18); // Should not compile
}

TEST(correctness, matrix_multiply) {
	Matrix<2, 2, int> m1({ {1,2}, {1,2} });
	Matrix_expression<Matrix<2, 2, int>>& matr1 = m1;

	Matrix<2, 2, int> m2({ {1,2,}, {1,2} });
	Matrix_expression<Matrix<2, 2, int>>& matr2 = m2;

	Matrix<2, 2, int> m3({ {1,2},{1,2} });
	Matrix_expression<Matrix<2, 2, int>>& matr3 = m3;

	auto mult = matr1 * matr2 * matr3;
	EXPECT_TRUE(mult[0][1] == 18);

	Matrix_multiply<Matrix<2, 2, int>, Matrix<2, 2, int>> mu1(m1, m2);
	Matrix_multiply<Matrix<2, 2, int>, Matrix<2, 2, int>> mu2(m1, m2);
	auto mult2 = mu1 * mu2;
	EXPECT_TRUE(mult2[0][1] == 54);

	Matrix_multiply<Matrix<2, 2, int>, Matrix_multiply<Matrix<2, 2, int>, Matrix<2, 2, int>>> temp(m1, mu1);
	Matrix_multiply<Matrix<2, 2, int>, Matrix_multiply<Matrix<2, 2, int>, Matrix_multiply<Matrix<2, 2, int>, Matrix<2, 2, int>>>> mu3(m1, temp);
	auto mult3 = mu3;
	EXPECT_TRUE(mult3[0][1] == 54);

	Matrix<1, 2, int> matr({ {1,2} });
	//auto mult4 = mu1 * mu2 * matr; // Should not compile
}

TEST(correctness, add) {
	Square_matrix<2, int> m1({ {1,2}, {1,2} });
	Square_matrix<2, double> m2({ {1,2}, {1,2} });
	Square_matrix<2, int> m3({ {1,2}, {1,2} });

	auto res = add(m1, m2, m3);
	Square_matrix<2, double> test({ {3,6}, {3,6} });
	EXPECT_EQ(res, test);
	EXPECT_TRUE(typeid(res) == typeid(test));

	Matrix<1, 2, int> m4({ {1,2} });
	//auto res2 = add(m1, m2, m3, m4);// Should not compile 
}

TEST(correctness, substract) {
	Square_matrix<2, int> m({ {8,8}, {8,8} });
	Square_matrix<2, int> m1({ {1,2}, {1,2} });
	Square_matrix<2, double> m2({ {1,2}, {1,2} });
	Square_matrix<2, int> m3({ {1,2}, {1,2} });

	auto res = substract(m, m1, m2, m3);
	Square_matrix<2, double> test({ {5,2}, {5,2} });
	EXPECT_EQ(res, test);

	Matrix<1, 2, int> m4({ {1,2} });
	//auto res2 = substract(m, m1, m2, m3, m4);  // Should not compile
}

TEST(correctness, minor) {
	Square_matrix<3, int> m({ {1,2,3}, {1,2,3}, {1,2,3} });

	Square_matrix<2, int> minor1({ {2,3},{2,3} });
	Square_matrix<2, int> minor2({ {1,3}, {1,3} });
	EXPECT_EQ(m.minor(0, 0), minor1);
	EXPECT_EQ(m.minor(1, 1), minor2);
}

TEST(correctness, rank) {
	Matrix<1, 5, int> m({ {0,0,0,1,0} });
	EXPECT_TRUE(m.rank() == 1);

	Matrix<1, 5, int> m2({ {0,0,0,0,0} });
	EXPECT_TRUE(m2.rank() == 0);

	Matrix<3, 3, int> m3({ {1,2,3},{2,4,6},{4,8,12} });
	EXPECT_TRUE(m3.rank() == 1);

	Matrix<4, 3, int> m4({ {1,2,3},{2,4,6},{4,8,12} ,{5,32,5} });
	EXPECT_TRUE(m4.rank() == 2);
}


TEST(correctness, determinant) {
	Matrix<3, 3, double> m2({ {2.5,2.7,5.6}, {6.3,4.2,2.6}, {4.3, 3.5,7.6} });
	EXPECT_DOUBLE_EQ(m2.det(), -19.696);

	Square_matrix<3, int> m3({ {0,4,6}, {6,5,3}, {3,4,6} });
	EXPECT_TRUE(m3.det() == 0);

	Square_matrix<3, int> m4({ {2,4,5}, {3,-1,2}, {-4,1,1} });
	EXPECT_TRUE(m4.det() == -55);

	Square_matrix<3, int> m5({ {1,2,1}, {2,1,2}, {2,3,3} });
	EXPECT_TRUE(m5.det() == -3);
}


TEST(correctness, inverted) {
	Square_matrix<3, int> m2({ {1,2,1}, {2,1,2}, {2,3,3} });
	Square_matrix<3, int> test({ {1,0,0}, {0,1,0}, {0,0,1} });
	EXPECT_EQ(m2.invert() * m2, test);
} 

TEST(correctness, transpous) {
	Square_matrix<3, int> m({ {1,2,3}, {4,5,6}, {7,8,9} });

	Square_matrix<3, int> test({ {1,4,7}, {2,5,8},{3,6,9} });
	EXPECT_TRUE(m.transpouse() == test);
}

TEST(correctness, trace) {
	Square_matrix<3, int> m({ {1,2,3}, {4,5,6}, {7,8,9} });
	EXPECT_TRUE(m.trace() == 15);
}

TEST(time_measuring, time_measuring_add) {
	Square_matrix<2, int> m1({ {1,2}, {1,2} });
	Square_matrix<2, int> m2({ {1,2}, {1,2} });
	Square_matrix<2, int> m3({ {1,2}, {1,2} });

	auto start = high_resolution_clock::now();
	auto res = add(m1, m2, m3);
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << "Time taken by 'add' function: " << duration.count() << " microseconds" << '\n';

	auto start_2 = high_resolution_clock::now();
	Square_matrix<2, int> res_2 = m1 + m2 + m3;
	auto stop_2 = high_resolution_clock::now();

	auto duration_2 = duration_cast<microseconds>(stop_2 - start_2);
	std::cout << "Time taken by opeator '+': " << duration_2.count() << " microseconds" << '\n';

	Square_matrix<2, int> test({ {3,6}, {3,6} });
	EXPECT_EQ(res, test);
	EXPECT_EQ(res_2, test);
}

TEST(time_measuring, time_measuring_sub) {
	Square_matrix<2, int> m({ {8,8}, {8,8} });
	Square_matrix<2, int> m1({ {1,2}, {1,2} });
	Square_matrix<2, int> m2({ {1,2}, {1,2} });
	Square_matrix<2, int> m3({ {1,2}, {1,2} });

	auto start = high_resolution_clock::now();
	auto res = substract(m, m1, m2, m3);
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);

	std::cout << "Time taken by 'substract' function: " << duration.count() << " microseconds" << '\n';

	auto start_2 = high_resolution_clock::now();
	Square_matrix<2, int> res_2 = m - m1 - m2 - m3;
	auto stop_2 = high_resolution_clock::now();

	auto duration_2 = duration_cast<microseconds>(stop_2 - start_2);

	std::cout << "Time taken by operator '-': " << duration_2.count() << " microseconds" << '\n';

	Square_matrix<2, double> test({ {5,2}, {5,2} });
	EXPECT_EQ(res, test);
	EXPECT_EQ(res_2, test);
}

TEST(time_measuring, time_subscript_operator_in_sum) {
	const Matrix<2, 2, int> m({ {1,2}, {3,4} });
	const Matrix_expression<Matrix<2, 2, int>>& matr1 = m;

	Square_matrix<2, int> m2({ {1,2}, {3,4} });
	Matrix_expression< Square_matrix<2, int>>& matr2 = m2;

	Square_matrix<2, int> m3({ {1,2}, {3,4} });
	Matrix_expression<Square_matrix<2, int>>& matr3 = m3;

	Square_matrix<2, int> m4({ {1,2}, {3,4} });
	Matrix_expression<Square_matrix<2, int>>& matr4 = m4;

	auto start = high_resolution_clock::now();
	auto res = matr1 + matr2 + matr3 + matr4;
	int item = res[1][1];
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);

	std::cout << "Time taken by opeator '[][]': " << duration.count() << " microseconds" << '\n';

	EXPECT_TRUE(item == 16);

	auto start_2 = high_resolution_clock::now();
	Square_matrix<2, int> res_2 = m + m2 + m3 + m4;
	int item_2 = res_2[1][1];
	auto stop_2 = high_resolution_clock::now();

	auto duration_2 = duration_cast<microseconds>(stop_2 - start_2);

	std::cout << "Time taken by operator '[][]' in compile - time: " << duration_2.count() << " microseconds" << '\n';

	EXPECT_TRUE(item_2 == 16);	
}

TEST(time_measuring, time_subscript_operator_in_multiply) {
	Matrix<2, 2, int> m1({ {1,2}, {1,2} });
	Matrix_expression<Matrix<2, 2, int>>& matr1 = m1;

	Matrix<2, 2, int> m2({ {1,2,}, {1,2} });
	Matrix_expression<Matrix<2, 2, int>>& matr2 = m2;

	Matrix<2, 2, int> m3({ {1,2},{1,2} });
	Matrix_expression<Matrix<2, 2, int>>& matr3 = m3;

	auto start = high_resolution_clock::now();
	auto mult = matr1 * matr2 * matr3;
	int item = mult[0][1];
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);

	std::cout << "Time taken by operator '[][]': " << duration.count() << "microseconds" << '\n';

	EXPECT_TRUE(item == 18);

	auto start_2 = high_resolution_clock::now();
	Square_matrix<2, int> res_2 = m1 * m2 * m3;
	int item_2 = res_2[0][1];
	auto stop_2 = high_resolution_clock::now();

	auto duration_2 = duration_cast<microseconds>(stop_2 - start_2);

	std::cout << "Time taken by operator '[][]' in compile - time: " << duration_2.count() << "microseconds" << '\n';
	EXPECT_TRUE(item_2 == 18);
}

TEST(correctness, add_big_integer) {
	big_integer a = "10000000000000000000000000000000000000000";
	big_integer b = "20000000000000000000000000000000000000000";
	big_integer c = "30000000000000000000000000000000000000000";
	big_integer d = "40000000000000000000000000000000000000000";

	big_integer e = "60000000000000000000000000000000000000000";
	big_integer f = "80000000000000000000000000000000000000000";

	Square_matrix<2, big_integer> matr({ {a,b}, {c,d} });
	Square_matrix<2, big_integer> matr_2({ {a, b}, {c, d} });

	Square_matrix<2, big_integer> res = matr + matr_2;

	Square_matrix<2, big_integer> test({ {b,d}, {e, f} });
	EXPECT_EQ(test, res);
}

TEST(correctness, mul_big_integer) {
	big_integer a = "10000000000000000000000000000000000000000";
	big_integer b = "20000000000000000000000000000000000000000";
	big_integer c = "30000000000000000000000000000000000000000";
	big_integer d = "40000000000000000000000000000000000000000";

	big_integer e = "700000000000000000000000000000000000000000000000000000000000000000000000000000000";
	big_integer f = "1000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	big_integer g = "1500000000000000000000000000000000000000000000000000000000000000000000000000000000";
	big_integer h = "2200000000000000000000000000000000000000000000000000000000000000000000000000000000";

	Square_matrix<2, big_integer> matr({ {a, b}, {c, d} });
	Square_matrix<2, big_integer> matr_2({ {a, b}, {c, d} });

	Square_matrix<2, big_integer> res = matr * matr_2;

	Square_matrix<2, big_integer> test({ {e, f}, {g, h} });
	EXPECT_EQ(test, res);
}

TEST(correctness, add_rational) {
	rational a = "10000000000000000000000000000000000000000.0000000000000000000000000000001";
	rational b = "20000000000000000000000000000000000000000.0000000000000000000000000000002";
	rational c = "30000000000000000000000000000000000000000.0000000000000000000000000000003";
	rational d = "40000000000000000000000000000000000000000.0000000000000000000000000000004";

	rational e = "60000000000000000000000000000000000000000.0000000000000000000000000000006";
	rational f = "80000000000000000000000000000000000000000.0000000000000000000000000000008";

	Square_matrix<2, rational> matr({ {a,b}, {c,d} });
	Square_matrix<2, rational> matr_2({ {a, b}, {c, d} });

	Square_matrix<2, rational> res = matr + matr_2;

	Square_matrix<2, rational> test({ {b,d}, {e, f} });
	EXPECT_EQ(test, res);
}

TEST(correctness, mul_rational) {
	rational a = "10000000000000000000000000000000000000000.00000000000000000000001";
	rational b = "20000000000000000000000000000000000000000.00000000000000000000002";
	rational c = "30000000000000000000000000000000000000000.00000000000000000000003";
	rational d = "40000000000000000000000000000000000000000.00000000000000000000004";

	rational e = "700000000000000000000000000000000000000000000000000000000000001400000000000000000.0000000000000000000000000000000000000000000007";
	rational f = "1000000000000000000000000000000000000000000000000000000000000002000000000000000000.0000000000000000000000000000000000000000000010";
	rational g = "1500000000000000000000000000000000000000000000000000000000000003000000000000000000.0000000000000000000000000000000000000000000015";
	rational h = "2200000000000000000000000000000000000000000000000000000000000004400000000000000000.0000000000000000000000000000000000000000000022";

	Square_matrix<2, rational> matr({ {a, b}, {c, d} });
	Square_matrix<2, rational> matr_2({ {a, b}, {c, d} });

	Square_matrix<2, rational> res = matr * matr_2;

	Square_matrix<2, rational> test({ {e, f}, {g, h} });
	EXPECT_EQ(test, res);
}

