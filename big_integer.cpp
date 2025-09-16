#include "big_integer.h"
#include <limits>
#include <random>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <cmath>


//      КОНСТРУКТОРЫ
big_integer::big_integer() {
	v.push_back(0);
}


big_integer::big_integer(int a) {
	evaluate(a);
}

big_integer::big_integer(unsigned a) {
	evaluate(a);
}
 

big_integer::big_integer(long long a) {
	evaluate(a);
}

big_integer::big_integer(long a) {
	evaluate(a);
}

big_integer::big_integer(unsigned long a) {
	evaluate(a);
}


big_integer::big_integer(unsigned long long a) {
	evaluate(a);
}

big_integer::big_integer(const char* ch) {
	this->CheckSignAndWrite(ch);
}

big_integer::big_integer(const std::string& s) {
	this->CheckSignAndWrite(s.c_str());
}


big_integer::big_integer(const std::vector<int>& vec) {
	if (vec.empty())
		v.push_back(0);
	for (size_t i = vec.size() - 1; i != -1; --i)
		v.push_back(vec[i]);
}

big_integer::big_integer(const big_integer& b) {
	sign = b.sign;
	v.assign(b.v.begin(), b.v.end());
}

big_integer::big_integer(double d) {
	if (d == 0)
		*this = 0;
	else if (d < 0) {
		double temp = d * -1;
		if (temp < 1)
			*this = 0;
		else {
			this->HelperForCtorFromDouble(temp);
			sign = false;
		}
	}
	else {
		if (d < 1) {
			*this = 0;
		}
		else {
			this->HelperForCtorFromDouble(d);
		}
	}
}

// ОПЕРАТОРЫ 
bool big_integer::operator!() const {
	return !(static_cast<bool>(*this));
}

big_integer big_integer::operator-() {
	if (*this == 0)
		return *this;

	big_integer temp = *this;
	temp.sign = (sign == false) ? true : false;
	return temp;
}
big_integer big_integer::operator+() {
	big_integer temp = *this;
	temp.sign = true;
	return temp;
}
big_integer operator"" _bigInt(unsigned long long a) {
	big_integer temp = static_cast<big_integer>(static_cast<long long>(a));
	return temp;
}

std::ostream& operator<<(std::ostream& out, const big_integer& b) {
	b.CheckSignForOut(out);
	for (size_t i = b.v.size() - 1; i != -1; --i) {
		out << static_cast<int>(b.v[i]);
	}
	return out;
}

std::istream& operator>>(std::istream& in, big_integer& b) {
	std::string temp;
	in >> temp;
	b = static_cast<big_integer>(temp);
	return in;
}

big_integer& big_integer::operator++() {
	*this += static_cast<big_integer>(1);
	return *this;
}

big_integer big_integer::operator++(int) {
	big_integer temp = *this;
	*this += static_cast<big_integer>(1);
	return temp;
}

big_integer& big_integer::operator--() {
	*this -= static_cast<big_integer>(1);
	return *this;
}

big_integer big_integer::operator--(int) {
	big_integer temp = *this;
	*this -= static_cast<big_integer>(1);
	return temp;
}

big_integer operator+(const big_integer& a, const big_integer& b) {
	big_integer temp = a;
	temp += b;
	return temp;
}

big_integer& big_integer::operator+=(const big_integer& b) {
	if (this->Abs() == b.Abs() && sign != b.sign) {
		sign = true;
		v.erase(v.begin() + 1, v.end());
		v[0] = 0;
		v.shrink_to_fit();
		return *this;
	}

	if (sign == b.sign) {
		sign = this->sign;
		this->PredSummer(b);
	}
	else {
		if (sign == false) {
			if (this->Abs() < b.Abs())
				sign = true;
			else
				sign = false;
		}
		else {
			if (this->Abs() < b.Abs())
				sign = false;
			else
				sign = true;
		}
		size_t countInsub = 0;
		this->PredDestr(b, countInsub);
		while (v.back() == 0 && v.size() > 1) {
			v.pop_back();
		}
	}
	return *this;
}

big_integer operator-(const big_integer& a, const big_integer& b) {
	big_integer temp = a;
	temp -= b;
	return temp;
}

big_integer& big_integer::operator-=(const big_integer& b) {

	size_t countInsub = 0;
	if (*this == b && sign == b.sign) {
		sign = true;
		v.erase(v.begin() + 1, v.end());
		v[0] = 0;
		v.shrink_to_fit();
		return *this;
	}
	if (sign == b.sign) {
		if (sign == false) {
			if (this->Abs() > b.Abs()) {
				sign = false;
			}
			else {
				sign = true;
			}
		}
		else {
			if (this->Abs() > b.Abs()) {
				sign = true;
			}
			else {
				sign = false;
			}
		}
		this->PredDestr(b, countInsub);
		while (v.back() == 0) {
			v.pop_back();
		}
	}
	else {
		if (sign == false) {
			sign = false;
		}
		else {
			sign = true;
		}
		this->PredSummer(b);
	}
	return *this;
}

void big_integer::PredSummer(const big_integer& b) {
	if (v.size() < b.v.size()) {
		size_t InitialSize = v.size();
		v.resize(b.v.size(), 0);
		v.reserve(b.v.size() + 1);
		this->Summer(v.size(), b.v.size(), b, true, InitialSize);
	}
	else
	{
		size_t InitialSize = b.v.size();
		v.reserve(v.size() + 1);
		this->Summer(b.v.size(), v.size(), b, false, InitialSize);
	}
}

void big_integer::PredDestr(const big_integer& b, size_t& countInsub) {
	if (this->v.size() >= b.v.size() && this->Abs() > b.Abs()) {
		this->Destructer(b, b.v.size(), v.size(), countInsub, true);
	}
	else {
		v.resize(b.v.size());
		this->Destructer(b, v.size(), b.v.size(), countInsub, false);
		if (countInsub != b.v.size()) {
			memcpy(&v[0] + countInsub, &b.v[0] + countInsub, (b.v.size() - countInsub));
		}
	}
}

void big_integer::Destructer(const big_integer& b, size_t a, size_t a2, size_t& count, bool bol) {
	int flag = 0;
	for (; count < a; ++count) {
		this->HelperDestructer1(b, flag, count, bol);
	}
	for (; count < a2 && flag != 0; ++count) {
		this->HelperDestructer2(b, flag, count, bol);
	}
}

void big_integer::HelperDestructer1(const big_integer& b, int& flag, size_t& count, bool bol) {
	if (bol == true) {
		if (v[count] - flag < b.v[count]) {
			v[count] = 10 + v[count] - flag - b.v[count];
			flag = 1;
		}
		else {
			v[count] = (v[count] - b.v[count] - flag);
			flag = 0;
		}
	}
	else {
		if (b.v[count] - flag < v[count]) {
			v[count] = 10 + b.v[count] - flag - v[count];
			flag = 1;
		}
		else {
			v[count] = b.v[count] - v[count] - flag;
			flag = 0;
		}
	}
}

void big_integer::HelperDestructer2(const big_integer& b, int& flag, size_t& count, bool bol) {
	if (bol == true) {
		if (v[count] - flag < 0) {
			v[count] = 9;
			flag = 1;
		}
		else {
			v[count] -= flag;
			flag = 0;
		}
	}
	else {
		if (b.v[count] - flag < 0) {
			v[count] = 9;
			flag = 1;
		}
		else {
			v[count] = (b.v[count] - flag);
			flag = 0;
		}
	}
}

big_integer& big_integer::operator*=(const big_integer& b) {
	sign = (sign == b.sign) ? true : false;
	if (v.size() <= b.v.size()) {
		this->Multiply(*this, b);
	}
	else {
		this->Multiply(b, *this);
	}
	return *this;
}

big_integer& big_integer::operator=(const big_integer& other) {
	if (this == &other)
		return *this;
	swap(other);
	return *this;
}


big_integer operator*(const big_integer& a, const big_integer& b) {
	big_integer temp = a;
	temp *= b;
	return temp;
}


big_integer& big_integer::operator/=(const big_integer& b) {
	if (b == 0)
		throw std::invalid_argument("devision by zero");

	int ans = 0;
	bool tempSign = (sign != b.sign) ? false : true;
	std::vector<int> delim;
	std::vector<int> answer;
	big_integer temp = b.Abs();
	big_integer temp2 = this->Abs();

	if (temp == 1) {
		sign = tempSign;
		return *this;
	}
	
	if (temp2 < temp) {
		sign = tempSign;
		*this = 0;
		return *this;
	}
	else if (temp2 == temp) {
		*this = static_cast<big_integer>(1);
		sign = tempSign;
		return *this;
	}

	big_integer del(delim);
	if (temp2 <= 2147483647) {
		*this = static_cast<int>(temp2) / static_cast<int>(temp);
		sign = tempSign;
		return *this;
	}
	else {
		for (int i = v.size() - 1; i != -1;) 
		{
			while (i != -1 && del < temp) 
			{
				delim.push_back(temp2.v[i--]);
				del = delim;
				if (del < temp && i > 0 && i < v.size() - b.v.size())
					answer.push_back(0);
			}
			while (del >= temp) 
			{
				del -= temp;
				++ans;
			}
			answer.push_back(ans);
			while (i > 0 && temp2.v[i] == 0 && del == 0)
			{
				answer.push_back(0);
				--i;
			}
			ans = 0;
			delim.clear();
			if (del != 0) 
			{
				for (int i = del.v.size() - 1; i != -1 ; --i)
					delim.push_back(del.v[i]);
			}
		}
		*this = answer;
		sign = tempSign;
		return *this;
	}
}

big_integer operator/(const big_integer& a, const big_integer& b) {
	big_integer temp = a;
	temp /= b;
	return temp;
}

big_integer& big_integer::operator%=(const big_integer& b) 
{
	bool tempSign = sign;
	if (b == 0)
		throw std::invalid_argument("division by zero");

	std::vector<int> delim;
	big_integer temp = b.Abs();
	big_integer temp2 = this->Abs();
	
	if (temp2 == temp) {
		*this = 0;
		return *this;
	}

	if (temp2 < temp) {
		return *this;
	}
	
	big_integer del(delim);
		for (int i = temp2.v.size() - 1; i != -1;)
		{
			while (i != -1 && del < temp)
			{
				delim.push_back(temp2.v[i--]);
				del = delim;
			}
			while (del >= temp)
			{
				del -= temp;
			}
			while (i > 0 && temp2.v[i] == 0 && del == 0)
			{
				--i;
			}
			delim.clear();
			if (del != 0)
			{
				for (int i = del.v.size() - 1; i != -1; --i)
					delim.push_back(del.v[i]);
			}
		}
		*this = del;
		sign = (*this != 0) ? tempSign : true;
		return *this;
}

big_integer operator%(const big_integer& a, const big_integer& b) {
	big_integer temp = a;
	temp %= b;
	return temp;
}

bool operator>(const big_integer& a, const big_integer& b) {
	if (a == b)
		return false;

	return !(a < b);
}

bool operator>=(const big_integer& a, const big_integer& b) {
	return (b < a || b == a);
}

bool operator<(const big_integer& a, const big_integer& b) {
	if (a.sign < b.sign)
		return true;
	if ((a.sign > b.sign) || (a.v.size() > b.v.size() && a.sign == true) || (a.v.size() < b.v.size() && (a.sign == false && b.sign == false))) {
		return false;
	}
	if (a.v.size() != b.v.size())
		return true;
	else if (a.sign == true) {
		return a.HelperForOperatorLowandBig(b, a);
	}
	else {
		return a.HelperForOperatorLowandBig(a, b);
	}
}

bool operator<=(const big_integer& a, const big_integer& b) {
	return (a < b || a == b);
}

bool operator==(const big_integer& a, const big_integer& b) {
	if (a.sign == b.sign) {
		return (a.v.size() != b.v.size()) ? false : a.HelperForMemcmp(a, b);
	}
	return false;
}

bool operator!=(const big_integer& a, const big_integer& b) {
	return !(a == b);
}

big_integer::operator bool() const {
	return (*this > static_cast<big_integer>(0)) ? true : false;
}

big_integer::operator int() const {
	int ans = 0;
	if (*this <= std::numeric_limits<int>::max() && *this >= std::numeric_limits<int>::min()) {
		for (size_t i = v.size() - 1; i != -1; --i) {
			ans = ans * 10 + v[i];
		}
		return (*this >= 0) ? ans : -ans;
	}
#define STRING2(x) #x
#define STRING(x) STRING2(x)

#pragma message (__FILE__"[" STRING(__LINE__) "]: warning: overflowing integer")
	return 0;
}

big_integer::operator double() const {
	if (this->Abs() > std::numeric_limits<double>::max())
		throw std::invalid_argument("too match big_integer");

	size_t count = 0;
	double ans = 0.0;

	if (v.size() > 16) {
		count = v.size() - 16;
		ans = v[v.size() - 16];

		for (size_t i = v.size() - 15; i < v.size(); ++i) {
			ans /= 10;
			ans += v[i];
		}

		for (int i = 0; i < 14; ++i) {
			ans *= 10;
		}
			
	}
	else {
		ans = v.front();
		for (size_t i = 1; i < v.size(); ++i) {
			ans /= 10;
			ans += v[i];
		}

		for (int i = 0; i < v.size() - 1; ++i) {
			ans *= 10;
		}
	}

	return (*this >= 0) ? ans : -ans;
}

bool big_integer::HelperForMemcmp(const big_integer& a, const big_integer& b) const {
	int temp = memcmp(a.to_string().c_str(), b.to_string().c_str(), b.to_string().size());
	return (temp == 0) ? true : false;
}

//                                      МЕТОДЫ
std::string big_integer::to_string() const {
	std::string temp;
	temp.reserve(v.size());
	this->CheckSignForOut(temp);
	this->HelperToString(temp);
	return temp;
}

void big_integer::HelperToString(std::string& temp) const {
	size_t i = v.size() - 1;
	while (i != 0 && v[i] == 0)
		--i;
	for (; i != -1; --i)
		temp.push_back(static_cast<char>(v[i] + 48));
}

void big_integer::CheckSignAndWrite(const char* ch) {
	this->HelperForCheckSingAndWrite(ch, this->CheckSignForChar(ch));
}

void big_integer::HelperForCheckSingAndWrite(const char* ch, int a) {
	if (strlen(ch) > 1 && ch[1] == 0)
		sign = true;
	try {
		if (strlen(ch) == 0 || (strlen(ch) == 1 && a == 0))
			throw std::invalid_argument("invalid argument");
	}
	catch(...) {
		sign = true;
		throw;
	}
	try {
		for (size_t i = strlen(ch) - 1; i != a; --i) {
			if (static_cast<int>(ch[i]) < 48 || static_cast<int>(ch[i]) > 57)
				throw std::invalid_argument("invalid argument");
			v.push_back((static_cast<int>(ch[i]) - 48));
		}
	}
	catch(...) {
		v.clear();
		v.shrink_to_fit();
		v.push_back(0);
		throw;
	}
}

int big_integer::CheckSignForChar(const char* ch) {
	if (strlen(ch) > 1 && ch[1] == '0' && ch[0] == '-') {
		sign = true;
		return 0;
	}
	if (ch[0] == '-') {
		sign = false;
		return 0;
	}
	else
		return -1;
}

template<typename T>
void big_integer::CheckSignForOut(T&) const {}
template<>
void big_integer::CheckSignForOut(std::string& s) const {
	if (!sign)
		s.push_back('-');
}
template<>
void big_integer::CheckSignForOut(std::ostream& out) const {
	if (sign == false)
		out << '-';
}

void big_integer::Summer(size_t vectorSizeFor1, size_t vectorSizeFor2, const big_integer& b, bool Ifsmaller, size_t InitialSizeVector1) {
	int flag = 0;
	size_t countInsum = 0;
	for (; countInsum < InitialSizeVector1; ++countInsum)
	{
		this->HelperForSummer(flag, countInsum, b.v[countInsum]);
	}
	if (Ifsmaller == true)
	{
		for (; countInsum < vectorSizeFor2 && flag != 0; ++countInsum) {
			this->HelperForSummer(flag, countInsum, b.v[countInsum]);
		}
		if (flag == 0 && countInsum != b.v.size())
			memcpy(&v[0] + countInsum, &b.v[countInsum], (b.v.size() - countInsum));
	}
	else {
		for (; countInsum < vectorSizeFor2 && flag != 0; ++countInsum) {
			this->HelperForSummer(flag, countInsum);
		}
	}
	if (flag != 0)
		v.push_back(flag);
}

void big_integer::HelperForSummer(int& flag, size_t countInsum, int temp) {
	v[countInsum] += (temp + flag);
	if (v[countInsum] > 9) {
		v[countInsum] %= 10;
		flag = 1;
	}
	else {
		flag = 0;
	}
}

bool big_integer::HelperForOperatorLowandBig(const big_integer& a, const big_integer& b) const {
	size_t count = a.v.size() - 1;
	for (; count > 0 && a.v[count] == b.v[count]; --count) {}
	return (a.v[count] > b.v[count]) ? true : false;
}

void big_integer::Multiply(const big_integer& a, const big_integer& b) {
	std::vector<int8_t> temp;
	int flag = 0;
	int flag2 = 0;
	int temp2 = 0;
	temp.resize(b.v.size() + a.v.size(), 0);
	for (size_t i = 0; i < a.v.size(); ++i) {
		for (size_t k = 0; k < b.v.size(); ++k) {
			temp2 = a.v[i] * b.v[k] + flag;
			if (temp2 > 9) {
				flag = temp2 / 10;
				temp2 %= 10;
			}
			else {
				flag = 0;
			}
			temp[k + i] = temp[k + i] + temp2 + flag2;
			if (temp[k + i] > 9) {
				flag2 = temp[k + i] / 10;
				temp[k + i] %= 10;
			}
			else {
				flag2 = 0;
			}
		}
		if (flag != 0 || flag2 != 0)
			temp[b.v.size() + i] = flag + flag2;
		flag = 0;
		flag2 = 0;
	}
	v.resize(temp.size());
	v.swap(temp);
	if (v.back() == 0)
		v.pop_back();
}

template <typename T>
void big_integer::evaluate(T a) {
	if (a == 0)
		sign = true;
	if (a < 0) {
		sign = false;
		while (a < -9)
		{
			v.push_back((a % 10) * -1);
			a /= 10;
		};
		v.push_back(a * -1);
	}
	else {
		while (a > 9)
		{
			v.push_back(a % 10);
			a /= 10;
		};
		v.push_back(a);
	}
}

big_integer big_integer::Abs() const {
	big_integer temp = *this;
	temp.sign = true;
	return temp;
}

big_integer& big_integer::reverse() {
	for (size_t i = 0, j = v.size() - 1; i < v.size() / 2; ++i, --j)
		std::swap(v[i], v[j]);
	return *this;
}


bool big_integer::getSign() const {
	return sign;
}

const big_integer* big_integer::getbiggerSize(const big_integer& other) const {
	return (v.size() >= other.v.size()) ? this : &other;
}

const big_integer* big_integer::getSmallestSize(const big_integer& other) const {
	return (v.size() < other.v.size()) ? this : &other;
}

void big_integer::swap(big_integer other) {
	v.swap(other.v);
	sign = other.sign;
}

big_integer big_integer::join(const big_integer& other) const {
	big_integer temp = other;
	temp.sign = sign;
	temp.v.insert(temp.v.end(), v.begin(), v.end());
	return temp;
}

rational big_integer::disjoin(size_t precious) {
	size_t count = v.size() - precious;
	big_integer nom = 0;
	big_integer denom = 0;
	nom.v.resize(count);
	memcpy(nom.v.data(), v.data() + precious, count);
	v.erase(v.begin() + precious, v.end());
	denom = *this;
	nom.sign = sign;
	return rational (nom, denom);
}

big_integer big_integer::round(size_t n) const {
	big_integer temp;
	temp.v.resize(n);
	size_t index = v.size() - n;
	memcpy(temp.v.data(), v.data() + index, n);
	if (v[index - 1] >= 5)
		++temp;
	return temp;
}

void big_integer::HelperForCtorFromDouble(double d) {
	std::vector<int> vec;
	
	double temp = (d < 0) ? d * -1 : d;
	double tempDouble = temp;
	int count = -1;
	
	while (tempDouble >= 0.1) { 
		++count;
		tempDouble /= 10;
	}

	tempDouble = temp;
	
	if (count < 15) {
		unsigned long long lg = temp;
		*this = lg;
	}
	else {
		std::string str = std::to_string(temp);
		std::string substr = str.substr(0, 15);
		substr.append(count - 15,'0');
		*this = static_cast<big_integer>(substr);
	}
	sign = (d < 0) ? false : true;
}

void swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}



//													RATIONAL 
rational::rational() {
	nom = big_integer(0);
	denom = big_integer(0);
}

rational::rational(const big_integer& a, const big_integer& b): nom(a), denom(b.Abs()) {}

rational::rational(const big_integer& a): nom(a), denom(0) {}

rational::rational(int a): nom(a), denom(0) {}

rational::rational(double d) {  // rational поддерживает 16 точных заков от double после запятой
	nom = d; 
	
	if (nom.v.size() > 15) {
		denom = 0;
	}
	else {
		big_integer tempDenom = 0;
		std::vector<int> temp;
		if (d < 0) {
			nom.sign = false;
			d *= -1;
		}
		long long a = d;                              
		double tempd = d - a;

		for (int i = 0; i < 310; ++i) {
			tempd *= 10;
			int a = tempd;
			temp.push_back(a);
			tempd -= a;
		}

		tempDenom = temp;
		tempDenom.reverse();
		
		size_t i = tempDenom.v.size() - 1;
		while (i != 0 && tempDenom.v.back() == 0) {
			tempDenom.v.pop_back();
			--i;
		}

		if (tempDenom.v.size() < 16 - nom.v.size())
			denom = tempDenom.reverse();
		else {
			
			size_t count = 16 - nom.v.size();
			if (tempDenom.v[count] >= 5) {
				denom = tempDenom.reverse().round(count);
				size_t i = denom.v.size() - 1;
				denom.reverse();
					
				while (i != -1 && denom.v.back() == 0) {
					denom.v.pop_back();
					--i;
				}
				denom.reverse();
			}
			else {
				size_t i = count - 1;  
				for (; i != -1 && tempDenom.v[i] == 0; --i);
				if (i == -1) {
					for (; tempDenom.v[count++] == 0;);
					for (; tempDenom.v[count++] != 0;);
					denom = tempDenom.reverse().round(count - 1);
				}
				else {
					denom = tempDenom.reverse().round(i + 1);
				}
			}
		}
	}
}

 
rational::rational(const std::string& str) {
	if (size_t indexPoint = str.find('.'); indexPoint != std::string::npos) {
		nom = static_cast<big_integer>(str.substr(0, indexPoint));
		denom = static_cast<big_integer>(str.substr(indexPoint + 1));
	}
	else {
		nom = static_cast<big_integer>(str);
		denom = 0;
	}
}

rational::rational(const char* ptr): rational(std::string(ptr)) {}

rational::rational(const rational& other): nom(other.nom), denom(other.denom) {}

rational& rational::operator=(const rational& other) {
	swap(other);
	return *this;
}

//												ОПЕРАТОРЫ RATIONAL

std::ostream& operator<<(std::ostream& out, const rational& rat) {
		out << rat.nom;
		if (!rat.denom.v.empty()) {
			out << '.' << rat.denom;
		}
		return out;
}

std::istream& operator>>(std::istream& in, rational& rat) {
	std::string temp;
	in >> temp;
	rat = static_cast<rational>(temp);
	return in;
}

rational& rational::operator+=(const rational& other) {
		if (*this == 0) {
			*this = other;
			return *this;
		}
		else if (other == 0)
			return *this;

		if (nom.sign != other.nom.sign && this->Abs() == other.Abs()) {
			*this = 0;
			return *this;
		}
		size_t count = denom.getbiggerSize(other.denom)->v.size();
		if (nom.sign == other.nom.sign)
		{
			evaluate2(count, other, true);
		}
		else {
			evaluate(count, other);
		}
	return *this;
}

rational operator+(const rational& a, const rational& b) {
	rational temp = a;
	temp += b;
	return temp;
}

rational& rational::operator-=(const rational& other) {
		if (nom.sign == other.nom.sign && this->Abs() == other.Abs()) {
			*this = 0;
			return *this;
		}
		size_t count = denom.getbiggerSize(other.denom)->v.size();

		if (nom.sign == other.nom.sign) {
			this->evaluate(count, other);
		}
		else {
			this->evaluate2(count, other, false);
		}
	return *this;
}

rational operator-(const rational& a, const rational& b) {
	rational temp = a;
	temp -= b;
	return temp;
}

rational& rational::operator*=(const rational& other) {
	size_t precious = denom.v.size() + other.denom.v.size();
	big_integer a = nom.join(denom);
	big_integer b = other.nom.join(other.denom);
	big_integer c = a * b;
	*this = c.disjoin(precious);
	return *this;
}

rational operator*(const rational& a, const rational& b) {
	rational temp = a;
	temp *= b;
	return temp;
}

rational& rational::operator/=(const rational& other) {
	try {
		if (other == 0) {
			throw std::invalid_argument("devision by zero");
		}
	}
	catch (std::invalid_argument& error) {
		std::cout << error.what();
		return *this;
	}
		
	if (this->Abs() == other.Abs()) {
		*this = ((this->nom).sign == other.nom.sign) ? 1 : -1;
		return *this;
	}

	bool tempSign = nom.sign;
	size_t precious = denom.getbiggerSize(other.denom)->v.size();
	size_t smallerSize = denom.getSmallestSize(other.denom)->v.size();
	big_integer a = nom.join(denom);
	big_integer b = other.nom.join(other.denom);
	a.sign = true;
	b.sign = true;
	std::vector<int> ans;

	while (a.v.back() == 0)
		a.v.pop_back();
	while (b.v.back() == 0)
		b.v.pop_back();

	if (denom.v.size() != other.denom.v.size()) {
		if (a.v.size() < b.v.size()) {
			for (int i = 0; i < precious - smallerSize; ++i)
				a *= 10;
		}
		else {
			for (int i = 0; i < precious - smallerSize; ++i)
				b *= 10;
		}
	}
	nom = a / b;
	if (a % b != 0) {
		a %= b;
		this->evaluateForDivide(a, b, ans);
	}
	else if (a % b == 0 && a < b) {
		this->evaluateForDivide(a, b, ans);
		
	}
	denom = ans;

	if (tempSign != other.nom.sign)
		nom.sign = false;
	else
		nom.sign = true;
	return *this;
}

rational operator/(const rational& a , const rational& b) {
	rational temp = a;
	temp /= b;
	return temp;
}


std::partial_ordering rational::operator<=>(const rational& other) const {
	if (*this == other)
		return std::partial_ordering::equivalent;
	if (nom < other.nom)
		return std::partial_ordering::less;
	else if (nom > other.nom) {
		return std::partial_ordering::greater;
	}
	else if (nom == other.nom)
	{
		if (denom.v.size() != other.denom.v.size())
		{
			size_t count = denom.getbiggerSize(other.denom)->v.size();
			const big_integer* ptr = denom.getSmallestSize(other.denom);
			big_integer temp = *ptr;
			while (temp.v.size() < count)
				temp *= 10;
			if (ptr == &denom)
			{
				if (nom.sign == true)
					return (temp < other.denom) ? std::partial_ordering::less : std::partial_ordering::greater;
				else
					return (temp < other.denom) ? std::partial_ordering::greater : std::partial_ordering::less;
			}
			else {
				if (nom.sign == true)
					return (denom < temp) ? std::partial_ordering::less : std::partial_ordering::greater;
				else
					return (denom < temp) ? std::partial_ordering::greater : std::partial_ordering::less;
			}
		}
		else {
			if (nom.sign == true)
				return (denom < other.denom) ? std::partial_ordering::less : std::partial_ordering::greater;
			else
				return (denom < other.denom) ? std::partial_ordering::greater : std::partial_ordering::less;
		}
	}
	return std::partial_ordering::unordered;
}


bool rational::operator==(const rational& other) const {
	if (nom.sign != other.nom.sign)
		return false;

	if (nom != other.nom)
		return false;
	else if (denom.v.size() != other.denom.v.size()) {
		size_t count = denom.getbiggerSize(other.denom)->v.size();
		const big_integer* ptr = denom.getSmallestSize(other.denom);
		big_integer temp = *ptr;
		while (temp.v.size() < count)
			temp *= 10;
		if (ptr = &denom)
			return (temp == other.denom) ? true : false;
		else
			return (denom == temp) ? true : false;
	}
	return (denom == other.denom) ? true : false;
}

rational& rational::operator++() {
	*this += 1;
	return *this;
}

rational rational::operator++(int) {
	rational temp = *this;
	*this += 1;
	return temp;
}

rational& rational::operator--() {
	*this -= 1;
	return *this;
}

rational rational::operator--(int) {
	rational temp = *this;
	*this -= 1;
	return temp;
}

rational rational::operator-() {
	if (*this == 0)
		return *this;

	rational temp = *this;
	temp.nom.sign = (nom.sign == false) ? true : false;
	return temp;
}

rational rational::operator+() {
	rational temp = *this;
	temp.nom.sign = true;
	return temp;
}

rational operator"" _rat(long double a) {
	return rational(static_cast<double>(a));
}

rational::operator bool() const {
	//return (nom > 0 || (nom == 0 && denom > 0)) ? true : false;
	return (*this != 0 ? true : false);
}

rational::operator double() const {
	double res = 0;
	big_integer count = 1;
	big_integer temp_nom = nom.Abs();
	
	for (size_t i = 0; i < denom.v.size(); ++i) {
		count *= 10;
	}

	temp_nom *= count;
	temp_nom += denom;
	
	res = static_cast<double>(temp_nom) / static_cast<double>(count);
	return (nom.sign ? res : res * -1);
}

std::string rational::to_string() const {
	std::string temp = nom.to_string();

	big_integer tempDenom;

	if (denom.v.size() < 16 - nom.v.size())
		tempDenom = denom;
	else {
		size_t count = 16 - nom.v.size();
		if (denom.v[count] >= 5) {
			tempDenom = denom.round(count);
			size_t i = denom.v.size() - 1;

			tempDenom.reverse();
			while (i != -1 && tempDenom.v.back() == 0) {
				tempDenom.v.pop_back();
				--i;
			}
			tempDenom.reverse();
		}
		else {
			size_t j = denom.v.size();
			size_t i = j - count - 1;
			for (; i != j && denom.v[i] == 0; ++i);
			if (i == j) {
				for (; denom.v[count--] == 0;);
				for (; denom.v[count--] != 0;);
				tempDenom = denom.round(count - 1);
			}
			else {
				tempDenom = denom.round(j - i);
			}
		}
	}

	if (tempDenom!= 0) {
		temp.append(".");
		temp.append(tempDenom.to_string());
	}
	return temp;
}

double rational::asDecimal(size_t n) const {
	if (n > 16 - nom.v.size())
		n = 16 - nom.v.size();

	rational temp = this->round(n);
	double ans = static_cast<double>(temp);
	return ans;
}
	
//												МЕТОДЫ RATIONAL

void rational::swap(const rational& other) {
	nom.swap(other.nom);
	denom.swap(other.denom);
}

rational rational::round(size_t count) const{
	if (count >= denom.v.size())
		return *this;

	for (auto i : denom.v)
		std::cout << +i;
	std::cout << '\n';

	rational rat = 0;
	rat.nom = nom;
	if (count == 0) {
		if (denom.v.size() < 15) {
			rat.denom.v.resize(denom.v.size());
			memcpy(rat.denom.v.data(), denom.v.data(), denom.v.size());
			if (rat.denom.v.back() == 9)
				++rat.denom;
			while (!rat.denom.v.empty() && rat.denom.v.back() == 0)
				rat.denom.v.pop_back();
		}
		else {
			rat.denom.v.resize(16 - nom.v.size());
			memcpy(rat.denom.v.data(), denom.v.data(), 16 - nom.v.size());
			for (auto i : rat.denom.v)
				std::cout << +i;
			std::cout << '\n';
		}
	}
	else {
		size_t index = denom.v.size() - count;
		rat.denom.v.resize(count);
		memcpy(rat.denom.v.data(), denom.v.data() + index, count);

		if (*(denom.v.end() - count - 1) >= 5) {
			++rat.denom;
		}
	}
	return rat;
}

rational rational::Abs() const {
	rational temp = *this;
	temp.nom.sign = true;
	return temp;
}

void rational::evaluate(size_t count, const rational& other) {
	rational temp = 0;
	const rational* ptrRat = solutionForNotEqualSize(temp, count, other);

	bool nomSign = nom.sign;
	bool othernomSign = other.nom.sign;
	nom.sign = true;
	ptrRat->nom.sign = true;

	big_integer precious = 1;
	for (int i = 0; i < count; ++i)
		precious *= 10;

	if (nomSign == true) {
		evaluate3(precious, ptrRat, true);
	}
	else {
		evaluate3(precious, ptrRat, false);
	}

	while (denom.v.size() < count) {
		denom.v.push_back(0);
	}
	other.nom.sign = othernomSign;
}

void rational::evaluate2(size_t count, const rational& other, bool b) {
	big_integer precious = 1;
	for (int i = 0; i < count; ++i)
		precious *= 10;

	rational temp = 0;
	const rational* ptrRat = solutionForNotEqualSize(temp, count, other);
	
	if (denom + ptrRat->denom >= precious) {
		(nom.sign == true) ? ++nom : --nom;
		denom = denom + ptrRat->denom - precious;
	}
	else {
		denom += ptrRat->denom;
	}

	(b) ? nom += ptrRat->nom : nom -= ptrRat->nom;

	while (denom.v.size() < count) {
		denom.v.push_back(0);
	}
}

void rational::evaluate3(big_integer& precious, const rational* ptrRat, bool b) {
	if (*this >= *ptrRat) {
		if (denom >= ptrRat->denom) {
			denom -= ptrRat->denom;
			nom -= ptrRat->nom;
			nom.sign = (b) ? true : false;
		}
		else {
			denom = denom + precious - ptrRat->denom;
			nom = nom - 1 - ptrRat->nom;
			nom.sign = (b) ? true : false;
		}
	}
	else {
		if (denom >= ptrRat->denom) {
			denom = ptrRat->denom + precious - denom;
			nom = ptrRat->nom - 1 - nom;
			nom.sign = (b) ? false: true;
		}
		else {
			denom = ptrRat->denom - denom;
			nom = ptrRat->nom - nom;
			nom.sign = (b) ? false : true;
		}
	}
}

const rational* rational::solutionForNotEqualSize(rational& temp , size_t count, const rational& other) {
	const big_integer* ptr = denom.getSmallestSize(other.denom);
	const rational* ptrRat = nullptr;

	if (ptr == &(denom)) {
		while (denom.v.size() < count)
			denom *= 10;
		ptrRat = &other;
	}
	else {
		temp = other;
		while (temp.denom.v.size() < count)
			temp.denom *= 10;
		ptrRat = &temp;
	}
	return ptrRat;
}

void rational::evaluateForDivide(big_integer& a, big_integer& b, std::vector<int>& ans) {
	size_t count = 0;
	size_t item = 0;

	while (a != 0 && count < 51) {
		while (a < b) {
			a *= 10;
			++item;
			if (item >= 2)
				ans.push_back(0);
		}
		item = 0;
		++count;
		int temp = static_cast<int>(a / b);
		ans.push_back(temp);
		a %= b;
	}
}


double random(double min, double max) {
	return static_cast<double>(rand()) / RAND_MAX * (max - min) + min;
}









