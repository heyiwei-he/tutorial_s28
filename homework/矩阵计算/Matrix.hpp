#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>
#include<vector>
class Matrix
{
private:
	int i;//i表示矩阵的行数
	int j;//j表示矩阵的列数
	std::vector<std::vector<double>> p;
public:
	Matrix(int i, int j);//构造函数，初始化矩阵的行数和列数
	void set_Matrix();
	void print_Matrix();
	 Matrix operator +(const Matrix& other);//重载加法运算符，实现矩阵的加法运算
	 Matrix operator -(const Matrix& other);//重载减法运算符，实现矩阵的减法运算
	 Matrix operator *(const Matrix& other);//重载乘法运算符，实现矩阵的乘法运算
	~Matrix();//析构函数，释放矩阵的内存空间
};

#endif
