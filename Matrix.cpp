#include <vector>
#include <iostream>
#include <cassert>
#include <type_traits>
#include <cstdio>
#include <utility>
#include "Matrix.h"

template <size_t N, size_t M, typename T, typename U>
std::vector<std::common_type_t<T,U>> operator*(const std::vector<T>& vec, const Matrix<N, M, U>& matr) {
	if (vec.size() != N)
		throw(std::invalid_argument("vector size not equal to count a row of matrix"));

	std::vector<std::common_type_t<T, U>> temp(M);
	for (size_t i = 0; i < M; ++i)
		for (size_t k = 0; k < N; ++k) {
			temp[i] += vec[k] * matr[k][i];
		}
	return temp;
}

template <size_t N, size_t M, typename T>
Get_matrix<Matrix<N, M, T>>::Get_matrix(const Matrix<N, M, T>& matr) : data(matr) {}

template <size_t N, size_t M, typename T>
const Matrix<N, M, T>& Get_matrix<Matrix<N, M, T>>::get() const {
	return data;
}

template <typename E1, typename E2>
Get_matrix<Matrix_multiply<E1, E2>>::Get_matrix(const Matrix_multiply<E1, E2>& matr) {
	data = Get_matrix<E1>(matr.e1_).get() * Get_matrix<E2>(matr.e2_).get();
}

template <typename E1, typename E2>
auto Get_matrix<Matrix_multiply<E1, E2>>::get() const ->
Matrix<Sizes<E1>::value_N, Sizes<E2>::value_M,
	std::common_type_t<get_type_t<E1>, get_type_t<E2>>> {
	return data;
}
 
template <typename T> 
return_type_t<T> Matrix_expression<T>::operator[](size_t i) const {
	return static_cast<const T&>(*this)[i];
}

template <typename E1, typename E2>
Matrix_pair<E1, E2>::Matrix_pair(const E1& e1, const E2& e2) : data(e1,e2) {}

template <typename E1, typename E2>
auto Matrix_pair<E1, E2>::operator[](size_t j) const ->
std::common_type_t<get_type_t<E1>, get_type_t<E2>> {
	return data.first[j] + data.second[j];
}

template <typename E1, typename E2>
Matrix_sum<E1, E2>::Matrix_sum(const E1& e1, const E2& e2) : e1_(e1), e2_(e2) {
	static_assert(Verify_dimension<E1, E2>::value, 
		"Summ of matrix can perform only for matrix with same dimension");
}

template <typename E1, typename E2>
auto Matrix_sum<E1, E2>::operator[](size_t i) const -> 
Matrix_pair<return_type_t<E1>, return_type_t<E2>> {
	return Matrix_pair<return_type_t<E1>, return_type_t<E2>>(e1_[i], e2_[i]);
}

template <typename T, typename E1, typename E2>
auto operator*(const std::vector<T>& vec, const Matrix_multiply<E1, E2>& matr) ->
std::vector<std::common_type_t<T, get_type_t<E1>, get_type_t<E2>>> {
	return vec * (Get_matrix<E1>(matr.e1_).get() * Get_matrix<E2>(matr.e2_).get());
}

template <typename E1, typename E2>
Matrix_multiply<E1, E2>::Matrix_multiply(const E1& e1, const E2& e2) : e1_(e1), e2_(e2) {
	static_assert(Verify_dimension_for_multiply<E1, E2>::value,
		"Count a col of first matrix must be equal to count a row of second matrix");
}

template <typename E1, typename E2>
auto Matrix_multiply<E1, E2>::operator[](size_t i) const ->
std::vector<std::common_type_t<get_type_t<E1>, get_type_t<E2>>>{
	std::vector<std::common_type_t<get_type_t<E1>, get_type_t<E2>>> temp(Sizes<E1>::value_M);
	return e1_[i] * e2_;
}

template <typename E1, typename E2>
Matrix_sum<E1,E2> operator+(const Matrix_expression<E1>& m1, const Matrix_expression<E2>& m2) {
	Matrix_sum summ(*static_cast<const E1*>(&m1), *static_cast<const E2*>(&m2));
	return summ;
}

template <typename E1, typename E2>
Matrix_multiply<E1, E2> operator*(const Matrix_expression<E1>& m1, const Matrix_expression<E2>& m2) {
	Matrix_multiply mult(*static_cast<const E1*>(&m1), *static_cast<const E2*>(&m2));
	return mult;
}

template <size_t N, size_t M, typename T>
Matrix<N,M,T>::Matrix() : data(2,std::vector<T>(2)) {}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(const std::vector<std::vector<T>>& vec) : data(vec) {}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(const Matrix<N, M, T>& other) try: data(other.data) 
{}
catch (...) {
	std::cout << "exception in ctor\n";
	data.clear();
	*this = Matrix<N, M, T>();
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(size_t a, size_t b) : data(a, std::vector<T>(b)) {}

template <size_t N, size_t M, typename T>
const std::vector<T>& Matrix<N, M, T>::get_row(size_t i) const {
	return data[i];
}

template <size_t N, size_t M, typename T>
std::vector<T> Matrix<N, M, T>::get_col(size_t i) const {
	std::vector<T> temp(N);
	for (size_t k = 0; k < N; ++k)
		temp[k] = data[k][i];
	return temp;
}

template <size_t N, size_t M, typename T>
T Matrix<N, M, T>::trace() const {
	static_assert(N == M, "trace is summ of elements at main diagonal of square matrix");
	T result = 0;
	for (size_t i = 0; i < N; ++i) 
		result += data[i][i];

	return result;
}

template <size_t N, size_t M, typename T>
Matrix<N - 1, N - 1, T> Matrix<N, M, T>::minor(size_t row, size_t col) const {
	static_assert(N == M);
	Matrix<N - 1, N - 1, T> result(N - 1, N - 1);
	size_t k = 0;
	size_t d = 0;
	for (size_t n = 0; n < N; ++n) {
		if (n == row)
			continue;
		for (size_t m = 0; m < N; ++m) {
			if (m == col)
				continue;
			result[k][d] = data[n][m];
			++d;
		}
		d = 0;
		++k;
	}
	return result;
}

template <size_t N, size_t M, typename T>
size_t Matrix<N, M, T>::rank() const {
	Matrix<N, M, T> temp = *this;
	int rank = std::min(N, M);
	int count = M;
	size_t i = 0;
	
	for (; i < rank; ++i) {
		if (temp[i][i]) {
			for (int k = 0; k < N; ++k) {
				if (i != k) {
					double mult = static_cast<double>(temp[k][i]) / temp[i][i];
					for (int z = 0; z < M; ++z)
						temp[k][z] -= (temp[i][z] * mult);
				}
			}
		}
		else if (count - 1 != i){
			bool reduce = true;
			for (int h = i + 1; h < N; ++h) {
				if (temp[h][i]) {
					reduce = false;
					temp.swap_row(i, h, M);
					break;
				}
			}
			if (reduce) {
				--count;
				for (int f = 0; f < N; ++f)
					temp[f][i] = temp[f][count];
			}
			--i;
		}
		else {
			break;
		}
	}
	
	return i;
}

template <size_t N, size_t M, typename T>
double Matrix<N, M, T>::det() const {
	Matrix<N, M, double> temp(N,M);
	temp = *this;
	static_assert(N == M, "Determinant can evaluate only for square matrix");

	double det = 1.0;

	for (int i = 0; i < N; ++i) {
		int item = i;
		if (temp[i][i]) {
			for (int j = i + 1; j < N; ++j) {
				if (ABS(temp[j][i] > ABS(temp[item][i])))
					item = j;
			}
		}

		if (item != i) {
			temp.swap_row(i, item, N);
			det *= -1;
		}

		if (temp[i][i] == 0)
			return 0;
		det *= temp[i][i];

		for (int k = i + 1; k < N; ++k) {
			double mult = static_cast<double>(temp[k][i]) / temp[i][i];
			for (int z = i; z < N; ++z)
				temp[k][z] -= mult * temp[i][z];
		}
	}

	return det;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, double> Matrix<N,M,T>::invert() const {
	static_assert(N == M, "Matrix must be square");
	double deter = det();

	if (deter == 0)
		throw std::invalid_argument("Determinant equal to zero");

	Matrix<N, M, double> temp(N,M);
	temp = *this;
	int count = 1;
	for (size_t i = 0; i < N; ++i)
		for (size_t k = 0; k < N; ++k) {
			temp[i][k] = minor(i,k).det() * count;
			count = -count;
		}

	double item = 1.0 / deter;
	return temp.transpouse() * item;
}

template <size_t N, size_t M, typename T>
void Matrix<N,M,T>::swap_row(size_t row_1, size_t row_2, size_t col) {
	for (size_t i = 0; i < col; ++i) {
		T temp = data[row_1][i];
		data[row_1][i] = data[row_2][i];
		data[row_2][i] = temp;
	}
}

template <size_t N, size_t M, typename T> 
Matrix<N, M, T>& Matrix<N, M, T>::operator*=(int item) {
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < M; ++j)
			data[i][j] *= item;
	return *this;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator*=(double item) {
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < M; ++j)
			data[i][j] *= item;
	return *this;
}

template <size_t N, size_t M, typename T> template <size_t K, typename U>
Matrix<N, M, T>& Matrix<N, M, T>::operator*=(const Matrix<M, K, U>& matr) {
	for (size_t n = 0; n < N; ++n)
		for (size_t k = 0; k < K; ++k)
			for (size_t m = 0; m < M; ++m)
				data[n][k] += matr[m][k];
	return *this;
}

template <size_t N, size_t M, typename T> template <typename U>
Matrix<N,M,T>& Matrix<N,M,T>::operator=(const Matrix<N, M, U>& other) {
	for (size_t n = 0; n < N; ++n)
		for (size_t m = 0; m < M; ++m) {
			data[n][m] = other[n][m];
		}
	return *this;
}

template <size_t N, size_t M, typename F>
std::ostream& operator<<(std::ostream& out, const Matrix<N,M,F>& matr) {
	for (size_t i = 0; i < N; ++i) {
		for (size_t k = 0; k < M; ++k)
			out << matr[i][k] << ' ';
		out << '\n';
	}
	return out;
}

template <size_t N, size_t M, typename T, typename U>
auto operator*(const Matrix<N, M, T>& matr, U a) -> 
Matrix<N,M,std::common_type_t<T,U>> {
	Matrix<N, M, std::common_type_t<T,U>> result(N,M);
	result = matr;
	result *= a;
	return result;
}

template <size_t N, size_t M, typename T> template <typename U>
Matrix<N, M, T>& Matrix<N,M,T>::operator+=(const Matrix<N, M, U>& other) {
		for (size_t i = 0; i < N; ++i)
			for (size_t k = 0; k < M; ++k)
				data[i][k] += other[i][k];
		return *this;
}

template <size_t N, size_t M, typename T> template <typename U>
Matrix<N, M, T>& Matrix<N,M,T>::operator-=(const Matrix<N, M, U>& other) {
		for (size_t i = 0; i < N; ++i)
			for (size_t k = 0; k < M; ++k)
				data[i][k] -= other[i][k];
		return *this;
}

template <size_t N, size_t M, typename T, typename U>
auto operator-(const Matrix<N, M, T>& left, const Matrix<N, M, U>& right) ->
Matrix<N,M,std::common_type_t<T,U>> {
	Matrix <N, M, std::common_type_t<T,U>> result(N, M);
		if constexpr (std::is_same_v<std::common_type_t<T,U>,T>) {
			result = left;
			result -= right;
			return result;
		}
		else {
			result = right;
			result -= left;
			return result;
		}
}

template <size_t N, size_t M, typename T, typename U>
auto operator+(const Matrix<N,M,T>& left, const Matrix<N,M,U>& right) ->
Matrix<N,M,std::common_type_t<T,U>> {
	Matrix < N, M, std::common_type_t<T,U>> result(N,M);
		if constexpr (std::is_same_v<std::common_type_t<T,U>,T>) {
			result = left;
			result += right;
			return result;
		}
		else {
			result = right;
			result += left;
			return result;
		}
}

template <size_t N, size_t M, size_t K, typename T, typename U>
auto operator*(const Matrix<N, M, T>& left, const Matrix<M, K, U>& right) ->
Matrix<N, K, std::common_type_t<T, U>> {
	Matrix<N, K, std::common_type_t<T, U>> result(N, K);
	for (size_t n = 0; n < N; ++n)
		for (size_t k = 0; k < K; ++k)
			for (size_t m = 0; m < M; ++m)
				result[n][k] += (left[n][m] * right[m][k]);
	
	return result;
}

template <size_t N, size_t M, typename T> template <typename U>
bool Matrix<N, M, T>::operator==(const Matrix<N, M, U>& other) const {
		for (size_t i = 0; i < N; ++i)
			for (size_t k = 0; k < M; ++k) {
				if (data[i][k] != other[i][k])
					return false;
			}
		return true;
}

template <size_t N, size_t M, typename T> template <typename U>
bool Matrix<N,M,T>::operator!= (const Matrix<N, M, U>& other) const {
	return !(*this == other);
}

template <size_t N, size_t M, typename T>
std::vector<T>& Matrix<N,M,T>::operator[](size_t i) {
	return data[i];
}

template <size_t N, size_t M, typename T>
const std::vector<T>& Matrix<N, M, T>::operator[](size_t i) const {
	return data[i];
}

template <size_t N, size_t M, typename T>
const std::vector<std::vector<T>>& Matrix<N,M,T>::get_data() const {
	return data;
}

template <size_t N, size_t M, typename T>
size_t Matrix<N, M, T>::size() const {
	return N * M;
}

template <size_t N, size_t M, typename T>
Matrix<M, N, T> Matrix<N, M, T>::transpouse() const {
	Matrix<M, N, T> result(M, N);
	for (size_t m = 0; m < M; ++m)
		for (size_t n = 0; n < N; ++n)
			result[m][n] = data[n][m];

	return result;
}

// Function add using fold exspressions, run faster for 2 times than operator +(Matrix, Matrix)
template <typename... Tail>
auto add(const Tail&... tail) -> 
Matrix<Sizes<Tail...>::value_N, Sizes<Tail...>::value_M, typename Common_type<Tail...>::type> {
		static_assert(Verify_size_for_add<Tail...>::value);
		using type = typename Common_type<Tail...>::type;
		const size_t N = Sizes<Tail...>::value_N;
		const size_t M = Sizes<Tail...>::value_M;
		Matrix<N, M, type> result(N, M);

		for (size_t i = 0; i < N; ++i)
			for (size_t k = 0; k < M; ++k)
				result[i][k] = ((tail[i][k]) + ...);

		return result;
}

template <typename... Tail>
auto substract(const Tail&... tail) ->
Matrix<Sizes<Tail...>::value_N, Sizes<Tail...>::value_M, typename Common_type<Tail...>::type> {
	static_assert(Verify_size_for_add<Tail...>::value);
	using type = typename Common_type<Tail...>::type;
	const size_t N = Sizes<Tail...>::value_N;
	const size_t M = Sizes<Tail...>::value_M;
	Matrix<N, M, type> result(N, M);

	for (size_t i = 0; i < N; ++i)
		for (size_t k = 0; k < M; ++k)
			result[i][k] = (... - (tail[i][k]));

	return result;
}

template <typename T>
T ABS(const T& value) {
	if constexpr (std::is_same_v<T, big_integer> || std::is_same_v<T, rational>) {
		return value.Abs();
	}
	else {
		return std::abs(value);
	}
}


