#include<iostream>
#include"Matrix.hpp"
#include<vector>
#include<iomanip>
#include <cstdlib>
using namespace std;
Matrix::Matrix(int m, int n)
{
	i = m;//i表示矩阵的行数
	j = n;//j表示矩阵的列数
	p.resize(i, std::vector<double>(j, 0));
}

void Matrix::set_Matrix()
{
	for (int m = 0;m < i;m++)
	{
		cout << "请输入第" << m + 1 << "行的元素"<<endl;
		for (int n = 0;n < j;n++)
		{
			cin >> p[m][n];
		}
	}
}
void Matrix::print_Matrix()
{
	
	for (int m = 0;m < i;m++)
	{
		cout << "|";
		for (int n = 0;n < j;n++)
		{
			cout << setw(3)<<p[m][n] << " ";
		}
		cout << "|" << endl;
	}
}
Matrix Matrix:: operator +(const Matrix& other)
{
	if (this->i != other.i || this->j != other.j)
	{
		cout << "您输入的两个矩阵无法进行计算！";
		exit(-1);//直接终止整个程序，立刻退出,代表异常退出
	}
	Matrix result(i, j);
	for (int m = 0;m < i;m++)
	{
		for (int n = 0;n < j;n++)
		{
			result.p[m][n]= this->p[m][n] + other.p[m][n];
		}
	}
	return result;
}

Matrix Matrix:: operator -(const Matrix& other)
{
	if (this->i != other.i || this->j != other.j)
	{
		cout << "您输入的两个矩阵无法进行计算！";
		exit(-1);//直接终止整个程序，立刻退出,代表异常退出
	}
	Matrix result(i, j);
	for (int m = 0;m < i;m++)
	{
		for (int n = 0;n < j;n++)
		{
			result.p[m][n] = this->p[m][n] - other.p[m][n];
		}
	}
	return result;
}
Matrix Matrix:: operator *(const Matrix& other)
{
	if (this->i != other.i || this->j != other.j)
	{
		cout << "您输入的两个矩阵无法进行计算！";
		exit(-1);//直接终止整个程序，立刻退出,代表异常退出
	}
	Matrix result(i, j);
	for (int m = 0;m < i;m++)
	{
		for (int n = 0;n < j;n++)
		{
			for (int h = 0;h < j;h++)
			{
				result.p[m][n] += this->p[m][h] * other.p[h][n];
			}
			
		}
	}
	return result;
}
Matrix::~Matrix()
{
	for (int k = 0; k < this->i; k++)
	{
		this->p[k].clear();
	}
	this->p.clear();
}
int main()
{
	int choice,N;
	cout << "您想要进行什么计算（1-加法；2-减法；3-乘法）\n";
	cin >> choice;
	cout << "您想要进行几个矩阵的计算？\n";
	cin >> N;
	
	int i, j;
	cout << "请输入第" << 1 << "个矩阵的行数" << endl;
	cin >> i;
	cout << "请输入第" << 1 << "个矩阵的列数" << endl;
	cin >> j;
	Matrix sum(i, j);
	sum.set_Matrix();
	cout << "第1个矩阵为：" << endl;
	sum.print_Matrix();
	for (int m = 1;m < N;m++)
	{
		cout << "请输入第" << m + 1 << "个矩阵的行数" << endl;
		cin >> i;
		cout << "请输入第" << m + 1 << "个矩阵的列数" << endl;
		cin >> j;
		Matrix mat(i, j);
		mat.set_Matrix();
		cout << "第" << m + 1 << "个矩阵为：" << endl;
		mat.print_Matrix();
		switch (choice)
		{
			case 1:
				sum = sum + mat;
				break;
			case 2:
				sum = sum - mat;
				break;
			case 3:
				sum = sum * mat;
				break;
			default:
				cout << "无效的选择！" << endl;
				exit(-1);
		}
	}
	cout << "计算结果为：" << endl;
	sum.print_Matrix();
	return 0;
}
